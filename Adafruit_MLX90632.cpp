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

#define MLX90632_DEBUG

/*!
 *    @brief  Instantiates a new MLX90632 class
 */
Adafruit_MLX90632::Adafruit_MLX90632() {
  TO0 = 25.0;  // Initialize previous object temperature
  TA0 = 25.0;  // Initialize previous ambient temperature
}

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
 *    @brief  Read the EEPROM version
 *    @return EEPROM version (16-bit value)
 */
uint16_t Adafruit_MLX90632::getEEPROMVersion() {
  Adafruit_BusIO_Register version_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_VERSION), 2, MSBFIRST, 2);
  return version_reg.read();
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
 *    @brief  Set the refresh rate for both measurement registers
 *    @param  refresh_rate The refresh rate to set
 *    @return True if both writes succeeded, false otherwise
 */
bool Adafruit_MLX90632::setRefreshRate(mlx90632_refresh_rate_t refresh_rate) {
  // Set refresh rate in EE_MEAS_1 register (bits 10:8)
  Adafruit_BusIO_Register meas1_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_MEAS_1), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits meas1_refresh_bits =
      Adafruit_BusIO_RegisterBits(&meas1_reg, 3, 8);
  
  if (!meas1_refresh_bits.write(refresh_rate)) {
    return false;
  }
  
  // Set refresh rate in EE_MEAS_2 register (bits 10:8)
  Adafruit_BusIO_Register meas2_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_MEAS_2), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits meas2_refresh_bits =
      Adafruit_BusIO_RegisterBits(&meas2_reg, 3, 8);
  
  return meas2_refresh_bits.write(refresh_rate);
}

/*!
 *    @brief  Get the refresh rate from EE_MEAS_1 register
 *    @return The current refresh rate
 */
mlx90632_refresh_rate_t Adafruit_MLX90632::getRefreshRate() {
  Adafruit_BusIO_Register meas1_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_MEAS_1), 2, MSBFIRST, 2);
  Adafruit_BusIO_RegisterBits meas1_refresh_bits =
      Adafruit_BusIO_RegisterBits(&meas1_reg, 3, 8);
  
  return (mlx90632_refresh_rate_t)meas1_refresh_bits.read();
}

/*!
 *    @brief  Helper function to read 32-bit values from consecutive registers
 *    @param  lsw_addr Address of the least significant word register
 *    @return 32-bit value (LSW + MSW)
 */
uint32_t Adafruit_MLX90632::read32BitRegister(uint16_t lsw_addr) {
  Adafruit_BusIO_Register lsw_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(lsw_addr), 2, MSBFIRST, 2);
  Adafruit_BusIO_Register msw_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(lsw_addr + 1), 2, MSBFIRST, 2);
  
  uint16_t lsw = lsw_reg.read();
  uint16_t msw = msw_reg.read();
  
  return ((uint32_t)msw << 16) | lsw;
}

/*!
 *    @brief  Read all calibration constants from EEPROM
 *    @return True if all reads succeeded, false otherwise
 */
bool Adafruit_MLX90632::getCalibrations() {
  // Read 32-bit calibration constants
  uint32_t ee_p_r = read32BitRegister(MLX90632_REG_EE_P_R_LSW);
  uint32_t ee_p_g = read32BitRegister(MLX90632_REG_EE_P_G_LSW);
  uint32_t ee_p_t = read32BitRegister(MLX90632_REG_EE_P_T_LSW);
  uint32_t ee_p_o = read32BitRegister(MLX90632_REG_EE_P_O_LSW);
  uint32_t ee_aa = read32BitRegister(MLX90632_REG_EE_AA_LSW);
  uint32_t ee_ab = read32BitRegister(MLX90632_REG_EE_AB_LSW);
  uint32_t ee_ba = read32BitRegister(MLX90632_REG_EE_BA_LSW);
  uint32_t ee_bb = read32BitRegister(MLX90632_REG_EE_BB_LSW);
  uint32_t ee_ca = read32BitRegister(MLX90632_REG_EE_CA_LSW);
  uint32_t ee_cb = read32BitRegister(MLX90632_REG_EE_CB_LSW);
  uint32_t ee_da = read32BitRegister(MLX90632_REG_EE_DA_LSW);
  uint32_t ee_db = read32BitRegister(MLX90632_REG_EE_DB_LSW);
  uint32_t ee_ea = read32BitRegister(MLX90632_REG_EE_EA_LSW);
  uint32_t ee_eb = read32BitRegister(MLX90632_REG_EE_EB_LSW);
  uint32_t ee_fa = read32BitRegister(MLX90632_REG_EE_FA_LSW);
  uint32_t ee_fb = read32BitRegister(MLX90632_REG_EE_FB_LSW);
  uint32_t ee_ga = read32BitRegister(MLX90632_REG_EE_GA_LSW);
  
  // Read 16-bit calibration constants
  Adafruit_BusIO_Register gb_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_GB), 2, MSBFIRST, 2);
  Adafruit_BusIO_Register ka_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_KA), 2, MSBFIRST, 2);
  Adafruit_BusIO_Register kb_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_KB), 2, MSBFIRST, 2);
  Adafruit_BusIO_Register ha_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_HA), 2, MSBFIRST, 2);
  Adafruit_BusIO_Register hb_reg =
      Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_EE_HB), 2, MSBFIRST, 2);
  
  // Convert to proper double values with scaling factors from datasheet  
  P_R = (double)(int32_t)ee_p_r * (double)pow(2, -8);    // 2^-8
  P_G = (double)(int32_t)ee_p_g * (double)pow(2, -20);   // 2^-20
  P_T = (double)(int32_t)ee_p_t * (double)pow(2, -44);   // 2^-44
  P_O = (double)(int32_t)ee_p_o * (double)pow(2, -8);    // 2^-8
  Aa = (double)(int32_t)ee_aa * (double)pow(2, -16);     // 2^-16
  Ab = (double)(int32_t)ee_ab * (double)pow(2, -8);      // 2^-8
  Ba = (double)(int32_t)ee_ba * (double)pow(2, -16);     // 2^-16
  Bb = (double)(int32_t)ee_bb * (double)pow(2, -8);      // 2^-8
  Ca = (double)(int32_t)ee_ca * (double)pow(2, -16);     // 2^-16
  Cb = (double)(int32_t)ee_cb * (double)pow(2, -8);      // 2^-8
  Da = (double)(int32_t)ee_da * (double)pow(2, -16);     // 2^-16
  Db = (double)(int32_t)ee_db * (double)pow(2, -8);      // 2^-8
  Ea = (double)(int32_t)ee_ea * (double)pow(2, -16);     // 2^-16
  Eb = (double)(int32_t)ee_eb * (double)pow(2, -8);      // 2^-8
  Fa = (double)(int32_t)ee_fa * (double)pow(2, -46);     // 2^-46
  Fb = (double)(int32_t)ee_fb * (double)pow(2, -36);     // 2^-36
  Ga = (double)(int32_t)ee_ga * (double)pow(2, -36);     // 2^-36
  
  // 16-bit signed values with scaling
  Gb = (double)(int16_t)gb_reg.read() * (double)pow(2, -10);  // 2^-10
  Ka = (double)(int16_t)ka_reg.read() * (double)pow(2, -10);  // 2^-10
  Kb = (int16_t)kb_reg.read();                                // No scaling
  Ha = (double)(int16_t)ha_reg.read() * (double)pow(2, -14);  // 2^-14
  Hb = (double)(int16_t)hb_reg.read() * (double)pow(2, -10);  // 2^-10
  
#ifdef MLX90632_DEBUG
  // Debug: Print calibration constants
  Serial.println(F("Calibration constants:"));
  Serial.print(F("  P_R = ")); Serial.println(P_R, 8);
  Serial.print(F("  P_G = ")); Serial.println(P_G, 8);
  Serial.print(F("  P_T = ")); Serial.println(P_T, 12);
  Serial.print(F("  P_O = ")); Serial.println(P_O, 8);
  Serial.print(F("  Aa = ")); Serial.println(Aa, 8);
  Serial.print(F("  Ab = ")); Serial.println(Ab, 8);
  Serial.print(F("  Ba = ")); Serial.println(Ba, 8);
  Serial.print(F("  Bb = ")); Serial.println(Bb, 8);
  Serial.print(F("  Ca = ")); Serial.println(Ca, 8);
  Serial.print(F("  Cb = ")); Serial.println(Cb, 8);
  Serial.print(F("  Da = ")); Serial.println(Da, 8);
  Serial.print(F("  Db = ")); Serial.println(Db, 8);
  Serial.print(F("  Ea = ")); Serial.println(Ea, 8);
  Serial.print(F("  Eb = ")); Serial.println(Eb, 8);
  Serial.print(F("  Fa = ")); Serial.println(Fa, 12);
  Serial.print(F("  Fb = ")); Serial.println(Fb, 10);
  Serial.print(F("  Ga = ")); Serial.println(Ga, 10);
  Serial.print(F("  Gb = ")); Serial.println(Gb, 8);
  Serial.print(F("  Ka = ")); Serial.println(Ka, 8);
  Serial.print(F("  Kb = ")); Serial.println(Kb);
  Serial.print(F("  Ha = ")); Serial.println(Ha, 8);
  Serial.print(F("  Hb = ")); Serial.println(Hb, 8);
#endif
  
  return true;
}

/*!
 *    @brief  Calculate ambient temperature
 *    @return Ambient temperature in degrees Celsius
 */
double Adafruit_MLX90632::getAmbientTemperature() {
  // Check measurement mode to determine which RAM registers to use
  mlx90632_meas_select_t meas_mode = getMeasurementSelect();
  
  int16_t ram_ambient, ram_ref;
  
  if (meas_mode == MLX90632_MEAS_EXTENDED_RANGE) {
    // Extended range mode: use RAM_54 and RAM_57
    Adafruit_BusIO_Register ram54_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_54), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram57_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_57), 2, MSBFIRST, 2);
    
    ram_ambient = (int16_t)ram54_reg.read();
    ram_ref = (int16_t)ram57_reg.read();
  } else {
    // Medical mode: use RAM_6 and RAM_9 (default)
    Adafruit_BusIO_Register ram6_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_6), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram9_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_9), 2, MSBFIRST, 2);
    
    ram_ambient = (int16_t)ram6_reg.read();
    ram_ref = (int16_t)ram9_reg.read();
  }
  
  // Pre-calculations for ambient temperature (same for both modes)
  // Gb = EE_Gb * 2^-10 (already calculated in getCalibrations())
  double VRTA = (double)ram_ref + Gb * ((double)ram_ambient / 12.0);
  double AMB = ((double)ram_ambient / 12.0) / VRTA * (double)pow(2, 19);
  
  // Calculate ambient temperature: P_O + (AMB - P_R)/P_G + P_T * (AMB - P_R)^2
  double amb_diff = AMB - P_R;
  double ambient_temp = P_O + (amb_diff / P_G) + P_T * (amb_diff * amb_diff);
  
#ifdef MLX90632_DEBUG
  // Debug output
  Serial.print(F("  Mode = ")); Serial.println(meas_mode == MLX90632_MEAS_EXTENDED_RANGE ? F("Extended") : F("Medical"));
  Serial.print(F("  RAM_ambient = ")); Serial.println(ram_ambient);
  Serial.print(F("  RAM_ref = ")); Serial.println(ram_ref);
  Serial.print(F("  Gb = ")); Serial.println(Gb, 8);
  Serial.print(F("  VRTA = ")); Serial.println(VRTA, 8);
  Serial.print(F("  AMB = ")); Serial.println(AMB, 8);
  Serial.print(F("  AMB - P_R = ")); Serial.println(amb_diff, 8);
  Serial.print(F("  Ambient Temp = ")); Serial.println(ambient_temp, 8);
#endif
  
  return ambient_temp;
}

/*!
 *    @brief  Calculate object temperature
 *    @return Object temperature in degrees Celsius or NaN if invalid cycle position
 */
double Adafruit_MLX90632::getObjectTemperature() {
  // Check measurement mode to determine which calculation to use
  mlx90632_meas_select_t meas_mode = getMeasurementSelect();
  
  double S;
  int16_t ram_ambient, ram_ref;
  
  if (meas_mode == MLX90632_MEAS_EXTENDED_RANGE) {
    // Extended range mode: use RAM_52-59
    Adafruit_BusIO_Register ram52_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_52), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram53_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_53), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram54_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_54), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram55_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_55), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram56_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_56), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram57_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_57), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram58_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_58), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram59_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_59), 2, MSBFIRST, 2);
    
    int16_t ram52 = (int16_t)ram52_reg.read();
    int16_t ram53 = (int16_t)ram53_reg.read();
    int16_t ram54 = (int16_t)ram54_reg.read();
    int16_t ram55 = (int16_t)ram55_reg.read();
    int16_t ram56 = (int16_t)ram56_reg.read();
    int16_t ram57 = (int16_t)ram57_reg.read();
    int16_t ram58 = (int16_t)ram58_reg.read();
    int16_t ram59 = (int16_t)ram59_reg.read();
    
    // Extended range S calculation
    S = ((double)ram52 - (double)ram53 - (double)ram55 + (double)ram56) / 2.0 + (double)ram58 + (double)ram59;
    ram_ambient = ram54;
    ram_ref = ram57;
    
  } else {
    // Medical mode: use cycle position and RAM_4-9
    uint8_t cycle_pos = readCyclePosition();
    
    Adafruit_BusIO_Register ram4_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_4), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram5_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_5), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram6_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_6), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram7_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_7), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram8_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_8), 2, MSBFIRST, 2);
    Adafruit_BusIO_Register ram9_reg =
        Adafruit_BusIO_Register(i2c_dev, swapBytes(MLX90632_REG_RAM_9), 2, MSBFIRST, 2);
    
    int16_t ram4 = (int16_t)ram4_reg.read();
    int16_t ram5 = (int16_t)ram5_reg.read();
    int16_t ram6 = (int16_t)ram6_reg.read();
    int16_t ram7 = (int16_t)ram7_reg.read();
    int16_t ram8 = (int16_t)ram8_reg.read();
    int16_t ram9 = (int16_t)ram9_reg.read();
    
    // Medical mode S calculation based on cycle position
    if (cycle_pos == 2) {
      S = ((double)ram4 + (double)ram5) / 2.0;
    } else if (cycle_pos == 1) {
      S = ((double)ram7 + (double)ram8) / 2.0;
    } else {
      // Invalid cycle position - return NaN
      return NAN;
    }
    
    ram_ambient = ram6;
    ram_ref = ram9;
  }
  
  // Pre-calculations for object temperature (same for both modes)
  // VRTO = ram_ref + Ka * (ram_ambient / 12)
  // Ka = EE_Ka * 2^-10 (already calculated in getCalibrations())
  double VRTO = (double)ram_ref + Ka * ((double)ram_ambient / 12.0);
  
  // STO = [S/12]/VRTO * 2^19
  double STO = ((S / 12.0) / VRTO) * (double)pow(2, 19);
  
  // Calculate AMB for ambient temperature (needed for TADUT)
  double VRTA = (double)ram_ref + Gb * ((double)ram_ambient / 12.0);
  double AMB = ((double)ram_ambient / 12.0) / VRTA * (double)pow(2, 19);
  
  // Additional temperature calculations
  double TADUT = (AMB - Eb) / Ea + 25.0;
  double TAK = TADUT + 273.15;
  double emissivity = 1.0;
  
  // For the first iteration, use current TADUT as TODUT approximation
  double TODUT = TADUT;  
  
  // Calculate final object temperature:
  // TO = pow( STO / (emiss * Fa * Ha * (1 + Ga * (TODUT - TO0) + Fb * (TADUT - TA0))) + TAK^4, 0.25) - 273.15 - Hb
  double denominator = emissivity * Fa * Ha * (1.0 + Ga * (TODUT - TO0) + Fb * (TADUT - TA0));
  double TAK4 = pow(TAK, 4);
  double TO_K4 = (STO / denominator) + TAK4;
  double TO = pow(TO_K4, 0.25) - 273.15 - Hb;
  
#ifdef MLX90632_DEBUG
  // Debug output
  Serial.print(F("  Mode = ")); Serial.println(meas_mode == MLX90632_MEAS_EXTENDED_RANGE ? F("Extended") : F("Medical"));
  if (meas_mode == MLX90632_MEAS_MEDICAL) {
    Serial.print(F("  Cycle Position = ")); Serial.println(readCyclePosition());
  }
  Serial.print(F("  RAM_ambient = ")); Serial.println(ram_ambient);
  Serial.print(F("  RAM_ref = ")); Serial.println(ram_ref);
  Serial.print(F("  S = ")); Serial.println(S, 8);
  Serial.print(F("  Ka = ")); Serial.println(Ka, 8);
  Serial.print(F("  VRTO = ")); Serial.println(VRTO, 8);
  Serial.print(F("  STO = ")); Serial.println(STO, 8);
  Serial.print(F("  VRTA = ")); Serial.println(VRTA, 8);
  Serial.print(F("  AMB = ")); Serial.println(AMB, 8);
  Serial.print(F("  TADUT = ")); Serial.println(TADUT, 8);
  Serial.print(F("  TODUT = ")); Serial.println(TODUT, 8);
  Serial.print(F("  TAK = ")); Serial.println(TAK, 8);
  Serial.print(F("  TAK^4 = "));
  if (TAK4 >= 1e9) {
    Serial.print(TAK4 / 1e9, 2);
    Serial.println(F("e+09"));
  } else if (TAK4 >= 1e6) {
    Serial.print(TAK4 / 1e6, 2);
    Serial.println(F("e+06"));
  } else {
    Serial.println(TAK4, 2);
  }
  Serial.print(F("  TO0 = ")); Serial.println(TO0, 8);
  Serial.print(F("  TA0 = ")); Serial.println(TA0, 8);
  Serial.print(F("  Emissivity = ")); Serial.println(emissivity, 8);
  Serial.print(F("  Denominator = ")); Serial.println(denominator, 8);
  Serial.print(F("  TO_K^4 = "));
  if (TO_K4 >= 1e9) {
    Serial.print(TO_K4 / 1e9, 2);
    Serial.println(F("e+09"));
  } else if (TO_K4 >= 1e6) {
    Serial.print(TO_K4 / 1e6, 2);
    Serial.println(F("e+06"));
  } else {
    Serial.println(TO_K4, 2);
  }
  Serial.print(F("  TO = ")); Serial.println(TO, 8);
#endif
  
  // Update TO0 and TA0 with current measurements for next calculation
  TO0 = TO;     // Use calculated object temperature
  TA0 = TADUT;  // Update with current ambient temperature calculation
  
  return TO;
}

/*!
 *    @brief  Byte swap helper for register addresses
 *    @param  value 16-bit value to swap
 *    @return Byte-swapped value
 */
uint16_t Adafruit_MLX90632::swapBytes(uint16_t value) {
  return (value << 8) | (value >> 8);
}