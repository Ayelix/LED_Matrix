/* matrix-mode-text - Implementation of MatrixMode for scrolling text across the
 * matrix.
 */
#ifndef _MATRIX_MODE_TEXT_H
#define _MATRIX_MODE_TEXT_H

#include <led-matrix/controller/mode/mode.hpp>
#include <led-matrix/font/font.hpp>

class MatrixModeText : public MatrixMode
{
public:
    MatrixModeText(MatrixDriver * driver)
      : MatrixMode(s_modeID, s_nameStr, s_descriptionStr,
        s_delayMs, driver) {};

    void begin();
    void update();
    
private:
    static MatrixMode::MatrixModeID s_modeID;
    static long int s_delayMs;
    static std::string const s_nameStr;
    static std::string const s_descriptionStr;
    
    // Default text to scroll
    static std::string const s_defaultText;
    
    // Text being scrolled across the matrix
    std::string m_text;
    // Current character being scrolled into view
    std::string::const_iterator m_textPosition;
    // Next column to become visible in the current character
    size_t m_charPosition;
    
    // Helper method to write one column of a font character at the given column
    // on the matrix
    void writeCharacterColumn(uint16_t columnValue, size_t col);
};

#endif