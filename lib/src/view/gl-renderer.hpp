/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_GL_RENDERER
#define DILAY_VIEW_GL_RENDERER

#include <QOpenGLWidget>
#include <glm/fwd.hpp>
#include "macro.hpp"
#include <QOpenGLFunctions_3_2_Core>

class Cache;
class Config;
class State;
class ToolMoveCamera;
class ViewFloorPlane;
class QSurface;

class ViewGlRenderer : public QOpenGLFunctions_3_2_Core  {
  public:
    DECLARE_BIG2 (ViewGlRenderer, QSurface&, Config&, Cache&)

    ToolMoveCamera& toolMoveCamera ();
    State&          state          ();
    ViewFloorPlane& floorPlane     ();
    glm::ivec2      cursorPosition ();
    void            fromConfig     ();
       
public:
    void paint();
  public:
    void initializeGL      ();
    void resizeGL          (int,int);
    void paintGL           ();


  private:
    IMPLEMENTATION
};

#endif
