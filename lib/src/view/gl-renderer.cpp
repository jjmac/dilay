/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QDebug>
#include <QPainter>
#include <QOpenGLPaintDevice>
#include <glm/glm.hpp>
#include "camera.hpp"
#include "opengl.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "state.hpp"
#include "tool.hpp"
#include "tool/move-camera.hpp"
#include "view/axis.hpp"
#include "view/floor-plane.hpp"
#include "view/gl-renderer.hpp"
#include "view/main-window.hpp"
#include "view/pointing-event.hpp"
#include "view/util.hpp"

struct ViewGlRenderer::Impl {
  typedef std::unique_ptr <State>          StatePtr;
  typedef std::unique_ptr <ViewAxis>       AxisPtr;
  typedef std::unique_ptr <ViewFloorPlane> FloorPlanePtr;
  typedef std::unique_ptr <QOpenGLContext> QOpenGLContextPtr;
  typedef std::unique_ptr <QOpenGLPaintDevice>       OpenGLDevicePtr;

  ViewGlRenderer*   self;
  QOpenGLContext    context;
  OpenGLDevicePtr   _device;
  QSurface&       surface;
  Config&         config;
  Cache&          cache;
  ToolMoveCamera  toolMoveCamera;
  StatePtr       _state;
  AxisPtr         axis;
  FloorPlanePtr  _floorPlane;
  bool            tabletPressed;

  Impl (ViewGlRenderer* s, QSurface& surf, Config& cfg, Cache& cch)
    : self           (s)
    , surface        (surf)
    , config         (cfg)
    , cache          (cch)
    , toolMoveCamera (cfg)
    ,_state          (nullptr)
    , axis           (nullptr)
    ,_floorPlane     (nullptr)
    , tabletPressed  (false)
  {
//      context = new QOpenGLContext() ;
      // from QWidget
    // this->self->setAutoFillBackground (false);
  }

  ~Impl () {
      // from QOpenGLWidget
    context.makeCurrent (&surface);

    this->_state     .reset (nullptr);
    this-> axis      .reset (nullptr);
    this->_floorPlane.reset (nullptr);

      // from QOpenGLWidget
    context.doneCurrent ();
  }

  State& state () {
    assert (this->_state);
    return *this->_state;
  }

  ViewFloorPlane& floorPlane () {
    assert (this->_floorPlane);
    return *this->_floorPlane;
  }

  glm::ivec2 cursorPosition () {
      return glm::ivec2();
//    return ViewUtil::toIVec2 (this->self->mapFromGlobal (QCursor::pos ()));
  }

  void fromConfig () {
    assert (this->axis);

    this->state ().fromConfig ();
    this->axis->fromConfig (this->config);

    this->floorPlane ().fromConfig (this->config);
    this->floorPlane ().update (this->state ().camera ());

    this->toolMoveCamera.fromConfig (this->config);
  }

  void initializeGL () {
    context.create();

    OpenGL::initializeFunctions ();

    this->_state     .reset (new State (this->config, this->cache));
    this-> axis      .reset (new ViewAxis (this->config));
    this->_floorPlane.reset (new ViewFloorPlane (this->config, this->state ().camera ()));

    _device.reset(new QOpenGLPaintDevice());
//    this->self->setMouseTracking (true);
  }

  void paint () {
      paintGL();
  }

  void paintGL () {
      qDebug() << "PAINTGL" ;
    QPainter painter (this->_device.get());
    painter.beginNativePainting ();

    this->state ().camera ().renderer ().setupRendering ();
    this->state ().scene  ().render (this->state ().camera ());
    this->floorPlane ().render (this->state ().camera ());

    if (this->state ().hasTool ()) {
      this->state ().tool ().render ();
    }
    this->axis->render (this->state ().camera ());

    OpenGL::glDisable (OpenGL::DepthTest ()); 
    painter.endNativePainting ();

    this->axis->render (this->state ().camera (), painter);
  }

  void resizeGL (int w, int h) {
    this->state ().camera ().updateResolution (glm::uvec2 (w,h));
    this->_device->setSize(QSize(w, h));
  }
};

DELEGATE3_BIG2_SELF (ViewGlRenderer, QSurface& , Config&, Cache&)
GETTER    (ToolMoveCamera&, ViewGlRenderer, toolMoveCamera)
DELEGATE  (State&         , ViewGlRenderer, state)
DELEGATE  (ViewFloorPlane&, ViewGlRenderer, floorPlane)
DELEGATE  (glm::ivec2     , ViewGlRenderer, cursorPosition)
DELEGATE  (void           , ViewGlRenderer, fromConfig)
DELEGATE  (void           , ViewGlRenderer, paint)
DELEGATE  (void           , ViewGlRenderer, initializeGL)
DELEGATE2 (void           , ViewGlRenderer, resizeGL, int, int)
DELEGATE  (void           , ViewGlRenderer, paintGL)
