#include "ms5611_hal.h"

/**
 * @brief Perform single-byte I2C write and read.
 * @param deviceaddr I2C device address.
 * @param dat Data to write.
 * @return Data read from the device.
 */
void IIC_SingleWirteAndRead(uint8_t deviceaddr, uint8_t reg, uint8_t dat)
{
    HAL_I2C_Mem_Write(
        &hi2c2,            // I2C ���
        deviceaddr << 1,      // �豸��ַ������ 1 λ��HAL Ҫ�� 7 λ��ַ��
        reg,           // Ŀ��Ĵ�����ַ
        I2C_MEMADD_SIZE_8BIT,  // �Ĵ�����ַ���ȣ�8 λ�� 16 λ��
        &dat,             // Ҫд�������
        1,                 // ���ݳ��ȣ��ֽ�����
        100                // ��ʱʱ�䣨ms��
    );
}

/**
 * @brief Perform multi-byte I2C read.
 * @param deviceaddr I2C device address.
 * @param out Pointer to the buffer to store the read data.
 * @param len Length of the data to read.
 * @return None
 */

void IIC_MultiRead(uint8_t deviceaddr, uint8_t reg, uint8_t *out, uint8_t len) {
    HAL_I2C_Mem_Read(
        &hi2c2,
        deviceaddr << 1,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        out,
        len,
        100
    );
}

/**
 * @brief Write a single register of the MS5611 sensor.
 * @param reg Register address.
 * @param value Value to write.
 * @return None
 */
void IIC_MS5611SingleWrite(uint8_t reg, uint8_t value)
{
    IIC_SingleWirteAndRead(MS5611_ADDR, reg, value);
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
    IIC_MultiRead(MS5611_ADDR, reg, data, length);
}

