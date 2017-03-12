/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_OPENGL_API
#define DILAY_OPENGL_API

#include <glm/fwd.hpp>
#include <string>

class OpenGLApi {
public:
    virtual ~OpenGLApi() {}

  // wrappers
  virtual unsigned int Always             () = 0;
  virtual unsigned int ArrayBuffer        () = 0;
  virtual unsigned int Back               () = 0;
  virtual unsigned int Blend              () = 0;
  virtual unsigned int ColorBufferBit     () = 0;
  virtual unsigned int CullFace           () = 0;
  virtual unsigned int CW                 () = 0;
  virtual unsigned int CCW                () = 0;
  virtual unsigned int Decr               () = 0;
  virtual unsigned int DecrWrap           () = 0;
  virtual unsigned int DepthBufferBit     () = 0;
  virtual unsigned int DepthTest          () = 0;
  virtual unsigned int DstColor           () = 0;
  virtual unsigned int ElementArrayBuffer () = 0;
  virtual unsigned int Equal              () = 0;
  virtual unsigned int Fill               () = 0;
  virtual unsigned int Float              () = 0;
  virtual unsigned int Front              () = 0;
  virtual unsigned int FrontAndBack       () = 0;
  virtual unsigned int FuncAdd            () = 0;
  virtual unsigned int Greater            () = 0;
  virtual unsigned int Incr               () = 0;
  virtual unsigned int IncrWrap           () = 0;
  virtual unsigned int Invert             () = 0;
  virtual unsigned int Keep               () = 0;
  virtual unsigned int LEqual             () = 0;
  virtual unsigned int Line               () = 0;
  virtual unsigned int Lines              () = 0;
  virtual unsigned int Never              () = 0;
  virtual unsigned int PolygonOffsetFill  () = 0;
  virtual unsigned int Replace            () = 0;
  virtual unsigned int StaticDraw         () = 0;
  virtual unsigned int StencilBufferBit   () = 0;
  virtual unsigned int StencilTest        () = 0;
  virtual unsigned int Triangles          () = 0;
  virtual unsigned int UnsignedInt        () = 0;
  virtual unsigned int Zero               () = 0;

  virtual void glBindBuffer               (unsigned int, unsigned int) = 0;
  virtual void glBlendEquation            (unsigned int) = 0;
  virtual void glBlendFunc                (unsigned int, unsigned) = 0;
  virtual void glBufferData               (unsigned int, unsigned int, const void*, unsigned int) = 0;
  virtual void glClear                    (unsigned int) = 0;
  virtual void glClearColor               (float, float, float, float) = 0;
  virtual void glClearStencil             (int) = 0;
  virtual void glColorMask                (bool, bool, bool, bool) = 0;
  virtual void glCullFace                 (unsigned int) = 0;
  virtual void glDepthFunc                (unsigned int) = 0;
  virtual void glDepthMask                (bool) = 0;
  virtual void glDisable                  (unsigned int) = 0;
  virtual void glDisableVertexAttribArray (unsigned int) = 0;
  virtual void glDrawElements             (unsigned int, unsigned int, unsigned int, const void*) = 0;
  virtual void glEnable                   (unsigned int) = 0;
  virtual void glEnableVertexAttribArray  (unsigned int) = 0;
  virtual void glFrontFace                (unsigned int) = 0;
  virtual void glGenBuffers               (unsigned int, unsigned int*) = 0;
  virtual int  glGetUniformLocation       (unsigned int, const char*) = 0;
  virtual bool glIsBuffer                 (unsigned int) = 0;
  virtual bool glIsProgram                (unsigned int) = 0;
  virtual void glPolygonMode              (unsigned int, unsigned int) = 0;
  virtual void glPolygonOffset            (float, float) = 0;
  virtual void glStencilFunc              (unsigned int, int, unsigned int) = 0;
  virtual void glStencilOp                (unsigned int, unsigned int, unsigned int) = 0;
  virtual void glUniform1f                (int, float) = 0;
  virtual void glUniformMatrix3fv         (int, unsigned int, bool, const float*) = 0;
  virtual void glUniformMatrix4fv         (int, unsigned int, bool, const float*) = 0;
  virtual void glUseProgram               (unsigned int) = 0;
  virtual void glVertexAttribPointer      (unsigned int, int, unsigned int, bool, unsigned int, const void*) = 0;
  virtual void glViewport                 (unsigned int, unsigned int, unsigned int, unsigned int) = 0;

  // utilities
  enum VertexAttributIndex { PositionIndex = 0
                           , NormalIndex   = 1
                           };

  virtual bool         supportsGeometryShader () = 0;
  virtual void         glUniformVec3          (unsigned int, const glm::vec3&) = 0;
  virtual void         glUniformVec4          (unsigned int, const glm::vec4&) = 0;
  virtual void         safeDeleteBuffer       (unsigned int&) = 0;
  virtual void         safeDeleteShader       (unsigned int&) = 0;
  virtual void         safeDeleteProgram      (unsigned int&) = 0;
  virtual unsigned int loadProgram            (const char*, const char*, bool) = 0;
};

#endif
