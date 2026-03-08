/*
 * modbus_sl_conf.h
 *
 * Created: 7.11.2015 12:57:04
 *  Author: Admin
 */ 


#ifndef MODBUS_SL_CONF_H_
#define MODBUS_SL_CONF_H_


/* civky jsou ke cteni i zapisu
 * Pocet: 
 *
 */
#define nCoils             40
#define nDiscreteInputs    30
#define nInputRegisters    10
#define nHoldingRegisters  10

//__Device Identification__________________________________
	//__Device INFO Povinne_______________
	#define CompanyName        "NNM Electronic"
	#define ProductID          "EC000123"
	#define Version            "1.0.0"

	//__Device INFO NePovinne_____________
	#define URL                "http://www.nnmelectronic.cz"
	#define DeviceName         "Tag reader v.1.0.0"
	#define ModelName          "User_interface"
	#define ApplicationName    ""
//__End identification_____________________________________






#endif /* MODBUS_SL_CONF_H_ */