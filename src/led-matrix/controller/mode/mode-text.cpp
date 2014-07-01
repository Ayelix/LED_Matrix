#include <led-matrix/controller/mode/mode-text.hpp>

#include <stdexcept>

#include <led-matrix/controller/mode/setting/setting.hpp>
#include <led-matrix/controller/mode/setting/setting-string.hpp>

MatrixMode::MatrixModeID const MatrixModeText::s_MODE_ID =
    MatrixMode::MATRIX_MODE_ID_TEXT;
long int const MatrixModeText::s_DELAY_MS = 50;
std::string const MatrixModeText::s_NAME_STR =
    "Text mode";
std::string const MatrixModeText::s_DESCRIPTION_STR =
    "Scroll text across the matrix";
    
std::string const MatrixModeText::s_DEFAULT_TEXT =
    "Text mode!";
    
MatrixModeText::MatrixModeText(MatrixDriver * driver)
  : MatrixMode(s_MODE_ID, s_NAME_STR, s_DESCRIPTION_STR, s_DELAY_MS, driver)
{
    // Add the setting for text to scroll across the matrix
    m_textSetting = (MatrixSettingString *) MatrixSetting::createSetting(
        MatrixSetting::MATRIX_SETTING_ID_STRING, "Scrolling Text",
        "Text to scroll across the matrix");
    if (NULL == m_textSetting)
    {
        throw std::runtime_error(
            "MatrixModeText::MatrixModeText(): Unable to create text setting");
    }
    m_textSetting->setString(s_DEFAULT_TEXT);
    m_settings.push_back(m_textSetting);
}

void MatrixModeText::begin()
{
    MatrixMode::begin();
    
    // Get current text from the text setting
    m_text = m_textSetting->getString();
    
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
        // Otherwise, scroll the text all the way out before restarting at the
        // first character
        else
        {
            if (m_charPosition < m_driver->COLUMNS)
            {
                // Leave the column blank and move to the next column
                m_charPosition++;
            }
            else
            {
                // Reset position in current character
                m_charPosition = 0;
                // Reset text position to the first character
                m_textPosition = m_text.begin();
            }
        }
        
        // Update the driver to display the scrolled text
        m_driver->update();
        
        // Reset the animation if the text setting has been changed
        if (m_textSetting->getString() != m_text)
        {
            begin();
        }
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