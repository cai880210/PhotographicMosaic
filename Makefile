CC=g++
ARCH=-arch x86_64

OCVDIR=/home/cshome/s/steven/Public/COSC450/OpenCV

INCLUDE_FLAGS=-I$(OCVDIR)/include

CFLAGS=-c -Wall $(INCLUDE_FLAGS)
LDFLAGS= -L$(OCVDIR)/lib -lopencv_core -lopencv_highgui -lopencv_imgproc 

SOURCES=mosaic.cpp RandomPatchDatabase.cpp AveragePatchDatabase.cpp RGBGridPatchDatabase.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=mosaic


all : $(SOURCES) $(EXECUTABLE)

clean :
	rm -rf $(OBJECTS) $(EXECUTABLE) *~ core

$(EXECUTABLE): $(OBJECTS) Makefile
	$(CC) $(ARCH) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@