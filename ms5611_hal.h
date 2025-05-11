#ifndef _MS5611_HAL_H_
#define _MS5611_HAL_H_

#define MS5611_ADDR 0x77

#include "stm32f0xx_hal.h"

extern I2C_HandleTypeDef hi2c2;

void IIC_MS5611SingleWrite(uint8_t reg, uint8_t value);
void IIC_MS5611MultiRead(uint8_t reg,uint8_t *data, uint8_t length);

#endif // _MS5611_HAL_H_