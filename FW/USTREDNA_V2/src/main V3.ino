#include <ModbusRTUMaster.h>
#include "sdhTimer.h"
#include "sdhDisplay.h"
#include <PCF8574.h>
#include "systemData.h"

#define MODBUS_SERIAL Serial
#define MODBUS_BUAD 115200
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_UNIT_ID 1
#define MODBUS_SLAVE1_UNIT_ID 2
#define MODBUS_SLAVE2_UNIT_ID 3

#define DE_RE 2

ModbusRTUMaster modbus(MODBUS_SERIAL, DE_RE);

TimerData timerL;
TimerData timerR;
DisplaySdh display;

unsigned long lastTime = 0;
unsigned long lastTimeSlaveHmi = 0;


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
bool targetCommOk = false;

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

void blinkTargetLigts(uint16_t period);
void setTargetLightsOff();
void targetLights(uint8_t L,uint8_t R);

void sendTargetInfoToHmi();
void sendActTimeToHmi();



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

    while (1)
    {
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

                // vychozi perioda blikani majaku na tercich
                t_outputs->blink_period = 500;

                d_inputs->status = SYS_DETECT_DEVICES;
                break;
            }

            case SYS_DETECT_DEVICES:
            {
                system_config.display_present = true;

                // HMI zatim neresime
                system_config.hmi_present = true;
                system_config.hmi_required_ok = true;

                // IO modul s terci - TODO detekce pripojeni
                system_config.target_present = detectTargetDevice();

                if(display.init())
                    d_inputs->status = SYS_INIT_REQUIRED;
                else
                    d_inputs->status = SYS_ERROR;

                break;
                
            }

            case SYS_INIT_REQUIRED:
            {
                if (!initRequiredDevices())
                    d_inputs->status = SYS_ERROR;
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
                break;
            }

            case SYS_STANDBY:
            {
                d_inputs->status = SYS_PROGRAM_SELECT;
                break;
            }

            case SYS_PROGRAM_SELECT:
            {
                // zatim natvrdo SDH stopky
                selected_program = prg_sdh_timer;
                d_inputs->status = SYS_PROGRAM_PREPARE;
                break;
            }

            case SYS_PROGRAM_PREPARE:
            {
                timerL.init();
                timerR.init();

                // vynulovani vystupu na IO desce
                //memset(target_reg, 0, sizeof(target_reg));
                //t_outputs->blink_period = 500;

                if ((selected_program == prg_sdh_timer) && (!system_config.target_ready))
                {
                    error = target_not_response;
                    d_inputs->status = SYS_ERROR;
                }
                else
                    d_inputs->status = SYS_PROGRAM_RUN;

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

                // po ukonceni programu vypnout IO vystupy
                t_outputs->target_l_light = 0;
                t_outputs->target_r_light = 0;
                t_outputs->target_l_light_blink = 0;
                t_outputs->target_r_light_blink = 0;
                t_outputs->target_valves = 0;

                d_inputs->status = SYS_STANDBY;
                break;
            }

            case SYS_ERROR:
            {
                d_outputs->errorCode = error;

                timerL.stopTimming();
                timerR.stopTimming();

                t_outputs->target_l_light = 1;
                t_outputs->target_r_light = 1;
                t_outputs->target_l_light_blink = 1;
                t_outputs->target_r_light_blink = 1;
                //t_outputs->relay = 0;
                t_outputs->blink_period = 2000;

                // bezpečný stav IO desky
                //memset(target_reg, 0, sizeof(target_reg));
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
        //Serial.println(d_inputs->status);
    }
}


// ======================================================
// MODBUS
// ======================================================

void processModbus()
{
    static uint8_t slave1_phase = 0;
    static uint8_t slave2_phase = 0;
    uint8_t comm_error = 0;

    if ((millis() - lastTime) > 10)
    {
        if ((millis() - lastTimeSlaveHmi) > 100)
        {
            if (slave2_phase == 0)
            {
                comm_error = modbus.writeMultipleHoldingRegisters(MODBUS_SLAVE2_UNIT_ID, 0, hmi_reg, (sizeof(hmi_reg) / 2));

                if (comm_error == 0)
                    slave2_phase = 1;                   
            }
            else
            {
                comm_error = modbus.readInputRegisters(MODBUS_SLAVE2_UNIT_ID, 0, hmi_ireg, (sizeof(hmi_ireg) / 2));

                if (comm_error == 0)
                    slave2_phase = 0;
                
            }

            lastTimeSlaveHmi = millis();
        }
        else
        {
            if (slave1_phase == 0)
            {
                comm_error = modbus.writeMultipleHoldingRegisters(MODBUS_SLAVE1_UNIT_ID, 0, target_reg, (sizeof(target_reg) / 2));

                if (comm_error == 0)
                    slave1_phase = 1;
            }
            else
            {
                comm_error = modbus.readInputRegisters(MODBUS_SLAVE1_UNIT_ID, 0, target_ireg, (sizeof(target_ireg) / 2));

                if (comm_error == 0)
                {
                    for (uint8_t i = 0; i < (sizeof(target_ireg) / 2); i++)
                        target_ireg[i] = swapBytes(target_ireg[i]);

                    slave1_phase = 0;
                }
            }
        }

        (void)comm_error;
        lastTime = millis();
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
    // SDH stopky vyzaduji IO desku s terci
    program_availability.allow_sdh_timer = system_config.target_ready;

    program_availability.allow_countdown = true;

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
    return true;
}

bool initDisplayDevice()
{
    return true;
}

bool initRequiredDevices()
{
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
    // pokud IO deska neni pritomna, system muze bezet bez ni
    if (!system_config.target_present)
    {
        system_config.target_ready = false;
        return true;
    }

    // v nove koncepci neni init target state machine,
    // staci, ze IO deska odpovida
    system_config.target_ready = true;
    return true;
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
    static bool leftDone = false;
    static bool rightDone = false;

    sendTargetInfoToHmi();

    switch (step)
    {
        case 0: // cekani na povoleni startu
            blinkTargetLigts(250);
            leftDone = false;
            rightDone = false;
            if(h_inputs->start_btn)
                step = 1;
            break;

        case 1: // cekame na start
            targetLights(1,1);
            timerL.init();
            timerR.init();
            sendActTimeToHmi();
            if ((h_inputs->sensor_enable == 1) && (h_inputs->start_sensor == 1))
                step = 2;
            break;

        case 2: // start mereni
            start();
            // na zacatku zhasnout svetla a zavrit / vypnout akce
            targetLights(0,0);

            step = 3;
            break;

        case 3:
    
            // zachycení zásahu levého terče
            if (t_inputs->target_l_full && !leftDone)
            {
                leftDone = true;
                timerL.stopTimming();
                t_outputs->target_l_light = 1;
            }

            // zachycení zásahu pravého terče
            if (t_inputs->target_r_full && !rightDone)
            {
                rightDone = true;
                timerR.stopTimming();
                t_outputs->target_r_light = 1;
            }

            // levý terč ještě neb
            if (!leftDone)
                timerL.Time();

            // pravý terč ještě neb
            if (!rightDone)
                timerR.Time();

            sendActTimeToHmi();
            display.sendData(timerL,timerR);

            // oba terce hotove
            if (leftDone && rightDone)
            {
                display.sendData(timerL,timerR);
                t_outputs->relay = 1;
                step = 4;
            }

            // docasne stop pres vstup stop_btn
            if (h_inputs->stop_btn)
            {
                timerL.stopTimming();
                timerR.stopTimming();
                t_outputs->relay = 1;
                leftDone = true;
                rightDone = true;
                step = 4;
            }

        break;

        case 4: //drain
        
            //if(!t_inputs->target_l_empty && !t_inputs->target_r_empty)        // ODJEBAT !!!! TO JE JEN NA TEST
            if(t_inputs->target_l_empty && t_inputs->target_r_empty)
            {
                t_outputs->relay = 0;
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
        case 0:
            if (h_inputs->sensor_enable)
                step = 1;
            break;

        case 1:
            start();
            step = 2;
            break;

        case 2:
            if (timerL.casSTART > 1)
            {
                timerL.Time();
                display.sendData(timerL, timerL);
            }

            if ((timerL.casTERC_M == 0) && (timerL.casTERC_S == 0))
            {
                timerL.stopTimming();
                timerR.stopTimming();
                display.sendData(timerL, timerR);

                step = 0;
                d_inputs->status = SYS_PROGRAM_FINISH;
            }

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

void blinkTargetLigts(uint16_t period)
{
    t_outputs->blink_period = period;
    t_outputs->target_l_light = 1;
    t_outputs->target_l_light_blink = 1;
    t_outputs->target_r_light = 1;
    t_outputs->target_r_light_blink = 1;
}

void setTargetLightsOff()
{
    t_outputs->target_l_light = 0;
    t_outputs->target_l_light_blink = 0;
    t_outputs->target_r_light = 0;
    t_outputs->target_r_light_blink = 0;

}

void targetLights(uint8_t L,uint8_t R)
{
    t_outputs->target_l_light = L;
    t_outputs->target_r_light = R;
    t_outputs->target_l_light_blink = 0;
    t_outputs->target_r_light_blink = 0;
}

void sendTargetInfoToHmi()
{
    h_outputs->target_l_empty = t_inputs->target_l_empty;
    h_outputs->target_r_empty = t_inputs->target_r_empty;
    h_outputs->target_l_full = t_inputs->target_l_full;
    h_outputs->target_r_full = t_inputs->target_r_full;
    h_outputs->target_l_light_blink = t_outputs->target_l_light_blink;
    h_outputs->target_r_light_blink = t_outputs->target_r_light_blink;
    h_outputs->target_valve = t_outputs->target_valves;
    h_outputs->target_vbatt = 1235;
}

void sendActTimeToHmi()
{
    h_outputs->casTERC1_M = timerL.casTERC_M & 0xFFFF;
    h_outputs->casTERC1_S = timerL.casTERC_S & 0xFFFF;
    h_outputs->casTERC1_ms = timerL.casTERC_ms & 0xFFFF;

    h_outputs->casTERC2_M = timerR.casTERC_M & 0xFFFF;
    h_outputs->casTERC2_S = timerR.casTERC_S & 0xFFFF;
    h_outputs->casTERC2_ms = timerR.casTERC_ms & 0xFFFF;  
}



