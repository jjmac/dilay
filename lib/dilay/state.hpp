/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_STATE
#define DILAY_STATE

#include "globals.hpp"

#include "macro.hpp"

class Cache;
class Camera;
class Config;
class History;
class Id;
class Mesh;
class Scene;
class Tool;
enum class ToolResponse;
class WingedMesh;

enum class EngineStatus {
  None, Terminate, Redraw
};

class DILAY_LIB_EXPORT State {
  public:                                   
    DECLARE_BIG2 (State, Config&)

    Config&         config             ();
    Cache&          cache              ();
    Camera&         camera             ();
    History&        history            ();
    Scene&          scene              ();
    bool            hasTool            ();
    Tool&           tool               ();
    void            setTool            (Tool&&);
    void            resetTool          (bool = true);
    void            fromConfig         ();
    void            undo               ();
    void            redo               ();
    EngineStatus    popStatus          ();
    EngineStatus    status             () const;
    void            setStatus          (EngineStatus);


  private:
    IMPLEMENTATION
};
#endif
