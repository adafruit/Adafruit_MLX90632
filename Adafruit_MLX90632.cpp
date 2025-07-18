/*!
 *  @file Adafruit_MLX90632.cpp
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

#include "Adafruit_MLX90632.h"

/*!
 *    @brief  Instantiates a new MLX90632 class
 */
Adafruit_MLX90632::Adafruit_MLX90632() {}

/*!
 *    @brief  Cleans up the MLX90632
 */
Adafruit_MLX90632::~Adafruit_MLX90632() {
  if (i2c_dev) {
    delete i2c_dev;
  }
}

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_address
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MLX90632::begin(uint8_t i2c_address, TwoWire *wire) {
  if (i2c_dev) {
    delete i2c_dev;
  }
  i2c_dev = new Adafruit_I2CDevice(i2c_address, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

  Adafruit_BusIO_Register product_code_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_PRODUCT_CODE), 2, MSBFIRST, 2);
  uint16_t product_code = product_code_reg.read();
  
  if (product_code == 0xFFFF || product_code == 0x0000) {
    return false;
  }

  return true;
}

/*!
 *    @brief  Read the 48-bit product ID
 *    @return Product ID (48-bit value in uint64_t)
 */
uint64_t Adafruit_MLX90632::getProductID() {
  Adafruit_BusIO_Register id0_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_ID0), 2, MSBFIRST, 2);
  Adafruit_BusIO_Register id1_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_ID1), 2, MSBFIRST, 2);
  Adafruit_BusIO_Register id2_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_ID2), 2, MSBFIRST, 2);
  
  uint16_t id0 = id0_reg.read();
  uint16_t id1 = id1_reg.read();
  uint16_t id2 = id2_reg.read();
  
  return ((uint64_t)id2 << 32) | ((uint64_t)id1 << 16) | id0;
}

/*!
 *    @brief  Byte swap helper for register addresses
 *    @param  value 16-bit value to swap
 *    @return Byte-swapped value
 */
uint16_t Adafruit_MLX90632::swapBytes(uint16_t value) {
  return (value << 8) | (value >> 8);
}