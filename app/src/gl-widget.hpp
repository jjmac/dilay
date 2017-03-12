/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_GL_WIDGET
#define DILAY_VIEW_GL_WIDGET

#include <QOpenGLWidget>
#include <glm/fwd.hpp>
#include "dilay/view/axis.hpp"
#include "dilay/tool/move-camera.hpp"

class Cache;
class Config;
class State;
class ToolMoveCamera;
class ViewFloorPlane;
class ViewMainWindow;

class ViewGlWidget : public QOpenGLWidget
{
    Q_OBJECT

  public:
    ViewGlWidget(ViewMainWindow&, Config&, Cache&);
    ~ViewGlWidget ();

    ToolMoveCamera& toolMoveCamera ();
    State&          state          ();
    ViewFloorPlane& floorPlane     ();
    glm::ivec2      cursorPosition ();
    void            fromConfig     ();
       
  protected:
    void initializeGL      ();
    void resizeGL          (int,int);
    void paintGL           ();
 
    void mouseMoveEvent    (QMouseEvent*);
    void mousePressEvent   (QMouseEvent*);
    void mouseReleaseEvent (QMouseEvent*);
    void wheelEvent        (QWheelEvent*);
    void tabletEvent       (QTabletEvent*);

private:
    void pointingEvent (const ViewPointingEvent& e);
    void updateCursorInTool ();

private:
    typedef std::unique_ptr <State>          StatePtr;
    typedef std::unique_ptr <ViewAxis>       AxisPtr;
    typedef std::unique_ptr <ViewFloorPlane> FloorPlanePtr;

    ViewMainWindow& m_mainWindow;
    Config&         m_config;
    Cache&          m_cache;
    ToolMoveCamera  m_toolMoveCamera;
    StatePtr        m_state;
    AxisPtr         m_axis;
    FloorPlanePtr   m_floorPlane;
    bool            m_tabletPressed;

};

#endif
