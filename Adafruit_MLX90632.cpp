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
 *    @brief  Read the product code
 *    @return Product code (16-bit value)
 */
uint16_t Adafruit_MLX90632::getProductCode() {
  Adafruit_BusIO_Register product_code_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_PRODUCT_CODE), 2, MSBFIRST, 2);
  return product_code_reg.read();
}

/*!
 *    @brief  Start a single measurement (SOC)
 *    @return True if write succeeded, false otherwise
 */
bool Adafruit_MLX90632::startSingleMeasurement() {
  Adafruit_BusIO_Register control_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_CONTROL), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits soc_bit =
      Adafruit_BusIO_RegisterBits(&control_reg, 1, 3);
  
  return soc_bit.write(1);
}

/*!
 *    @brief  Start a full measurement table (SOB)  
 *    @return True if write succeeded, false otherwise
 */
bool Adafruit_MLX90632::startFullMeasurement() {
  Adafruit_BusIO_Register control_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_CONTROL), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits sob_bit =
      Adafruit_BusIO_RegisterBits(&control_reg, 1, 11);
  
  return sob_bit.write(1);
}

/*!
 *    @brief  Set the measurement mode
 *    @param  mode The measurement mode to set
 *    @return True if write succeeded, false otherwise
 */
bool Adafruit_MLX90632::setMode(mlx90632_mode_t mode) {
  Adafruit_BusIO_Register control_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_CONTROL), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits mode_bits =
      Adafruit_BusIO_RegisterBits(&control_reg, 2, 1);
  
  return mode_bits.write(mode);
}

/*!
 *    @brief  Get the measurement mode
 *    @return The current measurement mode
 */
mlx90632_mode_t Adafruit_MLX90632::getMode() {
  Adafruit_BusIO_Register control_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_CONTROL), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits mode_bits =
      Adafruit_BusIO_RegisterBits(&control_reg, 2, 1);
  
  return (mlx90632_mode_t)mode_bits.read();
}

/*!
 *    @brief  Set the measurement select type
 *    @param  meas_select The measurement select type to set
 *    @return True if write succeeded, false otherwise
 */
bool Adafruit_MLX90632::setMeasurementSelect(mlx90632_meas_select_t meas_select) {
  Adafruit_BusIO_Register control_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_CONTROL), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits meas_select_bits =
      Adafruit_BusIO_RegisterBits(&control_reg, 5, 4);
  
  return meas_select_bits.write(meas_select);
}

/*!
 *    @brief  Get the measurement select type
 *    @return The current measurement select type
 */
mlx90632_meas_select_t Adafruit_MLX90632::getMeasurementSelect() {
  Adafruit_BusIO_Register control_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_CONTROL), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits meas_select_bits =
      Adafruit_BusIO_RegisterBits(&control_reg, 5, 4);
  
  return (mlx90632_meas_select_t)meas_select_bits.read();
}

/*!
 *    @brief  Check if device is busy with measurement
 *    @return True if device is busy, false otherwise
 */
bool Adafruit_MLX90632::isBusy() {
  Adafruit_BusIO_Register status_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_STATUS), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits device_busy_bit =
      Adafruit_BusIO_RegisterBits(&status_reg, 1, 10);
  
  return device_busy_bit.read();
}

/*!
 *    @brief  Check if EEPROM is busy
 *    @return True if EEPROM is busy, false otherwise
 */
bool Adafruit_MLX90632::isEEPROMBusy() {
  Adafruit_BusIO_Register status_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_STATUS), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits eeprom_busy_bit =
      Adafruit_BusIO_RegisterBits(&status_reg, 1, 9);
  
  return eeprom_busy_bit.read();
}

/*!
 *    @brief  Reset device using addressed reset command
 *    @return True if reset succeeded, false otherwise
 */
bool Adafruit_MLX90632::reset() {
  // Send addressed reset command: 0x3005, 0x0006
  uint8_t reset_cmd[] = {0x30, 0x05, 0x00, 0x06};
  if (!i2c_dev->write(reset_cmd, 4)) {
    return false;
  }
  
  // Wait for reset to complete (at least 150us as per datasheet)
  delay(1);
  
  return true;
}

/*!
 *    @brief  Read the cycle position
 *    @return Current cycle position (0-31)
 */
uint8_t Adafruit_MLX90632::readCyclePosition() {
  Adafruit_BusIO_Register status_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_STATUS), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits cycle_position_bits =
      Adafruit_BusIO_RegisterBits(&status_reg, 5, 2);
  
  return cycle_position_bits.read();
}

/*!
 *    @brief  Reset the new data flag to 0
 *    @return True if write succeeded, false otherwise
 */
bool Adafruit_MLX90632::resetNewData() {
  Adafruit_BusIO_Register status_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_STATUS), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits new_data_bit =
      Adafruit_BusIO_RegisterBits(&status_reg, 1, 0);
  
  return new_data_bit.write(0);
}

/*!
 *    @brief  Check if new data is available
 *    @return True if new data is available, false otherwise
 */
bool Adafruit_MLX90632::isNewData() {
  Adafruit_BusIO_Register status_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_STATUS), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits new_data_bit =
      Adafruit_BusIO_RegisterBits(&status_reg, 1, 0);
  
  return new_data_bit.read();
}

/*!
 *    @brief  Byte swap helper for register addresses
 *    @param  value 16-bit value to swap
 *    @return Byte-swapped value
 */
uint16_t Adafruit_MLX90632::swapBytes(uint16_t value) {
  return (value << 8) | (value >> 8);
}