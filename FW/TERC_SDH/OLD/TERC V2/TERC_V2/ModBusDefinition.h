/*
 * ModBusDefinition.h
 *
 * Created: 13.5.2015 16:37:03
 *  Author: AutomotiForHome
 */ 


#ifndef MODBUSDEFINITION_H_
#define MODBUSDEFINITION_H_

#include <stdint.h>

typedef enum                //Command code table
{	
	ReadCoils = 0x01,
	ReadDiscreteInputs,
	ReadHoldingRegisters,
	ReadInputsRegisters,
	WriteSingleCoil,
	WriteSingleRegister,
	ReadExceptionStatus,
	Diagnostics,
	/* FREE */
	GetCommEventCounter = 0x0B,
	GetCommEventLog,
	WriteMultipleCoils = 0x0F,
	WriteMultipleRegisters,
	ReportSlaveID,
	ReadFileRecord = 0x14,
	WriteFileRecord,
	MaskWriteRegister,
	RWMultipleRegister,
	ReadFIFO,
	ReadDeviceID = 0x0E /* 0x0E and 0x2B  */
}CMDCodeTab;

typedef enum                 //Error Code table
{
	IllegalFunction = 0x01,
	IllegalDataAddress,
	IllegaklDataValue,
	SlaveDeviceFailure,
	ACKNOWLEDGE,
	SlaveDeviceBussy,
	MemoryParityError = 0x08,
	GatwayPathUnawailable = 0x0A,
	GatewayTargetDeviceFailedToRespond
}ErrorCodeTab;

typedef enum 
{
	RectDataOk,
	TimeOut,
	ERROR,
	CRC,
	SlAddr,
	Waiting,
	NoData
}ReturnTab;

	
typedef enum         //Diagnostic
{
	ReturnQueryData,
	RestartCommOption,
	ReturnDiagnosticRegister,
	ChangeASCIIInputDelimiter,
	FoceListenOnlyMode,
	CLRControlsAndDiagRegister = 10,
	RetBusMsgCnt,
	RetBusCommErrCnt,
	RetBusExeptionErrCnt,
	RetSlaveMsgCnt,
	RetSlaveNoResponceCnt,
	RetSlaveNAKCnt,
	RetSlaveBusyCnt,
	RetBusCharOverrunErrCnt,
	RetOverrunErrCnt,
	ClrOverrunCntAndFlag,
	GetClrModbusPlusStatiscs
}DiagnosticCodeTable;


typedef struct 
{
	uint16_t MsgCnt; //All incoming message //OK
	uint16_t CommErrCnt; //CRC error //OK
	uint16_t ExceptionCnt; //Not used
	uint16_t SlaveMsgCnt; //Slave message counter //OK
	uint16_t SlaveNoRespovceCnt; //Message without return answer //OK
	uint16_t SlaveNAKCnt; //Negative answer //OK
	uint16_t SlaveBusyCnt; //Not used
	uint16_t CharacterOverrunCnt; //Not used
}DiagnosticTypedef;
	
#endif /* MODBUSDEFINITION_H_ */