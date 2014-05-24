/* matrix-font defines font data for display on the matrix.
 */
#ifndef _MATRIX_FONT_H
#define _MATRIX_FONT_H

#include <cstddef>
#include <vector>

typedef unsigned short int uint16_t;

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
    std::vector<uint16_t> const data;
} FontChar;

// Array containing all supported characters.
extern FontChar const font[];

// Helper function to get a pointer to the FontChar for a given character
static inline FontChar const * getFontChar(char c)
{
    const size_t index = c - font[0].code;
    return &font[index];
}

#endif