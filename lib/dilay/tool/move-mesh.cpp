/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <glm/glm.hpp>
#include "cache.hpp"
#include "state.hpp"
#include "tool/util/movement.hpp"
#include "tools.hpp"
#include "view/pointing-event.hpp"
#include "winged/mesh.hpp"
#include "winged/face-intersection.hpp"

struct ToolMoveMesh::Impl {
  ToolMoveMesh*    self;
  WingedMesh*      mesh;
  ToolUtilMovement movement;

  Impl (ToolMoveMesh* s) 
    : self     (s) 
    , mesh     (nullptr)
    , movement ( s->state ().camera ()
               , s->cache ().getFrom <MovementConstraint> ( "constraint"
                                                          , MovementConstraint::CameraPlane ))
  {
  }

  ToolResponse runMoveEvent (const ViewPointingEvent& e) {
    if (e.primaryButton () && this->mesh && this->movement.move (e, true)) {
      this->mesh->translate  (this->movement.delta ());
      return ToolResponse::Redraw;
    }
    else {
      return ToolResponse::None;
    }
  }

  ToolResponse runPressEvent (const ViewPointingEvent& e) {
    if (e.primaryButton ()) {
      WingedFaceIntersection intersection;
      if (this->self->intersectsScene (e, intersection)) {
        this->mesh = &intersection.mesh ();
        this->movement.resetPosition (intersection.position ());

        this->self->snapshotWingedMeshes ();
      }
    }
    return ToolResponse::None;
  }

  ToolResponse runReleaseEvent (const ViewPointingEvent& e) {
    if (e.primaryButton () && this->mesh) {
      this->mesh->normalize  ();
      this->mesh->bufferData ();
      this->mesh = nullptr;
    }
    return ToolResponse::None;
  }
};

DELEGATE_TOOL                   (ToolMoveMesh)
DELEGATE_TOOL_RUN_MOVE_EVENT    (ToolMoveMesh)
DELEGATE_TOOL_RUN_PRESS_EVENT   (ToolMoveMesh)
DELEGATE_TOOL_RUN_RELEASE_EVENT (ToolMoveMesh)

MovementConstraint ToolMoveMesh::constraint() const
{
    return impl->movement.constraint();
}

void ToolMoveMesh::constraint(MovementConstraint c)
{
    impl->movement.constraint(c);
	cache ().set ("constraint", impl->movement.constraint ());
}
