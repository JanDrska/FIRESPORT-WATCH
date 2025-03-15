/*
 * modbus.c
 *
 * Created: 13.5.2015 16:19:11
 *  Author: AutomotiForHome
 */ 

/* NUTNO DORESIT
	ve funkci pro vycenasobny zapis holding registru se neotaceji byty ve wordech 
		take nejsou doreseny chybove hlasky

*/


#include <string.h>
#include <avr/interrupt.h>

#include "ModBusDefinition.h"
#include "mb_rs485.h"
#include "slave_modbus.h"
#include "modbus_sl_conf.h"

uint8_t *Coils;				//Read and Write
uint8_t *DiscreteInputs;	//Only read
uint16_t *InputRegisters;	//Only read
uint16_t *HoldingRegisters; //Read and Write

void MB_ClearAllCounters(void);

IOTypedef io;
DiagnosticTypedef DiagnosticsStruct;

/*
ISR(TIMER1_COMPA_vect) { //Set period every 1ms
	TCNT1 = 0;
	MainLoopModBus();
}
*/

void ModBusInit(uint8_t *dis_outputs, uint8_t *dis_inputs, uint16_t *i_registers, uint16_t *h_registers)	
{	// This is function initializing mod bus
	InitModBusRTU();
	DeInitStruct(&io);
	
	//__Diagnostic counters reset______________
	MB_ClearAllCounters();
	
	Coils = dis_outputs;			 //Read and Write
	DiscreteInputs = dis_inputs;	 //Only read
	InputRegisters = i_registers;	 //Only read
	HoldingRegisters = h_registers;	 //Read and Write
	
	//__Timer setting 100us_____________________
	//OCR1A = 57;
	//TIMSK |= (1 << OCIE1A);
	//TCCR1A = 0;
	//TCNT1  = 0;
	//TCCR1B |= (1 << CS11)|(1 << CS10)|(1 << WGM12);
}

void MainLoopModBus(void) //It needs to call every 1ms
{
	MsgTypedef Msg;
	if(RectDataRTU(&Msg)) //Get receive data, When some data come
	{
		DiagnosticsStruct.SlaveMsgCnt++;
		switch(Msg.Function)
		{
			case ReadCoils:
				CoilsRead(&Msg);
				break;
			case ReadDiscreteInputs:
				DiscreteInputsRead(&Msg);
				break;
			case ReadHoldingRegisters:
				holdingRegistersRead(&Msg);
				break;
			case ReadInputsRegisters:
				InputRegistersRead(&Msg);
				break;
			case WriteSingleCoil:
				SingleCoilWrite(&Msg);
				break;
			case Diagnostics:
				DiagnosticsFx(&Msg);
				break;
			case WriteMultipleRegisters:
				write_multiple_registers(&Msg);
				break;
			default: // send error MSG with not supported function code
				Msg.Function |= 0x80;
				Msg.data[0] = IllegalFunction;
				Msg.len = 1;
				SendModBusData(&Msg);
			
		}
	}
}

void SendModBusData(MsgTypedef *Msg) //This is function is compatibility handler for send
{										//Input: Pointer on the Msg fill struct  
	if(Msg->Function >= 0x80) IncCountersModBus(RetSlaveNAKCnt); //Pocitadlo s negativni odpovedi
	SendDataRTU(Msg);
}

void LoadByte(MsgTypedef *Msg, uint8_t *pTab,uint16_t count, uint16_t offset) //This is function is load only bits
{
	if(offset != 0)
	{	
		uint8_t OffByte = offset / 8; //Cele byty
		uint8_t OffBit  = 0; //Zbyle bity
		if(offset < 8) 
		{
			OffBit = 8 - ( offset + 1 );
		}
		else 
		{
			OffBit = 8 - ((offset + 1) - (8 * OffByte ));
		}
		for(uint8_t i = 0; i < count; i++) 
			Msg->data[i + 1] = (uint8_t) ((pTab[OffByte + 1 + i] << OffBit) &  /* Vloz horni polovynu byte */
				(0xFF << OffBit)) | (pTab[OffByte + i] >> (8 - OffBit)); //Pridej dolni polovynu
	}
	else memcpy(&Msg->data[1], pTab, count);
}

void DeInitStruct(IOTypedef *IO)	// This is function is deinit pointers into the struct
{
	IO->pCoils           = NULL;
	IO->pDiscreteInputs  = NULL;
	IO->pHoldingRegister = NULL;
	IO->pInputRegistter  = NULL;
}

void CoilsRead(MsgTypedef *Msg)
{
	uint16_t StartAddress = (Msg->data[0] << 8) | Msg->data[1]; //Load start address
	uint16_t CoilsCount   = (Msg->data[2] << 8) | Msg->data[3]; //Load coils count
	//__Max coils check_______________________________________
	if(CoilsCount > 0 && CoilsCount <= 0x07D0 && CoilsCount <= nCoils)
	{
		//__Max address check_______________________________________
		if((StartAddress + CoilsCount) <= 0x07D0 && (StartAddress + CoilsCount) <= nCoils)
		{
			//__Calculation len_______________________
			Msg->len = CoilsCount / 8;
			if(CoilsCount % 8) Msg->len += 1;	
			Msg->data[0] = Msg->len;
			//__Load data____________________________
			LoadByte(Msg, Coils, Msg->len, StartAddress);
			Msg->len += 1;	
		}
		else
		{
			Msg->Function |= 0x80;
			Msg->data[0] = IllegalDataAddress;
			Msg->len = 1;		
		}
	}
	else 
	{
		Msg->Function |= 0x80;
		Msg->data[0] = IllegaklDataValue;
		Msg->len = 1;
	}
	SendModBusData(Msg);
}

void DiscreteInputsRead(MsgTypedef *Msg) // sending Inputs value from slave device
{														// Pointer on the Msg fill struct
	uint16_t StartAddress    = (Msg->data[0] << 8) | Msg->data[1]; //Load start address
	uint16_t DiscreteInCount = (Msg->data[2] << 8) | Msg->data[3]; //Load coils count

	//__Max coils check_______________________________________
	if(DiscreteInCount > 0 && DiscreteInCount <= 0x07D0 && DiscreteInCount <= nDiscreteInputs)
	{
		//__Max address check_______________________________________
		if((StartAddress + DiscreteInCount) <= 0x07D0 && (StartAddress + DiscreteInCount) <= nDiscreteInputs)
		{
			//__Calculation len_______________________
			Msg->len = DiscreteInCount / 8;
			if(DiscreteInCount % 8) Msg->len += 1;
			Msg->data[0] = Msg->len;
			//__Load data____________________________
			LoadByte(Msg, DiscreteInputs, Msg->len, StartAddress);
			Msg->len += 1;
		}
		else 
		{
			Msg->Function |= 0x80;
			Msg->data[0] = IllegalDataAddress;
			Msg->len = 1;
		}
	}
	else 
	{
		Msg->Function |= 0x80;
		Msg->data[0] = IllegaklDataValue;
		Msg->len = 1;
	}	
	SendModBusData(Msg);	
}

void holdingRegistersRead(MsgTypedef *Msg) // sending holding registers
{														  //Pointer on the Msg fill struct
	uint16_t StartAddress    = (Msg->data[0] << 8) | Msg->data[1]; //Load start address
	uint16_t RegisterCount   = (Msg->data[2] << 8) | Msg->data[3]; //Load coils count
	//__Max registers read____________________________________
	if(RegisterCount > 0 && RegisterCount < 0x7D && RegisterCount <= nHoldingRegisters)
	{
		//__Max address check________________________________________
		if((StartAddress + RegisterCount) <= 0x7D && (StartAddress + RegisterCount) <= nHoldingRegisters) 
		{	
			//__Load array______________________________________________
			for(uint8_t i = 0; i < RegisterCount; i++) {
				Msg->data[1 + ( i * 2 ) ] = ( uint8_t ) (*(HoldingRegisters + i + StartAddress) >> 8 ); //Load hi
				Msg->data[2 + ( i * 2 ) ] = ( uint8_t ) *(HoldingRegisters + i + StartAddress); //Load low
			}	
			//__Calculation len__________________________________________
			Msg->len = RegisterCount * 2;
			Msg->data[0] = Msg->len;
			Msg->len += 1;
		}
		else
		{
			Msg->Function |= 0x80;
			Msg->data[0] = IllegalDataAddress;
			Msg->len = 1;
		}
	}
	else 
	{
		Msg->Function |= 0x80;
		Msg->data[0] = IllegaklDataValue;
		Msg->len = 1;	
	}
	SendModBusData(Msg);
}

void InputRegistersRead(MsgTypedef *Msg) //sending Input registers
{														
	uint16_t StartAddress    = (Msg->data[0] << 8) | Msg->data[1]; //Load start address
	uint16_t RegisterCount   = (Msg->data[2] << 8) | Msg->data[3]; //Load reg count
	//__Max registers read____________________________________
	if(RegisterCount > 0 && RegisterCount < 0x7D && RegisterCount <= nInputRegisters) 
	{
		//__Max address check________________________________________
		if((StartAddress + RegisterCount) <= 0x7D && (StartAddress + RegisterCount) <= nInputRegisters)
		{	
			memcpy(Msg->data+1,InputRegisters,RegisterCount*2);
			//__Load array______________________________________________
			/*for(uint8_t i = 0; i < RegisterCount; i++)
			{
				Msg->data[1 + ( i * 2 ) ] = ( uint8_t ) (*(InputRegisters + i + StartAddress) >> 8 ); //Load hi
				Msg->data[2 + ( i * 2 ) ] = ( uint8_t ) *(InputRegisters + i + StartAddress); //Load low
			}*/
			//__Calculation len__________________________________________
			Msg->len = RegisterCount * 2;
			Msg->data[0] = Msg->len;
			Msg->len += 1;
		}
		else 
		{
			Msg->Function |= 0x80;
			Msg->data[0] = IllegalDataAddress;
			Msg->len = 1;
		}
	}
	else
	{
		Msg->Function |= 0x80;
		Msg->data[0] = IllegaklDataValue;
		Msg->len = 1;
	}
	SendModBusData(Msg);
}

void SingleCoilWrite(MsgTypedef *Msg)
{
	uint16_t Address  = (Msg->data[0] << 8) | Msg->data[1]; //Load start address
	uint16_t OutState = (Msg->data[2] << 8) | Msg->data[3]; //Load coils count
	
	//__Max coils check_______________________________________
	if(OutState == 0x0000 || OutState == 0xFF00)
	{
		//__Max address check_______________________________________
		if( Address <= 0x07D0 && Address <= nCoils)
		{
			uint16_t i = Address / 8;
			uint8_t ii = Address - (i * 8);
			if(Address % 8) Address += 1;
			if(OutState) *(Coils+i) |= (1 << ii);
			else *(Coils+i) &= ~(1 << ii);	
		}
		else
		{
			Msg->Function |= 0x80;
			Msg->data[0] = IllegalDataAddress;
			Msg->len = 1;
		}
	}
	else
	{
		Msg->Function |= 0x80;
		Msg->data[0] = IllegaklDataValue;
		Msg->len = 1;
	}
	SendModBusData(Msg);
}

void DiagnosticsFx(MsgTypedef *Msg) // This is function keep diagnostics informations
{
	switch(Msg->data[1])
	{
	case ReturnQueryData:
		break;
	case RetBusMsgCnt:
		Msg->data[2] = (uint8_t) (DiagnosticsStruct.MsgCnt >> 8);
		Msg->data[3] = (uint8_t) DiagnosticsStruct.MsgCnt;
		break;
	case RetBusCommErrCnt:
		Msg->data[2] = (uint8_t) (DiagnosticsStruct.CommErrCnt >> 8);
		Msg->data[3] = (uint8_t) DiagnosticsStruct.CommErrCnt;
		break;
	case RetBusExeptionErrCnt:
		Msg->data[2] = (uint8_t) (DiagnosticsStruct.ExceptionCnt >> 8);
		Msg->data[3] = (uint8_t) DiagnosticsStruct.ExceptionCnt;
		break;
	case RetSlaveMsgCnt:
		Msg->data[2] = (uint8_t) (DiagnosticsStruct.SlaveMsgCnt >> 8);
		Msg->data[3] = (uint8_t) DiagnosticsStruct.SlaveMsgCnt;
		break;
	case RetSlaveNoResponceCnt:
		Msg->data[2] = (uint8_t) (DiagnosticsStruct.SlaveNoRespovceCnt >> 8);
		Msg->data[3] = (uint8_t) DiagnosticsStruct.SlaveNoRespovceCnt;
		break;
	case RetSlaveNAKCnt:
		Msg->data[2] = (uint8_t) (DiagnosticsStruct.SlaveNAKCnt >> 8);
		Msg->data[3] = (uint8_t) DiagnosticsStruct.SlaveNAKCnt;
		break;
	case RetOverrunErrCnt:
		Msg->data[2] = (uint8_t) (DiagnosticsStruct.CharacterOverrunCnt >> 8);
		Msg->data[3] = (uint8_t) DiagnosticsStruct.CharacterOverrunCnt;
		break;
	case  CLRControlsAndDiagRegister:
		MB_ClearAllCounters();
		break;
	default:
		Msg->Function |= 0x80;
		Msg->data[0] = IllegalFunction;
		Msg->len = 1;
	}
	SendModBusData(Msg);
}

void write_multiple_registers(MsgTypedef *Msg)
{
	uint16_t reg_address  = (Msg->data[0] << 8) | Msg->data[1]; //Load start address
	uint16_t num_reg = (Msg->data[2] << 8) | Msg->data[3]; //Load reg count
	
	if((num_reg <= 120) && ((reg_address + num_reg) <= HOLDING_REG_SIZE)) //test, aby se nezapisovalo kam nema
	{
		memcpy(HoldingRegisters+reg_address,Msg->data+4,num_reg*2);
	}
	else 
	{
		Msg->Function |= 0x80;
		Msg->data[0] = IllegaklDataValue;
		Msg->len = 1;
	}
	Msg->len = 4;	// v odpovedi se neposilaji nazpet data, jenom hlavicka
	SendModBusData(Msg);
}

void IncCountersModBus(DiagnosticCodeTable OnlyCNT) //This is function increment cnt value 
{
	switch(OnlyCNT) {
		case RetBusMsgCnt:
			DiagnosticsStruct.MsgCnt++;
		break;
		case RetBusCommErrCnt:
			DiagnosticsStruct.CommErrCnt++;
		break;
		case RetBusExeptionErrCnt:
			DiagnosticsStruct.ExceptionCnt++;
		break;
		case RetSlaveMsgCnt:
			DiagnosticsStruct.SlaveMsgCnt++;
		break;
		case RetSlaveNoResponceCnt:
			DiagnosticsStruct.SlaveNoRespovceCnt++;
		break;
		case RetSlaveNAKCnt: 
			DiagnosticsStruct.SlaveNAKCnt++;
		break;
		case RetSlaveBusyCnt:
			DiagnosticsStruct.SlaveBusyCnt++;
		break;
		case RetBusCharOverrunErrCnt:
			DiagnosticsStruct.CharacterOverrunCnt++;
		break;
		default:
		break;
	}
}

void MB_ClearAllCounters(void) //This is function clear selected cnt 
{
	//__Diagnostic counters reset______________
	DiagnosticsStruct.CharacterOverrunCnt = 0;
	DiagnosticsStruct.CommErrCnt          = 0;
	DiagnosticsStruct.ExceptionCnt        = 0;
	DiagnosticsStruct.MsgCnt              = 0;
	DiagnosticsStruct.SlaveBusyCnt        = 0;
	DiagnosticsStruct.SlaveMsgCnt         = 0;
	DiagnosticsStruct.SlaveNAKCnt         = 0;
	DiagnosticsStruct.SlaveNoRespovceCnt  = 0;
}

void SETVAL(uint16_t val) {
	DiagnosticsStruct.SlaveNoRespovceCnt = val;
}


