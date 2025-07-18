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
  
  uint64_t productID = mlx.getProductID();
  Serial.print("Product ID: 0x");
  Serial.print((uint32_t)(productID >> 32), HEX);
  Serial.println((uint32_t)(productID & 0xFFFFFFFF), HEX);
}

void loop() {
  delay(500);
}