all: libimage
	mkdir -p build/bin/
	g++ src/main.cpp -o bmp-convolve -I include -l image -L build/lib

libimage:
	mkdir -p build/lib/
	mkdir -p build/objects
	g++ src/lib/image.cpp -c -I include
	ar -rs libimage.a image.o
	mv *.o build/objects
	mv *.a build/lib

clean:
	rm -rf build
	rm -rf *.bmp
	rm -rf bmp-convolve