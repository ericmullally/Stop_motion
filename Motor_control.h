
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
#define GET_PARAM	0x3F



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





uint8_t motor_setup(SPI_HandleTypeDef* spi);


















#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_CONTROL_H */
