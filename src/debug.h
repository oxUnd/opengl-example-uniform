#ifndef __GL_DEBUG_H__
#define __GL_DEBUG_H__

#include <iostream>

enum DEBUG_L {
    INFO = 0,
    WARN,
    ERROR
};

void DEBUG(const char *type, const char *msg, unsigned int level);

#endif
