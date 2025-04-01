#include "ms5611_hal.h"

/**
 * @brief Perform single-byte I2C write and read.
 * @param deviceaddr I2C device address.
 * @param dat Data to write.
 * @return Data read from the device.
 */
uint8_t IIC_SingleWirteAndRead(uint8_t deviceaddr, uint8_t dat)
{
    while (HAL_I2C_Master_Transmit(&hi2c1, deviceaddr, &dat, 1, 1000) != HAL_OK);
}

/**
 * @brief Perform multi-byte I2C read.
 * @param deviceaddr I2C device address.
 * @param out Pointer to the buffer to store the read data.
 * @param len Length of the data to read.
 * @return None
 */
void IIC_MultiWriteAndRead(uint8_t deviceaddr, uint8_t *out, int len)
{
    while (HAL_I2C_Master_Receive(&hi2c1, deviceaddr, out, len, 1000) != HAL_OK); // Receive the data
}

/**
 * @brief Write a single register of the MS5611 sensor.
 * @param reg Register address.
 * @param value Value to write.
 * @return None
 */
void IIC_MS5611SingleWrite(uint8_t reg, uint8_t value)
{
    IIC_SingleWirteAndRead(MS5611_ADDR, reg);
    IIC_SingleWirteAndRead(MS5611_ADDR, value);
}

/**
 * @brief Read multiple registers of the MS5611 sensor.
 * @param reg Register address.
 * @param data Pointer to the buffer to store the read data.
 * @param length Length of the data to read.
 * @return None
 */
void IIC_MS5611MultiRead(uint8_t reg, uint8_t *data, uint8_t length)
{
    IIC_SingleWirteAndRead(MS5611_ADDR, reg);
    IIC_MultiWriteAndRead(MS5611_ADDR, data, length);
}

