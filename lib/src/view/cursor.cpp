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
      OpenGL::instance().glClear         (OpenGL::instance().StencilBufferBit ());

      OpenGL::instance().glDepthMask     (false);
      OpenGL::instance().glColorMask     (false, false, false, false);
      OpenGL::instance().glEnable        (OpenGL::instance().StencilTest ());

      OpenGL::instance().glCullFace      (OpenGL::instance().Front ());

      OpenGL::instance().glEnable        (OpenGL::instance().StencilTest ());
      OpenGL::instance().glStencilFunc   (OpenGL::instance().Always (), 1, 255);
      OpenGL::instance().glStencilOp     (OpenGL::instance().Keep (), OpenGL::instance().Replace (), OpenGL::instance().Keep ());

      this->radiusMesh.render (camera);

      OpenGL::instance().glCullFace      (OpenGL::instance().Back ());
      OpenGL::instance().glColorMask     (true, true, true, true);

      OpenGL::instance().glStencilFunc   (OpenGL::instance().Equal (), 1, 255);
      OpenGL::instance().glStencilOp     (OpenGL::instance().Keep (), OpenGL::instance().Keep (), OpenGL::instance().Keep ());

      OpenGL::instance().glEnable        (OpenGL::instance().Blend ());
      OpenGL::instance().glBlendEquation (OpenGL::instance().FuncAdd ());
      OpenGL::instance().glBlendFunc     (OpenGL::instance().DstColor (), OpenGL::instance().Zero ());

      this->radiusMesh.render (camera);

      OpenGL::instance().glDisable       (OpenGL::instance().Blend ());
      OpenGL::instance().glDisable       (OpenGL::instance().StencilTest ());
      OpenGL::instance().glDepthMask     (true);
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
