# Makefile to build LED_Matrix

CXX = gcc
CXXFLAGS = -Wall -Werror

OBJECTS = matrix.o matrix-debug.o
EXECUTABLE = led_matrix

default: clean
default: CXXFLAGS += -O3
default: all

# Debug target removed as long as DEBUG is defined in source for all builds
#debug: clean
#debug: CXXFLAGS += -DDEBUG -O0 -g
#debug: all

all: CFLAGS = $(CXXFLAGS)
all: CC = $(CXX)
all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECTS) -lncurses

clean:
	rm -f *.o $(EXECUTABLE)