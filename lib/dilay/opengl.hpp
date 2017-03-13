#ifndef DILAY_OPENGL
#define DILAY_OPENGL

#include "globals.hpp"

#include "opengl-Api.hpp"
#include <memory>

class DILAY_LIB_EXPORT OpenGL
{
public:
    static OpenGLApi& instance();
    static void install(OpenGLApi* oga);
};

#endif

