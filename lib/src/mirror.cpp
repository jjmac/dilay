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

    OpenGLApi& opengl = OpenGL::instance();
    opengl.glClear         (opengl.StencilBufferBit ());
    opengl.glDepthMask     (false);
    opengl.glEnable        (opengl.StencilTest ());

    opengl.glColorMask     (false, false, false, false);

    opengl.glCullFace      (opengl.Front ());

    opengl.glEnable        (opengl.StencilTest ());
    opengl.glStencilFunc   (opengl.Always (), 1, 255);
    opengl.glStencilOp     (opengl.Keep (), opengl.Replace (), opengl.Keep ());

    this->mirrorMesh.render (camera);

    if (inside) {
      opengl.glDisable (opengl.DepthTest ());
    }
    else {
      opengl.glCullFace (opengl.Back ());
    }

    opengl.glColorMask     (true, true, true, true);

    opengl.glStencilFunc   (opengl.Equal (), 1, 255);
    opengl.glStencilOp     (opengl.Keep (), opengl.Keep (), opengl.Keep ());

    opengl.glEnable        (opengl.Blend ());
    opengl.glBlendEquation (opengl.FuncAdd ());
    opengl.glBlendFunc     (opengl.DstColor (), opengl.Zero ());

    this->mirrorMesh.render (camera);

    opengl.glDisable       (opengl.Blend ());

    if (inside) {
      opengl.glCullFace (opengl.Back ());
      opengl.glEnable   (opengl.DepthTest ());
    }
    opengl.glDisable   (opengl.StencilTest ());
    opengl.glDepthMask (true);
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
