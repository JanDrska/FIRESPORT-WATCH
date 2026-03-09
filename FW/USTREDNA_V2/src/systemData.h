#ifndef SYSTEMDATA_H
#define SYSTEMDATA_H

#include <Arduino.h>

// ======================================================
// MODBUS A HW KONSTANTY
// ======================================================

#define MODBUS_BUAD 115200
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_UNIT_ID 1
#define MODBUS_SLAVE_UNIT_ID 2

#define DE_RE 2

// ======================================================
// HLAVNI STAVY SYSTEMU
// ======================================================

typedef enum
{
    SYS_BOOT,
    SYS_DETECT_DEVICES,
    SYS_INIT_REQUIRED,
    SYS_INIT_OPTIONAL,
    SYS_VALIDATE_CONFIG,
    SYS_STANDBY,
    SYS_PROGRAM_SELECT,
    SYS_PROGRAM_PREPARE,
    SYS_PROGRAM_RUN,
    SYS_PROGRAM_FINISH,
    SYS_ERROR
} system_status;

// ======================================================
// KONFIGURACE SYSTEMU
// ======================================================

typedef struct
{
    bool display_present;
    bool hmi_present;
    bool target_present;

    bool display_required_ok;
    bool hmi_required_ok;
    bool target_ready;

    bool config_valid;
} system_config_t;

// ======================================================
// POVOLENE PROGRAMY PODLE HW
// ======================================================

typedef struct
{
    bool allow_sdh_timer;
    bool allow_countdown;
    bool allow_scoreboard;
    bool allow_timer;
    bool allow_clock;
    bool allow_temp_humid;
    bool allow_temp_humid_clock;
} program_availability_t;

// ======================================================
// PROGRAMY
// ======================================================

typedef enum
{
    prg_sdh_timer,
    prg_countdown,
    prg_scoreboard,
    prg_timer,
    prg_clock,
    prg_temperature_humidity,
    prg_temperature_humidity_clock
} program;

// ======================================================
// STAVY TERCE
// ======================================================

typedef enum
{
    target_init,
    target_wait_for_start,
    target_wait_for_target_filling,
    target_both_targets_filled,
    target_draining,
    target_drained,
    target_comm_error,
    target_vbat_error
} target_status;

// ======================================================
// STAVY HMI
// ======================================================

typedef enum
{
    hmi_init,
    hmi_ready,
    hmi_wait_for_start,
    hmi_watch_is_running,
    hmi_watch_stopped,
    hmi_countdown_active,
    hmi_scoreboard_active,
    hmi_error
} hmi_status;

// ======================================================
// CHYBOVE KODY
// ======================================================

typedef enum
{
    err_none = 0,
    target_init_failed,
    hmi_init_failed,
    target_not_response,
    hmi_not_response,
    display_init_failed,
    invalid_hw_configuration
} error_code;

// ======================================================
// PRIKAZY Z HMI
// ======================================================

typedef enum
{
    cmd_none = 0,
    cmd_start,
    cmd_stop,
    cmd_pause,
    cmd_reset
} hmi_cmd;

// ======================================================
// VYSTUPY / PRIKAZY
// ======================================================

typedef struct
{
    uint16_t horn : 1;
    uint16_t system_light : 1;
    uint16_t target_commands : 14;
    uint16_t errorCode : 14;
} display_outputs;

typedef struct
{
	uint16_t target_l_light: 1;		// OUT 1
	uint16_t target_l_light_blink: 1;	// OUT 1 blikani

    uint16_t reserva: 1;
	uint16_t reserva_blink: 1;

	uint16_t target_valves: 1;		
	uint16_t target_valves_blink: 1;	

	uint16_t target_r_light : 1;		
	uint16_t target_r_light_blink: 1;

	uint16_t relay: 1;
	uint16_t relay_blink: 1;		
	uint16_t target_commands : 6;
    uint16_t blink_period;
}target_outputs;

typedef struct
{
    uint16_t start_light : 1;
    uint16_t horn : 1;
    uint16_t status_light : 1;
    uint16_t system_status : 13;
} hmi_outputs;

// ======================================================
// VSTUPY / STAVY
// ======================================================

typedef struct
{
    system_status status;
} display_inputs;

typedef struct
{
	uint16_t target_l_full : 1;
	uint16_t target_l_empty : 1;
	uint16_t target_r_full : 1;
	uint16_t target_r_empty : 1;
    uint16_t reserva : 1;
	uint16_t status : 11;
	uint16_t vbatt;
	uint16_t cmd;

}target_inputs;

typedef struct
{
    uint16_t start_sensor : 1;
    uint16_t sensor_enable : 1;
    uint16_t start_btn : 1;
    uint16_t stop_btn : 1;
    uint16_t hmi_commands;
} hmi_inputs;

#endif