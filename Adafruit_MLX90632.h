/*!
 *  @file Adafruit_MLX90632.h
 *
 * 	I2C Driver for MLX90632 Far Infrared Temperature Sensor
 *
 * 	This is a library for the Adafruit MLX90632 breakout:
 * 	http://www.adafruit.com/products
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *	BSD license (see license.txt)
 */

#ifndef _ADAFRUIT_MLX90632_H
#define _ADAFRUIT_MLX90632_H

#include "Arduino.h"
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Wire.h>

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define MLX90632_DEFAULT_ADDR 0x3A ///< MLX90632 default i2c address
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
// EEPROM addresses
#define MLX90632_REG_MELEXIS_RESERVED0 0x2400 ///< Melexis reserved
#define MLX90632_REG_MELEXIS_RESERVED1 0x2401 ///< Melexis reserved
#define MLX90632_REG_MELEXIS_RESERVED2 0x2402 ///< Melexis reserved
#define MLX90632_REG_MELEXIS_RESERVED3 0x2403 ///< Melexis reserved
#define MLX90632_REG_MELEXIS_RESERVED4 0x2404 ///< Melexis reserved
#define MLX90632_REG_ID0 0x2405 ///< Chip ID
#define MLX90632_REG_ID1 0x2406 ///< Chip ID
#define MLX90632_REG_ID2 0x2407 ///< Chip ID
#define MLX90632_REG_ID_CRC16 0x2408 ///< CRC
#define MLX90632_REG_EE_PRODUCT_CODE 0x2409 ///< Sensor information
#define MLX90632_REG_MELEXIS_RESERVED10 0x240A ///< Melexis reserved
#define MLX90632_REG_EE_VERSION 0x240B ///< EEPROM version
#define MLX90632_REG_EE_P_R_LSW 0x240C ///< P_R calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_P_R_MSW 0x240D ///< P_R calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_P_G_LSW 0x240E ///< P_G calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_P_G_MSW 0x240F ///< P_G calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_P_T_LSW 0x2410 ///< P_T calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_P_T_MSW 0x2411 ///< P_T calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_P_O_LSW 0x2412 ///< P_O calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_P_O_MSW 0x2413 ///< P_O calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_AA_LSW 0x2414 ///< Aa calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_AA_MSW 0x2415 ///< Aa calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_AB_LSW 0x2416 ///< Ab calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_AB_MSW 0x2417 ///< Ab calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_BA_LSW 0x2418 ///< Ba calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_BA_MSW 0x2419 ///< Ba calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_BB_LSW 0x241A ///< Bb calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_BB_MSW 0x241B ///< Bb calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_CA_LSW 0x241C ///< Ca calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_CA_MSW 0x241D ///< Ca calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_CB_LSW 0x241E ///< Cb calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_CB_MSW 0x241F ///< Cb calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_DA_LSW 0x2420 ///< Da calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_DA_MSW 0x2421 ///< Da calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_DB_LSW 0x2422 ///< Db calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_DB_MSW 0x2423 ///< Db calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_EA_LSW 0x2424 ///< Ea calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_EA_MSW 0x2425 ///< Ea calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_EB_LSW 0x2426 ///< Eb calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_EB_MSW 0x2427 ///< Eb calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_FA_LSW 0x2428 ///< Fa calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_FA_MSW 0x2429 ///< Fa calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_FB_LSW 0x242A ///< Fb calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_FB_MSW 0x242B ///< Fb calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_GA_LSW 0x242C ///< Ga calibration constant (16-bit, Least Significant Word)
#define MLX90632_REG_EE_GA_MSW 0x242D ///< Ga calibration constant (16-bit, Most Significant Word)
#define MLX90632_REG_EE_GB 0x242E ///< Gb calibration constant (16-bit)
#define MLX90632_REG_EE_KA 0x242F ///< Ka calibration constant (16-bit)
#define MLX90632_REG_EE_KB 0x2430 ///< Kb calibration constant (16-bit)
#define MLX90632_REG_MELEXIS_RESERVED49 0x2431 ///< Melexis reserved
#define MLX90632_REG_MELEXIS_RESERVED127 0x247F ///< Melexis reserved
#define MLX90632_REG_MELEXIS_RESERVED128 0x2480 ///< Melexis reserved
#define MLX90632_REG_EE_HA 0x2481 ///< Ha Customer calibration constant (16 bit)
#define MLX90632_REG_EE_HB 0x2482 ///< Hb Customer calibration constant (16 bit)
#define MLX90632_REG_MELEXIS_RESERVED131 0x2483 ///< Melexis reserved
#define MLX90632_REG_CUSTOMER_DATA_START 0x24C0 ///< Customer data start
#define MLX90632_REG_CUSTOMER_DATA_END 0x24CF ///< Customer data end
#define MLX90632_REG_MELEXIS_RESERVED208 0x24D0 ///< Melexis reserved
#define MLX90632_REG_EE_CONTROL 0x24D4 ///< EEPROM Control register, measurement control
#define MLX90632_REG_EE_I2C_ADDRESS 0x24D5 ///< I2C slave address >> 1
#define MLX90632_REG_MELEXIS_RESERVED214 0x24D6 ///< Melexis reserved
#define MLX90632_REG_EE_MEAS_1 0x24E1 ///< Measurement settings 1
#define MLX90632_REG_EE_MEAS_2 0x24E2 ///< Measurement settings 2

// Control and Status registers
#define MLX90632_REG_I2C_ADDRESS 0x3000 ///< I2C slave address >> 1
#define MLX90632_REG_CONTROL 0x3001 ///< Control register, measurement mode
#define MLX90632_REG_STATUS 0x3FFF ///< Status register: data available

// RAM addresses
#define MLX90632_REG_RAM_1 0x4000 ///< Raw data 1
#define MLX90632_REG_RAM_2 0x4001 ///< Raw data 2
#define MLX90632_REG_RAM_3 0x4002 ///< Raw data 3
#define MLX90632_REG_RAM_4 0x4003 ///< Raw data 4
#define MLX90632_REG_RAM_5 0x4004 ///< Raw data 5
#define MLX90632_REG_RAM_6 0x4005 ///< Raw data 6
#define MLX90632_REG_RAM_7 0x4006 ///< Raw data 7
#define MLX90632_REG_RAM_8 0x4007 ///< Raw data 8
#define MLX90632_REG_RAM_9 0x4008 ///< Raw data 9
#define MLX90632_REG_RAM_52 0x4033 ///< Raw data 52
#define MLX90632_REG_RAM_53 0x4034 ///< Raw data 53
#define MLX90632_REG_RAM_54 0x4035 ///< Raw data 54
#define MLX90632_REG_RAM_55 0x4036 ///< Raw data 55
#define MLX90632_REG_RAM_56 0x4037 ///< Raw data 56
#define MLX90632_REG_RAM_57 0x4038 ///< Raw data 57
#define MLX90632_REG_RAM_58 0x4039 ///< Raw data 58
#define MLX90632_REG_RAM_59 0x403A ///< Raw data 59
#define MLX90632_REG_RAM_60 0x403B ///< Raw data 60
/*=========================================================================*/

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            MLX90632 Far Infrared Temperature Sensor
 */
class Adafruit_MLX90632 {
public:
  Adafruit_MLX90632();
  ~Adafruit_MLX90632();
  bool begin(uint8_t i2c_addr = MLX90632_DEFAULT_ADDR, TwoWire *wire = &Wire);
  uint64_t getProductID();

private:
  Adafruit_I2CDevice *i2c_dev; ///< Pointer to I2C bus interface
  uint16_t swapBytes(uint16_t value); ///< Byte swap helper for register addresses
};

#endif