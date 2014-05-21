#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#define PROGRAM_NAME ("led_matrix")

static void usage()
{
    fprintf(stderr, "Usage: %s [-e]\n"""
    "    -e  Enable matrix emulation mode.  In this mode, the LED matrix is emulated\n"""
    "        onscreen instead of driving the actual matrix.\n"""
    "\n",
    PROGRAM_NAME);
}

int main (int argc, char * argv[])
{
    // Default values for options which may be changed with arguments
    bool emulate = false;
    
    // Parse command-line arguments
    int c;
    while ((c = getopt(argc, argv, "e")) != -1)
    {
        switch (c)
        {
        case 'e':
            emulate = true;
            break;
        
        default:
            fprintf(stderr, "Unknown option -%c.\n", optopt);
            usage();
            exit(EXIT_FAILURE);
        }
    }
    
    if (emulate) printf("Starting in emulation mode.\n");
    else
    {
        fprintf(stderr, "Matrix control not implemented, use -e.\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}