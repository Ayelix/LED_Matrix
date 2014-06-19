# Makefile to build LED_Matrix

CXX = g++
CXXFLAGS = -Wall -Werror
# Prevent accidental build of C code
CC = <none>
CFLAGS = <none>

# Libraries which must be linked when building code with Pion
PION_LIBS = -lboost_thread -lboost_system -lboost_filesystem -lboost_regex \
	-lboost_date_time -lboost_signals -lboost_iostreams -lboost_atomic \
    -llog4cpp -lpion

OBJECTS = matrix.o matrix-debug.o \
	webserver/matrix-webserver.o webserver/matrix-webserver-launcher.o \
	matrix-controller.o \
	matrix-driver.o matrix-driver-console.o matrix-driver-HT1632C.o \
	matrix-font.o matrix-timer.o

EXECUTABLE = led_matrix

#default: clean
#default: CXXFLAGS += -O3
default: all

# Debug target removed as long as DEBUG is defined in source for all builds
#debug: clean
#debug: CXXFLAGS += -DDEBUG -O0 -g
#debug: all

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECTS) -lncurses $(PION_LIBS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
