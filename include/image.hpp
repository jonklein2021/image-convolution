#pragma once

#include <vector>

struct Color {
    float r, g, b;

    Color();
    Color(float r, float g, float b);
    ~Color();
};

class Image {
    private:
        int width, height;
        std::vector<Color> colors;
    public:
        Image();
        Image(int w, int h);
        ~Image();
        Color getColor(int x, int y);
        void setColor(const Color& c, int x, int y);
        void exportBmp(const char* path);
};
