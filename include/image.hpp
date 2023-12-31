#pragma once

#include <vector>

class Color {
    public:
        float r, g, b; // in [0, 1] NOT [0, 255]

        Color();
        Color(float r, float g, float b);
        ~Color();
        Color operator*(float k);
        Color operator+(Color color);
        Color operator+=(Color color);
};

class Image {
    private:
        int width, height;
        std::vector<Color> colors;
    public:
        Image(int w, int h);
        Image(const char* path);
        ~Image();
        int getWidth();
        int getHeight();
        Color getColor(int x, int y);
        void setColor(const Color& c, int x, int y);
        void importBmp(const char* path);
        void exportBmp(const char* path);
};
