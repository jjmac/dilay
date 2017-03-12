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

struct ToolSculptDrag::Impl {
  ToolSculptDrag*  self;
  ToolUtilMovement movement;

  Impl (ToolSculptDrag* s) 
    : self (s)
    , movement (this->self->state ().camera (), MovementConstraint::CameraPlane)
  {
	if (this->self->cache ().get <bool> ("alongPrimaryPlane", false)) {
      this->movement.constraint (MovementConstraint::PrimaryPlane);
    }
  }

  void runSetupBrush (SculptBrush& brush) {
    auto& params = brush.parameters <SBDraglikeParameters> ();

    params.smoothness       (this->self->cache ().get <float> ("smoothness", 0.5f));
    params.linearStep       (false);
	params.discardBackfaces (this->self->cache ().get <bool>  ("discardBackfaces", false));
  }

  void runSetupCursor (ViewCursor&) {}

  bool runSculptPointingEvent (const ViewPointingEvent& e) {
    return e.pressEvent () ? this->self->initializeDraglikeStroke (e, this->movement)
                           : this->self->draglikeStroke (e, this->movement);
  }
};

DELEGATE_TOOL_SCULPT (ToolSculptDrag)

DELEGATE_TOOL_SCULPT_PARAM(ToolSculptDrag, float, smoothness, SBDraglikeParameters)
DELEGATE_TOOL_SCULPT_PARAM(ToolSculptDrag, bool, discardBackfaces, SBDraglikeParameters)
MovementConstraint ToolSculptDrag::constraint()
{
    return impl->movement.constraint();
}
void ToolSculptDrag::constraint(MovementConstraint c)
{
    impl->movement.constraint(c);
	cache ().set ("alongPrimaryPlane", c);
}

