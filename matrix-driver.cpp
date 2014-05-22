#include "matrix-driver.h"

#include <cstring>

MatrixDriver::MatrixDriver()
  // Set stateChanged flag so that blank matrix will be displayed initially
: stateChanged(true)
{
    // Initialize matrix state in memory
    memset(matrix, 0, COLUMNS*ROWS*sizeof(matrix[0][0]));
}

MatrixDriver::~MatrixDriver()
{
    // Nothing to do
}

void MatrixDriver::setPixel(size_t col, size_t row)
{
    assignPixel(col, row, true);
}

void MatrixDriver::clearPixel(size_t col, size_t row)
{
    assignPixel(col, row, false);
}

bool MatrixDriver::getPixel(size_t col, size_t row) const
{
    bool retVal = false;
    
    if (coordsValid(col, row))
    {
        retVal = matrix[col][row];
    }
    else
    {
        DBG_PRINTF("getPixel(%zu, %zu): invalid coordinates.\n", col, row);
        retVal = false;
    }
    
    return retVal;
}

bool MatrixDriver::coordsValid(size_t col, size_t row) const
{
    return ((col < COLUMNS) && (row < ROWS));
}

void MatrixDriver::assignPixel(size_t col, size_t row, bool value)
{
    if (coordsValid(col, row))
    {
        matrix[col][row] = value;
        stateChanged = true;
    }
    else
    {
        char const * const boolStr = value ? "true" : "false";
        DBG_PRINTF("assignPixel(%zu, %zu, %s): invalid coordinates.\n", 
            col, row, boolStr);
    }
}