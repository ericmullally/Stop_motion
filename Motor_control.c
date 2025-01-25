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


motor_t m1;

/*
 * @brief initial setup of the L6474
 * @param pointer to the SPI handle
 * @retval
 * */
uint8_t motor_setup(SPI_HandleTypeDef *spi) {

	/*
	 * Turn the motor on MOTOR_ON_Pin reset
	 *  set CS low
	 *  Disable the motor
	 *  Write the CONFIG register
	 * 	enable the motor
	 * 	Read the CONFIG register
	 *
	 * 
	 * the CS line must be toggled after each byte
	 * 
	 *  
	 * */

	uint8_t command[3]  = {CONFIG | GET_PARAM, 0x00, 0x00};
	uint8_t response[3] = {0};

	// turn on the motor driver.
	L6474_power_toggle(POWER_ON);
	L6474_write_receive_multi(spi, command, response, 3);


	return response[0];

}

/*
 * @brief Switches the driver (or drivers if using a bus) to the passed sate.
 * @param State you would like the L6474 to be in. ON/OFF
 * */
void L6474_power_toggle(Power_states state ){

	if(state == POWER_OFF){
		HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
	}else{
		HAL_GPIO_WritePin(MOTOR_ON_GPIO_Port, MOTOR_ON_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
	}
}

/*
 * @brief Sends and receives a Single byte to the L6474
 * @param spi: Pointer to the SPI handle
 * @param byte: byte to be sent
 * @param buffer: pointer to the buffer to fill.
 * @retval uint8_t: Status of the attempt
 */
uint8_t L6474_send_recieve_byte(SPI_HandleTypeDef *spi, uint8_t byte, uint8_t *buffer){

	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);

	if (HAL_SPI_TransmitReceive(spi, &byte, buffer,  1, HAL_MAX_DELAY) != HAL_OK) {
		HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
		return SPI_ERROR;
	}

	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	return HAL_OK;


}

/**
 * @brief Sends an array of bytes to the L6474 and fills the provided buffer with the response.
 *
 * @CAUTION Ensure that the size of the `rxbuffer` is sufficient to store the received data.
 *          The function will write exactly as many bytes to `rxbuffer` as it sends.
 *          `data` and `rxbuffer` must both be large enough to accommodate `size` bytes.
 *
 * @param spi: Pointer to the SPI handle.
 * @param data: Pointer to the array containing the bytes to be sent.
 * @param rxbuffer: Pointer to the buffer where the received bytes will be stored.
 * @param size: Number of bytes to transmit and receive. This must not exceed the size of `data` or `rxbuffer`.
 *
 * @retval uint8_t: Status of the operation (HAL_OK for success, or error codes for failure).
 */
uint8_t L6474_write_receive_multi(SPI_HandleTypeDef *spi, uint8_t *data, uint8_t *rxbuffer, int size){

	for (int i = 0; i < size; i++) {
		uint8_t status = L6474_send_recieve_byte(spi, data[i], &rxbuffer[i]);
		if (status != HAL_OK) {
			return SPI_ERROR;
		}
	}
	return HAL_OK;
}

/*
 * @brief Sends a Single byte to the L6474
 * @param spi: Pointer to the SPI handle
 * @param byte: byte to be sent
 * @retval uint8_t: Status of the attempt
 */
uint8_t L6474_send_byte(SPI_HandleTypeDef *spi, uint8_t byte) {

	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);

	if (HAL_SPI_Transmit(spi, &byte, 1, HAL_MAX_DELAY) != HAL_OK) {
		HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
		return SPI_ERROR;
	}


	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	return HAL_OK;

}

/*
 * @brief Sends an array of bytes to the L6474
 * @param spi: Pointer to the SPI handle
 * @param data: Array containing the bytes to be sent
 * @param size: Number of bytes to send
 * @retval uint8_t: Status of the attempt
 */
uint8_t L6474_write_multi(SPI_HandleTypeDef *spi, uint8_t *data, int size) {


	for (int i = 0; i < size; i++) {
		uint8_t status = L6474_send_byte(spi, data[i]);
		if (status != HAL_OK) {
			return SPI_ERROR;
		}
	}
	return HAL_OK;

}

/*
 * @brief Sends an array of bytes to a daisy chain of L6474's
 * @param spi: Pointer to the SPI handle
 * @param packages: Combined data array for all chips (see Application note AN4290 by ST)
 * @param sizeOfPackage: How many bytes per driver. This includes the register address.
 * @param sizeOfChain: How many drivers.
 * @retval uint8_t: Status of the attempt
 */
uint8_t L6474_daisy_write_multi(SPI_HandleTypeDef *spi, uint8_t *packages,
								int sizeOfPackage, int sizeOfChain) {

	// its not a daisy chain if there is only one. use write_multi
	if(sizeOfChain < 2){
		return SPI_ERROR;
	}

	for (int package = 0; package < sizeOfPackage; package++) {
		uint8_t bytes[sizeOfChain]; // Temporary buffer for the current transmission

		// Fill the `bytes` array with the current chain's data
		for (int i = 0; i < sizeOfChain; i++) {
			bytes[i] = *packages++; // Copy data and increment pointer
		}

		// Perform SPI transmission for the current chain
		HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);
		HAL_Delay(1);

		if (HAL_SPI_Transmit(spi, bytes, sizeOfChain, HAL_MAX_DELAY) != HAL_OK) {
			HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
			return SPI_ERROR;
		}

		HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
		HAL_Delay(1);

	}

	return HAL_OK; // Return 0 to indicate success

}

