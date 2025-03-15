/*
 * UART.h
 *
 * Created: 16.5.2015 11:51:57
 *  Author: AutomotiForHome
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>

//__Definitions___________________________________________________________
#define BAUDRATE          76800
/* Delka pomlky mezi ramci je min 3.5 znaku, 1znak ma 11bitu
	1 - start bit, 8 datových bitù, 1 bit parita (suda), 1 stop bit
	udaj v us 1000000 * 11 bitu * 3.5 = 38500000 / BAUDRATE
	pro 115200 = 334us
	pro 76800 = 501us
*/
#define SLAVE_ADDRESS       2        // 1 - 255 | 0 = broadcast
#define RX_BUFFER_SIZE      128
#define TX_BUFFER_SIZE      128


//__Enums_____________________________________________
typedef enum {
	Success,
	CRCError,
	SlaveAddressErr
}RetTypedef;

//__Structures________________________________________
typedef struct {
	uint8_t SlaveID;
	uint8_t Function;
	uint16_t len;
	uint8_t data[256];
}MsgTypedef;

typedef struct {
	uint8_t *pCoils;
	uint8_t *pDiscreteInputs;
	uint16_t *pInputRegistter;
	uint16_t *pHoldingRegister;
}IOTypedef;


//__Functions________________________________________

void InitModBusRTU(void);
void mb_rs485_interupt (bool enabled);
bool SendDataRTU(MsgTypedef *Frame);
bool RectDataRTU(MsgTypedef *Msg);
void mb_do_transmission (void);
void datatest (void);
#endif /* UART_H_ */