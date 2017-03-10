/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "cache.hpp"
#include "sculpt-brush.hpp"
#include "tools.hpp"

struct ToolSculptCrease::Impl {
  ToolSculptCrease* self;

  Impl (ToolSculptCrease* s) : self (s) {}

  void runSetupBrush (SculptBrush& brush) {
    auto& params = brush.parameters <SBCreaseParameters> ();

    brush.detailFactor (brush.detailFactor () + 0.5f);
    params.intensity   (this->self->cache ().get <float> ("intensity", 0.5f));
    params.invert      (this->self->cache ().get <bool>  ("invert"   , false));
  }

  void runSetupCursor (ViewCursor&) {}

  bool runSculptPointingEvent (const ViewPointingEvent& e) {
    const std::function <void ()> toggleInvert = [this] () {
      this->self->brush ().parameters <SBCreaseParameters> ().toggleInvert ();
    };
    return this->self->carvelikeStroke (e, true, &toggleInvert);
  }
};

DELEGATE_TOOL_SCULPT (ToolSculptCrease)

DELEGATE_TOOL_SCULPT_PARAM(ToolSculptCrease, float, intensity, SBCreaseParameters)
DELEGATE_TOOL_SCULPT_PARAM(ToolSculptCrease, bool , invert, SBCreaseParameters)
