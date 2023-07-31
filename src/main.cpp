/**
 * @file main.cpp
 * @author Jon Klein (jonklein2021@gmail.com)
 * @brief Image Convolutions in C++
 * @version 0.1
 * @date 2023-07-19
 * 
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <cstring>
#include <unordered_map>
#include "image.hpp"

#define K 3

// definitions are at the end of this file
Image convolve(Image& src, std::vector<float> kernel);
void printHelp();

const std::vector<float> copy = {
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0
};

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

enum Kernel {
    BOX_BLUR,
    GAUSSIAN_BLUR_3X3,
    GAUSSIAN_BLUR_5X5
};

std::vector<float> kernels[K] = {boxBlur, gaussianBlur3x3, gaussianBlur5x5};
std::string kernelStrings[K] = {"Box Blur", "3x3 Gaussian Blur", "5x5 Gaussian Blur"};

std::unordered_map<std::string, Kernel> argMap = {
        {"--box-blur", BOX_BLUR},
        {"-1", BOX_BLUR},
        {"--g-blur-3x3", GAUSSIAN_BLUR_3X3},
        {"-2", GAUSSIAN_BLUR_3X3},
        {"--g-blur-5x5", GAUSSIAN_BLUR_5X5},
        {"-3", GAUSSIAN_BLUR_5X5}
    };

int main(int argc, char** argv) {
    // ensure correct number of arguments
    if (argc != 2 && argc != 4) { // trips for "./bmp-convolve"
        std::cerr << "Usage: ./convolve <option> <input> <output>\nTry './bmp-convolve --help' for more information" << std::endl;
        return 2;
    }

    // argc = 2 only valid for -h and --help flags
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            // print help menu
            printHelp();
            return 0;
        } else {
            std::cerr << "Usage: ./convolve <option> <input> <output>\nTry './bmp-convolve --help' for more information" << std::endl;
            return 2;
        }
    }

    std::vector<float> kernel;
    std::string kernelName;
    char *inputPath = NULL, *outputPath;

    // argc = 4 at this point
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // assume argument is double hyphen flag
            auto iter = argMap.find(argv[i]);
            if (iter == argMap.end()) {
                // flag not found in mapping
                std::cerr << "Unknown Flag: " << argv[i] << "\nTry './bmp-convolve --help' for more information" << std::endl;
                return 2;
            } else {
                kernel = kernels[iter->second];
                kernelName = kernelStrings[iter->second];
            }
        } else {
            // not flag => this arg must be some path to image
            if (inputPath == NULL) {
                inputPath = argv[i];
            } else {
                outputPath = argv[i];
            }
        }
    }

    std::cout << inputPath << " " << outputPath << std::endl;

    // create image object from file
    Image original(inputPath);

    // start time
    auto t1 = std::chrono::high_resolution_clock::now();

    // convolution time baby
    Image blurred = convolve(original, kernel);

    // end time
    auto t2 = std::chrono::high_resolution_clock::now();
    
    // total execution time
    std::chrono::duration<double, std::milli> ms = t2 - t1;
    
    std::cout << kernelName << " convolution successful (" << ms.count() << " ms)" << std::endl;

    blurred.exportBmp(outputPath);

}

Image convolve(Image& src, std::vector<float> kernel) {
    const int M = static_cast<int>(sqrt(kernel.size()));
    const int MM = M*M;

    // ensure that kernel is an odd perfect square
    if (MM & 1 == 0 || MM != kernel.size()) {
        throw std::runtime_error("ERROR: Kernel size must be an odd perfect square (Ex. 3x3 or 5x5)");
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

void printHelp() {
    std::cout << "bmp-convolve\n\n";

    std::cout << "DESCRIPTION\n";
    std::cout << "\tConvolves a bitmap image with some kernel and exports result to a new copy\n";

    std::cout << "USAGE\n";
    std::cout << "\t./bmp-convolve [KERNEL] [INPUT] [OUTPUT]\n";

    std::cout << "FLAGS\n";
    std::cout << "\t-1, --box-blur\n";
    std::cout << "\t\tUse box blur kernel\n";

    std::cout << "\t\t\t| 1/9 1/9 1/9 |\n";
    std::cout << "\t\t\t| 1/9 1/9 1/9 |\n";
    std::cout << "\t\t\t| 1/9 1/9 1/9 |\n";

    std::cout <<  "\t-2, --g-blur-3x3\n";
    std::cout <<  "\t\tUse 3x3 gaussian blur kernel\n";

    std::cout <<  "\t\t\t| 1/16 2/16 1/16 |\n";
    std::cout <<  "\t\t\t| 2/16 4/16 2/16 |\n";
    std::cout <<  "\t\t\t| 1/16 2/16 1/16 |\n";

    std::cout <<  "\t-3, --g-blur-5x5\n";
    std::cout <<  "\t\tUse 5x5 gaussian blur kernel\n";

    std::cout <<  "\t\t\t| 1/256  4/256  6/256  4/256 1/256 |\n";
    std::cout <<  "\t\t\t| 4/256 16/256 24/256 16/256 4/256 |\n";
    std::cout <<  "\t\t\t| 6/256 24/256 36/256 24/256 6/256 |\n";
    std::cout <<  "\t\t\t| 4/256 16/256 24/256 16/256 4/256 |\n";
    std::cout <<  "\t\t\t| 1/256  4/256  6/256  4/256 1/256 |\n";
    
    std::cout << "\t-h, --help\n";
    std::cout << "\t\tDisplays this help menu\n";

    std::cout << "RETURN CODES\n";
    std::cout << "\t0   Returned if all runs successfully\n";
    std::cout << "\t1   Returned if there are any problems with producing output image\n";
    std::cout << "\t2   Returned if command is misused\n";

    std::cout << "Author: Jon Klein\n";
    std::cout << "Started from tutorials by Aerideus on YouTube:\n";
    std::cout << "https://www.youtube.com/watch?v=vqT5j38bWGg\n";
    std::cout << "https://www.youtube.com/watch?v=NcEE5xmpgQ0&t=463s" << std::endl;
}
