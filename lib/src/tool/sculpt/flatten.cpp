/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QObject>
#include "cache.hpp"
#include "sculpt-brush.hpp"
#include "tools.hpp"
#include "view/cursor.hpp"

struct ToolSculptFlatten::Impl {
  ToolSculptFlatten* self;

  Impl (ToolSculptFlatten* s) : self (s) {}

  void runSetupBrush (SculptBrush& brush) {
    auto& params = brush.parameters <SBFlattenParameters> ();

    params.intensity (this->self->cache ().get <float> ("intensity", 0.5f));
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

DELEGATE_TOOL_SCULPT (ToolSculptFlatten)
DELEGATE_TOOL_SCULPT_PARAM(ToolSculptFlatten, float, intensity, SBFlattenParameters)
