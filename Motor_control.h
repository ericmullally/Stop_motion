
#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32h7xx_hal.h"
#include "Motor_defines.h"


#define CURRENT_MODE

// Register addresses
#define ABS_POS      0x01  // 16‐bit two’s complement position
#define EL_POS       0x02  // 9‐bit electrical position
#define MARK         0x03  // 22‐bit mark position
#define SPEED        0x04  // 20‐bit speed
#define ACC          0x05  // 12‐bit acceleration
#define DEC          0x06  // 12‐bit deceleration
#define MAX_SPEED    0x07  // 10‐bit maximum speed
#define MIN_SPEED    0x08  // 12‐bit minimum speed
#define ADC_OUT      0x12  // 8 bits (lower 5 bits valid) = ADC output
#define OCD_TH       0x13  // 8 bits (lower 5 bits valid) = overcurrent detection threshold
#define FS_SPD		 0x15  // 11 bits Full-step speed
#define STEP_MODE    0x16  // 8 bits step mode configuration
#define ALARM_EN     0x17  // 8 bits alarm enable
#define GATECFG_1    0x18  // 8 Gate driver configuration
#define GATECFG_1    0x19  // 8 Gate driver configuration
#define STATUS		 0x1B  // 16 bits status
#define CONFIG		 0x1A  // 16 bits Configuration


#ifdef CURRENT_MODE
    // In current (PWM) mode the chip uses “TVAL” registers
	// (Note: In voltage mode the register at 0x09 is KVAL_HOLD.)
    #define TVAL_HOLD    0x09  // 8 bits (only lower 7 used) = target current (holding)
	#define TVAL_RUN	 0x0A  // 7 bit	Constant speed reference voltage
	#define TVAL_ACC     0x0B  // 7 bit	Acceleration reference voltage
	#define TVAL_DEC	 0x0C  // 7 bit	Deceleration reference voltage
	#define T_FAST       0x0E  // 8 bits fast decay control (shares address with ST_SLP in voltage mode)
    #define TON_MIN      0x0F  // 8 bits (lower 7 bits valid) = minimum on‐time
    #define TOFF_MIN     0x10  // 8 bits (lower 7 bits valid) = minimum off‐time
#endif

#ifdef VOLTAGE_MODE
    // In voltage (PWM duty cycle) mode the following registers are used:
    #define KVAL_HOLD    0x09  // 8 bits holding duty cycle
    #define KVAL_RUN     0x0A  // 8 bits running duty cycle
    #define KVAL_ACC     0x0B  // 8 bits acceleration duty cycle
    #define KVAL_DEC     0x0C  // 8 bits deceleration duty cycle
    #define INT_SPEED    0x0D  // 14 bits intersect speed for BEMF compensation
    #define ST_SLP       0x0E  // 8 bits start slope for BEMF compensation
    #define FN_SLP_ACC   0x0F  // 8 bits fine slope acceleration
    #define FN_SLP_DEC   0x10  // 8 bits fine slope deceleration
    #define K_THERM      0x11  // 4 bits thermal compensation factor
    #define STALL_TH     0x14  // 8 bits (only lower 5 valid) = stall threshold
#endif


typedef struct{
	uint8_t 	Id;
	abs_pos_t 	abs_pos;
	el_pos_t 	el_pos;
	mark_t 		mark;

	adc_out_t	adc_out;
	ocd_th_t	ocd_th;
	step_mode_t step_mode;
	alarm_en_t	alarm_en;
	config_t	config;
	status_t	status;

#ifdef CURRENT_MODE
	t_val_t tval_hold;  // to be used for TVAL_HOLD, TVAL_RUN, TVAL_ACC, and TVAL_DEC
	t_fast_t tfast;
    ton_min_t ton_min;
    toff_min_t toff_min;
#endif


}motor_t;


typedef enum{
	POWER_OFF,
	POWER_ON
}Power_states;

uint8_t motor_setup(SPI_HandleTypeDef* spi);

uint8_t L6474_write_receive_multi(SPI_HandleTypeDef *spi, uint8_t *data, uint8_t *rxbuffer, int size);

void L6474_power_toggle(Power_states state);










#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_CONTROL_H */
