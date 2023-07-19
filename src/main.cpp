/**
 * @file main.cpp
 * @author Jon Klein (jonklein2021@gmail.com)
 * @brief Image Convolutions in C++
 * @version 0.1
 * @date 2023-07-19
 * 
 */

#include <iostream>
#include <cmath>
#include "image.hpp"
#include <chrono>

// definition is at the end of this file
Image convolve(Image& src, std::vector<float> kernel);

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
    1.0/256.0,  4.0/256.0,  6.0/256.0,  4.0/256.0, 1.0/256.0, 
    4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0, 4.0/256.0, 
    6.0/256.0, 24.0/256.0, 36.0/256.0, 24.0/256.0, 6.0/256.0,
    4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0, 4.0/256.0, 
    1.0/256.0,  4.0/256.0,  6.0/256.0,  4.0/256.0, 1.0/256.0
};

int main() {
    char file[255];
    puts("Convolutions for Image Processing");
    puts("========================================");
    puts("Enter path to bitmap image to convolve (Ex: samples/doggo.bmp)");
    
    std::cin >> file;

    // create image object from bmp file
    Image original(file);

    puts("\nPlease enter the number of a kernel below:");
    puts("1. Box Blur");
    puts("2. Gaussian Blur (3x3)");
    puts("3. Gaussian Blur (5x5)");

    char selection[256];
    std::cin >> selection;
    std::vector<float> kernel;
    
    if (atoi(selection) == 1) {
        kernel = boxBlur;
    } else if (atoi(selection) == 2) {
        kernel = gaussianBlur3x3;
    } else if (atoi(selection) == 3) {
        kernel = gaussianBlur5x5;
    } else {
        puts("Invalid selection.");
        return 1;
    }

    auto t1 = std::chrono::high_resolution_clock::now(); // start time
    Image blurred = convolve(original, kernel);
    auto t2 = std::chrono::high_resolution_clock::now(); // end time
    std::chrono::duration<double, std::milli> ms = t2 - t1; // convolution execution time
    
    std::cout << "Convolution successful (" << ms.count() << " ms)" << std::endl;

    blurred.exportBmp("blurred.bmp");

}

Image convolve(Image& src, std::vector<float> kernel) {
    const int M = static_cast<int>(sqrt(kernel.size()));
    const int MM = M*M;

    // ensure that kernel is an odd perfect square
    if (MM & 1 == 0 || MM != kernel.size()) {
        throw std::runtime_error("ERROR: Kernel size must be an odd perfect square");
    }

    // ensure that kernel is not too big
    if (M > 9) {
        throw std::runtime_error("ERROR: Kernel may be no bigger than 9x9");
    }

    int width = src.getWidth(), height = src.getHeight();

    Image dest(width, height);

    // flip the kernel horizontally and vertically to conform with the mathematical definition of convolution
    for (int k = 0; k < MM/2; k++) {
        float temp = kernel[k];
        kernel[k] = kernel[MM-k-1];
        kernel[MM-k-1] = temp;
    }

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
