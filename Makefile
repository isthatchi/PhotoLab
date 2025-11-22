#Makefile: PhotoLab

CC     = gcc
DEBUG  = -DDEBUG -g
CFLAGS = -ansi -std=c11 -Wall -c
LFLAGS = -ansi -std=c11 -Wall -lm
PPM = bigresize.ppm smallresize.ppm blur.ppm border.ppm colorfilter.ppm bw.ppm crop.ppm edge.ppm fisheye.ppm hmirror.ppm posterize.ppm negative.ppm rotate.ppm shuffle.ppm vflip.ppm

all: PhotoLab PhotoLabTest

Test.o: Test.c Test.h
	$(CC) $(CFLAGS) Test.c -o Test.o

Test_DEBUG.o: Test.c Test.h
	$(CC) $(CFLAGS) $(DEBUG) Test.c -o Test_DEBUG.o

Image.o: Image.c Image.h
	$(CC) $(CFLAGS) Image.c -o Image.o

DIPs.o: DIPs.h DIPs.c Constants.h
	$(CC) $(CFLAGS)  DIPs.c -o DIPs.o

FileIO.o: FileIO.h FileIO.c Constants.h
	$(CC) $(CFLAGS) FileIO.c -o FileIO.o

Advanced.o: Advanced.h Advanced.c Constants.h
	$(CC) $(CFLAGS) Advanced.c -o Advanced.o

libFilter.a: DIPs.o Advanced.o
	ar rc libFilter.a Advanced.o DIPs.o
	ranlib libFilter.a

PhotoLab.o: PhotoLab.c DIPs.h Advanced.h FileIO.h Constants.h
	$(CC) $(CFLAGS) PhotoLab.c -o PhotoLab.o

PhotoLabTest.o: PhotoLab.c DIPs.h Advanced.h FileIO.h Constants.h
	$(CC) $(CFLAGS) $(DEBUG) -c PhotoLab.c -o PhotoLabTest.o

PhotoLab: PhotoLab.o FileIO.o Image.o Test.o libFilter.a
	$(CC) $(LFLAGS) PhotoLab.o FileIO.o Test.o Image.o -L. -lFilter -o PhotoLab

PhotoLabTest: PhotoLabTest.o FileIO.o Image.o Test_DEBUG.o libFilter.a
	$(CC) $(LFLAGS) PhotoLabTest.o FileIO.o Image.o Test_DEBUG.o -L. -lFilter -o PhotoLabTest

clean:
	rm -f *.o
	rm -f PhotoLab
	rm -f PhotoLabTest
	rm -f $(PPM)

