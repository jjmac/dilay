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
class ViewWheelEvent;
class State;
class ViewPointingEvent;
class WingedFaceIntersection;

class Tool {
  public:
    DECLARE_BIG3_VIRTUAL (Tool, State&, const char*)

    void             initialize             ();
    void             render                 () const;
    void             pointingEvent          (const ViewPointingEvent&);
    void             wheelEvent             (const ViewWheelEvent&);
    void             cursorUpdate           (const glm::ivec2&);
    void             close                  ();
    void             fromConfig             ();

    bool             hasMirror              () const;
    void             mirror                 (bool);

  protected:
    State&           state                  () const;
    Config&          config                 () const;
    CacheProxy&      cache                  ();
    CacheProxy       cache                  (const char*) const;
    void             snapshotAll            ();
    void             snapshotWingedMeshes   ();
    void             snapshotSketchMeshes   ();
    bool             intersectsRecentOctree (const glm::ivec2&, Intersection&) const;
    const Mirror&    mirror                 () const;
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
    virtual void         runInitialize    ()                         {}
    virtual void         runRender        () const                   {}
    virtual void         runPointingEvent (const ViewPointingEvent&);
    virtual void         runPressEvent    (const ViewPointingEvent&) {}
    virtual void         runMoveEvent     (const ViewPointingEvent&) {}
    virtual void         runReleaseEvent  (const ViewPointingEvent&) {}
    virtual void         runWheelEvent    (const ViewWheelEvent&)    {}
    virtual void         runCursorUpdate  (const glm::ivec2&)        {}
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

#define DECLARE_TOOL_RUN_INITIALIZE        void         runInitialize    ();
#define DECLARE_TOOL_RUN_RENDER            void         runRender        () const;
#define DECLARE_TOOL_RUN_POINTING_EVENT    void         runPointingEvent (const ViewPointingEvent&);
#define DECLARE_TOOL_RUN_PRESS_EVENT       void         runPressEvent    (const ViewPointingEvent&);
#define DECLARE_TOOL_RUN_MOVE_EVENT        void         runMoveEvent     (const ViewPointingEvent&);
#define DECLARE_TOOL_RUN_RELEASE_EVENT     void         runReleaseEvent  (const ViewPointingEvent&);
#define DECLARE_TOOL_RUN_MOUSE_WHEEL_EVENT void         runWheelEvent    (const ViewWheelEvent&);
#define DECLARE_TOOL_RUN_CURSOR_UPDATE     void         runCursorUpdate  (const glm::ivec2&);
#define DECLARE_TOOL_RUN_CLOSE             void         runClose         ();
#define DECLARE_TOOL_RUN_FROM_CONFIG       void         runFromConfig    ();

#define DELEGATE_TOOL(name) \
  DELEGATE_BIG2_BASE (name, (State& s), (this), Tool, (s, this->key ()))

#define DELEGATE_TOOL_RUN_INITIALIZE(n)        DELEGATE       (void        , n, runInitialize)
#define DELEGATE_TOOL_RUN_RENDER(n)            DELEGATE_CONST (void        , n, runRender)
#define DELEGATE_TOOL_RUN_POINTING_EVENT(n)    DELEGATE1      (void        , n, runPointingEvent, const ViewPointingEvent&)
#define DELEGATE_TOOL_RUN_PRESS_EVENT(n)       DELEGATE1      (void        , n, runPressEvent, const ViewPointingEvent&)
#define DELEGATE_TOOL_RUN_MOVE_EVENT(n)        DELEGATE1      (void        , n, runMoveEvent, const ViewPointingEvent&)
#define DELEGATE_TOOL_RUN_RELEASE_EVENT(n)     DELEGATE1      (void        , n, runReleaseEvent, const ViewPointingEvent&)
#define DELEGATE_TOOL_RUN_MOUSE_WHEEL_EVENT(n) DELEGATE1      (void        , n, runWheelEvent, const ViewWheelEvent&)
#define DELEGATE_TOOL_RUN_CURSOR_UPDATE(n)     DELEGATE1      (void        , n, runCursorUpdate, const glm::ivec2&)
#define DELEGATE_TOOL_RUN_CLOSE(n)             DELEGATE       (void        , n, runClose)
#define DELEGATE_TOOL_RUN_FROM_CONFIG(n)       DELEGATE       (void        , n, runFromConfig)

#endif
