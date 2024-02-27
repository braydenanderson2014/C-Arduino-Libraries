# ColorMapper Library

The ColorMapper library is designed for easy color management in Arduino and PlatformIO projects. It allows the creation, manipulation, and conversion of colors between different formats.

## Features

- Define colors using RGB values.
- Convert colors to and from hexadecimal format.
- Manage a dynamic list of colors.
- Supports up to 15 colors in the dynamic list.

## Installation

### For Arduino

1. Download the `ColorMapper` library.
2. In the Arduino IDE, go to Sketch > Include Library > Add .ZIP Library and select the downloaded file.

### For PlatformIO

1. Add `ColorMapper` to your `platformio.ini` dependencies.

lib_deps =
  git_user/ColorMapper


```c++

  #include "ColorMapper.h"

ColorMapper colorMapper;

void setup() {
  Serial.begin(9600);
  
  // Adding a color
  Color myColor(128, 128, 128); // Grey color
  colorMapper.addColor(myColor);
  
  // Getting a color
  Color firstColor = colorMapper.getColor(0);
  Serial.print("First color RGB: ");
  Serial.print(firstColor.getR());
  Serial.print(", ");
  Serial.print(firstColor.getG());
  Serial.print(", ");
  Serial.println(firstColor.getB());
  
  // Converting color to Hex
  unsigned long hex = colorMapper.colorToHex(firstColor);
  Serial.print("Hex: #");
  Serial.println(hex, HEX);
}

void loop() {
  // Your code here
}
```


# ARDUINO
## Changelog
### Version 1.0.0 (YYYY-MM-DD)
Initial release: Basic color management.
1.1.0 (YYYY-MM-DD)
Added dynamic color list management.


# PLATFORMIO
## Changelog
### Version 1.0.0 (YYYY-MM-DD)
* Initial release compatible with PlatformIO projects.



Contributing
Contributions to the ColorMapper library are welcome. Please submit pull requests or open issues to discuss proposed changes.