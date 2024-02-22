#ifndef COLORMAPPER_h
#define COLORMAPPER_h


class ColorMapper {
    private:

    class Color {
        public:
            Color() : r(0), g(0), b(0) {}
            Color(int r, int g, int b) : r(r), g(g), b(b) {}
            int r, g, b;
    };
    
    Color colors[3];
    public:
        ColorMapper() {
            colors[0] = Color(255, 0, 0);
            colors[1] = Color(0, 255, 0);
            colors[2] = Color(0, 0, 255);
        }

        Color getColor(int index) {
            return colors[index];
        }

        unsigned long colorToHex(Color color) {
            return ((unsigned long)color.r << 16) | ((unsigned long)color.g << 8) | color.b;
        }
};


#endif // COLORMAPPER_h