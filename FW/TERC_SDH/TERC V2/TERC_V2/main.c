#include <avr/io.h>
#include "hw.h"
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>

#define FW_VERSION	1

#include "stop_timer_var.h"
#include "utils.h"
#include "slave_modbus.h"
#include "mb_rs485.h"
#include "USART.h"

uint16_t inputs;
uint16_t outputs;

uint16_t i_reg[sizeof(target_inputs)/2 + ((sizeof(target_inputs)%2)*2)];
uint16_t h_reg[sizeof(target_outputs)/2 + ((sizeof(target_outputs)%2)*2)];

target_inputs *ins = i_reg;
target_outputs *outs = h_reg;

//target_inputs *inputs = (target_inputs *)i_reg;;
//target_outputs *outputs = (target_outputs *)h_reg;

int main(void)
{
	hwInit();

	ModBusInit(outputs, inputs, i_reg, h_reg);
	mb_rs485_interupt(false);

	sei();
	uint8_t LED_init = 4;
	uint16_t LED_timer = 300;
	uint16_t init_timer = 1000;

	ins->status = 0;
	
	while(1)
	{		
		
	// INICIALIZACNI SEKVENCE + BLIKANI STAVOVYCH LED
	
		if ( TCA0.SPLIT.INTFLAGS & TCA_SPLIT_LUNF_bm)	// pri F_CPU 14745600 je to cca 1.1ms
		{	
			if(LED_init>0)
			{
				if(LED_timer>0) 
				{
					LED_timer--;
					switch(LED_init)
					{
						case 4:
							SET(LED_R);
							break;
						case 3:
							SET(LED_G);
							break;
						case 2:
							SET(LED_Y);
							break;
						case 1:
							RESET(LED_Y);
							RESET(LED_R);
						break;
					}
				}
				else
				{
					LED_timer = 300;
					LED_init--;
				}
			}
			else 
			{
				if(--LED_timer<1)
				{
					TOGGLE(LED_G);
					LED_timer = 200;
					readInputs();
				}
			}

					
	// HLAVNI KOD
			
			if(LED_init == 0)
			{
				switch (ins->status)
				{
					case initialization:
						SET(TARGET_L_LIGHT);
						SET(TARGET_R_LIGHT);
						SET(VALVE);
						if(--init_timer<1)
						{
							RESET(VALVE);
							RESET(TARGET_L_LIGHT);
							RESET(TARGET_R_LIGHT);
							ins->status++;
						}
					break;
					case wait_for_start_command:
						if(outs->target_commands = 0x01)

						{
							if(!IS_SET(TARGET_L_LOW) && !IS_SET(TARGET_L_FULL) && !IS_SET(TARGET_R_LOW) && !IS_SET(TARGET_R_FULL))
							{
								RESET(TARGET_L_LIGHT);
								RESET(TARGET_R_LIGHT);
								RESET(VALVE);
								ins->status++;
							}	
						}
						
					break;
					case wait_for_target_filling:
						if(IS_SET(TARGET_L_FULL))
						{
							SET(TARGET_L_LIGHT);
							ins->target_l_full = 1;
						}
						if(IS_SET(TARGET_R_FULL))
						{
							SET(TARGET_R_LIGHT);
							ins->target_r_full = 1;
						}
						if(IS_SET(TARGET_L_FULL) && IS_SET(TARGET_R_FULL))
							ins->status++;
					break;
					case both_targets_filled:
						SET(VALVE);
						ins->status++;
					break;
					case draining:
						if(!IS_SET(TARGET_L_LOW) && !IS_SET(TARGET_L_FULL) && !IS_SET(TARGET_R_LOW) && !IS_SET(TARGET_R_FULL))
						{
							ins->target_l_full = 0;
							ins->target_r_full = 0;
							ins->status++;
						}
					break;
					case drained:
						RESET(TARGET_L_LIGHT);
						RESET(TARGET_R_LIGHT);
						RESET(VALVE);
						ins->target_l_empty = 1;
						ins->target_r_empty = 1;
						ins->status = 1;
					break;
				}
			}
			
			// doplnit battery status - ADC - mereni napeti na baterce + doplnit automatickou kontrolu a v pripade nizkeho napeti dojde k vypnuti vsech vystupu a bude blikat cervena LED v malém intervalu
			
			TCA0.SPLIT.INTFLAGS |= TCA_SPLIT_LUNF_bm;
			
			MainLoopModBus();
			
		}
		mb_do_transmission();
	}

}

void hwInit()
{
	 // PORTA: PA2, PA3 a PA7 jako výstupy
	 PORTA.DIR |= (1 << 2) | (1 << 3) | (1 << 7); // Nastavení pinù PA2, PA3, PA7 jako výstupy
	 PORTA.OUT &= ~((1 << 2) | (1 << 3)); // Inicializace na LOW

	 // PORTF: PF6, PF4 a PF3 jako výstupy
	 PORTF.DIR |= (1 << 5) | (1 << 4) | (1 << 3); // Nastavení pinù PF5, PF4, PF3 jako výstupy
	 PORTF.OUT &= ~((1 << 5) | (1 << 4) | (1 << 3)); // Inicializace na LOW

	 // PORTD: Všechny piny jako vstupy
	 PORTD.DIR = 0x00; // Nastavení všech pinù na PORTD jako vstupy

	 // PORTC: PC0-PC3 jako výstupy
	 PORTC.DIR |= (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // Nastavení bitù 0-3 (PC0-PC3) jako výstupy
	 PORTC.OUT &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)); // Inicializace PC0-PC3 na LOW
	 
	 SET(LED_R);
	 
	 /* Enable crystal oscillator with frequency range 16 MHz and 4K cycles start-up time */
	 ccp_write_io((uint8_t *) &CLKCTRL.XOSCHFCTRLA, CLKCTRL_RUNSTDBY_bm
	 | CLKCTRL_CSUTHF_4K_gc
	 | CLKCTRL_FRQRANGE_16M_gc
	 | CLKCTRL_SELHF_CRYSTAL_gc
	 | CLKCTRL_ENABLE_bm);
	 
	 /* Confirm crystal oscillator start-up */
	 while(!(CLKCTRL.MCLKSTATUS & CLKCTRL_EXTS_bm)){;}
	 
	 /* Clear Main Clock Prescaler */
	 ccp_write_io((uint8_t *) &CLKCTRL.MCLKCTRLB, 0x00);
	 
	 /* Set the main clock to use XOSCHF as source */
	 ccp_write_io((uint8_t *) &CLKCTRL.MCLKCTRLA,
	 CLKCTRL_CLKSEL_EXTCLK_gc);
	 
	 /* Wait for system oscillator changing to complete */
	 while(CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm){;}
	 
	 /* set Normal mode */
	 TCA0.SPLIT.CTRLD = TCA_SPLIT_ENABLE_bm;
	 TCA0.SPLIT.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
	 
	 TCA0.SPLIT.CTRLA =  TCA_SINGLE_CLKSEL_DIV64_gc  | TCA_SINGLE_ENABLE_bm;
	 //TCA0.SINGLE.CNT = 0xBFFF;

	 SET(LED_R);
}

void readInputs()
{
	ins->target_l_empty = IS_SET(TARGET_L_LOW);
	ins->target_l_full = IS_SET(TARGET_L_FULL);
	ins->target_r_empty = IS_SET(TARGET_R_LOW);
	ins->target_r_full = IS_SET(TARGET_R_FULL);
}

