#include "debug.h"



void DEBUG(const char *type, const char *msg, unsigned int level) {

#ifdef __GL_DEBUG_FLAG__
    std::cout << type << "::ERROR " << msg << std::endl;
#endif
    
}
