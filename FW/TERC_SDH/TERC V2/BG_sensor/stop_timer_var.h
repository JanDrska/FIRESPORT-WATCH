/*
 * stop_timer_var.h
 *
 * Created: 4.12.2024 22:45:14
 *  Author: Admin
 */ 


#ifndef STOP_TIMER_VAR_H_
#define STOP_TIMER_VAR_H_
/*
// ENUMY PREDELAT !!!
typedef enum
{
	initialization,
	wait_for_start_command,
	wait_for_target_filling,
	both_targets_filled,
	draining
}machine_state;

// ENUMY PREDELAT !!!
typedef enum
{
	check_suply_voltage,
	check_modbus,
	check_target_status,
	test_valves,
	test_lights,
	init_done
}machine_initialization;
*/
/*
//##############################################
// MAIN CONTROL UNIT

typedef struct
{
	machine_initialization init_state;			// prubeh inicializace stroje
	machine_state grinder_state;		// v jake fazi se program aktualne nachazi
	uint8_t process_flow;		// odhasd prubehu procesu v procentech
	uint8_t backlight;
	uint8_t hw_output;
	uint16_t speed_rpm;			// aktualni rzchlost brusneho kotouce
	uint16_t posicion_mm;		// poyice linearniho aktuatoru
	uint16_t material_removal;  // nastaveny uber materialu pri automatickem brouseni
}global_machine_status;

typedef struct
{
	uint8_t set_material_removal;
	uint8_t hw_inputs;	
	uint8_t encoder_button;		
}user_interface_inputs;
*/
//##############################################
// TARGET CONTROL UNIT

typedef enum
{
	initialization,
	wait_for_start_command,
	wait_for_target_filling,
	both_targets_filled,
	draining,
	drained
} target_status;

typedef struct
{
	uint16_t target_l_light : 1;
	uint16_t target_r_light : 1;
	uint16_t target_valves : 1;
}target_outputs;

typedef struct
{
	uint16_t target_l_full : 1;
	uint16_t target_l_empty : 1;
	uint16_t target_r_full : 1;
	uint16_t target_r_empty : 1;
	uint16_t battery_status : 1;
	uint16_t target_commands : 11;
	target_status status;
}target_inputs;

//##############################################
// USER INTERFACE CONTROL UNIT
/*
typedef enum
{
	initialization,
	wait_for_start_command,
	timecount,
	wait_for_target_filling,
	both_targets_filled,
	draining
} target_status;

typedef struct
{
	uint16_t target_l_light : 1;
	uint16_t target_r_light : 1;
	uint16_t target_valves : 1;
}target_outputs;

typedef struct
{
	uint16_t target_l_full : 1;
	uint16_t target_l_empty : 1;
	uint16_t target_r_full : 1;
	uint16_t target_r_empty : 1;
	uint16_t battery_status : 1;
	target_status sensor_status;
}target_inputs;
*/
#endif /* STOP_TIMER_VAR_H_ */