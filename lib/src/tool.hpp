/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_TOOL
#define DILAY_TOOL

#include <glm/fwd.hpp>
#include "macro.hpp"
#include "sketch/fwd.hpp"

class CacheProxy;
class Config;
enum class Dimension;
class Intersection;
class Mirror;
class QWheelEvent;
class State;
class ViewPointingEvent;
class ViewProperties;
class ViewToolTip;
class WingedFaceIntersection;

enum class ToolResponse {
  None, Terminate, Redraw
};

class Tool {
  public:
    DECLARE_BIG3_VIRTUAL (Tool, State&, const char*)

    ToolResponse     initialize             ();
    void             render                 () const;
    ToolResponse     pointingEvent          (const ViewPointingEvent&);
    ToolResponse     wheelEvent             (const QWheelEvent&);
    ToolResponse     cursorUpdate           (const glm::ivec2&);
    void             close                  ();
    void             fromConfig             ();

  protected:
    State&           state                  () const;
    void             updateGlWidget         ();
    ViewProperties&  properties             () const;
    void             showToolTip            (const ViewToolTip&);
    Config&          config                 () const;
    CacheProxy&      cache                  ();
    CacheProxy       cache                  (const char*) const;
    glm::ivec2       cursorPosition         () const;
    void             snapshotAll            ();
    void             snapshotWingedMeshes   ();
    void             snapshotSketchMeshes   ();
    bool             intersectsRecentOctree (const glm::ivec2&, Intersection&) const;
    bool             hasMirror              () const;
    const Mirror&    mirror                 () const;
    void             mirror                 (bool);
    void             renderMirror           (bool);
    const Dimension* mirrorDimension        () const;
    void             mirrorWingedMeshes     ();
    void             mirrorSketchMeshes     ();

    template <typename T, typename ... Ts>
    bool intersectsScene (const glm::ivec2&, T&, Ts ...);
    template <typename T, typename ... Ts>
    bool intersectsScene (const ViewPointingEvent&, T&, Ts ...);

  private:
    IMPLEMENTATION

    virtual const char*  key              () const = 0;
    virtual ToolResponse runInitialize    ()                         { return ToolResponse::None; }
    virtual void         runRender        () const                   {}
    virtual ToolResponse runPointingEvent (const ViewPointingEvent&);
    virtual ToolResponse runPressEvent    (const ViewPointingEvent&) { return ToolResponse::None; }
    virtual ToolResponse runMoveEvent     (const ViewPointingEvent&) { return ToolResponse::None; }
    virtual ToolResponse runReleaseEvent  (const ViewPointingEvent&) { return ToolResponse::None; }
    virtual ToolResponse runWheelEvent    (const QWheelEvent&)       { return ToolResponse::None; }
    virtual ToolResponse runCursorUpdate  (const glm::ivec2&)        { return ToolResponse::None; }
    virtual void         runClose         ()                         {}
    virtual void         runFromConfig    ()                         {}
};

#define DECLARE_TOOL2(name,theKey,otherMethods, publicMethods)       \
  class name : public Tool { public:                                 \
    DECLARE_BIG2 (name, State&)                                      \
    private:                                                         \
      IMPLEMENTATION                                                 \
      const char* key () const { return theKey ; }                   \
      otherMethods                                                   \
    public:                                                          \
      publicMethods};

#define DECLARE_TOOL(name,theKey,otherMethods)   DECLARE_TOOL2(name, theKey, otherMethods, )

#define DECLARE_TOOL_RUN_INITIALIZE        ToolResponse runInitialize    ();
#define DECLARE_TOOL_RUN_RENDER            void         runRender        () const;
#define DECLARE_TOOL_RUN_POINTING_EVENT    ToolResponse runPointingEvent (const ViewPointingEvent&);
#define DECLARE_TOOL_RUN_PRESS_EVENT       ToolResponse runPressEvent    (const ViewPointingEvent&);
#define DECLARE_TOOL_RUN_MOVE_EVENT        ToolResponse runMoveEvent     (const ViewPointingEvent&);
#define DECLARE_TOOL_RUN_RELEASE_EVENT     ToolResponse runReleaseEvent  (const ViewPointingEvent&);
#define DECLARE_TOOL_RUN_MOUSE_WHEEL_EVENT ToolResponse runWheelEvent    (const QWheelEvent&);
#define DECLARE_TOOL_RUN_CURSOR_UPDATE     ToolResponse runCursorUpdate  (const glm::ivec2&);
#define DECLARE_TOOL_RUN_CLOSE             void         runClose         ();
#define DECLARE_TOOL_RUN_FROM_CONFIG       void         runFromConfig    ();

#define DELEGATE_TOOL(name) \
  DELEGATE_BIG2_BASE (name, (State& s), (this), Tool, (s, this->key ()))

#define DELEGATE_TOOL_RUN_INITIALIZE(n)        DELEGATE       (ToolResponse, n, runInitialize)
#define DELEGATE_TOOL_RUN_RENDER(n)            DELEGATE_CONST (void        , n, runRender)
#define DELEGATE_TOOL_RUN_POINTING_EVENT(n)    DELEGATE1      (ToolResponse, n, runPointingEvent, const ViewPointingEvent&)
#define DELEGATE_TOOL_RUN_PRESS_EVENT(n)       DELEGATE1      (ToolResponse, n, runPressEvent, const ViewPointingEvent&)
#define DELEGATE_TOOL_RUN_MOVE_EVENT(n)        DELEGATE1      (ToolResponse, n, runMoveEvent, const ViewPointingEvent&)
#define DELEGATE_TOOL_RUN_RELEASE_EVENT(n)     DELEGATE1      (ToolResponse, n, runReleaseEvent, const ViewPointingEvent&)
#define DELEGATE_TOOL_RUN_MOUSE_WHEEL_EVENT(n) DELEGATE1      (ToolResponse, n, runWheelEvent, const QWheelEvent&)
#define DELEGATE_TOOL_RUN_CURSOR_UPDATE(n)     DELEGATE1      (ToolResponse, n, runCursorUpdate, const glm::ivec2&)
#define DELEGATE_TOOL_RUN_CLOSE(n)             DELEGATE       (void        , n, runClose)
#define DELEGATE_TOOL_RUN_FROM_CONFIG(n)       DELEGATE       (void        , n, runFromConfig)

#endif
