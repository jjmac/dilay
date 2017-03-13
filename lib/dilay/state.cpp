/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <memory>
#include "cache.hpp"
#include "camera.hpp"
#include "config.hpp"
#include "history.hpp"
#include "mesh.hpp"
#include "mesh-util.hpp"
#include "scene.hpp"
#include "state.hpp"
#include "tool.hpp"

struct State::Impl {
  State*                 self;
  Config&                config;
  Cache                  cache;
  Camera                 camera;
  History                history;
  Scene                  scene;
  std::unique_ptr <Tool> toolPtr;
  EngineStatus           _status;

  Impl (State* s, Config& cfg)
    : self       (s)
    , config     (cfg)
    , camera     (this->config)
    , history    (this->config)
    , scene      (this->config)
    , _status    (EngineStatus::None)
  {
    this->scene.newWingedMesh (this->config, MeshUtil::icosphere (3));
  }

  ~Impl () {
    this->toolPtr.reset (nullptr);
  }

  bool hasTool () const { 
    return bool (this->toolPtr); 
  }

  Tool& tool () { 
    assert (this->hasTool ()); 
    return *this->toolPtr; 
  }

  void setTool (Tool&& tool) { 
    assert (this->hasTool () == false);

    this->toolPtr.reset (&tool); 

    tool.initialize ();
  }

  void resetTool (bool deselect) {
    if (this->hasTool ()) {
      this->toolPtr->close ();

      this->toolPtr.reset (); 
    }
  }

  void fromConfig () {
    this->camera .fromConfig (this->config);
    this->history.fromConfig (this->config);
    this->scene  .fromConfig (this->config);

    if (this->hasTool ()) {
      this->toolPtr->fromConfig ();
    }
  }

  void undo () {
    this->history.undo (*this->self);
  }

  void redo () {
    this->history.redo (*this->self);
  }

  EngineStatus popStatus () {
    EngineStatus s = _status;
    _status = EngineStatus::None;
    return s;
  }

  EngineStatus status () const {
      return _status;
  }

  void setStatus (EngineStatus s) {
      if (_status == EngineStatus::None) {
          _status = s;
      }
      else if (s == EngineStatus::Terminate) {
          _status = s;
      }
  }
};

DELEGATE1_BIG2_SELF (State, Config&)

GETTER    (Config&           , State, config)
GETTER    (Cache&            , State, cache)
GETTER    (Camera&           , State, camera)
GETTER    (History&          , State, history)
GETTER    (Scene&            , State, scene)
DELEGATE  (bool              , State, hasTool)
DELEGATE  (Tool&             , State, tool)
DELEGATE1 (void              , State, setTool, Tool&&)
DELEGATE1 (void              , State, resetTool, bool)
DELEGATE  (void              , State, fromConfig)
DELEGATE  (void              , State, undo)
DELEGATE  (void              , State, redo)
DELEGATE  (EngineStatus      , State, popStatus)
DELEGATE_CONST (EngineStatus , State, status)
DELEGATE1 (void              , State, setStatus, EngineStatus)
