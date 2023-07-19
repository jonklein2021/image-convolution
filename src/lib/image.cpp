#include "image.hpp"
#include <iostream>
#include <fstream>

Color::Color() :
    r(0), g(0), b(0) {}

Color::Color(float r, float g, float b) :
    r(r), g(g), b(b) {}

Color::~Color() {}

Color Color::operator*(float k) {
    return Color(r*k, g*k, b*k);
}

Color Color::operator+(Color color) {
    return Color(r+color.r, g+color.g, b+color.b);
}

Color Color::operator+=(Color color) {
    r += color.r;
    g += color.g;
    b += color.b;
    return *this;
}

Image::Image(int w, int h) : 
    width(w), height(h), colors(std::vector<Color>(w*h)) {}

Image::Image(const char* path) {
    importBmp(path);
}

Image::~Image() {}

int Image::getWidth() {
    return width;
}

int Image::getHeight() {
    return height;
}

Color Image::getColor(int x, int y) {
    if (x < 0 && y < 0) return colors[0]; // top left corner
    if (x >= width && y < 0) return colors[width-1]; // top right corner
    if (x < 0 && y >= height) return colors[width*(height-1)]; // bottom left corner
    if (x >= width && y >= height) return colors[(width*height)-1]; // bottom right corner

    if (x < 0) return colors[width*y]; // left edge
    if (x >= width) { return colors[(width*(y+1))-1]; }// right edge
    if (y < 0) return colors[y]; // top edge
    if (y >= height) return colors[(width*(height-1))+x]; // bottom edge

    return colors[y*width+x];
}

void Image::setColor(const Color& color, int x, int y) {
    colors[y*width+x].r = color.r;
    colors[y*width+x].g = color.g;
    colors[y*width+x].b = color.b;
}

void Image::importBmp(const char* path) {
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);
    
    if (!f.is_open()) {
        throw std::runtime_error("ERROR: File could not be found");
    }

    const int fileHeaderSize = 14;
    const int infoHeaderSize = 40;

    unsigned char fileHeader[fileHeaderSize];
    f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
        throw std::runtime_error("ERROR: File is not a valid bitmap image");
    }
    
    unsigned char infoHeader[infoHeaderSize];
    f.read(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

    const int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
    width = infoHeader[4] + (infoHeader[5] << 8) + (infoHeader[6] << 16) + (infoHeader[7] << 24);
    height = infoHeader[8] + (infoHeader[9] << 8) + (infoHeader[10] << 16) + (infoHeader[11] << 24);

    colors.resize(width*height);

    const int paddingSize = ((4 - (width * 3) & 3) & 3); // = ((4 - (width * 3) % 4) % 4);
    for (int row = 0; row < height; row++) { // y
        for (int col = 0; col < width; col++) { // x
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);
            colors[row*width+col].r = static_cast<float>(color[2]) / 255.0f;
            colors[row*width+col].g = static_cast<float>(color[1]) / 255.0f;
            colors[row*width+col].b = static_cast<float>(color[0]) / 255.0f;
        }
        f.ignore(paddingSize);
    }
    f.close();

    std::cout << "File \"" << path << "\" read successfully" << std::endl;

}

void Image::exportBmp(const char* path) {
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if (!f.is_open()) {
        throw std::runtime_error("ERROR: File could not be created");
    }

    unsigned char bmpPad[3] = {0, 0, 0};
    const int paddingSize = ((4 - (width * 3) & 3) & 3); // = ((4 - (width * 3) % 4) % 4);
    const int fileHeaderSize = 14;
    const int infoHeaderSize = 40;
    const int fileSize = fileHeaderSize + infoHeaderSize + width*height*3 + paddingSize*height;

    unsigned char fileHeader[fileHeaderSize];

    // file type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // file size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    // reserved 1
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    // reserved 2
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    // pixel data offset
    fileHeader[10] = fileHeaderSize + infoHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char infoHeader[infoHeaderSize];
    
    // header size
    infoHeader[0] = infoHeaderSize;
    infoHeader[1] = 0;
    infoHeader[2] = 0;
    infoHeader[3] = 0;
    // image width
    infoHeader[4] = width;
    infoHeader[5] = width >> 8;
    infoHeader[6] = width >> 16;
    infoHeader[7] = width >> 24;
    // image height
    infoHeader[8] = height;
    infoHeader[9] = height >> 8;
    infoHeader[10] = height >> 16;
    infoHeader[11] = height >> 24;
    // planes
    infoHeader[12] = 1;
    infoHeader[13] = 0;
    // bits per pixel (RGB)
    infoHeader[14] = 24;
    infoHeader[15] = 0;
    // compression (no compression)
    infoHeader[16] = 0;
    infoHeader[17] = 0;
    infoHeader[18] = 0;
    infoHeader[19] = 0;
    // image size (no compression)
    infoHeader[20] = 0;
    infoHeader[21] = 0;
    infoHeader[22] = 0;
    infoHeader[23] = 0;
    // x pixels per meter
    infoHeader[24] = 0;
    infoHeader[25] = 0;
    infoHeader[26] = 0;
    infoHeader[27] = 0;
    // y pixels per meter
    infoHeader[28] = 0;
    infoHeader[29] = 0;
    infoHeader[30] = 0;
    infoHeader[31] = 0;
    // total colors (color palette not used)
    infoHeader[32] = 0;
    infoHeader[33] = 0;
    infoHeader[34] = 0;
    infoHeader[35] = 0;
    // important colors (generally ignored)
    infoHeader[36] = 0;
    infoHeader[37] = 0;
    infoHeader[38] = 0;
    infoHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

    for (int row = 0; row < height; row++) { // y
        for (int col = 0; col < width; col++) { // x
            unsigned char r = static_cast<unsigned char>(getColor(col, row).r * 255.0f);
            unsigned char g = static_cast<unsigned char>(getColor(col, row).g * 255.0f);
            unsigned char b = static_cast<unsigned char>(getColor(col, row).b * 255.0f);

            unsigned char color[] = {b, g, r};
            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingSize);
    }
    f.close();
    
    std::cout << "Image created at \"" << path << "\" successfully" << std::endl;
}
