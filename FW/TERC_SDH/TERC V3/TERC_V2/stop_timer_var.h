/*
 * stop_timer_var.h
 *
 * Vytvoøeno: 16.03.2024
 * Author: Ing. Jan Drška
 */ 


#ifndef STOP_TIMER_VAR_H_
#define STOP_TIMER_VAR_H_

// OUTPUTS, COMMANDS
typedef struct
{
	uint16_t Output_0: 1;		// OUT 1
	uint16_t Output_0_BL: 1;	// OUT 1 blikani
	uint16_t Output_1 : 1;		
	uint16_t Output_1_BL: 1;	
	uint16_t Output_2: 1;		
	uint16_t Output_2_BL: 1;	
	uint16_t Output_3: 1;
	uint16_t Output_3_BL: 1;
	uint16_t Output_4: 1;
	uint16_t Output_4_BL: 1;		
	uint16_t target_commands : 6;
	uint16_t blink_period;
}target_outputs;

// INPUTS, STATUSES
typedef struct
{
	uint16_t Input_0 : 1;
	uint16_t Input_1 : 1;
	uint16_t Input_2 : 1;
	uint16_t Input_3 : 1;
	uint16_t Input_4 : 1;
	uint16_t status : 11;
	uint16_t vbatt;
	uint16_t cmd;

}target_inputs;

#endif /* STOP_TIMER_VAR_H_ */