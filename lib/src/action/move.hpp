#ifndef DILAY_ACTION_MOVE
#define DILAY_ACTION_MOVE

#include <list>
#include <glm/fwd.hpp>
#include "action.hpp"
#include "macro.hpp"

class SphereMesh;

class ActionMove : public Action {
  public: 
    DECLARE_BIG3 (ActionMove)

    void translate (const std::list <SphereMesh*>&, const glm::vec3&);
    void position  (SphereMesh&, const glm::vec3&);

  private:
    void runUndo ();
    void runRedo ();

    class Impl;
    Impl* impl;
};

#endif
