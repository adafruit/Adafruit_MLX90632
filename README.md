# Adafruit MLX90632 [![Build Status](https://github.com/adafruit/Adafruit_MLX90632/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_MLX90632/actions)

Arduino library for the MLX90632 Far Infrared Temperature Sensor

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

Written by Adafruit Industries.

MIT license, check license.txt for more information.
All text above must be included in any redistribution.