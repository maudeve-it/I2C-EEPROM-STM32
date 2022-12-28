/*
 * z_i2c_eeprom.h
 *
 *  Created on: 7 mar 2022
 *      Author: Mauro
 */

#ifndef INC_Z_I2C_EEPROM_H_
#define INC_Z_I2C_EEPROM_H_


//stm32 i2c port connecting EEPROM chip
#define EEPROM_PORT				hi2c1			// here using I2C1 port

// information about I2C chip
#define EEPROM_I2C_ADDR			0xA0			// the I2C chip address
// information about EEPROM:
#define EE_PAGE_LEN				0x0040			// (for 24C256) length of EEPROM page: 64B
#define UPPER_LIMIT_EEADDR		0x7FFF			// (for 24C256) 0x7FFF = 32 kBytes = 256 kbits

// HAL API parameters
#define EE_AVAIL_TRIALS			20				// no. of attempts testing EEPROM availability
#define EE_AVAIL_TIMEOUT		HAL_MAX_DELAY	// ms timeout for each availability trial
#define I2C_COMM_TIMEOUT		20   			// ms timeout for the I2C communication (consider data length and I2C speed)

//EEprom memory addresses used in the project
#define PARAM_EEADDR 			0x0000			// save test string ad EEPROM beginning



void test_EEPROM();
uint8_t read_p_EEPROM(uint16_t addr, char *data);
uint8_t read_n_EEPROM(uint16_t addr, char *data, uint8_t size);
uint8_t write_s_EEPROM(uint16_t addr, char *data);
uint8_t write_n_EEPROM(uint16_t addr, char *data, uint8_t size);

#endif /* INC_Z_I2C_EEPROM_H_ */
