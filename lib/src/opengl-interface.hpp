/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_OPENGL_INTERFACE
#define DILAY_OPENGL_INTERFACE

#include <glm/fwd.hpp>
#include <string>

class OpenGLInterface {
  virtual unsigned int Always             ();
  virtual unsigned int ArrayBuffer        ();
  virtual unsigned int Back               ();
  virtual unsigned int Blend              ();
  virtual unsigned int ColorBufferBit     ();
  virtual unsigned int CullFace           ();
  virtual unsigned int CW                 ();
  virtual unsigned int CCW                ();
  virtual unsigned int Decr               ();
  virtual unsigned int DecrWrap           ();
  virtual unsigned int DepthBufferBit     ();
  virtual unsigned int DepthTest          ();
  virtual unsigned int DstColor           ();
  virtual unsigned int ElementArrayBuffer ();
  virtual unsigned int Equal              ();
  virtual unsigned int Fill               ();
  virtual unsigned int Float              ();
  virtual unsigned int Front              ();
  virtual unsigned int FrontAndBack       ();
  virtual unsigned int FuncAdd            ();
  virtual unsigned int Greater            ();
  virtual unsigned int Incr               ();
  virtual unsigned int IncrWrap           ();
  virtual unsigned int Invert             ();
  virtual unsigned int Keep               ();
  virtual unsigned int LEqual             ();
  virtual unsigned int Line               ();
  virtual unsigned int Lines              ();
  virtual unsigned int Never              ();
  virtual unsigned int PolygonOffsetFill  ();
  virtual unsigned int Replace            ();
  virtual unsigned int StaticDraw         ();
  virtual unsigned int StencilBufferBit   ();
  virtual unsigned int StencilTest        ();
  virtual unsigned int Triangles          ();
  virtual unsigned int UnsignedInt        ();
  virtual unsigned int Zero               ();

  virtual void glBindBuffer               (unsigned int, unsigned int);
  virtual void glBlendEquation            (unsigned int);
  virtual void glBlendFunc                (unsigned int, unsigned);
  virtual void glBufferData               (unsigned int, unsigned int, const void*, unsigned int);
  virtual void glClear                    (unsigned int);
  virtual void glClearColor               (float, float, float, float);
  virtual void glClearStencil             (int);
  virtual void glColorMask                (bool, bool, bool, bool);
  virtual void glCullFace                 (unsigned int);
  virtual void glDepthFunc                (unsigned int);
  virtual void glDepthMask                (bool);
  virtual void glDisable                  (unsigned int);
  virtual void glDisableVertexAttribArray (unsigned int);
  virtual void glDrawElements             (unsigned int, unsigned int, unsigned int, const void*);
  virtual void glEnable                   (unsigned int);
  virtual void glEnableVertexAttribArray  (unsigned int);
  virtual void glFrontFace                (unsigned int);
  virtual void glGenBuffers               (unsigned int, unsigned int*);
  virtual int  glGetUniformLocation       (unsigned int, const char*);
  virtual bool glIsBuffer                 (unsigned int);
  virtual bool glIsProgram                (unsigned int);
  virtual void glPolygonMode              (unsigned int, unsigned int);
  virtual void glPolygonOffset            (float, float);
  virtual void glStencilFunc              (unsigned int, int, unsigned int);
  virtual void glStencilOp                (unsigned int, unsigned int, unsigned int);
  virtual void glUniform1f                (int, float);
  virtual void glUniformMatrix3fv         (int, unsigned int, bool, const float*);
  virtual void glUniformMatrix4fv         (int, unsigned int, bool, const float*);
  virtual void glUseProgram               (unsigned int);
  virtual void glVertexAttribPointer      (unsigned int, int, unsigned int, bool, unsigned int, const void*);
  virtual void glViewport                 (unsigned int, unsigned int, unsigned int, unsigned int);

  // utilities
  enum VertexAttributIndex { PositionIndex = 0
                           , NormalIndex   = 1
                           };

  virtual bool         supportsGeometryShader ();
  virtual void         glUniformVec3          (unsigned int, const glm::vec3&);
  virtual void         glUniformVec4          (unsigned int, const glm::vec4&);
  virtual void         safeDeleteBuffer       (unsigned int&);
  virtual void         safeDeleteShader       (unsigned int&);
  virtual void         safeDeleteProgram      (unsigned int&);
  virtual unsigned int loadProgram            (const char*, const char*, bool);
}

#endif
