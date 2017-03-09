/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QMouseEvent>
#include <QPainter>
#include <glm/glm.hpp>
#include "camera.hpp"
#include "opengl.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "state.hpp"
#include "tool.hpp"
#include "view/floor-plane.hpp"
#include "gl-widget.hpp"
#include "controller.h"
#include "view/pointing-event.hpp"
#include "view/util.hpp"
#include "main-window.hpp"

ViewGlWidget::ViewGlWidget (ViewMainWindow& mW, Config& cfg, Cache& cch)
    : QOpenGLWidget(&mW)
    , m_mainWindow     (mW)
    , m_config         (cfg)
    , m_cache          (cch)
    , m_toolMoveCamera (cfg)
    , m_state          (nullptr)
    , m_axis           (nullptr)
    , m_floorPlane     (nullptr)
    , m_tabletPressed  (false)
{
    setAutoFillBackground (false);
}

ViewGlWidget::~ViewGlWidget ()
{
    makeCurrent ();

    m_state     .reset (nullptr);
    m_axis      .reset (nullptr);
    m_floorPlane.reset (nullptr);

    doneCurrent ();
}

ToolMoveCamera& ViewGlWidget::toolMoveCamera ()
{
    return m_toolMoveCamera;
}

State& ViewGlWidget::state ()
{
    assert (m_state);
    return *m_state;
}

ViewFloorPlane& ViewGlWidget::floorPlane ()
{
    assert (m_floorPlane);
    return *m_floorPlane;
}

glm::ivec2 ViewGlWidget::cursorPosition ()
{
    return ViewUtil::toIVec2 (mapFromGlobal (QCursor::pos ()));
}

void ViewGlWidget::fromConfig ()
{
    assert (m_axis);

    state ().fromConfig ();
    m_axis->fromConfig (m_config);

    floorPlane ().fromConfig (m_config);
    floorPlane ().update (state ().camera ());

    m_toolMoveCamera.fromConfig (m_config);
}

void ViewGlWidget::initializeGL ()
{
    OpenGL::initializeFunctions ();

    m_state     .reset (new State (&(m_mainWindow), m_config, m_cache));
    m_axis      .reset (new ViewAxis (m_config));
    m_floorPlane.reset (new ViewFloorPlane (m_config, state ().camera ()));

    setMouseTracking (true);
}

void ViewGlWidget::paintGL ()
{
    QPainter painter (this);
    painter.beginNativePainting ();

    state ().camera ().renderer ().setupRendering ();
    state ().scene  ().render (state ().camera ());

    floorPlane ().render (state ().camera ());

    if (state ().hasTool ()) {
        state ().tool ().render ();
    }
    m_axis->render (state ().camera ());

    OpenGL::glDisable (OpenGL::DepthTest ());
    painter.endNativePainting ();

//    m_axis->render (state ().camera (), painter);
    m_mainWindow.showNumFaces (state ().scene ().numFaces ());
}

void ViewGlWidget::resizeGL (int w, int h) {
    state ().camera ().updateResolution (glm::uvec2 (w,h));
}

void ViewGlWidget::pointingEvent (const ViewPointingEvent& e) {
    if (e.valid ()) {
        if (e.secondaryButton () && e.moveEvent ()) {
            m_toolMoveCamera.moveEvent (state (), e);
            updateCursorInTool ();
        }
        else if (e.secondaryButton () && e.pressEvent ()) {
            m_toolMoveCamera.pressEvent (state (), e);
            updateCursorInTool ();
        }
        else if (state ().hasTool ()) {
            state ().handleToolResponse (state ().tool ().pointingEvent (e));
        }
    }
}

void ViewGlWidget::mouseMoveEvent (QMouseEvent* e)
{
    if (m_tabletPressed == false) {
        pointingEvent (ViewPointingEvent (*e));
    }
}

void ViewGlWidget::mousePressEvent (QMouseEvent* e) {
    if (m_tabletPressed == false) {
        pointingEvent (ViewPointingEvent (*e));
    }
}

void ViewGlWidget::mouseReleaseEvent (QMouseEvent* e) {
    if (m_tabletPressed == false) {
        pointingEvent (ViewPointingEvent (*e));
    }
}

void ViewGlWidget::wheelEvent (QWheelEvent* e) {
    if (e->modifiers () == Qt::NoModifier) {
        m_toolMoveCamera.wheelEvent (state (), *e);
        updateCursorInTool ();
        floorPlane ().update (state ().camera ());
    }
    else if (state ().hasTool ()) {
        state ().handleToolResponse (state ().tool ().wheelEvent (*e));
    }
}

void ViewGlWidget::tabletEvent (QTabletEvent* e) {
    const ViewPointingEvent pe (*e);

    if (pe.pressEvent ()) {
        m_tabletPressed = true;
    }
    else if (pe.releaseEvent ()) {
        m_tabletPressed = false;
    }
    pointingEvent (pe);
}

void ViewGlWidget::updateCursorInTool () {
    if (state ().hasTool ()) {
        state ().handleToolResponse (state ().tool ()
                                     .cursorUpdate (cursorPosition ()));
    }
}

