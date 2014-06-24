#include <led-matrix/mode/mode.hpp>

#include <stdexcept>
#include <sstream>

#include <led-matrix/debug/debug.hpp>
#include <led-matrix/timer/timer.hpp>

// Mode implementations
#include <led-matrix/mode/mode-idle.hpp>
#include <led-matrix/mode/mode-text.hpp>

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
        
        // TODO: cast to appropriate implementation and delete
        switch (mode->getID())
        {
            case MATRIX_MODE_ID_IDLE:
            {
                MatrixModeIdle * modeIdle = (MatrixModeIdle *)mode;
                delete modeIdle;
                DBG_PRINTF("MatrixMode::destroyMode(): destroyed MatrixModeIdle instance.\n");
                return;
            }
            
            case MATRIX_MODE_ID_TEXT:
            {
                MatrixModeText * modeText = (MatrixModeText *)mode;
                delete modeText;
                DBG_PRINTF("MatrixMode::destroyMode(): destroyed MatrixModeText instance.\n");
                return;
            }
            
            case MATRIX_MODE_ID_VU:
            {
                DBG_PRINTF("MatrixMode::destroyMode(): mode is VU.\n");
                return;
            }
            
            case MATRIX_MODE_ID_SINE:
            {
                DBG_PRINTF("MatrixMode::destroyMode(): mode is SINE.\n");
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
    
    // TODO: allocate appropriate implementation instance
    switch (id)
    {
        case MATRIX_MODE_ID_IDLE:
        {
            MatrixModeIdle * modeIdle = new MatrixModeIdle(driver);
            DBG_PRINTF("MatrixMode::createMode(): created MatrixModeIdle instance.\n");
            return modeIdle;
        }
        
        case MATRIX_MODE_ID_TEXT:
        {
            MatrixModeText * modeText = new MatrixModeText(driver);
            DBG_PRINTF("MatrixMode::createMode(): created MatrixModeText instance.\n");
            return modeText;
        }
        
        case MATRIX_MODE_ID_VU:
        {
            DBG_PRINTF("MatrixMode::createMode(): mode is VU.\n");
            return NULL;
        }
        
        case MATRIX_MODE_ID_SINE:
        {
            DBG_PRINTF("MatrixMode::createMode(): mode is SINE.\n");
            return NULL;
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
