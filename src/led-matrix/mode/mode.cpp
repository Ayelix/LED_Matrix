#include <led-matrix/mode/mode.hpp>

#include <stdexcept>
#include <sstream>

#include <led-matrix/debug/debug.hpp>

MatrixMode::MatrixMode(MatrixModeID id)
{
    if (id < MATRIX_MODE_ID_COUNT)
    {
        m_id = id;
    }
    else
    {
        std::ostringstream oss;
        oss << "MatrixMode::MatrixMode(): Invalid mode ID " << id;
        std::string const errorStr = oss.str();
        DBG_PRINTF("%s\n", errorStr.c_str());
        throw std::invalid_argument(errorStr);
    }
}

void MatrixMode::destroyMode(MatrixMode * mode)
{
    if (mode != NULL)
    {
        // TODO: cast to appropriate implementation and delete
        switch (mode->getID())
        {
            case MATRIX_MODE_ID_IDLE:
            {
                DBG_PRINTF("MatrixMode::destroyMode(): mode is IDLE.\n");
                return;
            }
            
            case MATRIX_MODE_ID_TEXT:
            {
                DBG_PRINTF("MatrixMode::destroyMode(): mode is TEXT.\n");
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
        
        // If this point is reached, the mode is of an unknown type
        std::ostringstream oss;
        oss << "MatrixMode::destroyMode(): Invalid mode ID " << mode->getID();
        std::string const errorStr = oss.str();
        DBG_PRINTF("%s\n", errorStr.c_str());
        throw std::invalid_argument(errorStr);
    }
    else
    {
        std::string const errorStr =
            "MatrixMode::destroyMode(): mode must not be NULL";
        DBG_PRINTF("%s\n", errorStr.c_str());
        throw std::invalid_argument(errorStr);
    }
}

MatrixMode * MatrixMode::createMode(MatrixModeID id)
{
    // TODO: allocate appropriate implementation instance
    switch (id)
    {
        case MATRIX_MODE_ID_IDLE:
        {
            DBG_PRINTF("MatrixMode::createMode(): mode is IDLE.\n");
            return NULL;
        }
        
        case MATRIX_MODE_ID_TEXT:
        {
            DBG_PRINTF("MatrixMode::createMode(): mode is TEXT.\n");
            return NULL;
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
    
    // If this point is reached, the mode is of an unknown type
    std::ostringstream oss;
    oss << "MatrixMode::createMode(): Invalid mode ID " << id;
    std::string const errorStr = oss.str();
    DBG_PRINTF("%s\n", errorStr.c_str());
    throw std::invalid_argument(errorStr);
}
