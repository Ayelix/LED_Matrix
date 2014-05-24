# Makefile to build LED_Matrix

CXX = g++
CXXFLAGS = -Wall -Werror -std=gnu++0x
# Prevent accidental build of C code
CC = <none>
CFLAGS = <none>

OBJECTS = matrix.o matrix-debug.o matrix-driver.o matrix-driver-console.o \
	matrix-controller.o matrix-font.o

EXECUTABLE = led_matrix

#default: clean
#default: CXXFLAGS += -O3
default: all

# Debug target removed as long as DEBUG is defined in source for all builds
#debug: clean
#debug: CXXFLAGS += -DDEBUG -O0 -g
#debug: all

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECTS) -lncurses

clean:
	rm -f *.o $(EXECUTABLE)
