/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "cache.hpp"
#include "scene.hpp"
#include "sculpt-brush.hpp"
#include "state.hpp"
#include "tools.hpp"

struct ToolSculptReduce::Impl {
  ToolSculptReduce* self;

  Impl (ToolSculptReduce* s) : self (s) {}

  void runSetupBrush (SculptBrush& brush) {
    auto& params = brush.parameters <SBReduceParameters> ();

    params.intensity (this->self->cache ().get <float> ("intensity", 0.75f));
  }

  void runSetupCursor (ViewCursor&) {}

  bool runSculptPointingEvent (const ViewPointingEvent& e) {
    const bool result = this->self->carvelikeStroke (e, false);
    this->self->state ().scene ().deleteEmptyMeshes ();
    return result;
  }
};

DELEGATE_TOOL_SCULPT (ToolSculptReduce)
DELEGATE_TOOL_SCULPT_PARAM(ToolSculptReduce, float , intensity, SBReduceParameters)
