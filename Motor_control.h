
#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stdint.h"




/*The devices (always slave) can be driven by an MCU (always master) sending commands
through an 8-bit SPI interface. The 8-bit shift register of the device is kept enabled while CS
input is forced low. During this time, at every raising edge of the serial clock (CK) the SDI
input is stored into the shift register. At CK falling edges the SDO output is updated
according to the last bit of the shift register (see Figure 1).
When the CS input is raised, the device catches the shift register content and interprets its
value as a command or an argument of the command received previously.
All the bytes are transmitted starting from the most significant bit.
At every transmission cycle, that is the time between the falling and raising edge of the CS
line, the number of bytes transmitted by the master is equal to those received.*/




// REGISTERS
#define NO_OP		0x00
#define ABS_POS   	0x01  // 22 bits    R/W returns 2's complement of the position
#define EL_POS    	0x02  // 9 bits		R/W
#define MARK      	0x03  // 22 bits 	R/W Mark position
#define TVAL      	0x09  // 7 bits 	R/W
#define T_FAST    	0x0E  // 8 bits		R/WH
#define TON_MIN   	0x0F  // 7 bits		R/WH
#define TOFF_MIN  	0x10  // 7 bits		R/WH
#define ADC_OUT   	0x12  // 5 bits		R
#define OCD_TH 		0x13  // 4 bits		R/W
#define STEP_MODE 	0x16  // 8 bits		R/WH
#define ALARM_EN 	0x17  // 8 bits		R/W
#define CONFIG 		0x18  // 16 bits	R/WH
#define STATUS 		0x19  // 16 bits	R
#define DISABLE		0xA8
#define ENABLE		0xB8

#define SPI_ERROR 	0xFF



// get mask
#define GET_PARAM	0x20
#define SET_PARAM	0x00


typedef union{
	uint16_t response;
	struct{
		uint16_t HIZ 			: 1;
		uint16_t bit1_3			: 3;
		uint16_t DIR			: 1;
		uint16_t bit5_6			: 2;
		uint16_t NOTPREF_CMD 	: 1;
		uint16_t WRONG_CMD 		: 1;
		uint16_t UVLO 			: 1;
		uint16_t TH_WRN 		: 1;
		uint16_t TH_SD 			: 1;
		uint16_t OCD 			: 1;
		uint16_t bit13_15 		: 3;
	};
}status_t;

typedef union{
	uint16_t configuration;
	struct{
		uint16_t OSC_SEL 		: 3;
		uint16_t EXT_CLK		: 1;
		uint16_t bit4			: 1;
		uint16_t EN_TQREG		: 1;
		uint16_t bit6		 	: 1;
		uint16_t OCD_SD 		: 1;
		uint16_t POW_SR 		: 2;
		uint16_t TOFF 			: 5;  // TOFF = N*4uS
		uint16_t bit15 			: 1;

	};
}config_t;

// ABS_POS Register
typedef union {
    uint32_t absolute_position;
    struct {
        uint32_t POS_0_21 : 22; // Absolute position in microsteps (2's complement format)
        uint32_t reserved : 10;
    };
} abs_pos_t;

// EL_POS Register
typedef union {
    uint16_t electrical_position;
    struct {
        uint16_t MICROSTEP : 7; // Microstep position within the current step (0 to 127)
        uint16_t STEP : 2;      // Current step (0 to 3)
        uint16_t reserved : 7;
    };
} el_pos_t;

// MARK Register
typedef union {
    uint32_t mark_position;
    struct {
        uint32_t MARK_0_21 : 22; // Mark position in microsteps (2's complement format)
        uint32_t reserved : 10;
    };
} mark_t;

// TVAL Register
typedef union {
    uint8_t target_current;
    struct {
        uint8_t TVAL_0_6 : 7;   // Current value for torque regulation DAC
        uint8_t reserved : 1;
    };
} t_val_t;

// TFAST Register
typedef union {
    uint8_t decay_time;
    struct {
        uint8_t TOFF_FAST : 4;  // Maximum fast decay time
        uint8_t FALL_STEP : 4;  // Maximum fall step time
    };
} t_fast_t;

// TON_MIN Register
typedef union {
    uint8_t min_on_time;
    struct {
        uint8_t TON_MIN_0_6 : 7; // Minimum ON time for current control
        uint8_t reserved : 1;
    };
} ton_min_t;

// TOFF_MIN Register
typedef union {
    uint8_t min_off_time;
    struct {
        uint8_t TOFF_MIN_0_6 : 7; // Minimum OFF time for current control
        uint8_t reserved : 1;
    };
} toff_min_t;

// ADC_OUT Register
typedef union {
    uint8_t adc_value;
    struct {
        uint8_t ADC_OUT_0_4 : 5; // ADC output value
        uint8_t reserved : 3;
    };
} adc_out_t;

// OCD_TH Register
typedef union {
    uint8_t ocd_threshold;
    struct {
        uint8_t OCD_TH_0_3 : 4; // Overcurrent detection threshold
        uint8_t reserved : 4;
    };
} ocd_th_t;

// STEP_MODE Register
typedef union {
    uint8_t step_mode;
    struct {
        uint8_t SYNC_SEL : 3;   // Sync output selection
        uint8_t reserved : 1;   // Should be set to 1 when writing
        uint8_t STEP_SEL : 3;   // Step mode selection
        uint8_t reserved2 : 1;
    };
} step_mode_t;

// ALARM_EN Register
typedef union {
    uint8_t alarm_enable;
    struct {
        uint8_t ALARM_OVERCURRENT : 1;  // Overcurrent alarm
        uint8_t ALARM_THERMAL_SD : 1;   // Thermal shutdown alarm
        uint8_t ALARM_THERMAL_WRN : 1;  // Thermal warning alarm
        uint8_t ALARM_UVLO : 1;         // Undervoltage lockout alarm
        uint8_t reserved : 2;           // Reserved
        uint8_t ALARM_SWITCH_TURN_ON : 1; // Switch turn-on event alarm
        uint8_t ALARM_CMD_ERR : 1;     // Command error alarm (wrong or not performable)
    };
} alarm_en_t;

typedef struct{
	uint8_t 	Id;
	abs_pos_t 	abs_pos;
	el_pos_t 	el_pos;
	mark_t 		mark;
	t_val_t		t_val;
	t_fast_t	t_fast;
	ton_min_t	t_on_min;
	toff_min_t	t_off_min;
	adc_out_t	adc_out;
	ocd_th_t	ocd_th;
	step_mode_t step_mode;
	alarm_en_t	alarm_en;
	config_t	config;
	status_t	status;
}motor_t;


uint16_t motor_setup(SPI_HandleTypeDef* spi);













#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_CONTROL_H */
