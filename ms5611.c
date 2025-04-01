#include "ms5611.h"

/**
 * @brief Initialize the MS5611 sensor registers.
 * @param None
 * @return None
 */
void MS5611_Init(void)
{
    MS5611_Reset();
    HAL_Delay(3);
    MS5611_Read_Prom();

    MS5611_Start_T();
}

/**
 * @brief Reset the MS5611 sensor.
 * @param None
 * @return None
 */
static void MS5611_Reset(void)
{
    IIC_MS5611SingleWrite(CMD_RESET, 0x01);
}

/**
 * @brief Read factory calibration parameters from MS5611 PROM.
 * @param None
 * @return None
 */
static void MS5611_Read_Prom(void)
{
    uint8_t rxbuf[2] = { 0, 0 };

    for (u8 i = 0; i < PROM_NB; i++)
    {
        IIC_MS5611MultiRead((CMD_PROM_RD + i * 2), rxbuf, 2);
        ms5611.prom[i] = rxbuf[0] << 8 | rxbuf[1];
    }
}

/**
 * @brief Read temperature measurement value from MS5611.
 * @param None
 * @return None
 */
static void MS5611_Read_Adc_T(void)
{
    IIC_MS5611MultiRead(CMD_ADC_READ, ms5611.t_rxbuf, 3);
}

/**
 * @brief Read pressure measurement value from MS5611.
 * @param None
 * @return None
 */
static void MS5611_Read_Adc_P(void)
{
    IIC_MS5611MultiRead(CMD_ADC_READ, ms5611.p_rxbuf, 3);
}

/**
 * @brief Send temperature measurement command to MS5611.
 * @param None
 * @return None
 */
static void MS5611_Start_T(void)
{
    IIC_MS5611SingleWrite(CMD_ADC_CONV + CMD_ADC_D2 + MS5611_OSR, 0x01);
}

/**
 * @brief Send pressure measurement command to MS5611.
 * @param None
 * @return None
 */
static void MS5611_Start_P(void)
{
    IIC_MS5611SingleWrite(CMD_ADC_CONV + CMD_ADC_D1 + MS5611_OSR, 0x01);
}

/**
 * @brief Update MS5611 data.
 * @param None
 * @return None
 */
void MS5611_Update(void)
{
    static int state = 0;

    if (state)
    {
        MS5611_Read_Adc_P();
        MS5611_Start_T();
        MS5611_BaroAltCalculate();
        state = 0;
    }
    else
    {
        MS5611_Read_Adc_T();
        MS5611_Start_P();
        state = 1;
    }
}

/**
 * @brief Perform data calibration and temperature compensation, and convert pressure to altitude.
 * @param None
 * @return None
 */
static void MS5611_BaroAltCalculate(void)
{
    int32_t  off2 = 0, sens2 = 0, delt;

    ms5611.ut = (ms5611.t_rxbuf[0] << 16) | (ms5611.t_rxbuf[1] << 8) | ms5611.t_rxbuf[2];
    ms5611.up = (ms5611.p_rxbuf[0] << 16) | (ms5611.p_rxbuf[1] << 8) | ms5611.p_rxbuf[2];

    int32_t dT = ms5611.ut - ((uint32_t)ms5611.prom[5] << 8);
    int64_t off = ((uint32_t)ms5611.prom[2] << 16) + (((int64_t)dT * ms5611.prom[4]) >> 7);
    int64_t sens = ((uint32_t)ms5611.prom[1] << 15) + (((int64_t)dT * ms5611.prom[3]) >> 8);
    ms5611.temperature = 2000 + (((int64_t)dT * ms5611.prom[6]) >> 23);

    if (ms5611.temperature < 2000)
    {   // temperature lower than 20degC
        delt = ms5611.temperature - 2000;
        delt = delt * delt;
        off2 = (5 * delt) >> 1;
        sens2 = (5 * delt) >> 2;
        if (ms5611.temperature < -1500)
        {   // temperature lower than -15degC
            delt = ms5611.temperature + 1500;
            delt = delt * delt;
            off2  += 7 * delt;
            sens2 += (11 * delt) >> 1;
        }
    }
    off  -= off2;
    sens -= sens2;
    ms5611.pressure = (((ms5611.up * sens ) >> 21) - off) >> 15;

    ms5611.baroAlt = (int32_t)((1.0f - pow(ms5611.pressure / 101325.0f, 0.190295f)) * 4433000.0f); // centimeter
}

/**
 * @brief Read the altitude value from MS5611.
 * @param baroAlt Pointer to store the altitude value.
 * @return None
 */
void MS5611_Read(int32_t* baroAlt)
{
    *baroAlt = ms5611.baroAlt;
}

/**
 * @brief Read the temperature value from MS5611.
 * @param temp Pointer to store the temperature value.
 * @return None
 */
void MS5611_ReadTemp(float* temp)
{
    *temp = (float)ms5611.temperature * 0.01f;
}

