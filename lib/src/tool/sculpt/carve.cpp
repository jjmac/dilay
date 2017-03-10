/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "cache.hpp"
#include "sculpt-brush.hpp"
#include "tools.hpp"

struct ToolSculptCarve::Impl {
  ToolSculptCarve* self;

  Impl (ToolSculptCarve* s) : self (s) {}

  void runSetupBrush (SculptBrush& brush) {
    auto& params = brush.parameters <SBCarveParameters> ();

    params.intensity (this->self->cache ().get <float> ("intensity", 0.02f));
    params.invert    (this->self->cache ().get <bool>  ("invert",    false));
    params.inflate   (this->self->cache ().get <bool>  ("inflate",   false));
  }

  void runSetupCursor (ViewCursor&) {}

  bool runSculptPointingEvent (const ViewPointingEvent& e) {
    const std::function <void ()> toggleInvert = [this] () {
      this->self->brush ().parameters <SBCarveParameters> ().toggleInvert ();
    };
    return this->self->carvelikeStroke (e, false, &toggleInvert);
  }
};

DELEGATE_TOOL_SCULPT (ToolSculptCarve)

DELEGATE_TOOL_SCULPT_PARAM(ToolSculptCarve, float, intensity, SBCarveParameters)
DELEGATE_TOOL_SCULPT_PARAM(ToolSculptCarve, bool , invert, SBCarveParameters)
DELEGATE_TOOL_SCULPT_PARAM(ToolSculptCarve, bool , inflate, SBCarveParameters)
