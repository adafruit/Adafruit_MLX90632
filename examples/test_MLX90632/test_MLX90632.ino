// Basic test sketch for Adafruit MLX90632 Far Infrared Temperature Sensor

#include "Adafruit_MLX90632.h"

Adafruit_MLX90632 mlx = Adafruit_MLX90632();

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println(F("Adafruit MLX90632 test"));

  if (!mlx.begin()) {
    Serial.println(F("Failed to find MLX90632 chip"));
    while (1) { delay(10); }
  }
  Serial.println(F("MLX90632 Found!"));
  
  // Reset the device
  if (!mlx.reset()) {
    Serial.println(F("Device reset failed"));
    while (1) { delay(10); }
  }
  Serial.println(F("Device reset: SUCCESS"));
  
  uint64_t productID = mlx.getProductID();
  Serial.print(F("Product ID: 0x"));
  Serial.print((uint32_t)(productID >> 32), HEX);
  Serial.println((uint32_t)(productID & 0xFFFFFFFF), HEX);
  
  uint16_t productCode = mlx.getProductCode();
  Serial.print(F("Product Code: 0x"));
  Serial.println(productCode, HEX);
  
  uint16_t eepromVersion = mlx.getEEPROMVersion();
  Serial.print(F("EEPROM Version: 0x"));
  Serial.println(eepromVersion, HEX);
  
  // Decode product code bits
  uint8_t fov = (productCode >> 8) & 0x3;
  uint8_t package = (productCode >> 5) & 0x7; 
  uint8_t accuracy = productCode & 0x1F;
  
  Serial.print(F("FOV: "));
  Serial.println(fov == 0 ? F("50°") : F("Unknown"));
  
  Serial.print(F("Package: "));
  Serial.println(package == 1 ? F("SFN 3x3") : F("Unknown"));
  
  Serial.print(F("Accuracy: "));
  if (accuracy == 1) {
    Serial.println(F("Medical"));
  } else if (accuracy == 2) {
    Serial.println(F("Standard")); 
  } else {
    Serial.println(F("Unknown"));
  }
  
  // Set and get mode (continuous)
  Serial.println(F("\n--- Mode Settings ---"));
  if (!mlx.setMode(MLX90632_MODE_CONTINUOUS)) {
    Serial.println(F("Failed to set mode to Continuous"));
    while (1) { delay(10); }
  }
  
  mlx90632_mode_t currentMode = mlx.getMode();
  Serial.print(F("Current mode: "));
  switch (currentMode) {
    case MLX90632_MODE_HALT:
      Serial.println(F("Halt"));
      break;
    case MLX90632_MODE_SLEEPING_STEP:
      Serial.println(F("Sleeping Step"));
      break;
    case MLX90632_MODE_STEP:
      Serial.println(F("Step"));
      break;
    case MLX90632_MODE_CONTINUOUS:
      Serial.println(F("Continuous"));
      break;
    default:
      Serial.println(F("Unknown"));
  }
  
  // Set and get measurement select (medical)
  Serial.println(F("\n--- Measurement Select Settings ---"));
  if (!mlx.setMeasurementSelect(MLX90632_MEAS_MEDICAL)) {
    Serial.println(F("Failed to set measurement select to Medical"));
    while (1) { delay(10); }
  }
  
  mlx90632_meas_select_t currentMeasSelect = mlx.getMeasurementSelect();
  Serial.print(F("Current measurement select: "));
  switch (currentMeasSelect) {
    case MLX90632_MEAS_MEDICAL:
      Serial.println(F("Medical"));
      break;
    case MLX90632_MEAS_EXTENDED_RANGE:
      Serial.println(F("Extended Range"));
      break;
    default:
      Serial.println(F("Unknown"));
  }
  
  // Set and get refresh rate (default to 2Hz)
  Serial.println(F("\n--- Refresh Rate Settings ---"));
  if (!mlx.setRefreshRate(MLX90632_REFRESH_2HZ)) {
    Serial.println(F("Failed to set refresh rate to 2Hz"));
    while (1) { delay(10); }
  }
  
  mlx90632_refresh_rate_t currentRefreshRate = mlx.getRefreshRate();
  Serial.print(F("Current refresh rate: "));
  switch (currentRefreshRate) {
    case MLX90632_REFRESH_0_5HZ:
      Serial.println(F("0.5 Hz"));
      break;
    case MLX90632_REFRESH_1HZ:
      Serial.println(F("1 Hz"));
      break;
    case MLX90632_REFRESH_2HZ:
      Serial.println(F("2 Hz"));
      break;
    case MLX90632_REFRESH_4HZ:
      Serial.println(F("4 Hz"));
      break;
    case MLX90632_REFRESH_8HZ:
      Serial.println(F("8 Hz"));
      break;
    case MLX90632_REFRESH_16HZ:
      Serial.println(F("16 Hz"));
      break;
    case MLX90632_REFRESH_32HZ:
      Serial.println(F("32 Hz"));
      break;
    case MLX90632_REFRESH_64HZ:
      Serial.println(F("64 Hz"));
      break;
    default:
      Serial.println(F("Unknown"));
  }
  
  // Load calibration constants
  Serial.println(F("\n--- Calibration Constants ---"));
  if (!mlx.getCalibrations()) {
    Serial.println(F("Failed to load calibration constants"));
    while (1) { delay(10); }
  }
  Serial.println(F("Calibration constants loaded successfully"));
}

void loop() {
  Serial.print(F("Device Busy: "));
  Serial.print(mlx.isBusy() ? F("YES") : F("NO"));
  Serial.print(F("  EEPROM Busy: "));
  Serial.print(mlx.isEEPROMBusy() ? F("YES") : F("NO"));
  Serial.print(F("  New Data: "));
  Serial.print(mlx.isNewData() ? F("YES") : F("NO"));
  Serial.print(F("  Cycle Position: "));
  Serial.println(mlx.readCyclePosition());
  
  // Read ambient temperature
  double ambientTemp = mlx.getAmbientTemperature();
  Serial.print(F("Ambient Temperature: "));
  Serial.print(ambientTemp, 4);
  Serial.println(F(" °C"));
  
  // Read object temperature
  double objectTemp = mlx.getObjectTemperature();
  Serial.print(F("Object Temperature: "));
  if (isnan(objectTemp)) {
    Serial.println(F("NaN (invalid cycle position)"));
  } else {
    Serial.print(objectTemp, 4);
    Serial.println(F(" °C"));
  }
  
  delay(500);
}