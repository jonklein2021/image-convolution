#include <cstdlib>
#include <ctime>
#include <cmath>
#include "image.hpp"

Image convolve(Image& src, const std::vector<float>& kernel) {
    const int M = static_cast<int>(sqrt(kernel.size())); // kernel size should be perfect square
    const int MM = M*M;

    int width = src.getWidth(), height = src.getHeight();

    Image dest(width, height);

    // TODO: "flip" the kernel to conform with the mathematical definition of convolution

    // "march" kernel along image colors, convolving each color channel linear combination style
    for (int row = 0; row < height; row++) { // y
        for (int col = 0; col < width; col++) { // x
            Color newColor;

            // iterate over kernel and apply to current image segment
            for (int i = 0; i < MM; i++) {
                newColor += src.getColor(col-(M/2)+(i%M), row-(M/2)+(i/M))*kernel[i];
            }
            
            dest.setColor(newColor, col, row);
        }
    }

    return dest;

}

int main() {
    Image original("samples/sample_640x426.bmp");

    const std::vector<float> boxBlur = {
        1.0/9.0, 1.0/9.0, 1.0/9.0, 
        1.0/9.0, 1.0/9.0, 1.0/9.0, 
        1.0/9.0, 1.0/9.0, 1.0/9.0
    };

    const std::vector<float> gaussianBlur3x3 = {
        1.0/16.0, 2.0/16.0, 1.0/16.0, 
        2.0/16.0, 4.0/16.0, 2.0/16.0, 
        1.0/16.0, 2.0/16.0, 1.0/16.0
    };

    const std::vector<float> gaussianBlur5x5 = {
        1.0/256.0, 4.0/256.0,  6.0/256.0,  4.0/256.0,  1.0/256.0, 
        4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0, 4.0/256.0, 
        6.0/256.0, 24.0/256.0, 36.0/256.0, 24.0/256.0, 6.0/256.0,
        4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0, 4.0/256.0, 
        1.0/256.0, 4.0/256.0,  6.0/256.0,  4.0/256.0,  1.0/256.0
    };

    Image blurred = convolve(original, gaussianBlur5x5);

    blurred.exportBmp("blurred.bmp");

}
