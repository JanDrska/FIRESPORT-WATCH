/*
 * stop_timer_var.h
 *
 * Vytvoøeno: 16.03.2024
 * Author: Ing. Jan Drška
 */ 


#ifndef STOP_TIMER_VAR_H_
#define STOP_TIMER_VAR_H_

// TARGET SYSTEM STATUS
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

#endif /* STOP_TIMER_VAR_H_ */