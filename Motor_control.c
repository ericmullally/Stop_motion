/*
 * Motor_control.c
 *
 *  Created on: Jan 14, 2025
 *      Author: eric
 */

#include "Motor_control.h"





#define MAX_CURRENT 0x60   // 3A  0 =  31.25 mA with a 31.25 mA increment. Current (mA)=N×31.25
#define NO_OP 		0x00
#define ENABLE 		0xB8
#define DISABLE		0xA8
#define GET_STATUS	0xD0
#define GET_PARAM	0x20   // needs to be ORed with the address when requesting information.




uint8_t motor_setup(SPI_HandleTypeDef* spi){

	// first send set param then the config
	config_t conf;
	conf.OSC_SEL 	= 0;  // 16Mhz
	conf.EXT_CLK 	= 0;  // internal clock
	conf.EN_TQREG 	= 0;  // torque regulation selected by registers
	conf.OCD_SD 	= 1;  // bridges shut down on over current
	conf.POW_SR		= 1;  // 75 bridge output slew rate values
	conf.TOFF 		= 4;  // 16 microseconds

	uint8_t command = CONFIG;
	uint8_t message[4];
	message[0] = command;
	message[1] = (conf.configuration  & 0xff);
	message[2] = ((conf.configuration >> 8) & 0xff);


}



