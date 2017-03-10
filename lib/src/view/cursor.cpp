/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <glm/glm.hpp>
#include "mesh.hpp"
#include "mesh-util.hpp"
#include "opengl.hpp"
#include "render-mode.hpp"
#include "view/cursor.hpp"

struct ViewCursor::Impl {
  Mesh       radiusMesh;
  float     _radius;
  bool       isEnabled;

  static const unsigned int numSectors = 40;

  Impl () 
    : _radius            (0.0f)
    ,  isEnabled         (false) 
  {
    this->radiusMesh = MeshUtil::icosphere (2);
    this->radiusMesh.renderMode ().constantShading (true);
    this->radiusMesh.bufferData ();
  }

  float radius () const {
    return this->_radius;
  }

  glm::vec3 position () const {
    return this->radiusMesh.position ();
  }

  const Color& color () const {
    return this->radiusMesh.color ();
  }

  void radius (float r) {
    this->_radius = r;
    this->update ();
  }

  void position (const glm::vec3& p) { 
    this->radiusMesh.position (p); 
  }

  void color (const Color& color) {
    this->radiusMesh.color (color);
  }

  void enable  () { this->isEnabled = true;  }
  void disable () { this->isEnabled = false; }

  void render (Camera& camera) const {
    if (this->isEnabled) {
      OpenGLApi& opengl = OpenGL::instance();
      opengl.glClear         (opengl.StencilBufferBit ());

      opengl.glDepthMask     (false);
      opengl.glColorMask     (false, false, false, false);
      opengl.glEnable        (opengl.StencilTest ());

      opengl.glCullFace      (opengl.Front ());

      opengl.glEnable        (opengl.StencilTest ());
      opengl.glStencilFunc   (opengl.Always (), 1, 255);
      opengl.glStencilOp     (opengl.Keep (), opengl.Replace (), opengl.Keep ());

      this->radiusMesh.render (camera);

      opengl.glCullFace      (opengl.Back ());
      opengl.glColorMask     (true, true, true, true);

      opengl.glStencilFunc   (opengl.Equal (), 1, 255);
      opengl.glStencilOp     (opengl.Keep (), opengl.Keep (), opengl.Keep ());

      opengl.glEnable        (opengl.Blend ());
      opengl.glBlendEquation (opengl.FuncAdd ());
      opengl.glBlendFunc     (opengl.DstColor (), opengl.Zero ());

      this->radiusMesh.render (camera);

      opengl.glDisable       (opengl.Blend ());
      opengl.glDisable       (opengl.StencilTest ());
      opengl.glDepthMask     (true);
    }
  }

  void update () {
    this->radiusMesh.scaling (glm::vec3 (this->radius ()));
  }
};

DELEGATE_BIG6    (ViewCursor)
DELEGATE_CONST   (float           , ViewCursor, radius)
DELEGATE_CONST   (glm::vec3       , ViewCursor, position)
DELEGATE_CONST   (const Color&    , ViewCursor, color)
GETTER_CONST     (bool            , ViewCursor, isEnabled)
DELEGATE1        (void            , ViewCursor, radius, float)
DELEGATE1        (void            , ViewCursor, position, const glm::vec3&)
DELEGATE1        (void            , ViewCursor, color, const Color&)
DELEGATE         (void            , ViewCursor, enable)
DELEGATE         (void            , ViewCursor, disable)
DELEGATE1_CONST  (void            , ViewCursor, render, Camera&)
