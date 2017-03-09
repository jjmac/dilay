/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "cache.hpp"
#include "sculpt-brush.hpp"
#include "tools.hpp"
#include "view/properties.hpp"
#include "view/util.hpp"

struct ToolSculptSmooth::Impl {
  ToolSculptSmooth* self;

  Impl (ToolSculptSmooth* s) 
    : self (s) 
  {}

  void runSetupBrush (SculptBrush& brush) {
    auto& params = brush.parameters <SBSmoothParameters> ();

    params.intensity (this->self->cache ().get <float> ("intensity", 0.5f));
	params.relaxOnly (this->self->cache ().get <bool>  ("relaxOnly", false));
  }

  void runSetupCursor (ViewCursor&) {}


  void runSetupToolTip (ViewToolTip& toolTip) {
    this->self->addDefaultToolTip        (toolTip, false);
    this->self->addSecSliderWheelToolTip (toolTip, QObject::tr ("Change intensity"));
  }

  bool runSculptPointingEvent (const ViewPointingEvent& e) {
    return this->self->carvelikeStroke (e, false);
  }
};

DELEGATE_TOOL_SCULPT (ToolSculptSmooth)

DELEGATE_TOOL_SCULPT_PARAM(ToolSculptSmooth, float, intensity, SBSmoothParameters)
DELEGATE_TOOL_SCULPT_PARAM(ToolSculptSmooth, bool, relaxOnly, SBSmoothParameters)

