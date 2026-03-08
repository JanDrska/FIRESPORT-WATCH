
/*
 * Serial.c
 *
 * Created: 25.7.2014 18:07:36
 *  Author: AutomotiForHome
 */ 

#ifndef MODBUS_PROTOCOL 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "USART.h"


// bufferu 64 bytu se pri rzchlosti 115200 naplni asi za 5.5ms tedy bude nutne 

#define IN_BUFFER_SIZE 32
#define OUT_BUFFER_SIZE 256

volatile char in_buffer[IN_BUFFER_SIZE], out_buffer[OUT_BUFFER_SIZE];
volatile uint16_t in_buf_point=0, out_buf_poin=0, out_buf_fill=0;
bool in_buffer_overflow = false;
bool interupt_enebled = true ;

uint16_t FIFO_Write = 0;
uint16_t FIFO_Read  = 0;
uint16_t FIFO_Cnt   = 0;

void Init_Serial_Port(void)
{
	/* Init BAUD rate */	
	/*UBRRH = 0;
	UBRRL = 7; // 250000 BAUD
	UCSRA |= (1 << U2X);
	
	UCSRB = (1 << RXCIE)|(1 << UDRIE)|(1 << RXEN)|(1 << TXEN);
	UCSRC = (1 << URSEL | 1 << UCSZ1 | 1 << UCSZ0);
	*/
	#define USART0_BAUD_RATE(BAUD_RATE)     ((float)(64 * 16000000 / (16 *(float)BAUD_RATE)) + 0.5)
	
	USART0.BAUD = (uint16_t)(USART0_BAUD_RATE(250000));   /* set the baud rate*/
	
	USART0.CTRLC = USART_CHSIZE0_bm | USART_CHSIZE1_bm; /* set the data format to 8-bit*/
	
	USART0.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;      // enable receiver and transmitter
}

void serial_port_interupt (bool enabled)
{	
	//if(enabled) UCSRB |= (1 << RXCIE)|(1 << UDRIE);
	//else UCSRB &= ~((1 << RXCIE)|(1 << UDRIE));
	
	if(enabled) USART0.CTRLA |= USART_RXCIE_bm | USART_TXCIE_bm;
	else USART0.CTRLA &= ~(USART_RXCIE_bm | USART_TXCIE_bm);
	
	interupt_enebled = enabled;
}

/*USART_status USART_get_in_buffer(uint8_t *data_len, char *data)
{
	USART_status result=received;
	cli();
	
	uint8_t i=0, j = 0;
	for (i=0;i<in_buf_point;i++)
	{
		if((in_buffer[i] == 13) || (in_buffer[i] == 10))
		{
			*data_len = i;
			if(in_buffer[i+1] == 10) i++;
			
			for (;i<in_buf_point;i++) //presune data za entrem na zacatek bufferu
			{
				in_buffer[j] = in_buffer[i];
				j++; 
			}	
			in_buf_point = j;
			break;
		}
		else *(data+i) = in_buffer[i]; 
	}
	sei();
	return result;
} */

USART_status USART_get_in_buffer(uint8_t *data_len, char *data)
{
	USART_status result=received;
	cli();

	uint8_t i=0;
	for (i=0;i<in_buf_point;i++)
	{
		*(data+i) = in_buffer[i];
	}
	*data_len = i;
	in_buf_point = 0;
	sei();
	return result;
}

USART_status USART_send_string(char *string)
{
	USART_Write_Data_In_To_Buffer(string, strlen(string));
	//USART_send_data(string, strlen(string));
	return added;
}

USART_status USART_send_data(char *data, uint8_t data_len)
{

	USART_status result;
	if(interupt_enebled) cli();
	if ((data_len < 1) || (data_len > OUT_BUFFER_SIZE)) result = incorect_data_len;
	else if ((out_buf_fill + data_len) < OUT_BUFFER_SIZE)
	{
  		memcpy(&out_buffer[out_buf_fill], data, data_len);
		if(out_buf_fill == 0) 
			USART0.TXDATAL = out_buffer[out_buf_poin];
		out_buf_fill+= data_len;
		result = added;
	
	} 
	else result = buffer_full;
	//if(interupt_enebled) UCSRB |= (1 << UDRIE);
	if(interupt_enebled) USART0.CTRLA |= (1 << USART_RXCIE_bp);
	sei();
	return result;

}
USART_status USART_send_data_P(char *data, uint8_t data_len)
{

	USART_status result;
	if(interupt_enebled) cli();
	if ((data_len < 1) || (data_len > OUT_BUFFER_SIZE)) result = incorect_data_len;
	else if ((out_buf_fill + data_len) < OUT_BUFFER_SIZE)
	{
  		memcpy_P(&out_buffer[out_buf_fill], data, data_len);
		if(out_buf_fill == 0)
			USART0.TXDATAL = out_buffer[out_buf_poin];
		out_buf_fill+= data_len;
		result = added;

	}
	else result = buffer_full;
	if(interupt_enebled)USART0.CTRLA |= (1 << USART_RXCIE_bp);
	sei();
	return result;

}

USART_status USART_send_char(char dato)
{
	USART_status result;
	cli();
	if (out_buf_fill<OUT_BUFFER_SIZE)
	{
  		out_buffer[out_buf_fill] = dato;
		if(out_buf_fill == 0) USART0.TXDATAL = out_buffer[out_buf_poin];
		out_buf_fill++;
		result = added;
	} 
	else result = buffer_full;
	if(interupt_enebled) USART0.CTRLA |= (1 << USART_RXCIE_bp);
	sei();
	return result;
}






USART_status USART_Write_Data_In_To_Buffer(char *data, uint8_t data_len) {
	if(FIFO_Cnt >= OUT_BUFFER_SIZE) return incorect_data_len;

	for(uint16_t i = 0; i < data_len; i++) {
		out_buffer[FIFO_Write++] = data[i];
		FIFO_Cnt++;

		if(FIFO_Write >= OUT_BUFFER_SIZE) FIFO_Write = 0;

		if(FIFO_Cnt >= OUT_BUFFER_SIZE) return incorect_data_len;
	}
	return buffer_full;
}

USART_status USART_send_FIFO(void) {
	if(FIFO_Cnt == 0) return buffer_empty;

	USART0.TXDATAL = out_buffer[FIFO_Read++];
	FIFO_Cnt--;

	if(FIFO_Read >= OUT_BUFFER_SIZE) FIFO_Read = 0;

	return buffer_full;
}



/*
ISR (USART_RXC_vect)
{	
	in_buffer[in_buf_point] = UDR;
	if(++in_buf_point>=IN_BUFFER_SIZE)
	{
		in_buf_point=0;
		in_buffer_overflow = true;
	}	
}

ISR (USART_UDRE_vect)
{
	if (++out_buf_poin >= out_buf_fill)
	{
		UCSRB &= ~(1 << UDRIE);
		out_buf_fill=0;	// uvolneni celeho bufferu
		out_buf_poin=0;	
	}
	else UDR = out_buffer[out_buf_poin];

}
*/
void do_transmission (void)
{
	//if( (UCSRA & (1<<UDRE)) && FIFO_Cnt > 0)// pokud je prazdny UDR registr a je-li co odeslat
	if( (USART0.STATUS & USART_DREIF_bm) && FIFO_Cnt > 0)// pokud je prazdny UDR registr a je-li co odeslat
	{
		USART_send_FIFO();
	}
	//if(UCSRA & (1<RXC))
	if(USART0.STATUS & USART_RXCIF_bm)
	{
		in_buffer[in_buf_point] = USART0.RXDATAL;
		if(++in_buf_point>=IN_BUFFER_SIZE)
		{
			in_buf_point=0;
			in_buffer_overflow = true;
		}
	}
}

#endif