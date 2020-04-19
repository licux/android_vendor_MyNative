#include <GLES2/gl2.h>
#include <string.h>

#include "util.h"

bool hasExtension(const char* extension){
    const auto* EXTENSION = glGetString(GL_EXTENSIONS);

    return strstr((const char*)EXTENSION, extension) != NULL;
}
