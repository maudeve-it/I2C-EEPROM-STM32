/*
 * z_i2c_eeprom.c
 *
 *  Created on: 7 mar 2022
 *      Author: Mauro
 *
 *  The below test function refers to what shown in the video https://youtu.be/Rd5CeMbla5g 
 *  and you don't need it in the production project
 *
 *  The above youtube video shows also all detail to add this library to your project
 * 
 */


#include <string.h>
#include "main.h"

extern I2C_HandleTypeDef EEPROM_PORT;




void test_EEPROM() {

	uint8_t result[10]={0};
	uint16_t delta[10]={0};
	char dataIn[EE_PAGE_LEN]={0};
	char dataOut[EE_PAGE_LEN]={0};
	uint16_t time;
	uint8_t k=0;

	k=0;
	TIM7->EGR=1;  //reset TIM7->CNT

	// 0 = TIM7 test: measuring 20 ms delay
	HAL_Delay(20);
	delta[k++]=TIM7->CNT;

	// 1 = IsDeviceReady with a free EEprom
	time=TIM7->CNT;
	result[k]=HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
	delta[k++]=TIM7->CNT-time;

	// 2 = Reading 1 byte
	time=TIM7->CNT;
	result[k]=HAL_I2C_Mem_Read(&EEPROM_PORT, EEPROM_I2C_ADDR, PARAM_EEADDR, I2C_MEMADD_SIZE_16BIT, (uint8_t *)dataIn, 1, I2C_COMM_TIMEOUT);
	delta[k++]=TIM7->CNT-time;

	// 3 = Reading a whole page (64 bytes)
	time=TIM7->CNT;
	result[k]=HAL_I2C_Mem_Read(&EEPROM_PORT, EEPROM_I2C_ADDR, PARAM_EEADDR, I2C_MEMADD_SIZE_16BIT, (uint8_t *)dataIn, EE_PAGE_LEN, I2C_COMM_TIMEOUT);
	delta[k++]=TIM7->CNT-time;

	// 4 = Sending a 1 byte writing command
	time=TIM7->CNT;
	result[k]=HAL_I2C_Mem_Write(&EEPROM_PORT, EEPROM_I2C_ADDR, PARAM_EEADDR, I2C_MEMADD_SIZE_16BIT, (uint8_t *)dataOut, 1, I2C_COMM_TIMEOUT);
	delta[k++]=TIM7->CNT-time;

	// 5 = a ready device after writing 1 byte
	time=TIM7->CNT;
	result[k]=HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
	delta[k++]=TIM7->CNT-time;

	// 6 = Sending a 64 byte page writing command
	time=TIM7->CNT;
	result[k]=HAL_I2C_Mem_Write(&EEPROM_PORT, EEPROM_I2C_ADDR, PARAM_EEADDR, I2C_MEMADD_SIZE_16BIT, (uint8_t *)dataOut, EE_PAGE_LEN, I2C_COMM_TIMEOUT);
	delta[k++]=TIM7->CNT-time;

	// 7 = Waiting a ready device after writing 64 byte
	time=TIM7->CNT;
	result[k]=HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
	delta[k++]=TIM7->CNT-time;

	k=0;

}



/***************************************
 * @brief	read an EEPROM page to a string
 * @param	addr	memory address to start reading
 * 			data	pointer to string receiving data read
 * 					PLEASE NOTE: string size must be >= EE_PAGE_LEN
 * @return			result code from I2C device
 ***************************************/
uint8_t read_p_EEPROM(uint16_t addr, char *data) {
	uint8_t result;

	result=HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
	if (result==HAL_OK)
		result=HAL_I2C_Mem_Read(&EEPROM_PORT, EEPROM_I2C_ADDR, addr, I2C_MEMADD_SIZE_16BIT, (uint8_t *)data, EE_PAGE_LEN, I2C_COMM_TIMEOUT);
	return result;
}






/***************************************
 * @brief	read n characters from EEPROM to a string
 * @param	addr	memory address to start reading
 * 			data	pointer to string receiving data read
 * 					PLEASE NOTE: function doesn't control
 * 					string is null-terminatesd
 * 			size	number fo bytes to read
 * @return			result code from I2C device
 ***************************************/
uint8_t read_n_EEPROM(uint16_t addr, char *data, uint8_t size) {
	uint8_t result;

	result=HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
	if (result==HAL_OK)
		result=HAL_I2C_Mem_Read(&EEPROM_PORT, EEPROM_I2C_ADDR, addr, I2C_MEMADD_SIZE_16BIT, (uint8_t *)data, size, I2C_COMM_TIMEOUT);
	return result;
}




/***************************************
 * @brief	write a string to EEPROM
 * @param	addr	memory address to start writing
 * 			data	pointer to string to write into EEPROM
 * 					PLEASE NOTE: function doesn't control
 * 					if write overrides page boundaries.
 * 					Function doesn't control if last byte is a null
 * @return			result code from I2C device
 ***************************************/
uint8_t write_s_EEPROM(uint16_t addr, char *data) {
	uint8_t result;

	result=HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
	if (result==HAL_OK)
		result=HAL_I2C_Mem_Write(&EEPROM_PORT, EEPROM_I2C_ADDR, addr, I2C_MEMADD_SIZE_16BIT, (uint8_t *)data, strlen(data)+1, I2C_COMM_TIMEOUT);
	return result;
}



/***************************************
 * @brief	write n bytes from a string to EEPROM
 * @param	addr	memory address to start writing
 * 			data	pointer to string to write into EEPROM
 * 					PLEASE NOTE: function doesn't control
 * 					if overriding page boundaries.
 * 					Function doesn't control if last byte is a null
 * @return			result code from I2C device
 ***************************************/
uint8_t write_n_EEPROM(uint16_t addr, char *data, uint8_t size) {
	uint8_t result;

	result=HAL_I2C_IsDeviceReady(&EEPROM_PORT, EEPROM_I2C_ADDR, EE_AVAIL_TRIALS, EE_AVAIL_TIMEOUT);
	if (result==HAL_OK)
		result=HAL_I2C_Mem_Write(&EEPROM_PORT, EEPROM_I2C_ADDR, addr, I2C_MEMADD_SIZE_16BIT, (uint8_t *)data, size, I2C_COMM_TIMEOUT);
	return result;
}

