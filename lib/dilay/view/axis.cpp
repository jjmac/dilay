/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "camera.hpp"
#include "color.hpp"
#include "config.hpp"
#include "dimension.hpp"
#include "mesh.hpp"
#include "mesh-util.hpp"
#include "opengl.hpp"
#include "render-mode.hpp"
#include "view/axis.hpp"

struct ViewAxis::Impl {
  Mesh         cylinderMesh;
  Mesh         gridMesh;
  glm::uvec2   axisResolution;
  Color        axisColor;
  Color        axisLabelColor;
  glm::vec3    axisScaling;
  glm::vec3    axisArrowScaling;
  unsigned int gridResolution;


  Impl (const Config& config) {
    this->runFromConfig (config);

    this->axisResolution = glm::uvec2 (200,200);
    this->gridResolution = 6;


    this->cylinderMesh   = MeshUtil::cylinder(6);

    this->cylinderMesh.renderMode ().constantShading (true);
    this->cylinderMesh.renderMode ().cameraRotationOnly (true);
    this->cylinderMesh.bufferData ();

    this->initializeGrid          ();
  }

  void initializeGrid () {
    float gridSpace = 1.0f / float (gridResolution - 1);

    for (unsigned int j = 0; j < gridResolution; j++) {
      for (unsigned int i = 0; i < gridResolution; i++) {
        this->gridMesh.addVertex (glm::vec3 ( float (i) * gridSpace
                                            , float (j) * gridSpace
                                            , 0.0f));
      }
    }
    for (unsigned int j = 1; j < gridResolution; j++) {
      for (unsigned int i = 1; i < gridResolution; i++) {
        this->gridMesh.addIndex ((j*gridResolution)+i-1);
        this->gridMesh.addIndex ((j*gridResolution)+i);
        this->gridMesh.addIndex (((j-1)*gridResolution)+i);
        this->gridMesh.addIndex ((j*gridResolution)+i);
      }
    }
    this->gridMesh.renderMode ().constantShading (true);
    this->gridMesh.renderMode ().cameraRotationOnly (true);
    this->gridMesh.bufferData ();
  }

  void render (Camera& camera) {
    OpenGL::instance().glClear (OpenGL::instance().DepthBufferBit ());

    const glm::uvec2  resolution = camera.resolution ();
    camera.updateResolution (glm::uvec2 (200,200));

    this->cylinderMesh.scaling        (this->axisScaling);

    this->cylinderMesh.position       (glm::vec3 (0.0f, this->axisScaling.y * 0.5f, 0.0f));
    this->cylinderMesh.rotationMatrix (glm::mat4x4 (1.0f));
    this->cylinderMesh.color          (Color(.5f, .2f, .2f));
    this->cylinderMesh.render         (camera);

    this->cylinderMesh.position       (glm::vec3 (this->axisScaling.y * 0.5f, 0.0f, 0.0f));
    this->cylinderMesh.rotationZ      (0.5f * glm::pi<float> ());
    this->cylinderMesh.color          (Color(.2f, .5f, .2f));
    this->cylinderMesh.render         (camera);

    this->cylinderMesh.position       (glm::vec3 (0.0f, 0.0f, this->axisScaling.y * 0.5f));
    this->cylinderMesh.rotationX      (0.5f * glm::pi<float> ());
    this->cylinderMesh.color          (Color(.2f, .5f, .2f));
    this->cylinderMesh.render         (camera);


    this->renderGrid                  (camera);

    camera.updateResolution (resolution);
  }

  void renderGrid (Camera& camera) {
    switch (camera.primaryDimension ()) {
      case Dimension::X:
        this->gridMesh.rotationY (- 0.5f * glm::pi<float> ());
        break;
      case Dimension::Y:
        this->gridMesh.rotationX (0.5f * glm::pi<float> ());
        break;
      case Dimension::Z:
        this->gridMesh.rotationMatrix (glm::mat4x4 (1.0f));
        break;
    }
    this->gridMesh.scaling     (glm::vec3 (this->axisScaling.y));
    this->gridMesh.color       (this->axisColor);
    this->gridMesh.renderLines (camera);
  }

  void runFromConfig (const Config& config) {
    this->axisLabelColor   = config.get <Color>     ("editor/axis/color/label");
    this->axisColor        = config.get <Color>     ("editor/axis/color/normal");
    this->axisScaling      = config.get <glm::vec3> ("editor/axis/scaling");
	this->axisArrowScaling = config.get <glm::vec3> ("editor/axis/arrowScaling");
  }
};

DELEGATE1_BIG3 (ViewAxis, const Config&)
DELEGATE1 (void, ViewAxis, render, Camera&)
//DELEGATE2 (void, ViewAxis, render, Camera&, QPainter&)
DELEGATE1 (void, ViewAxis, runFromConfig, const Config&)
