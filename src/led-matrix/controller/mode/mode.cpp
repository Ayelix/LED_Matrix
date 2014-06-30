#include <led-matrix/controller/mode/mode.hpp>

#include <stdexcept>
#include <sstream>
#include <cmath>

#include <led-matrix/debug/debug.hpp>
#include <led-matrix/timer/timer.hpp>

// Mode implementations
#include <led-matrix/controller/mode/mode-idle.hpp>
#include <led-matrix/controller/mode/mode-text.hpp>
#include <led-matrix/controller/mode/mode-vu.hpp>
#include <led-matrix/controller/mode/mode-sine.hpp>

MatrixMode::MatrixMode(MatrixModeID id, std::string name, std::string description,
    long int delayMs, MatrixDriver * driver)
 : m_driver(driver)
 , m_nameStr(name), m_descriptionStr(description)
 , m_delayMs(delayMs)
{
    checkID(id, "MatrixMode::MatrixMode()");
    m_id = id;
}

void MatrixMode::begin()
{
    // Start the update timer so it will be triggered for update()
    MatrixTimer::startTimer(m_delayMs);
    m_driver->clearAllPixels();
    m_driver->update();
}

bool MatrixMode::needsUpdate()
{
    bool retVal = false;
    // If the timer has elapsed, restart it and return true
    if (MatrixTimer::checkTimer())
    {
        MatrixTimer::startTimer(m_delayMs);
        retVal = true;
    }
    return retVal;
}

void MatrixMode::destroyMode(MatrixMode * mode)
{
    if (mode != NULL)
    {
        checkID(mode->getID(), "MatrixMode::destroyMode()");
        
        // Cast to appropriate implementation and delete
        switch (mode->getID())
        {
            case MATRIX_MODE_ID_IDLE:
            {
                MatrixModeIdle * const modeIdle = (MatrixModeIdle *)mode;
                delete modeIdle;
                return;
            }
            
            case MATRIX_MODE_ID_TEXT:
            {
                MatrixModeText * const modeText = (MatrixModeText *)mode;
                delete modeText;
                return;
            }
            
            case MATRIX_MODE_ID_VU:
            {
                MatrixModeVu * const modeVu = (MatrixModeVu *)mode;
                delete modeVu;
                return;
            }
            
            case MATRIX_MODE_ID_SINE:
            {
                MatrixModeSine * const modeSine = (MatrixModeSine *)mode;
                delete modeSine;
                return;
            }
            
            // No default case to preserve compiler warnings for unhandled enum
            // values
            case MATRIX_MODE_ID_COUNT:
            break;
        }
    
        // If this point is reached, the mode is of an unknown type and should
        // have been caught by checkID.
        throw std::runtime_error("MatrixMode::checkID() failure");
    }
    else
    {
        std::string const errorStr =
            "MatrixMode::destroyMode(): mode must not be NULL";
        DBG_PRINTF("%s\n", errorStr.c_str());
        throw std::invalid_argument(errorStr);
    }
}

MatrixMode * MatrixMode::createMode(MatrixModeID id, MatrixDriver * driver)
{
    checkID(id, "MatrixMode::createMode()");
    
    // Allocate appropriate implementation instance
    switch (id)
    {
        case MATRIX_MODE_ID_IDLE:
        {
            MatrixModeIdle * const modeIdle = new MatrixModeIdle(driver);
            return modeIdle;
        }
        
        case MATRIX_MODE_ID_TEXT:
        {
            MatrixModeText * const modeText = new MatrixModeText(driver);
            return modeText;
        }
        
        case MATRIX_MODE_ID_VU:
        {
            MatrixModeVu * const modeVu = new MatrixModeVu(driver);
            return modeVu;
        }
        
        case MATRIX_MODE_ID_SINE:
        {
            MatrixModeSine * const modeSine = new MatrixModeSine(driver);
            return modeSine;
        }
        
        // No default case to preserve compiler warnings for unhandled enum
        // values
        case MATRIX_MODE_ID_COUNT:
        break;
    }
    
    // If this point is reached, the mode is of an unknown type and should have
    // been caught by checkID.
    throw std::runtime_error("MatrixMode::checkID() failure");
}

void MatrixMode::plotLevel(unsigned int level, PlotType type, bool fill)
{
    // Coordinates for the rectangle which represents the plotted point.
    // Maximums are not inclusive, i.e. they must be 1 greater than the target.
    size_t col_min = 0, col_max = 0;
    size_t row_min = 0, row_max = 0;
    
    switch (type)
    {
    case MATRIX_MODE_PLOT_TYPE_VERTICAL:
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
    
    case MATRIX_MODE_PLOT_TYPE_HORIZONTAL:
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
    
    case MATRIX_MODE_PLOT_TYPE_SHIFTING:
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
    
    case MATRIX_MODE_PLOT_TYPE_COUNT:
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

void MatrixMode::checkID(MatrixModeID id, std::string const & prefix)
{
    if (id >= MATRIX_MODE_ID_COUNT)
    {
        std::ostringstream oss;
        oss << prefix << ": Invalid mode ID " << id;
        std::string const errorStr = oss.str();
        DBG_PRINTF("%s\n", errorStr.c_str());
        throw std::invalid_argument(errorStr);
    }
}
