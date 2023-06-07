/*
 * i2c.h
 *
 *      Created on: Jun 8, 2023
 *      Author: Habib Ur Rahman
 */
#include "stm32f4xx.h"

#ifndef I2C_H_
#define I2C_H_
void I2C1_init(void);
void I2C1_byteRead(char saddr, char maddr, char* data);

#endif /* I2C_H_ */
