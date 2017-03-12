/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QMouseEvent>
#include <QPainter>
#include <QGuiApplication>
#include <glm/glm.hpp>
#include "dilay/controller.h"
#include "dilay/view/floor-plane.hpp"
#include "dilay/view/pointing-event.hpp"
#include "dilay/camera.hpp"
#include "dilay/opengl.hpp"
#include "dilay/renderer.hpp"
#include "dilay/scene.hpp"
#include "dilay/state.hpp"
#include "dilay/tool.hpp"
#include "gl-widget.hpp"
#include "view-util.hpp"
#include "main-window.hpp"
#include "opengl-impl.hpp"

ViewPointingEvent toPointingEvent (const QMouseEvent& event)
{
    bool moveEvent = event.type () == QEvent::MouseMove;
    return ViewPointingEvent(
                KeyboardModifiers((int)event.modifiers ()),
                event.type () == QEvent::MouseButtonPress,
                event.type () == QEvent::MouseMove,
                event.type () == QEvent::MouseButtonRelease,
                moveEvent ? event.buttons () == Qt::LeftButton
                          : event.button  () == Qt::LeftButton,
                moveEvent ? event.buttons () == Qt::MiddleButton
                          : event.button  () == Qt::MiddleButton,
                glm::ivec2 (event.x (), event.y ()),
                1.0f
    );
}

ViewPointingEvent toPointingEvent (const QTabletEvent& event)
{
    bool moveEvent = event.type () == QEvent::TabletMove;
    ViewPointingEvent vpe(
                KeyboardModifiers((int)QGuiApplication::queryKeyboardModifiers ()),
                event.type () == QEvent::TabletPress,
                moveEvent,
                event.type () == QEvent::TabletRelease,
                moveEvent ? event.buttons () == Qt::LeftButton
                          : event.button  () == Qt::LeftButton,
                moveEvent ? event.buttons () == Qt::MiddleButton
                          : event.button  () == Qt::MiddleButton,
                glm::ivec2 (event.x (), event.y ()),
                2.0f * event.pressure ()
    );
    return vpe;
}
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

    OpenGL::install(new  OpenGLImpl());

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

    OpenGL::instance().glDisable (OpenGL::instance().DepthTest ());
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
            state ().tool ().pointingEvent (e);
            handleEngineState();
       }
    }
}

void ViewGlWidget::mouseMoveEvent (QMouseEvent* e)
{
    if (m_tabletPressed == false) {
        pointingEvent (toPointingEvent (*e));
    }
}

void ViewGlWidget::mousePressEvent (QMouseEvent* e) {
    if (m_tabletPressed == false) {
        pointingEvent (toPointingEvent (*e));
    }
}

void ViewGlWidget::mouseReleaseEvent (QMouseEvent* e) {
    if (m_tabletPressed == false) {
        pointingEvent (toPointingEvent (*e));
    }
}

void ViewGlWidget::wheelEvent (QWheelEvent* e) {
	int d = e->delta();
	ViewWheelEvent we(KeyboardModifiers((int)e->modifiers()), e->orientation() == Qt::Vertical, e->delta());
	if (we.modifiers () == KeyboardModifiers::NoModifier) {
		m_toolMoveCamera.wheelEvent (state (), we);
        updateCursorInTool ();
        floorPlane ().update (state ().camera ());
    }
    else if (state ().hasTool ()) {
        state ().tool ().wheelEvent (we);
    }
    handleEngineState();
}

void ViewGlWidget::tabletEvent (QTabletEvent* e) {
    const ViewPointingEvent pe  = toPointingEvent (*e);

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
        state ().tool ().cursorUpdate (cursorPosition ());
        handleEngineState();
    }
}

#include <QTimer>
void ViewGlWidget::handleEngineState() {
    static QTimer* timer = nullptr;
    if (!timer) {
        timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(50);

        connect(timer, &QTimer::timeout, [this]() {
            switch(state().popStatus()) {
            case EngineStatus::Redraw:
                this->update();
                break;
            case EngineStatus::Terminate:
                state().resetTool(true);
                this->update();
                break;
            default:
                break;
            }
        });
    }
    if (!timer->isActive()) timer->start();
}
