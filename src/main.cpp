#include <cstdlib>
#include <ctime>
#include <cmath>
#include "image.hpp"

Image convolve(Image& src, const std::vector<float>& kernel) {
    int m = static_cast<int>(sqrt(kernel.size())); // kernel size should be perfect square

    int width = src.getWidth(), height = src.getHeight();

    Image dest(width, height);

    // "march" kernel along image colors, convolving each color channel linear combination style
    
    for (int row = 0; row < width; row++) {
        for (int col = 0; col < height; col++) {

            float red = kernel[0]*src.getColor(row-1, col-1).r + kernel[1]*src.getColor(row-1, col).r + kernel[2]*src.getColor(row-1, col+1).r +
            kernel[m]*src.getColor(row, col-1).r + kernel[m+1]*src.getColor(row, col).r + kernel[m+2]*src.getColor(row, col+1).r + 
            kernel[2*m]*src.getColor(row+1, col-1).r + kernel[2*m+1]*src.getColor(row+1, col).r + kernel[2*m+2]*src.getColor(row+1, col+1).r;
            
            float green = kernel[0]*src.getColor(row-1, col-1).g + kernel[1]*src.getColor(row-1, col).g + kernel[2]*src.getColor(row-1, col+1).g +
            kernel[m]*src.getColor(row, col-1).g + kernel[m+1]*src.getColor(row, col).g + kernel[m+2]*src.getColor(row, col+1).g + 
            kernel[2*m]*src.getColor(row+1, col-1).g + kernel[2*m+1]*src.getColor(row+1, col).g + kernel[2*m+2]*src.getColor(row+1, col+1).g;
            
            float blue = kernel[0]*src.getColor(row-1, col-1).b + kernel[1]*src.getColor(row-1, col).b + kernel[2]*src.getColor(row-1, col+1).b +
            kernel[m]*src.getColor(row, col-1).b + kernel[m+1]*src.getColor(row, col).b + kernel[m+2]*src.getColor(row, col+1).b + 
            kernel[2*m]*src.getColor(row+1, col-1).b + kernel[2*m+1]*src.getColor(row+1, col).b + kernel[2*m+2]*src.getColor(row+1, col+1).b;

            Color newColor(red, green, blue);
            
            dest.setColor(newColor, col, row);
        }
    }

    return dest;

}

int main() {
    // const int width = 640;
    // const int height = 480;

    // srand(static_cast<unsigned>(time(0)));

    // Image image(width, height);
    // for (int row = 0; row < height; row++) {
    //     for (int col = 0; col < width; col++) {
    //         image.setColor(Color((float) col / (float) width, 1.0f - (float) col / (float) width, (float) row / (float) height), row, col);
    //         image.setColor(Color((float) rand()/(float) RAND_MAX, (float) rand()/(float) RAND_MAX, (float) rand()/(float) RAND_MAX), row, col);
    //     }
    // }
    // image.exportBmp("image.bmp");

    Image original("samples/sample_640x426.bmp");

    std::vector<float> boxBlur = {
        1.0/9.0, 1.0/9.0, 1.0/9.0, 
        1.0/9.0, 1.0/9.0, 1.0/9.0, 
        1.0/9.0, 1.0/9.0, 1.0/9.0
    };
    Image blurred = convolve(original, boxBlur);

    blurred.exportBmp("blurred.bmp");

}
