/* matrix-font defines font data for display on the matrix.
 */
#ifndef _MATRIX_FONT_H
#define _MATRIX_FONT_H

#include <led-matrix/types/types.h>

#include <cstddef>
#include <vector>

// Maximum width of the font, this determines the size of FontChar.data and
// depends on the font used
size_t const FONT_MAX_WIDTH = 9;

// Structure containing data for a single character as represented in the font.
typedef struct
{
    // The character represented by this FontChar.
    char const code;
    // Width of the character when displayed in the font.
    size_t const width;
    // Pixel values for the character.  Each value is a column of 16 pixels
    // with each bit representing a pixel.  Most significant bit is the upper-
    // most pixel.
    uint16_t const data[FONT_MAX_WIDTH];
} FontChar;

// Height of the tallest character in the font
extern size_t const font_height;

// Range of characters included in the font (inclusive)
extern char const font_min;
extern char const font_max;

// Array containing all supported characters.
extern FontChar const font[];

// Helper function to get a pointer to the FontChar for a given character
static inline FontChar const * getFontChar(char c)
{
    const size_t index = c - font[0].code;
    return &font[index];
}

#endif