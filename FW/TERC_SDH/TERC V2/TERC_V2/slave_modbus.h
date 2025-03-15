/*
 * modbus.h
 *
 * Created: 13.5.2015 16:19:26
 *  Author: AutomotiForHome
 */ 


#ifndef MODBUS_H_
#define MODBUS_H_

//__Includes__________________________________
#include <stdbool.h>
#include <stdint.h>
#include "ModBusDefinition.h"
#include "mb_rs485.h"

#define HOLDING_REG_SIZE	32 

//__Functions_____________________________________
void ModBusInit(uint8_t *dis_outputs, uint8_t *dis_inputs, uint16_t *i_egisters, uint16_t *h_registers);	
void ModBusSend(void);

void MainLoopModBus(void);
void SendModBusData(MsgTypedef *Msg);
void LoadByte(MsgTypedef *Msg, uint8_t *pTab,uint16_t count, uint16_t offset);

void DeInitStruct(IOTypedef *IO);

void CoilsRead(MsgTypedef *Msg);
void DiscreteInputsRead(MsgTypedef *Msg);
void holdingRegistersRead(MsgTypedef *Msg);
void InputRegistersRead(MsgTypedef *Msg);
void SingleCoilWrite(MsgTypedef *Msg);

void write_multiple_registers(MsgTypedef *Msg);

//__Diagnostics FX and Sub FX_________________________
void DiagnosticsFx(MsgTypedef *Msg);
void IncCountersModBus(DiagnosticCodeTable OnlyCNT);
void SETVAL(uint16_t val);
#endif /* MODBUS_H_ */

