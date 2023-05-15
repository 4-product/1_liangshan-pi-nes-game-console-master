#ifndef _I2C_H
#define _I2C_H

#include "gd32f4xx.h"
#include "systick.h"

#define I2CX_SLAVE_ADDRESS7     0xA0
#define I2C_SPEED               100000
#define I2C_PAGE_SIZE           8
#define I2CX                    I2C0
#define RCU_GPIO_I2C            RCU_GPIOB
#define RCU_I2C                 RCU_I2C0
#define I2C_SCL_PORT            GPIOB
#define I2C_SDA_PORT            GPIOB
#define I2C_SCL_PIN             GPIO_PIN_6
#define I2C_SDA_PIN             GPIO_PIN_7
#define I2C_GPIO_AF             GPIO_AF_4

/* function declarations */
/* configure the I2CX interface */
void i2c_config(void);
/* reset I2C bus */
void i2c_bus_reset(void);

#endif /* I2C_H */
