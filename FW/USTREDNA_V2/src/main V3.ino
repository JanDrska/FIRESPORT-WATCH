#include <ModbusRTUMaster.h>
#include "sdhTimer.h"
#include "sdhDisplay.h"
#include <PCF8574.h>
#include "systemData.h"

#define MODBUS_SERIAL Serial
#define MODBUS_BUAD 115200
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_UNIT_ID 1
#define MODBUS_SLAVE_UNIT_ID 2

#define DE_RE 2

ModbusRTUMaster modbus(MODBUS_SERIAL, DE_RE);

TimerData timerL;
TimerData timerR;
DisplaySdh display;

unsigned long lastTime = 0;


// ======================================================
// POLE MODBUS REGISTRU
// ======================================================

uint16_t system_ireg[sizeof(display_inputs)/2 + ((sizeof(display_inputs)%2)*2)];
uint16_t system_reg[sizeof(display_outputs)/2 + ((sizeof(display_outputs)%2)*2)];

uint16_t target_ireg[sizeof(target_inputs)/2 + ((sizeof(target_inputs)%2)*2)];
uint16_t target_reg[sizeof(target_outputs)/2 + ((sizeof(target_outputs)%2)*2)];

uint16_t hmi_ireg[sizeof(hmi_inputs)/2 + ((sizeof(hmi_inputs)%2)*2)];
uint16_t hmi_reg[sizeof(hmi_outputs)/2 + ((sizeof(hmi_outputs)%2)*2)];


// ======================================================
// UKAZATELE NA STRUKTURY NAD REGISTRY
// ======================================================

display_inputs *d_inputs;
display_outputs *d_outputs;

target_inputs *t_inputs;
target_outputs *t_outputs;

hmi_inputs *h_inputs;
hmi_outputs *h_outputs;


// ======================================================
// GLOBALNI PROMENNE
// ======================================================

bool startCmd = false;
bool stopCmd = false;
bool resetCmd = false;

error_code error = err_none;
uint8_t errorNumber = 0;

system_config_t system_config;
program_availability_t program_availability;
program selected_program = prg_countdown;


// ======================================================
// PREDDEKLARACE FUNKCI
// ======================================================

uint16_t swapBytes(uint16_t value);

void clearSystemConfig();
void updateProgramAvailability();
bool isProgramAllowed(program prg);

bool detectTargetDevice();

bool initDisplayDevice();
bool initRequiredDevices();
bool initOptionalDevices();

void processModbus();

void start();
void sdhTimer();
void countdown();
void scoreboard();
void timer();
void clock();
void temp_humid();
void temp_humid_clock();


// ======================================================
// SETUP
// ======================================================

void setup()
{
    pinMode(DE_RE, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(13, OUTPUT);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);

    MODBUS_SERIAL.begin(MODBUS_BUAD, MODBUS_CONFIG);
    modbus.begin(MODBUS_BUAD, MODBUS_CONFIG);

    t_inputs = (target_inputs*) target_ireg;
    t_outputs = (target_outputs*) target_reg;

    h_inputs = (hmi_inputs*) hmi_ireg;
    h_outputs = (hmi_outputs*) hmi_reg;

    d_inputs = (display_inputs*) system_ireg;
    d_outputs = (display_outputs*) system_reg;

    clearSystemConfig();
    updateProgramAvailability();
}


// ======================================================
// HLAVNI SMYCKA
// ======================================================

void loop()
{
    d_inputs->status = SYS_BOOT;

    while(1)
    {

        //Serial.print("status - "); 
        //Serial.println(d_inputs->status); 

        switch (d_inputs->status)
        {

            case SYS_BOOT:
            {
                error = err_none;
                errorNumber = 0;

                clearSystemConfig();
                updateProgramAvailability();

                memset(system_reg, 0, sizeof(system_reg));
                memset(target_reg, 0, sizeof(target_reg));
                memset(hmi_reg, 0, sizeof(hmi_reg));

                d_inputs->status = SYS_DETECT_DEVICES;
                break;
            }


            case SYS_DETECT_DEVICES:
            {
                system_config.display_present = true;

                // HMI zatim neresime
                system_config.hmi_present = false;
                system_config.hmi_required_ok = true;

                // detekce tercu
                system_config.target_present = detectTargetDevice();

                d_inputs->status = SYS_INIT_REQUIRED;
                break;
            }


            case SYS_INIT_REQUIRED:
            {
                
                if (!initRequiredDevices())
                {
                    d_inputs->status = SYS_ERROR;
                }
                    
                else
                    d_inputs->status = SYS_INIT_OPTIONAL;
            
                break;
            }


            case SYS_INIT_OPTIONAL:
            {
                initOptionalDevices();
                updateProgramAvailability();

                d_inputs->status = SYS_VALIDATE_CONFIG;
                break;
            }


            case SYS_VALIDATE_CONFIG:
            {
                if (system_config.display_required_ok)
                {
                    system_config.config_valid = true;
                    d_inputs->status = SYS_STANDBY;
                }
                else
                {
                    error = invalid_hw_configuration;
                    d_inputs->status = SYS_ERROR;
                }
                t_outputs->target_valves = true;
                break;
            }


            case SYS_STANDBY:
            {
                d_inputs->status = SYS_PROGRAM_SELECT;
                break;
            }


            case SYS_PROGRAM_SELECT:
            {

                /*
                // vyber programu pres vstup A1
                if (digitalRead(A1) == LOW)
                    selected_program = prg_sdh_timer;
                else
                    selected_program = prg_countdown;

                if (isProgramAllowed(selected_program))
                    d_inputs->status = SYS_PROGRAM_PREPARE;
                else
                    d_inputs->status = SYS_STANDBY;

                
                */
                    selected_program = prg_sdh_timer;
                    d_inputs->status = SYS_PROGRAM_PREPARE;
                break;
            }


            case SYS_PROGRAM_PREPARE:
            {
                timerL.init();
                timerR.init();
                //display.init();

                if ((selected_program == prg_sdh_timer) && (!system_config.target_ready))
                {
                    error = target_not_response;
                    d_inputs->status = SYS_ERROR;
                }
                else
                {
                    d_inputs->status = SYS_PROGRAM_RUN;
                }

                break;
            }


            case SYS_PROGRAM_RUN:
            {
                switch (selected_program)
                {
                    case prg_sdh_timer: sdhTimer(); break;
                    case prg_countdown: countdown(); break;
                    case prg_scoreboard: scoreboard(); break;
                    case prg_timer: timer(); break;
                    case prg_clock: clock(); break;
                    case prg_temperature_humidity: temp_humid(); break;
                    case prg_temperature_humidity_clock: temp_humid_clock(); break;
                    default: d_inputs->status = SYS_STANDBY; break;
                }

                break;
            }


            case SYS_PROGRAM_FINISH:
            {
                timerL.stopTimming();
                timerR.stopTimming();

                d_inputs->status = SYS_STANDBY;
                break;
            }


            case SYS_ERROR:
            {
                d_outputs->errorCode = error;

                timerL.stopTimming();
                timerR.stopTimming();

                t_outputs->target_valves = 0;
                t_outputs->target_l_light = 0;
                t_outputs->target_r_light = 0;

                //Serial.println("SYS ERROR"); 

                break;
            }


            default:
            {
                error = invalid_hw_configuration;
                d_inputs->status = SYS_ERROR;
                break;
            }

        }

        processModbus();
    }
}


// ======================================================
// MODBUS
// ======================================================

void processModbus()
{
    static uint8_t distributor = 0;
    uint8_t comm_error = 0;

    if ((millis() - lastTime) > 9)
    {
        switch (++distributor)
        {

            case 1:
                comm_error = modbus.writeMultipleHoldingRegisters(
                    MODBUS_SLAVE_UNIT_ID,
                    0,
                    target_reg,
                    (sizeof(target_reg)/2));
                break;

            case 2:
                comm_error = modbus.readInputRegisters(
                    MODBUS_SLAVE_UNIT_ID,
                    0,
                    target_ireg,
                    (sizeof(target_ireg)/2));

                for (uint8_t i = 0; i < (sizeof(target_ireg)/2); i++)
                    target_ireg[i] = swapBytes(target_ireg[i]);
                break;

            case 3:
                distributor = 0;
                break;
        }

        (void)comm_error;
        lastTime = millis();

        //Serial.println("MODBUS");
    }
}

// ======================================================
// POMOCNE FUNKCE PRO KONFIGURACI
// ======================================================

void clearSystemConfig()
{
    system_config.display_present = false;
    system_config.hmi_present = false;
    system_config.target_present = false;

    system_config.display_required_ok = false;
    system_config.hmi_required_ok = false;
    system_config.target_ready = false;

    system_config.config_valid = false;
}

void updateProgramAvailability()
{
    // stopky vyzaduji funkcni terce
    program_availability.allow_sdh_timer = system_config.target_ready;

    // odpocet muze bezet i bez terci
    program_availability.allow_countdown = true;

    // ostatni programy zatim nepouzivame
    program_availability.allow_scoreboard = false;
    program_availability.allow_timer = false;
    program_availability.allow_clock = false;
    program_availability.allow_temp_humid = false;
    program_availability.allow_temp_humid_clock = false;
}

bool isProgramAllowed(program prg)
{
    switch (prg)
    {
        case prg_sdh_timer:
            return program_availability.allow_sdh_timer;

        case prg_countdown:
            return program_availability.allow_countdown;

        case prg_scoreboard:
            return program_availability.allow_scoreboard;

        case prg_timer:
            return program_availability.allow_timer;

        case prg_clock:
            return program_availability.allow_clock;

        case prg_temperature_humidity:
            return program_availability.allow_temp_humid;

        case prg_temperature_humidity_clock:
            return program_availability.allow_temp_humid_clock;

        default:
            return false;
    }
}

// ======================================================
// DETEKCE A INICIALIZACE ZARIZENI
// ======================================================

bool detectTargetDevice()
{
    // jednoducha detekce terce podle smysluplneho stavu v registrech
    switch (t_inputs->status)
    {
        case target_init:
        case target_wait_for_start:
        case target_wait_for_target_filling:
        case target_both_targets_filled:
        case target_draining:
        case target_drained:
        case target_comm_error:
        case target_vbat_error:
            return true;

        default:
            return false;
    }
}

bool initDisplayDevice()
{
    // inicializace lokalniho displeje
    //display.init();
    return true;
}

bool initRequiredDevices()
{
    // zatim je povinny pouze displej
    system_config.display_required_ok = initDisplayDevice();
    if (!system_config.display_required_ok)
    {
        error = display_init_failed;
        return false;
    }

    // HMI zatim neresime
    system_config.hmi_required_ok = true;

    return true;
}

bool initOptionalDevices()
{
    // pokud terc neni pritomen, system jede dal bez nej
    if (!system_config.target_present)
    {
        system_config.target_ready = false;
        return true;
    }

    // pokud terc pritomen je a je pripraven, muzeme ho povazovat za ready
    if (t_inputs->status == target_wait_for_start)
    {
        system_config.target_ready = true;
        return true;
    }

    // pokud terc jeste neni pripraven, posleme init prikaz
    t_outputs->target_commands = 0x01;

    if (t_inputs->status == target_wait_for_start)
    {
        system_config.target_ready = true;
        t_outputs->target_commands = 0;
        return true;
    }

    // terc je pritomen, ale zatim neni pripraven
    system_config.target_ready = false;
    return false;
}

// ======================================================
// POMOCNE FUNKCE
// ======================================================

uint16_t swapBytes(uint16_t value)
{
    return (value >> 8) | (value << 8);
}

void start()
{
    timerL.startTimming();
    timerR.startTimming();
}

// ======================================================
// PROGRAM SDH CASOMIRA
// ======================================================

void sdhTimer()
{
    static uint8_t step = 0;

    switch (step)
    {
        case 0: // cekani na povoleni startu - odblokovani zavory
            if (h_inputs->sensor_enable != 1)
                step = 1;
            break;

        case 1: // zavora odblokovana, cekame na start
            if ((h_inputs->sensor_enable == 1) && (h_inputs->start_sensor == 1))
                step = 2;
            break;

        case 2: // start mereni
            start();
            step = 3;
            break;

        case 3:
            // prubezny cas leveho terce
            if (!t_outputs->target_l_light)
            {
                timerL.Time();
                display.sendData(timerL, timerR);
            }

            // prubezny cas praveho terce
            if (!t_outputs->target_r_light)
            {
                timerR.Time();
                display.sendData(timerL, timerR);
            }

            // konec leveho terce
            if (t_outputs->target_l_light)
            {
                timerL.stopTimming();
                display.sendData(timerL, timerR);
            }

            // konec praveho terce
            if (t_outputs->target_r_light)
            {
                timerR.stopTimming();
                display.sendData(timerL, timerR);
            }

            // docasne stop pres vstup stop_btn
            if (h_inputs->stop_btn)
            {
                timerL.stopTimming();
                timerR.stopTimming();
                step = 0;
                d_inputs->status = SYS_PROGRAM_FINISH;
            }

            break;

        default:
            step = 0;
            d_inputs->status = SYS_PROGRAM_FINISH;
            break;
    }
}

// ======================================================
// PROGRAM ODPOCET
// ======================================================

void countdown()
{
    static uint8_t step = 0;

    switch (step)
    {
        case 0: // cekani na povoleni startu odpocitu
            if (h_inputs->sensor_enable)
                step = 1;
            break;

        case 1: // start mereni
            start();
            step = 2;
            break;

        case 2:
            if (timerL.casSTART > 1) // bylo odstartovano
            {
                timerL.Time();
                display.sendData(timerL, timerL);
            }

            // konec odpocitu
            if ((timerL.casTERC_M == 0) && (timerL.casTERC_S == 0))
            {
                timerL.stopTimming();
                timerR.stopTimming();

                // TODO: sirena, blikani svetla / displeje
                display.sendData(timerL, timerR);

                step = 0;
                d_inputs->status = SYS_PROGRAM_FINISH;
            }

            // docasne stop pres vstup stop_btn
            if (h_inputs->stop_btn)
            {
                timerL.stopTimming();
                timerR.stopTimming();
                step = 0;
                d_inputs->status = SYS_PROGRAM_FINISH;
            }
            break;

        default:
            step = 0;
            d_inputs->status = SYS_PROGRAM_FINISH;
            break;
    }
}

// ======================================================
// OSTATNI PROGRAMY - ZATIM POUZE KOSTRA
// ======================================================

void scoreboard()
{
}

void timer()
{
}

void clock()
{
}

void temp_humid()
{
}

void temp_humid_clock()
{
}