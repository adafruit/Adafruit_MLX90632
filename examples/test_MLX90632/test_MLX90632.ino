// Basic test sketch for Adafruit MLX90632 Far Infrared Temperature Sensor

#include "Adafruit_MLX90632.h"

Adafruit_MLX90632 mlx = Adafruit_MLX90632();

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("Adafruit MLX90632 test");

  if (!mlx.begin()) {
    Serial.println("Failed to find MLX90632 chip");
    while (1) { delay(10); }
  }
  Serial.println("MLX90632 Found!");
  
  // Reset the device
  if (!mlx.reset()) {
    Serial.println("Device reset failed");
    while (1) { delay(10); }
  }
  Serial.println("Device reset: SUCCESS");
  
  uint64_t productID = mlx.getProductID();
  Serial.print("Product ID: 0x");
  Serial.print((uint32_t)(productID >> 32), HEX);
  Serial.println((uint32_t)(productID & 0xFFFFFFFF), HEX);
  
  uint16_t productCode = mlx.getProductCode();
  Serial.print("Product Code: 0x");
  Serial.println(productCode, HEX);
  
  uint16_t eepromVersion = mlx.getEEPROMVersion();
  Serial.print("EEPROM Version: 0x");
  Serial.println(eepromVersion, HEX);
  
  // Decode product code bits
  uint8_t fov = (productCode >> 8) & 0x3;
  uint8_t package = (productCode >> 5) & 0x7; 
  uint8_t accuracy = productCode & 0x1F;
  
  Serial.print("FOV: ");
  Serial.println(fov == 0 ? "50°" : "Unknown");
  
  Serial.print("Package: ");
  Serial.println(package == 1 ? "SFN 3x3" : "Unknown");
  
  Serial.print("Accuracy: ");
  if (accuracy == 1) {
    Serial.println("Medical");
  } else if (accuracy == 2) {
    Serial.println("Standard"); 
  } else {
    Serial.println("Unknown");
  }
  
  // Set and get mode (continuous)
  Serial.println("\n--- Mode Settings ---");
  if (!mlx.setMode(MLX90632_MODE_CONTINUOUS)) {
    Serial.println("Failed to set mode to Continuous");
    while (1) { delay(10); }
  }
  
  mlx90632_mode_t currentMode = mlx.getMode();
  Serial.print("Current mode: ");
  switch (currentMode) {
    case MLX90632_MODE_HALT:
      Serial.println("Halt");
      break;
    case MLX90632_MODE_SLEEPING_STEP:
      Serial.println("Sleeping Step");
      break;
    case MLX90632_MODE_STEP:
      Serial.println("Step");
      break;
    case MLX90632_MODE_CONTINUOUS:
      Serial.println("Continuous");
      break;
    default:
      Serial.println("Unknown");
  }
  
  // Set and get measurement select (medical)
  Serial.println("\n--- Measurement Select Settings ---");
  if (!mlx.setMeasurementSelect(MLX90632_MEAS_MEDICAL)) {
    Serial.println("Failed to set measurement select to Medical");
    while (1) { delay(10); }
  }
  
  mlx90632_meas_select_t currentMeasSelect = mlx.getMeasurementSelect();
  Serial.print("Current measurement select: ");
  switch (currentMeasSelect) {
    case MLX90632_MEAS_MEDICAL:
      Serial.println("Medical");
      break;
    case MLX90632_MEAS_EXTENDED_RANGE:
      Serial.println("Extended Range");
      break;
    default:
      Serial.println("Unknown");
  }
  
  // Set and get refresh rate (default to 2Hz)
  Serial.println("\n--- Refresh Rate Settings ---");
  if (!mlx.setRefreshRate(MLX90632_REFRESH_2HZ)) {
    Serial.println("Failed to set refresh rate to 2Hz");
    while (1) { delay(10); }
  }
  
  mlx90632_refresh_rate_t currentRefreshRate = mlx.getRefreshRate();
  Serial.print("Current refresh rate: ");
  switch (currentRefreshRate) {
    case MLX90632_REFRESH_0_5HZ:
      Serial.println("0.5 Hz");
      break;
    case MLX90632_REFRESH_1HZ:
      Serial.println("1 Hz");
      break;
    case MLX90632_REFRESH_2HZ:
      Serial.println("2 Hz");
      break;
    case MLX90632_REFRESH_4HZ:
      Serial.println("4 Hz");
      break;
    case MLX90632_REFRESH_8HZ:
      Serial.println("8 Hz");
      break;
    case MLX90632_REFRESH_16HZ:
      Serial.println("16 Hz");
      break;
    case MLX90632_REFRESH_32HZ:
      Serial.println("32 Hz");
      break;
    case MLX90632_REFRESH_64HZ:
      Serial.println("64 Hz");
      break;
    default:
      Serial.println("Unknown");
  }
  
  // Load calibration constants
  Serial.println("\n--- Calibration Constants ---");
  if (!mlx.getCalibrations()) {
    Serial.println("Failed to load calibration constants");
    while (1) { delay(10); }
  }
  Serial.println("Calibration constants loaded successfully");
}

void loop() {
  Serial.print("Device Busy: ");
  Serial.print(mlx.isBusy() ? "YES" : "NO");
  Serial.print("  EEPROM Busy: ");
  Serial.print(mlx.isEEPROMBusy() ? "YES" : "NO");
  Serial.print("  New Data: ");
  Serial.print(mlx.isNewData() ? "YES" : "NO");
  Serial.print("  Cycle Position: ");
  Serial.println(mlx.readCyclePosition());
  
  // Read ambient temperature
  double ambientTemp = mlx.getAmbientTemperature();
  Serial.print("Ambient Temperature: ");
  Serial.print(ambientTemp, 4);
  Serial.println(" °C");
  
  delay(500);
}