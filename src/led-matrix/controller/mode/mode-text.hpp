/* matrix-mode-text - Implementation of MatrixMode for scrolling text across the
 * matrix.
 */
#ifndef _MATRIX_MODE_TEXT_H
#define _MATRIX_MODE_TEXT_H

#include <led-matrix/controller/mode/mode.hpp>
#include <led-matrix/font/font.hpp>
#include <led-matrix/controller/mode/setting/setting-string.hpp>

class MatrixModeText : public MatrixMode
{
public:
    MatrixModeText(MatrixDriver * driver);

    void begin();
    void update();
    
private:
    static MatrixMode::MatrixModeID const s_MODE_ID;
    static long int const s_DELAY_MS;
    static std::string const s_NAME_STR;
    static std::string const s_DESCRIPTION_STR;
    
    // Setting to modify text being scrolled
    MatrixSettingString * m_textSetting;
    
    // Default text to scroll
    static std::string const s_DEFAULT_TEXT;
    
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