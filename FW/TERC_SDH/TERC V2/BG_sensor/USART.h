/*
 * Serial.h
 *
 * Created: 25.7.2014 18:07:51
 *  Author: AutomotiForHome
 */ 
#ifndef MODBUS_PROTOCOL 

#ifndef SERIAL_H_
#define SERIAL_H_
#include <stdbool.h>
#include <stdint.h>

/* Structure */
typedef enum 
	{
	added, 
	buffer_full, 
	buffer_empty, 
	incorect_data_len, 
	received
	}
	USART_status;


void Init_Serial_Port(void);

void serial_port_interupt (bool enabled);

USART_status USART_get_in_buffer(uint8_t *data_len, char *data);

USART_status USART_send_string(char *string);

USART_status USART_send_data(char *data, uint8_t data_len);

USART_status USART_send_char(char dato);

USART_status USART_Write_Data_In_To_Buffer(char *data, uint8_t data_len);

USART_status USART_send_FIFO(void);

void do_transmission (void);

#endif /* SERIAL_H_ */

#endif