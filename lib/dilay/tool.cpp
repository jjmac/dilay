/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "cache.hpp"
#include "camera.hpp"
#include "dimension.hpp"
#include "history.hpp"
#include "index-octree.hpp"
#include "intersection.hpp"
#include "mesh.hpp"
#include "mirror.hpp"
#include "primitive/ray.hpp"
#include "primitive/triangle.hpp"
#include "scene.hpp"
#include "sketch/mesh.hpp"
#include "state.hpp"
#include "tool.hpp"
#include "view/pointing-event.hpp"
#include "winged/mesh.hpp"

struct Tool::Impl {
  Tool*           self;
  State&          state;
  CacheProxy     _cache;
  Maybe <Mirror> _mirror;
  bool            renderMirror;

  Impl (Tool* s, State& st, const char* key) 
    : self         (s) 
    , state        (st)
    ,_cache        (this->cache (key))
    , renderMirror (true)
  {
    this->mirror (this->hasMirror ());
  }

  void initialize () {
    this->fromConfig ();
    this->self->runInitialize ();
  }

  void render () const { 
    this->self->runRender (); 
    if (this->_mirror && this->renderMirror) {
      this->_mirror->render (this->state.camera ());
    }
  }

  void pointingEvent (const ViewPointingEvent& e) {
    this->self->runPointingEvent (e);

    if (e.releaseEvent ()) {
      this->state.scene ().sanitizeMeshes ();
    }
  }

  void wheelEvent (const ViewWheelEvent& e) {
    this->self->runWheelEvent (e);
  }

  void cursorUpdate (const glm::ivec2& pos) {
    this->self->runCursorUpdate (pos);
  }

  void close () { 
    return this->self->runClose (); 
  }

  void fromConfig () {
    if (this->hasMirror ()) {
      this->_mirror->fromConfig (this->config ());
    }
    this->self->runFromConfig ();
  }

  Config& config () const {
    return this->state.config ();
  }

  CacheProxy& cache () {
    return this->_cache;
  }

  CacheProxy cache (const char* key) const {
    return CacheProxy (this->state.cache (), "editor/tool/" + std::string (key) + "/");
  }

  void snapshotAll () {
    this->state.history ().snapshotAll (this->state.scene ());
  }

  void snapshotWingedMeshes () {
    this->state.history ().snapshotWingedMeshes (this->state.scene ());
  }

  void snapshotSketchMeshes () {
    this->state.history ().snapshotSketchMeshes (this->state.scene ());
  }

  bool intersectsRecentOctree (const glm::ivec2& pos, Intersection& intersection) const {
    assert (this->state.history ().hasRecentOctrees ());

    const PrimRay ray = this->state.camera ().ray (pos);

    this->state.history ().forEachRecentOctree (
      [&ray, &intersection] (const Mesh& mesh, const IndexOctree& octree) {
        octree.intersects (ray, [&ray, &mesh, &intersection] (unsigned int i) {
          const PrimTriangle tri ( mesh.vertex (mesh.index ((3 * i) + 0))
                                 , mesh.vertex (mesh.index ((3 * i) + 1))
                                 , mesh.vertex (mesh.index ((3 * i) + 2)) );
          float t;

          if (IntersectionUtil::intersects (ray, tri, &t)) {
            intersection.update (t, ray.pointAt (t), tri.normal ());
          }
        });
      }
    );
    return intersection.isIntersection ();
  }

  bool hasMirror () const {
    return this->state.cache ().get ("editor/tool/mirror", true);
  }

  const Mirror& mirror () const {
    assert (this->hasMirror ());
    return *this->_mirror;
  }


  void mirror (bool m) {
    this->state.cache ().set ("editor/tool/mirror", m);

    if (m) {
      this->_mirror = Maybe <Mirror>::make (this->config (), Dimension::X);
    }
    else {
      this->_mirror.reset ();
    }
 }

  const Dimension* mirrorDimension () const {
    const static Dimension d = Dimension::X;

    return this->hasMirror () ? &d : nullptr;
  }

  void mirrorWingedMeshes () {
    assert (this->hasMirror ());

    this->snapshotWingedMeshes ();
    this->state.scene ().forEachMesh (
      [this] (WingedMesh& mesh) {
        mesh.mirror (this->mirror ().plane ());
        mesh.bufferData ();
      }
    );
  }
  void mirrorSketchMeshes () {
    assert (this->hasMirror ());

    this->snapshotSketchMeshes ();
    this->state.scene ().forEachMesh (
      [this] (SketchMesh& mesh) {
        mesh.mirror (*this->mirrorDimension ());
      }
    );
  }

  template <typename T, typename ... Ts>
  bool intersectsScene (const glm::ivec2& pos, T& intersection, Ts ... args) {
    return this->state.scene ().intersects ( this->state.camera ().ray (pos), intersection
                                           , std::forward <Ts> (args) ... );
  }

  template <typename T, typename ... Ts>
  bool intersectsScene (const ViewPointingEvent& e, T& intersection, Ts ... args) {
    return this->intersectsScene (e.ivec2 (), intersection, std::forward <Ts> (args) ...);
  }

  void runPointingEvent (const ViewPointingEvent& e) {
    if (e.pressEvent ()) {
      this->self->runPressEvent (e);
    }
    else if (e.moveEvent ()) {
      this->self->runMoveEvent (e);
    }
    else if (e.releaseEvent ()) {
      this->self->runReleaseEvent (e);
    }
  }
};

DELEGATE2_BIG3_SELF (Tool, State&, const char*)
DELEGATE        (void    , Tool, initialize)
DELEGATE_CONST  (void            , Tool, render)
DELEGATE1       (void    , Tool, pointingEvent, const ViewPointingEvent&)
DELEGATE1       (void    , Tool, wheelEvent, const ViewWheelEvent&)
DELEGATE1       (void    , Tool, cursorUpdate, const glm::ivec2&)
DELEGATE        (void            , Tool, close)
DELEGATE        (void            , Tool, fromConfig)
GETTER_CONST    (State&          , Tool, state)
DELEGATE_CONST  (Config&         , Tool, config)
DELEGATE        (CacheProxy&     , Tool, cache)
DELEGATE1_CONST (CacheProxy      , Tool, cache, const char*)
DELEGATE        (void            , Tool, snapshotAll)
DELEGATE        (void            , Tool, snapshotWingedMeshes)
DELEGATE        (void            , Tool, snapshotSketchMeshes)
DELEGATE2_CONST (bool            , Tool, intersectsRecentOctree, const glm::ivec2&, Intersection&)
DELEGATE_CONST  (bool            , Tool, hasMirror)
DELEGATE_CONST  (const Mirror&   , Tool, mirror)
DELEGATE1       (void            , Tool, mirror, bool)
SETTER          (bool            , Tool, renderMirror)

DELEGATE_CONST  (const Dimension*, Tool, mirrorDimension)
DELEGATE        (void            , Tool, mirrorWingedMeshes)
DELEGATE        (void            , Tool, mirrorSketchMeshes)
DELEGATE1       (void    , Tool, runPointingEvent, const ViewPointingEvent&)

template <typename T, typename ... Ts>
bool Tool :: intersectsScene (const glm::ivec2& pos, T& intersection, Ts ... args) {
  return this->impl->intersectsScene (pos, intersection, std::forward <Ts> (args) ...);
}

template <typename T, typename ... Ts>
bool Tool :: intersectsScene (const ViewPointingEvent& e, T& intersection, Ts ... args) {
  return this->impl->intersectsScene (e, intersection, std::forward <Ts> (args) ...);
}

template bool Tool :: intersectsScene (const glm::ivec2&, WingedFaceIntersection&);
template bool Tool :: intersectsScene (const ViewPointingEvent&, WingedFaceIntersection&);
template bool Tool :: intersectsScene (const glm::ivec2&, SketchNodeIntersection&);
template bool Tool :: intersectsScene (const ViewPointingEvent&, SketchNodeIntersection&);
template bool Tool :: intersectsScene (const glm::ivec2&, SketchBoneIntersection&);
template bool Tool :: intersectsScene (const ViewPointingEvent&, SketchBoneIntersection&);
template bool Tool :: intersectsScene (const glm::ivec2&, SketchMeshIntersection&);
template bool Tool :: intersectsScene (const ViewPointingEvent&, SketchMeshIntersection&);
template bool Tool :: intersectsScene (const glm::ivec2&, SketchMeshIntersection&, unsigned int);
template bool Tool :: intersectsScene (const ViewPointingEvent&, SketchMeshIntersection&, unsigned int);
template bool Tool :: intersectsScene (const glm::ivec2&, SketchPathIntersection&);
template bool Tool :: intersectsScene (const ViewPointingEvent&, SketchPathIntersection&);
template bool Tool :: intersectsScene (const glm::ivec2&, Intersection&);
template bool Tool :: intersectsScene (const ViewPointingEvent&, Intersection&);
