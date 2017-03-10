#ifndef DILAY_OPENGL
#define DILAY_OPENGL

#include "opengl-Api.hpp"
#include <memory>

class OpenGL
{
public:
    inline static OpenGLApi& instance() { return *impl; }
    static void install(OpenGLApi* oga) { return impl.reset(oga); }

private:
    static std::unique_ptr<OpenGLApi> impl;
};

#endif

