#ifndef _EEPROM_H
#define _EEPROM_H

#include "gd32f4xx.h"
#include "systick.h"


/* PB6 SCL  PB7 SDA */

typedef enum {
    I2C_START = 0,
    I2C_SEND_ADDRESS,
    I2C_CLEAR_ADDRESS_FLAG,
    I2C_TRANSMIT_DATA,
    I2C_STOP
} i2c_process_enum;

#define I2C_TIME_OUT    (uint16_t)(5000)
#define EEP_FIRST_PAGE  0x00
#define I2C_OK          1
#define I2C_FAIL        0
#define I2C_END         1

/* function declarations */
/* I2C read and write functions */
uint8_t i2c_24c02_test(void);
/* initialize EEPROM address */
void i2c_eeprom_init(void);
/* write one byte to the EEPROM and use timeout function */
uint8_t eeprom_byte_write_timeout(uint8_t *p_buffer, uint8_t write_address);
/* write more than one byte to the EEPROM use timeout function */
uint8_t eeprom_page_write_timeout(uint8_t *p_buffer, uint8_t write_address, uint8_t number_of_byte);
/* write buffer of data to the EEPROM use timeout function */
void eeprom_buffer_write_timeout(uint8_t *p_buffer, uint8_t write_address, uint16_t number_of_byte);
/* read data from the EEPROM use timeout function */
uint8_t eeprom_buffer_read_timeout(uint8_t *p_buffer, uint8_t read_address, uint16_t number_of_byte);
/* wait for EEPROM standby state use timeout function */
uint8_t eeprom_wait_standby_state_timeout(void);

#endif
