/*
 * UART.c
 *
 * Created: 16.5.2015 11:51:47
 *  Author: AutomotiForHome
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "hw.h"
#include "utils.h"
#include "mb_rs485.h"

#define start_timer TCB1.CTRLA |= TCB_ENABLE_bm //TCCR2 |= ( 1 << CS21)|( 1 << CS20 ) //CTC mode, presscaler 32
#define reset_timer { TCB1.CNT = 0; TCB1.INTFLAGS |= TCB_CAPT_bm; }
#define stop_timer  TCB1.CTRLA &= ~(TCB_ENABLE_bm) //TCCR2 &= ~(( 1 << CS21)|( 1 << CS20 )) //CTC mode, presscaler 32

bool TxEn, RecDataState;
volatile bool udrie = false,rxcie=false,txcie=false;

uint8_t TxBuffer[TX_BUFFER_SIZE], RxBuffer[RX_BUFFER_SIZE];
volatile uint16_t CNTRx, CNTTx, crc = 0xFFFF, TxLen; 
uint16_t RXCounter;   
uint16_t CRCErrorCNT = 0;

void mb_rtu_crc_counter(uint8_t dato);

void tim_vector (void);
void rxc_vector (void);
void txc_vector (void);
void udr_vector (void);

void InitModBusRTU(void)	// This is function initializing mod bus rtu protocol
{	
	// Nutno prepracovat
	#if F_CPU == 14745600UL // nastaveno pro atmega8 timer2, timer 0 nema PWM	// nutno prepocitat !!!!
		/*TCNT2 = 0;
		OCR2  = 231;	// tj 501.3us	pro 76800 kbps			// 102 s preddelickou 32 je to 334.18us = 3xslovo na 115200
		//TIMSK |= ( 1 << OCIE2 );
		TCCR2 = ( 1 << WGM21 )|( 1 << CS21)|( 1 << CS20 ); //CTC mode, presscaler 32
		*/
	#elif F_CPU == 16000000UL // nastaveno pro atmega8 timer2, timer 0 nema PWM	// nutno prepocitat !!!!
		
		/*TCNT2 = 0;
		OCR2  = 250;	// tj 500us	pro 76800 kbps			// 102 s preddelickou 32 je to 334.18us = 3xslovo na 115200
		//TIMSK |= ( 1 << OCIE2 );
		TCCR2 = ( 1 << WGM21 )|( 1 << CS21)|( 1 << CS20 ); //CTC mode, presscaler 32
		*/
		// casovac je uz spusten  z mainu s preddelickou 64
			
		
			
	#else
		#error this F_CPU is not supported
	#endif
	
	TCB1.CNT = 0;
	TCB1.CCMP = 2429;
	TCB1.CTRLA = TCB_CLKSEL_DIV2_gc | TCB_ENABLE_bm;
	
	SET_OUTPUT(DE_RE);
	SET_INPUT(RX);	// kurva dopici.. tohle musim udelat pokazde !!!! nebo zas bude pul dne v pici proc ta kokotina nechodi.....
	SET_OUTPUT(TX);	// kurva dopici.. tohle musim udelat pokazde !!!! nebo zas bude pul dne v pici proc ta kokotina nechodi.....
	
	#define TIMERX_COMP_vect	TCB1_INT_vect
	#define USARTX_RXC_vect USART0_RXC_vect
	#define USARTX_TXC_vect USART0_TXC_vect
	#define USARTX_UDRE_vect USART0_DRE_vect
	
	#define MB_USART_BAUD_RATE(BAUD_RATE) ((float)(16000000 * 64 / (16 * (float)BAUD_RATE)) + 0.5)
	USART0.BAUD = (uint16_t)(MB_USART_BAUD_RATE(BAUDRATE));
	
	//#define USART0_BAUD_RATE(BAUD_RATE) (uint32_t)((64 * 16000000) / (16 * (uint32_t)(BAUD_RATE)))
	
	//USART0.BAUD = (uint16_t)(USART0_BAUD_RATE(38400));   /* set the baud rate*/
	//USART0.BAUD = (uint16_t)(USART0_BAUD_RATE(BAUDRATE));
	//USART0.BAUD=0x0340;
	
	PORTMUX.USARTROUTEA = PORTMUX_USART0_ALT1_gc;
	
	USART0.CTRLC = USART_PMODE_DISABLED_gc | USART_CHSIZE0_bm | USART_CHSIZE1_bm; /* set the parity DISABLED,data format to 8-bit*/	
	USART0.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;      // enable receiver and transmitter
	USART0.CTRLA = USART_RXCIE_bm | USART_TXCIE_bm | USART_DREIE_bm | USART_RS485_bm;

	CNTRx = 0;
	CNTTx = 0;
	RecDataState = false;
	TxEn = false;
}

void mb_rs485_interupt (bool enabled)
{
	if(enabled) 
	{
		USART0.CTRLA |= USART_RXCIE_bm | USART_TXCIE_bm | USART_DREIE_bm;
	}
	else 
	{
		USART0.CTRLA &= ~(USART_RXCIE_bm | USART_TXCIE_bm | USART_DREIE_bm);
		rxcie=true;
		txcie=false;
		udrie=false;
	}
	//interupt_enebled = enabled;
}

void mb_rtu_crc_counter(uint8_t dato) // This is function calculating CRC
{										// *buf - pointer data buffer,  len  - input data len	
	crc ^= (uint16_t)dato;              // XOR byte into least sig. byte of crc
	for (uint16_t i = 8; i != 0; i--)       // Loop over each bit
	{
		if ((crc & 0x0001) != 0)			// If the LSB is set
		{
			crc >>= 1;                      // Shift right and XOR 0xA001
			crc ^= 0xA001;
		}
		else crc >>= 1;                          // Else LSB is not set,  Just shift right
	}
	// Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
	//CRC must be SWAP
}

bool SendDataRTU(MsgTypedef *Msg) //This is function prepare Msg struct for send
{
	if(!TxEn) return false;
	//UartWrite;
	TxEn = false;
	TxBuffer[0] = Msg->SlaveID; //Add slave ID
	TxBuffer[1] = Msg->Function; //Add function code
	memcpy(&TxBuffer[2], Msg->data, Msg->len); //Copy data array
	TxLen = Msg->len + 2;
	//UCSRXB &= ~( 1 << RXEN);
	rxcie = false;
	USART0.TXDATAL = TxBuffer[0];
	crc=0xffff;
	mb_rtu_crc_counter(TxBuffer[0]);
	//UCSRXB |= (1 << UDRIE);
	udrie = true;
	return true;
}

bool RectDataRTU(MsgTypedef *Msg) // This is function receive data and put in to the struct
{
	if(RecDataState)
	{
		Msg->SlaveID = RxBuffer[0];
		Msg->Function = RxBuffer[1];
		Msg->len = RXCounter - 4;
		if(Msg->len < 256) memcpy(Msg->data, &RxBuffer[2], Msg->len);
		RecDataState = false;
		return true;
	}
	return false;
}

void mb_do_transmission (void)
{
	if( TCB1.INTFLAGS & TCB_CAPT_bm)
	{
		TCB1.INTFLAGS |= TCB_CAPT_bm;
		tim_vector();
	}
	if(( USART0.STATUS & USART_RXCIF_bm)&& rxcie)
	{
		//UCSRXA |= (1<<RXC_X);	 //asi neni treba cistit, mel bz to udelat HW dle datasheetu
		rxc_vector();
	}
	if((USART0.STATUS & USART_DREIF_bm) && udrie)// pokud je prazdny UDR registr a je-li co odeslat
	{
		//UCSRXA |= (1<<UDRE_X); //asi neni treba cistit, mel bz to udelat HW dle datasheetu
		udr_vector();
	}
	if((USART0.STATUS & USART_TXCIF_bm)&& txcie)// pokud je prazdny UDR registr a je-li co odeslat
	{
		USART0.STATUS |= USART_TXCIF_bm;
		txc_vector();
	}
}

/*
ISR(TIMERX_COMP_vect) // 3.5 slova 
{
	tim_vector();
}

ISR(USARTX_RXC_vect) //RX handler
{
	rxc_vector();
}

ISR(USARTX_TXC_vect) //TX finish handler
{
	txc_vector();
}

ISR(USARTX_UDRE_vect) //TX handler
{
	udr_vector();	
}
*/

void tim_vector (void)
{
	if(CNTRx > 2)	// nejmensi mozny platny paket ma 4 byty, pocitano od nuly musi byt CNTRx alespon 3
	{
		uint16_t crcRect = ( RxBuffer[CNTRx - 1] << 8 )|RxBuffer[CNTRx - 2];
		if(crcRect == crc)
		{
			
			RecDataState = true; //Set flag rect complette
			RXCounter = CNTRx;
			//TOGGLE(LED_Y);
		}
		else CRCErrorCNT++;
	}
	//UartRead;
	CNTRx = 0;
	TxEn = true;
	//UCSRXB |= ( 1 << RXEN);
	rxcie=true;
	stop_timer;	
	txcie = false;
}

void rxc_vector (void)
{
	volatile static bool valid_address = false;
	RxBuffer[CNTRx] = USART0.RXDATAL;
	TxEn = false;
	if(CNTRx == 0)
	{
		if((RxBuffer[CNTRx]==0) || (RxBuffer[CNTRx]==SLAVE_ADDRESS))
		{
			valid_address = true;
			crc = 0XFFFF;
			TOGGLE(LED_Y);
		}
		else
		{
			valid_address = false;
			CNTRx++;		// aby se zabranilo opetovnemu vstupu do podminky a neporovnavaly se nesmysly
		}
		start_timer;
	}
	reset_timer;
	if(valid_address)
	{
		if(CNTRx > 1) mb_rtu_crc_counter(RxBuffer[CNTRx-2]);	// aby se do CRC nezapocitaval samotny CRC na konci
		CNTRx++;
	}
}

void txc_vector (void)
{
	//UartRead;
	reset_timer;			// zabezpeci prodlevu mezi dalsim vysilanim, nebo prijmem 3.5 slova
	start_timer;
	txcie = false;
}

void udr_vector (void)
{
	CNTTx++;
	if(CNTTx < TxLen)
	{
		USART0.TXDATAL = TxBuffer[CNTTx];
		mb_rtu_crc_counter(TxBuffer[CNTTx]);
	}
	else if(CNTTx == TxLen) USART0.TXDATAL = (crc & 0x00FF);
	else if(CNTTx == TxLen + 1) USART0.TXDATAL = (crc >> 8);
	else
	{
		CNTTx = 0;
		udrie = false;
		txcie = true;
		reset_timer;			// zabezpeci prodlevu mezi dalsim vysilanim, nebo prijmem 3.5 slova
		start_timer;
	}
}


void datatest (void)
{
	USART0.TXDATAL = 0xAA;
}
