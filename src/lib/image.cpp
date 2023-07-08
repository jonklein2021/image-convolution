#include "image.hpp"
#include <iostream>
#include <fstream>

Color::Color() :
    r(0), g(0), b(0) {}

Color::Color(float r, float g, float b) :
    r(r), g(g), b(b) {}

Color::~Color() {}

Image::Image(int w, int h) : 
    width(w), height(h), colors(std::vector<Color>(w*h)) {}

Image::~Image() {}

Color Image::getColor(int x, int y) {
    return colors[x*width+y];
}

void Image::setColor(const Color& color, int x, int y) {
    colors[x*width+y].r = color.r;
    colors[x*width+y].g = color.g;
    colors[x*width+y].b = color.b;
}

void Image::exportBmp(const char* path) {
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);
}
