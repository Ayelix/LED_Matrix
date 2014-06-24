#include <led-matrix/mode/mode-text.hpp>

#include <led-matrix/debug/debug.hpp>

MatrixMode::MatrixModeID MatrixModeText::s_modeID =
    MatrixMode::MATRIX_MODE_ID_TEXT;
long int MatrixModeText::s_delayMs = 50;
std::string const MatrixModeText::s_nameStr =
    "Text mode";
std::string const MatrixModeText::s_descriptionStr =
    "Scroll text across the matrix.";
    
std::string const MatrixModeText::s_defaultText =
    "Text mode! ";

void MatrixModeText::begin()
{
    MatrixMode::begin();
    
    //TODO: get current text from settings
    m_text = s_defaultText + ' ';
    
    // Reset text positions
    m_textPosition = m_text.begin();
    m_charPosition = 0;
}
    
void MatrixModeText::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        // Shift matrix contents left to scroll the text
        m_driver->shiftLeftAllPixels();
        
        // If the entire string has not yet been displayed, continue to scroll
        // in the current character
        if (m_textPosition < m_text.end())
        {
            // Get the current character's data
            FontChar const * const currentChar =
                getFontChar(*m_textPosition);
            
            // If the entire character has not yet been displayed, display the
            // next column
            if (m_charPosition < currentChar->width)
            {
                // Display the current column
                writeCharacterColumn(currentChar->data[m_charPosition],
                    m_driver->COLUMNS - 1);
                // Move to the next column in the character
                m_charPosition++;
            }
            // Otherwise, leave the column blank as a separator between
            // characters and move to the next character in the string.
            else
            {
                // Reset position in current character
                m_charPosition = 0;
                // Move to the next character in the string
                m_textPosition++;
            }
        }
        // Otherwise, leave the column blank as a separator between the end and
        // beginning of the string and reset the string position
        else
        {
            // Reset text position to the first character
            m_textPosition = m_text.begin();
        }
        
        // Update the driver to display the scrolled text
        m_driver->update();
    }
}

void MatrixModeText::writeCharacterColumn(uint16_t columnValue, size_t col)
{
    // Iterate through each pixel value.  Pixels are stored with the MSB as the
    // top pixel, but this loop goes through it from the LSB (bottom).
    for (int row = font_height - 1; row >= 0; row--)
    {
        // Write the current pixel value
        bool pixelVal = (columnValue & 0x01);
        m_driver->assignPixel(col, row, pixelVal);
        columnValue >>= 1;
    }
}