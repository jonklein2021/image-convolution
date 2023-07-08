#include "image.hpp"

int main() {
    const int width = 640;
    const int height = 480;

    Image image(width, height);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            image.setColor(Color((float) col / (float) width, 1.0f - (float) col / (float) width, (float) row / (float) height), row, col);
        }
    }
    image.exportBmp("image.bmp");
}
