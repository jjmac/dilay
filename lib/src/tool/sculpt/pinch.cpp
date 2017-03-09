/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QCheckBox>
#include "cache.hpp"
#include "sculpt-brush.hpp"
#include "tools.hpp"
#include "view/properties.hpp"
#include "view/util.hpp"

struct ToolSculptPinch::Impl {
  ToolSculptPinch* self;

  Impl (ToolSculptPinch* s) : self (s) {}

  void runSetupBrush (SculptBrush& brush) {
    auto& params = brush.parameters <SBPinchParameters> ();

    params.invert (this->self->cache ().get <bool>  ("invert"   , false));
  }

  void runSetupCursor (ViewCursor&) {}

  void runSetupProperties (ViewTwoColumnGrid& properties) {
    auto& params = this->self->brush ().parameters <SBPinchParameters> ();

    QCheckBox& invertEdit = ViewUtil::checkBox (QObject::tr ("Invert"), params.invert ());
    ViewUtil::connect (invertEdit, [this,&params] (bool i) {
      params.invert (i);
      this->self->cache ().set ("invert", i);
    });
    properties.add (invertEdit);
  }

  void runSetupToolTip (ViewToolTip& toolTip) {
    this->self->addDefaultToolTip (toolTip, true);
  }

  bool runSculptPointingEvent (const ViewPointingEvent& e) {
    const std::function <void ()> toggleInvert = [this] () {
      this->self->brush ().parameters <SBPinchParameters> ().toggleInvert ();
    };
    return this->self->carvelikeStroke (e, false, &toggleInvert);
  }
};

DELEGATE_TOOL_SCULPT (ToolSculptPinch)
DELEGATE_TOOL_SCULPT_PARAM(ToolSculptPinch, bool , invert, SBPinchParameters)
