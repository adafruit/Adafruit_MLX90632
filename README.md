# Adafruit MLX90632 [![Build Status](https://github.com/adafruit/Adafruit_MLX90632/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_MLX90632/actions)[![Documentation](https://github.com/adafruit/ci-arduino/blob/master/assets/doxygen_badge.svg)](http://adafruit.github.io/Adafruit_MLX90632/html/index.html)

Arduino library for the MLX90632 Far Infrared Temperature Sensor

This library provides a complete interface for the MLX90632 Far Infrared Temperature Sensor, supporting both medical and extended range measurement modes with automatic calibration loading and comprehensive temperature calculations using the Stefan-Boltzmann law.

## Features

- Complete MLX90632 register control and status monitoring  
- Support for all measurement modes: Continuous, Step, and Sleeping Step
- Automatic calibration constant loading from EEPROM
- Both medical and extended range measurement support
- Stefan-Boltzmann law temperature calculations for accurate object temperature
- Double precision calibration with proper scaling factors
- Efficient new data flag handling for optimal performance
- Debug output control with preprocessor directives
- Hardware tested and verified functionality

## Dependencies
 * [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)

## Contributing

Contributions are welcome! Please read our [Code of Conduct](https://github.com/adafruit/Adafruit_MLX90632/blob/main/CODE_OF_CONDUCT.md)
before contributing to help this project stay welcoming.

## Documentation and doxygen
Documentation is produced by doxygen. Contributions should include documentation for any new features.

## Formatting and clang-format
This library uses [`clang-format`](https://releases.llvm.org/download.html) to standardize the formatting of `.cpp` and `.h` files. 
Contributions should be formatted using `clang-format`:

The `-i` flag will make the changes to the file.
```bash
clang-format -i *.cpp *.h
```
If you prefer to make the changes yourself, running `clang-format` without the `-i` flag will print out a formatted version of the file. You can save this to a file and diff it against the original to see the changes.

Note that the formatting output by `clang-format` is what the automated formatting checker will expect. Any pull requests that don't conform to the formatting style will not pass the automated checks.

## About this Driver

Written by Limor "Ladyada" Fried for Adafruit Industries with assistance from Claude Code.

MIT license, check LICENSE for more information.
All text above must be included in any redistribution.
