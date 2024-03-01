#ifndef COLORMAPPER_h
#define COLORMAPPER_h

#include <SimpleVector.h>
class Color {
    public:
        Color() : r(0), g(0), b(0) {}
        Color(byte r, byte g, byte b) : r(r), g(g), b(b) {}
        byte r, g, b;

        byte getR() { return r; }
        byte getG() { return g; }
        byte getB() { return b; }

        bool operator==(const Color& other) const {
            return r == other.r && g == other.g && b == other.b;
        }
};

class ColorMapper {
    private:
            Color colors[3];
            byte colorCount = 0;
            SimpleVector <Color> colorList = SimpleVector <Color>();
        public:
            ColorMapper() {
                colors[0] = Color(255, 0, 0);
                colors[1] = Color(0, 255, 0);
                colors[2] = Color(0, 0, 255);
            }

            ColorMapper(byte color1, byte color2, byte color3) {
                colors[0] = hexToColor(color1);
                colors[1] = hexToColor(color2);
                colors[2] = hexToColor(color3);
            }

        Color getColor(byte index) {
            return colors[index];
        }

        void addColor(Color color) {
            // Add code here to add the color to the colors array
            if (colorCount < 15) {
                colorList.put(color);
                colorCount++;
            } else {
                // Handle error: array is full
                Serial.println("Error: array is full");
            }
        }

        void removeColor(byte index) {
            // Add code here to remove the color at the specified index from the colors array
            if (colorCount > 0) {
                colorList.remove(colorList[index]);
                colorCount--;
            } else {
                // Handle error: array is empty
                Serial.println("Error: array is empty");
            }
        }

        unsigned long colorToHex(Color color) {
            return ((unsigned long)color.r << 16) | ((unsigned long)color.g << 8) | color.b;
        }

        Color hexToColor(unsigned long hex) {
            return Color((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
        }
};

#endif // COLORMAPPER_h