/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "cache.hpp"
#include "sculpt-brush.hpp"
#include "state.hpp"
#include "tool/util/movement.hpp"
#include "tools.hpp"
#include "view/pointing-event.hpp"

struct ToolSculptGrab::Impl {
  ToolSculptGrab*  self;
  ToolUtilMovement movement;

  Impl (ToolSculptGrab* s) 
    : self (s)
    , movement (this->self->state ().camera (), MovementConstraint::CameraPlane)
  {
	if (this->self->cache ().get <bool> ("alongPrimaryPlane", false)) {
      this->movement.constraint (MovementConstraint::PrimaryPlane);
    }
  }

  void runSetupBrush (SculptBrush& brush) {
    auto& params = brush.parameters <SBDraglikeParameters> ();

    params.smoothness       (1.0f);
    params.linearStep       (true);
	params.discardBackfaces (this->self->cache ().get <bool>  ("discardBackfaces", false));
  }

  void runSetupCursor (ViewCursor&) {}

  bool runSculptPointingEvent (const ViewPointingEvent& e) {
    return e.pressEvent () ? this->self->initializeDraglikeStroke (e, this->movement)
                           : this->self->draglikeStroke (e, this->movement);
  }
};

DELEGATE_TOOL_SCULPT (ToolSculptGrab)
DELEGATE_TOOL_SCULPT_PARAM(ToolSculptGrab, bool, discardBackfaces, SBDraglikeParameters)
MovementConstraint ToolSculptGrab::constraint()
{
	return impl->movement.constraint();
}
void ToolSculptGrab::constraint(MovementConstraint c)
{
	impl->movement.constraint(c);
	cache ().set ("alongPrimaryPlane", c);
}

