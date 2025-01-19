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

#define SPI_ERROR 	0xFF


/*
 * @brief initial setup of the L6474
 * @param pointer to the SPI handle
 * @retval TOFF value for the L6464
 * */
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
	uint8_t message[3];
	message[0] = command;
	message[1] = (conf.configuration  & 0xff);
	message[2] = ((conf.configuration >> 8) & 0xff);
	if (HAL_SPI_Transmit(spi, message, sizeof(message), HAL_MAX_DELAY) != HAL_OK) {
		return SPI_ERROR;
	}

	// check that the settings are correct
	config_t new_conig;
	uint8_t address = (CONFIG ^ GET_PARAM);
	uint8_t response[2] = {0};

	if(HAL_SPI_Transmit(spi, &address, 1, HAL_MAX_DELAY) != HAL_OK){
		return SPI_ERROR;
	}

	if(HAL_SPI_Receive(spi, response, sizeof(response), HAL_MAX_DELAY) != HAL_OK){
		return SPI_ERROR;
	}
	new_conig.configuration =  (response[1] << 8) | response[0];
	return (new_conig.TOFF & 0xff);

}



