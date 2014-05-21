# Makefile to build LED_Matrix

CC = gcc
CFLAGS = -Wall -Werror

SOURCES = matrix.cpp
EXECUTABLE = led_matrix

default: CFLAGS += -O3
default:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)

debug: CFLAGS += -DDEBUG -O0 -g
debug: clean
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)

clean:
	rm -f *.o $(EXECUTABLE)