# Makefile to build LED_Matrix

# Get the Makefile's directory to be used as the base for file paths
TOP := $(dir $(lastword $(MAKEFILE_LIST)))

# The final executable will be named:
EXECUTABLE = led_matrix

# Find all .cpp files in the current directory
CPP_FILES := $(shell find $(TOP) -type f -name '*.cpp')
# Get the list of corresponding object files by replacing '.cpp' with '.o'
OBJ_FILES := $(patsubst %.cpp,%.o,$(CPP_FILES))

# Build files with g++, adding the current directory to the search path (-I)
CXX = g++
CXXFLAGS = -Wall -Werror -I $(TOP)

# Prevent accidental build of C code
CC = <none>
CFLAGS = <none>

# Libraries required by the project
LIBS = -lncurses -lboost_atomic

# Libraries required when building code with Pion
PION_LIBS = -lboost_thread -lboost_system -lboost_filesystem -lboost_regex \
	-lboost_date_time -lboost_signals -lboost_iostreams \
    -llog4cpp -lpion

# Build all object files and link into the final executable
default: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $^ $(LIBS) $(PION_LIBS)

# Target to build a single object file
obj/%.o: led-matrix/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Remove all object files and the executable
clean:
	rm -rf $(OBJ_FILES) $(TOP)obj/ $(EXECUTABLE)
