/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "camera.hpp"
#include "color.hpp"
#include "config.hpp"
#include "dimension.hpp"
#include "mesh.hpp"
#include "mesh-util.hpp"
#include "mirror.hpp"
#include "opengl.hpp"
#include "primitive/plane.hpp"
#include "render-mode.hpp"

struct Mirror::Impl {
  Dimension                   _dimension;
  float                        width;
  Mesh                         mirrorMesh;
  std::unique_ptr <PrimPlane> _plane;

  Impl (const Config& config, Dimension d)
    : _dimension (d)
    ,  width     (1.0f)
  {
    this->mirrorMesh = MeshUtil::cube ();
    this->mirrorMesh.renderMode ().constantShading (true);
    this->mirrorMesh.bufferData ();

    this->runFromConfig (config);
    this->dimension     (d);
  }

  Dimension dimension () const {
    return this->_dimension;
  }

  void dimension (Dimension d) {
    this->_dimension = d;
    this->_plane     = std::make_unique <PrimPlane> (glm::vec3 (0.0f), DimensionUtil::vector (d));
  }

  const PrimPlane& plane () const {
    return *this->_plane;
  }

  void render (Camera& camera) const {
    const glm::vec3 pos    = camera.position ();
    const float     width2 = this->width * 0.5f;
    const bool      inside = 
        (this->_dimension == Dimension::X && glm::abs (pos.x) <= width2)
     || (this->_dimension == Dimension::Y && glm::abs (pos.y) <= width2)
     || (this->_dimension == Dimension::Z && glm::abs (pos.z) <= width2);

    OpenGL::instance().glClear         (OpenGL::instance().StencilBufferBit ());
    OpenGL::instance().glDepthMask     (false);
    OpenGL::instance().glEnable        (OpenGL::instance().StencilTest ());

    OpenGL::instance().glColorMask     (false, false, false, false);

    OpenGL::instance().glCullFace      (OpenGL::instance().Front ());

    OpenGL::instance().glEnable        (OpenGL::instance().StencilTest ());
    OpenGL::instance().glStencilFunc   (OpenGL::instance().Always (), 1, 255);
    OpenGL::instance().glStencilOp     (OpenGL::instance().Keep (), OpenGL::instance().Replace (), OpenGL::instance().Keep ());

    this->mirrorMesh.render (camera);

    if (inside) {
      OpenGL::instance().glDisable (OpenGL::instance().DepthTest ());
    }
    else {
      OpenGL::instance().glCullFace (OpenGL::instance().Back ());
    }

    OpenGL::instance().glColorMask     (true, true, true, true);

    OpenGL::instance().glStencilFunc   (OpenGL::instance().Equal (), 1, 255);
    OpenGL::instance().glStencilOp     (OpenGL::instance().Keep (), OpenGL::instance().Keep (), OpenGL::instance().Keep ());

    OpenGL::instance().glEnable        (OpenGL::instance().Blend ());
    OpenGL::instance().glBlendEquation (OpenGL::instance().FuncAdd ());
    OpenGL::instance().glBlendFunc     (OpenGL::instance().DstColor (), OpenGL::instance().Zero ());

    this->mirrorMesh.render (camera);

    OpenGL::instance().glDisable       (OpenGL::instance().Blend ());

    if (inside) {
      OpenGL::instance().glCullFace (OpenGL::instance().Back ());
      OpenGL::instance().glEnable   (OpenGL::instance().DepthTest ());
    }
    OpenGL::instance().glDisable   (OpenGL::instance().StencilTest ());
    OpenGL::instance().glDepthMask (true);
  }

  void updateMesh () {
    const float extent = 1000.0f;

    switch (this->_dimension) {
      case Dimension::X:
        this->mirrorMesh.scaling (glm::vec3 (this->width, extent, extent));
        break;
      case Dimension::Y:
        this->mirrorMesh.scaling (glm::vec3 (extent, this->width, extent));
        break;
      case Dimension::Z:
        this->mirrorMesh.scaling (glm::vec3 (extent, extent, this->width));
        break;
    }
  }

  void runFromConfig (const Config& config) {
    this->width = config.get <float> ("editor/tool/sculpt/mirror/width");
    this->mirrorMesh.color (config.get <Color> ("editor/tool/sculpt/mirror/color"));
    this->updateMesh ();
  }
};

DELEGATE2_BIG2 (Mirror, const Config&, Dimension)
DELEGATE_CONST  (Dimension       , Mirror, dimension)
GETTER_CONST    (float           , Mirror, width)
DELEGATE1       (void            , Mirror, dimension, Dimension)
DELEGATE_CONST  (const PrimPlane&, Mirror, plane)
DELEGATE1_CONST (void            , Mirror, render, Camera&)
DELEGATE1       (void     , Mirror, runFromConfig, const Config&)
