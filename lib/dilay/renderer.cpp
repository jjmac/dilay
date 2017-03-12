/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <glm/glm.hpp>
#include "color.hpp"
#include "config.hpp"
#include "opengl.hpp"
#include "render-mode.hpp"
#include "renderer.hpp"
#include "util.hpp"

namespace {
  const unsigned int numLights = 2;

  struct LightIds {
    int directionId;
    int colorId;
    int irradianceId;

    LightIds () : directionId  (0)
                , colorId      (0)
                , irradianceId (0)
    {}
  };
  
  struct ShaderIds {
    unsigned int programId;
    int          modelId;
    int          modelNormalId;
    int          viewId;
    int          projectionId;
    int          colorId;
    int          wireframeColorId;
    int          eyePointId;
    int          barycentricId;
    LightIds     lightIds [numLights];

    ShaderIds () : programId        (0)
                 , modelId          (0)
                 , modelNormalId    (0)
                 , viewId           (0)
                 , projectionId     (0)
                 , colorId          (0)
                 , wireframeColorId (0)
                 , eyePointId       (0)
                 , barycentricId    (0)
    {}
  };

  struct GlobalLightUniforms {
    glm::vec3 direction;
    Color     color;
    float     irradiance;
  };

  struct GlobalUniforms {
    GlobalLightUniforms lightUniforms [numLights];
    glm::vec3           eyePoint;
  };
};

struct Renderer::Impl {
  static const unsigned int numShaders = 6;

  ShaderIds      shaderIds [Impl::numShaders];
  ShaderIds*     activeShaderIndex;
  GlobalUniforms globalUniforms;
  Color          clearColor;

  Impl (const Config& config) 
    : activeShaderIndex (nullptr) 
  {
    this->runFromConfig (config);
  }

  ~Impl () {
    for (unsigned int i = 0; i < Impl::numShaders; i++) {
      OpenGL::instance().safeDeleteProgram (this->shaderIds[i].programId);
    }
  }

  void setupRendering () {
    OpenGLApi& opengl = OpenGL::instance();
    opengl.glClearColor   ( this->clearColor.r ()
                           , this->clearColor.g ()
                           , this->clearColor.b (), 0.0f);
    opengl.glClearStencil (0);
    opengl.glFrontFace    (opengl.CCW       ());
    opengl.glEnable       (opengl.CullFace  ());
    opengl.glCullFace     (opengl.Back      ());
    opengl.glEnable       (opengl.DepthTest ());
    opengl.glDepthFunc    (opengl.LEqual    ());
    opengl.glClear        ( opengl.ColorBufferBit ()
                           | opengl.DepthBufferBit () );
  }

  unsigned int shaderIndex (const RenderMode& renderMode) {
    if (renderMode.smoothShading ()) {
      return renderMode.renderWireframe () ? 0 : 1;
    }
    else if (renderMode.flatShading ()) {
      return renderMode.renderWireframe () ? 2 : 3;
    }
    else if (renderMode.constantShading ()) {
      return renderMode.renderWireframe () ? 4 : 5;
    }
    else {
      DILAY_IMPOSSIBLE
    }
  }

  void initalizeProgram (const RenderMode& renderMode) {
    OpenGLApi& opengl = OpenGL::instance();
    assert ( renderMode.renderWireframe () == false
          || opengl.supportsGeometryShader () );

    const unsigned int id = opengl.loadProgram ( renderMode.vertexShader ()
                                                , renderMode.fragmentShader ()
                                                , renderMode.renderWireframe () );

    unsigned int index = this->shaderIndex (renderMode);
    assert (this->shaderIds[index].programId == 0);

    ShaderIds *s = &this->shaderIds [index];

    s->programId                = id;
    s->modelId                  = opengl.glGetUniformLocation (id, "model");
    s->modelNormalId            = opengl.glGetUniformLocation (id, "modelNormal");
    s->viewId                   = opengl.glGetUniformLocation (id, "view");
    s->projectionId             = opengl.glGetUniformLocation (id, "projection");
    s->colorId                  = opengl.glGetUniformLocation (id, "color");
    s->wireframeColorId         = opengl.glGetUniformLocation (id, "wireframeColor");
    s->eyePointId               = opengl.glGetUniformLocation (id, "eyePoint");
    s->barycentricId            = opengl.glGetUniformLocation (id, "barycentric");
    s->lightIds[0].directionId  = opengl.glGetUniformLocation (id, "light1Direction");
    s->lightIds[0].colorId      = opengl.glGetUniformLocation (id, "light1Color");
    s->lightIds[0].irradianceId = opengl.glGetUniformLocation (id, "light1Irradiance");
    s->lightIds[1].directionId  = opengl.glGetUniformLocation (id, "light2Direction");
    s->lightIds[1].colorId      = opengl.glGetUniformLocation (id, "light2Color");
    s->lightIds[1].irradianceId = opengl.glGetUniformLocation (id, "light2Irradiance");
  }

  void setProgram (const RenderMode& renderMode) {
    const unsigned int index = this->shaderIndex (renderMode);

    if (this->shaderIds[index].programId == 0) {
      this->initalizeProgram (renderMode);
    }
    assert (this->shaderIds[index].programId);
    OpenGLApi& opengl = OpenGL::instance();

    this->activeShaderIndex = &this->shaderIds[index];
    opengl.glUseProgram (this->activeShaderIndex->programId);

    opengl.glUniformVec3
      (this->activeShaderIndex->eyePointId, this->globalUniforms.eyePoint);

    for (unsigned int i = 0; i < numLights; i++) {
      opengl.glUniformVec3
        ( this->activeShaderIndex->lightIds[i].directionId
        , this->globalUniforms.lightUniforms[i].direction);
      opengl.glUniformVec3
        ( this->activeShaderIndex->lightIds[i].colorId
        , this->globalUniforms.lightUniforms[i].color.vec3 ());
      opengl.glUniform1f ( this->activeShaderIndex->lightIds[i].irradianceId
                          , this->globalUniforms.lightUniforms[i].irradiance );
    }
  }

  void setModel (const float* model, const float* modelNormal) {
    assert (this->activeShaderIndex);
    OpenGL::instance().glUniformMatrix4fv (this->activeShaderIndex->modelId, 1, false, model);
    OpenGL::instance().glUniformMatrix3fv (this->activeShaderIndex->modelNormalId, 1, false, modelNormal);
  }

  void setView (const float* view) {
    assert (this->activeShaderIndex);
    OpenGL::instance().glUniformMatrix4fv (this->activeShaderIndex->viewId, 1, false, view);
  }

  void setProjection (const float* projection) {
    assert (this->activeShaderIndex);
    OpenGL::instance().glUniformMatrix4fv (this->activeShaderIndex->projectionId, 1, false, projection);
  }

  void setColor3 (const Color& c) {
    assert (this->activeShaderIndex);
    OpenGL::instance().glUniformVec3 (this->activeShaderIndex->colorId, c.vec3 ());
  }

  void setColor4 (const Color& c) {
    assert (this->activeShaderIndex);
    OpenGL::instance().glUniformVec4 (this->activeShaderIndex->colorId, c.vec4 ());
  }

  void setWireframeColor3 (const Color& c) {
    assert (this->activeShaderIndex);
    OpenGL::instance().glUniformVec3 (this->activeShaderIndex->wireframeColorId, c.vec3 ());
  }

  void setWireframeColor4 (const Color& c) {
    assert (this->activeShaderIndex);
    OpenGL::instance().glUniformVec4 (this->activeShaderIndex->wireframeColorId, c.vec4 ());
  }

  void setEyePoint (const glm::vec3& e) {
    this->globalUniforms.eyePoint = e;
  }

  void setLightDirection (unsigned int i, const glm::vec3& d) {
    assert (i < numLights);
    this->globalUniforms.lightUniforms[i].direction = d;
  }

  void setLightColor (unsigned int i, const Color& c) {
    assert (i < numLights);
    this->globalUniforms.lightUniforms[i].color = c;
  }

  void setLightIrradiance (unsigned int i, float irr) {
    assert (i < numLights);
    this->globalUniforms.lightUniforms[i].irradiance = irr;
  }

  void runFromConfig (const Config& config) {
    this->clearColor = config.get<Color> ("editor/background");

    for (unsigned int i = 0; i < numLights; i++) {
      const std::string key = "editor/light/light" + std::to_string (i+1) + "/";

      const glm::vec3 dir = config.get<glm::vec3> (key + "direction");
      this->setLightDirection  (i, glm::normalize (dir));
      this->setLightColor      (i, config.get<Color> (key + "color"));
      this->setLightIrradiance (i, config.get<float> (key + "irradiance"));
    }
  }
};

DELEGATE1_BIG3 (Renderer, const Config&)

DELEGATE  (void, Renderer, setupRendering)
DELEGATE1 (void, Renderer, setProgram        , const RenderMode&)
DELEGATE2 (void, Renderer, setModel          , const float*, const float*)
DELEGATE1 (void, Renderer, setView           , const float*)
DELEGATE1 (void, Renderer, setProjection     , const float*)
DELEGATE1 (void, Renderer, setColor3         , const Color&)
DELEGATE1 (void, Renderer, setColor4         , const Color&)
DELEGATE1 (void, Renderer, setWireframeColor3, const Color&)
DELEGATE1 (void, Renderer, setWireframeColor4, const Color&)
DELEGATE1 (void, Renderer, setEyePoint       , const glm::vec3&)
DELEGATE2 (void, Renderer, setLightDirection , unsigned int, const glm::vec3&)
DELEGATE2 (void, Renderer, setLightColor     , unsigned int, const Color&)
DELEGATE2 (void, Renderer, setLightIrradiance, unsigned int, float)
DELEGATE1 (void, Renderer, runFromConfig     , const Config&)
