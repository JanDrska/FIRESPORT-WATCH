#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>

#include "hw.h"
#include "utils.h"
#include "slave_modbus.h"
#include "mb_rs485.h"
#include "USART.h"
#include "stop_timer_var.h"


// MODBUS PROMENNE A REGISTRY

uint8_t inputs;
uint8_t outputs;

// registry pro input / holding registry
uint16_t i_reg[sizeof(target_inputs) / 2 + ((sizeof(target_inputs) % 2) * 2)];
uint16_t h_reg[sizeof(target_outputs) / 2 + ((sizeof(target_outputs) % 2) * 2)];

// ukazatele na registry jako struktury
target_inputs *ins = (target_inputs *)i_reg;
target_outputs *outs = (target_outputs *)h_reg;



// INICIALIZACE HW
void hwInit()
{
	// PORTA: PA2, PA3, PA7 jako vystupy
	PORTA.DIR |= (1 << 2) | (1 << 3) | (1 << 7);
	PORTA.OUT &= ~((1 << 2) | (1 << 3));

	// PORTF: PF5, PF4, PF3 jako vystupy
	PORTF.DIR |= (1 << 5) | (1 << 4) | (1 << 3);
	PORTF.OUT &= ~((1 << 5) | (1 << 4) | (1 << 3));

	// PORTD: vsechny piny jako vstupy
	PORTD.DIR = 0x00;

	// PORTC: PC0-PC3 jako vystupy
	PORTC.DIR |= (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);
	PORTC.OUT &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0));

	// prepnuti na externi krystal
	ccp_write_io((uint8_t *)&CLKCTRL.XOSCHFCTRLA,
	CLKCTRL_RUNSTDBY_bm |
	CLKCTRL_CSUTHF_4K_gc |
	CLKCTRL_FRQRANGE_16M_gc |
	CLKCTRL_SELHF_CRYSTAL_gc |
	CLKCTRL_ENABLE_bm);

	while (!(CLKCTRL.MCLKSTATUS & CLKCTRL_EXTS_bm)) {;}

	// bez delicky hodin
	ccp_write_io((uint8_t *)&CLKCTRL.MCLKCTRLB, 0x00);

	// hlavni clock z externiho krystalu
	ccp_write_io((uint8_t *)&CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_EXTCLK_gc);

	while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm) {;}

	// TCA0 split mode - casovy zaklad programu
	TCA0.SPLIT.CTRLD = TCA_SPLIT_ENABLE_bm;
	TCA0.SPLIT.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
	TCA0.SPLIT.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;
}

// NACTENI FYZICKYCH VSTUPU DO INPUT REGISTRU
void readInputs()
{
	ins->Input_0 = IS_SET(IN0);
	ins->Input_1 = IS_SET(IN1);
	ins->Input_2 = IS_SET(IN2);
	ins->Input_3 = IS_SET(IN3);
	ins->Input_4 = IS_SET(IN4);
}

// STARTOVNI LED SEKVENCE
uint8_t runInitLedSequence(void)
{
	static uint8_t step = 0;
	static uint16_t timer = 300;

	if (step >= 5)
	return 0;

	if (timer > 0)
	{
		timer--;
		return 1;
	}

	timer = 300;

	switch (step)
	{
		case 0:
		RESET(LED_R);
		RESET(LED_G);
		RESET(LED_Y);
		break;

		case 1:
		SET(LED_R);
		break;

		case 2:
		SET(LED_G);
		break;

		case 3:
		SET(LED_Y);
		break;

		case 4:
		RESET(LED_R);
		RESET(LED_G);
		RESET(LED_Y);
		break;
	}

	step++;
	return 1;
}

// ZAPIS VYSTUPU PODLE HOLDING REGISTRU
void writeOutputs(uint16_t reg, uint8_t blink_phase)
{
	static uint8_t last_blink_phase = 0;

	// vystupy bez blikani / vypnute vystupy
	if (reg & (1 << 0)) { if (!(reg & (1 << 1))) SET(OUT0); } else RESET(OUT0);
	if (reg & (1 << 2)) { if (!(reg & (1 << 3))) SET(OUT1); } else RESET(OUT1);
	if (reg & (1 << 4)) { if (!(reg & (1 << 5))) SET(OUT2); } else RESET(OUT2);
	if (reg & (1 << 6)) { if (!(reg & (1 << 7))) SET(OUT3); } else RESET(OUT3);
	if (reg & (1 << 8)) { if (!(reg & (1 << 9))) SET(RELAY); } else RESET(RELAY);

	// blikaci vystupy prepnout jen pri zmene faze
	if (blink_phase != last_blink_phase)
	{
		last_blink_phase = blink_phase;

		if ((reg & (1 << 0)) && (reg & (1 << 1))) TOGGLE(OUT0);
		if ((reg & (1 << 2)) && (reg & (1 << 3))) TOGGLE(OUT1);
		if ((reg & (1 << 4)) && (reg & (1 << 5))) TOGGLE(OUT2);
		if ((reg & (1 << 6)) && (reg & (1 << 7))) TOGGLE(OUT3);
		// if ((reg & (1 << 8)) && (reg & (1 << 9))) TOGGLE(RELAY);
	}
}

// GENERATOR BLIKACI FAZE
void updateBlinkPhase(uint16_t *timer, uint8_t *phase, uint16_t period)
{
	if (period == 0)
	period = 1;

	if (*timer > 0)
	(*timer)--;
	else
	{
		*timer = period;
		*phase = !(*phase);
	}
}

// ======================================================
// HLAVNI PROGRAM
int main(void)
{
	hwInit();

	// inicializace Modbus stacku
	ModBusInit(&outputs, &inputs, i_reg, h_reg);
	mb_rs485_interupt(false);

	sei();

	// casovace a stavove promenne
	uint16_t led_timer = 200;
	uint16_t blink_timer = 500;
	uint8_t blink_phase = 0;
	uint8_t last_blink_phase = 0;

	uint8_t init_running = 1;
	uint8_t force_output_refresh = 1;

	uint16_t last_hreg0 = 0;
	uint16_t reg_shadow = 0;

	// zakladni stav modulu
	ins->status = 15;
	outs->blink_period = 400;

	while (1)
	{
		// hlavni casovy tick ~ 1.1 ms
		if (TCA0.SPLIT.INTFLAGS & TCA_SPLIT_LUNF_bm)
		{
			TCA0.SPLIT.INTFLAGS |= TCA_SPLIT_LUNF_bm;

			// nejdriv probehne startovni LED sekvence
			if (init_running)
			{
				if (!runInitLedSequence())
				{
					init_running = 0;
					force_output_refresh = 1;
				}
			}
			else
			{
				// heartbeat LED - pomale blikani zelene
				if (led_timer > 0)
				led_timer--;
				else
				{
					led_timer = 200;
					TOGGLE(LED_G);
				}

				readInputs();
				updateBlinkPhase(&blink_timer, &blink_phase, outs->blink_period);
				MainLoopModBus();

				// kopie prvniho holding registru po dokonceni komunikace
				reg_shadow = h_reg[0];

				// vystupy zapisujeme jen pri zmene dat nebo blikaci faze
				if (force_output_refresh || (reg_shadow != last_hreg0) || (blink_phase != last_blink_phase))
				{
					last_hreg0 = reg_shadow;
					last_blink_phase = blink_phase;
					force_output_refresh = 0;

					writeOutputs(reg_shadow, blink_phase);
				}
			}
		}
		mb_do_transmission();
	}
}