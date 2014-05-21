# Makefile to build LED_Matrix

CC = gcc
FLAGS = -Wall -Werror

OBJECTS = matrix.o
EXECUTABLE = led_matrix

all: $(OBJECTS)
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(OBJECTS)

clean:
	rm -f *.o $(EXECUTABLE)