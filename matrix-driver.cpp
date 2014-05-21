#include "matrix-driver.h"

#include <cstring>

MatrixDriver::MatrixDriver()
  // Set stateChanged flag so that blank matrix will be displayed initially
: stateChanged(true)
{
    // Initialize matrix state in memory
    memset(matrix, 0, ROWS*COLS);
}

MatrixDriver::~MatrixDriver()
{
    // Nothing to do
}

void MatrixDriver::setPixel(size_t row, size_t col)
{
    stateChanged = true;
    matrix[row][col] = true;
}

bool MatrixDriver::getPixel(size_t row, size_t col) const
{
    return matrix[row][col];
}