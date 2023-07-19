#include <cstdlib>
#include <ctime>
#include <cmath>
#include "image.hpp"

Image convolve(Image& src, const std::vector<float>& kernel) {
    int m = static_cast<int>(sqrt(kernel.size())); // kernel size should be perfect square

    int width = src.getWidth(), height = src.getHeight();

    Image dest(width, height);

    // "march" kernel along image colors, convolving each color channel linear combination style
    
    for (int row = 0; row < height; row++) { // y
        for (int col = 0; col < width; col++) { // x

            Color newColor = src.getColor(col-1, row-1)*kernel[0] + src.getColor(col, row-1)*kernel[1] + src.getColor(col+1, row-1)*kernel[2] +
            src.getColor(col-1, row)*kernel[m] + src.getColor(col, row)*kernel[m+1] + src.getColor(col+1, row)*kernel[m+2] + 
            src.getColor(col-1, row+1)*kernel[2*m] + src.getColor(col, row+1)*kernel[2*m+1] + src.getColor(col+1, row+1)*kernel[2*m+2];
            
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

    const std::vector<float> gaussianBlur = {
        1.0/16.0, 2.0/16.0, 1.0/16.0, 
        2.0/16.0, 4.0/16.0, 2.0/16.0, 
        1.0/16.0, 2.0/16.0, 1.0/16.0
    };

    Image blurred = convolve(original, gaussianBlur);

    blurred.exportBmp("blurred.bmp");

}
