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



#define SPI2_CS_Pin GPIO_PIN_0
#define SPI2_CS_GPIO_Port GPIOI

#define MOTOR_ON_Pin GPIO_PIN_7
#define MOTOR_ON_GPIO_Port GPIOI

/*
 * @brief initial setup of the L6474
 * @param pointer to the SPI handle
 * @retval TOFF value for the L6464
 * */
uint8_t motor_setup(SPI_HandleTypeDef* spi){

	/*
	 * Turn the motor on MOTOR_ON_Pin reset
	 *  set CS low
	 *  Disable the motor
	 *  Write the CONFIG register
	 * 	enable the motor
	 * 	Read the CONFIG register
	 *
	 * */
	config_t current_config;
	uint8_t address = CONFIG ^ GET_PARAM;
	uint8_t response[2]= {0};

	HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);

	if(HAL_SPI_TransmitReceive(spi, (uint8_t*)&address, response, 3, HAL_MAX_DELAY) != HAL_OK){
		return SPI_ERROR;
	}
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);

	current_config.configuration = (response[0] << 8) | response[1] ;
	return response[0];

}

