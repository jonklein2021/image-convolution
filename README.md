# image-manipulation
C++ image processing using convolution matrices

Started from Aerideus' YouTube tutorials:
https://www.youtube.com/watch?v=vqT5j38bWGg
https://www.youtube.com/watch?v=NcEE5xmpgQ0&t=463s

## Build & Run Instructions
```
make
./bmp-convolve [KERNEL] [INPUT] [OUTPUT]
```

## TODOs
- Enable support for kernels with negative entries
- Parallelize convolution code via multi-threading
- Enable support for JPEG (and maybe PNG) images
- Apply FFT to convolution code