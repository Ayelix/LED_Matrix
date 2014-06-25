#include <led-matrix/mode/mode-vu.hpp>

#include <cmath>

MatrixMode::MatrixModeID MatrixModeVu::s_modeID =
    MatrixMode::MATRIX_MODE_ID_VU;
long int MatrixModeVu::s_delayMs = 5;
std::string const MatrixModeVu::s_nameStr =
    "Vu mode";
std::string const MatrixModeVu::s_descriptionStr =
    "Use the matrix as a VU meter.";

void MatrixModeVu::update()
{
    // If the update timer has elapsed, update the animation
    if (needsUpdate())
    {
        static bool msgPrinted = false;
        if (!msgPrinted)
        {
            DBG_PRINTF("VU Mode not implemented.  Demo cycles through """
                "available VU level display types.\n");
                msgPrinted = true;
        }
        
        static unsigned int level = 0;
        static int incr = 1;
        static bool fill = false;
        static PlotType type = MATRIX_CONTROLLER_PLOT_TYPE_VERTICAL;
        
        level += incr;
        if (level >= 100 || level <= 0)
        {
            incr = -incr;
            if (incr > 0)
            {
                int intType = static_cast<int>(type) + 1;
                if (intType >= MATRIX_CONTROLLER_PLOT_TYPE_COUNT)
                {
                    fill = !fill;
                    intType = 0;
                }
                type = static_cast<PlotType>(intType);
            }
        }
        
        plotLevel(level, type, fill);
    }
}

void MatrixModeVu::plotLevel(unsigned int level, PlotType type, bool fill)
{
    // Coordinates for the rectangle which represents the plotted point.
    // Maximums are not inclusive, i.e. they must be 1 greater than the target.
    size_t col_min = 0, col_max = 0;
    size_t row_min = 0, row_max = 0;
    
    switch (type)
    {
    case MATRIX_CONTROLLER_PLOT_TYPE_VERTICAL:
    {
        // Scale the level to get the row to turn on
        level = round((double)(level * (m_driver->ROWS - 2)) / 100.0);
        level = m_driver->ROWS - 2 - level;
        // Calculate the min/max values
        col_min = 0;
        col_max = m_driver->COLUMNS;
        row_min = level;
        row_max = (fill) ? (m_driver->ROWS) : (row_min + 2);
        // Clear all other pixels for this plot type
        m_driver->clearAllPixels();
        break;
    }
    
    case MATRIX_CONTROLLER_PLOT_TYPE_HORIZONTAL:
    {
        // Scale the level to get the column to turn on
        level = round((double)(level * (m_driver->COLUMNS - 2)) / 100.0);
        //level = m_driver->COLUMNS - 2 - level;
        // Calculate the min/max values
        col_min = (fill) ? (0) : (level);
        col_max = level + 2;
        row_min = 0;
        row_max = m_driver->ROWS;
        // Clear all other pixels for this plot type
        m_driver->clearAllPixels();
        break;
    }
    
    case MATRIX_CONTROLLER_PLOT_TYPE_SHIFTING:
    {
        // Scale the level to get the row to turn on
        level = round((double)(level * (m_driver->ROWS - 2)) / 100.0);
        level = m_driver->ROWS - 2 - level;
        // Calculate the min/max values
        col_min = m_driver->COLUMNS - 1;
        col_max = col_min + 1;
        row_min = level;
        row_max = (fill) ? (m_driver->ROWS) : (row_min + 2);
        // Shift all other pixels for this plot type
        m_driver->shiftLeftAllPixels();
        break;
    }
    
    case MATRIX_CONTROLLER_PLOT_TYPE_COUNT:
    {
        DBG_PRINTF("Invalid plot type in MatrixController::plotLevel(%d, %d).\n",
            level, type);
    }
    }
    
    // Set the pixels according to the coordinates set above
    for (size_t row = row_min; row < row_max; row++)
    {
        for (size_t col = col_min; col < col_max; col++)
        {
            m_driver->setPixel(col, row);
        }
    }
    
    m_driver->update();
}