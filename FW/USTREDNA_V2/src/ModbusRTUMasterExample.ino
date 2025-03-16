
#include <ModbusRTUMaster.h>

#define MODBUS_SERIAL Serial
#define MODBUS_BUAD 115200
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_UNIT_ID 1
#define MODBUS_SLAVE_UNIT_ID 2

#define DE_RE 2

ModbusRTUMaster modbus(MODBUS_SERIAL, DE_RE);

unsigned long lastTime = 0; 

typedef enum
{
	initialization,
	wait_for_start_command,
	wait_for_target_filling,
	both_targets_filled,
	draining,
	drained,
	comm_error,
	vbat_error
} target_status;

// OUTPUTS, COMMANDS
typedef struct
{
	uint16_t target_l_light : 1;
	uint16_t target_r_light : 1;
	uint16_t target_valves : 1;
	uint16_t target_commands : 13;
}target_outputs;

// INPUTS, STATUSES
typedef struct
{
	uint16_t target_l_full : 1;
	uint16_t target_l_empty : 1;
	uint16_t target_r_full : 1;
	uint16_t target_r_empty : 1;
	uint16_t battery_status : 1;
	target_status status;
}target_inputs;

uint16_t target_ireg[sizeof(target_inputs)/2 + ((sizeof(target_inputs)%2)*2)];    // nastaveni pole pro hodnoty
uint16_t target_reg[sizeof(target_outputs)/2 + ((sizeof(target_outputs)%2)*2)];

target_inputs *t_inputs;    // definice ukazatelu na strukturu t_inputs
target_outputs *t_outputs;

void setup() 
{
  pinMode(DE_RE, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(A0, INPUT);

  MODBUS_SERIAL.begin(MODBUS_BUAD, MODBUS_CONFIG);
  modbus.begin(MODBUS_BUAD, MODBUS_CONFIG);
}

void loop() 
{
  t_inputs = (target_inputs*) target_ireg;  // prirazeni ukazatelu na zacatek pole target_ireg
  t_outputs = (target_outputs*) target_reg;

  while(1)
  {
     static uint8_t distributor = 0;
    uint8_t error;

    if((millis()-lastTime)>9)
    {
      switch (++distributor)
      {
      case 1:
        error = modbus.writeMultipleHoldingRegisters(MODBUS_SLAVE_UNIT_ID, 0, target_reg, (sizeof(target_reg)/2));
        break;
      case 2:
        error = modbus.readInputRegisters(MODBUS_SLAVE_UNIT_ID, 0, target_ireg, (sizeof(target_ireg)/2));
        for (uint8_t i = 0; i < (sizeof(target_ireg)/2); i++) 
          target_ireg[i] = swapBytes(target_ireg[i]);
        break;
      case 3:
        distributor =0;
        break;
      }
  
    if(t_inputs->target_l_empty)
      digitalWrite(4, true);
    else
    digitalWrite(4, false);
  
    if(t_inputs->target_l_full)
      digitalWrite(5, true);
    else
    digitalWrite(5, false);

    if(t_inputs->target_r_empty)
      digitalWrite(6, true);
    else
    digitalWrite(6, false);

    if(t_inputs->target_r_full)
      digitalWrite(7, true);
    else
    digitalWrite(7, false);
    
      lastTime = millis();
    }
  }
}

void printTargetInputs(target_inputs *inputs) 
{
    Serial.println("=== target_inputs ===");
    Serial.print(" target_l_full: "); Serial.println(inputs->target_l_full);
    Serial.print(" target_l_empty: "); Serial.println(inputs->target_l_empty);
    Serial.print(" target_r_full: "); Serial.println(inputs->target_r_full);
    Serial.print(" target_r_empty: "); Serial.println(inputs->target_r_empty);
    Serial.print(" battery_status: "); Serial.println(inputs->battery_status);
    Serial.print(" status: "); Serial.println(inputs->status);  // Vypíše číslo odpovídající hodnotě enumu
    Serial.println("=====================");
}

uint16_t swapBytes(uint16_t value) 
{
    return (value >> 8) | (value << 8);
}



