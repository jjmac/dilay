#include "opengl.hpp"

#include <assert.h>

namespace {
static OpenGLApi* self = nullptr;
}

OpenGLApi& OpenGL::instance()
{
    assert(self);
    return *self;
}


void OpenGL::install(OpenGLApi* oga) {
    self = oga;
}

