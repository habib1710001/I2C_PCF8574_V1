/*
 * i2c.h
 *
 *      Created on: Jun 8, 2023
 *      Author: Habib Ur Rahman
 */
#include "stm32f4xx.h"

#ifndef I2C_H_
#define I2C_H_

void I2C_init(void);
void I2C_Start(void);
void I2C_Address(uint8_t address);
void I2C_Write(uint8_t data);
void I2C_WriteMulti(uint8_t *data, uint8_t size);
void I2C_Stop(void);

#endif /* I2C_H_ */
