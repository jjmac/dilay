/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "render-mode.hpp"
#include "scene.hpp"
#include "state.hpp"
#include "tools.hpp"
#include "sketch/mesh.hpp"
#include "sketch/node-intersection.hpp"
#include "view/pointing-event.hpp"

struct ToolRebalanceSketch::Impl {
  ToolRebalanceSketch* self;
  const bool           renderWireframe;

  Impl (ToolRebalanceSketch* s) 
    : self            (s)
    , renderWireframe (s->state ().scene ().renderWireframe ())
  {
    this->self->renderMirror (false);
    this->self->state ().scene ().renderWireframe (true);
  }

  ToolResponse runReleaseEvent (const ViewPointingEvent& e) {
    if (e.primaryButton ()) {
      SketchNodeIntersection intersection;
      if (this->self->intersectsScene (e, intersection)) {
        this->self->snapshotSketchMeshes ();
        intersection.mesh ().rebalance (intersection.node ());
        return ToolResponse::Redraw;
      }
    }
    return ToolResponse::None;
  }

  void runClose () {
    this->self->state ().scene ().renderWireframe (this->renderWireframe);
  }
};

DELEGATE_TOOL                   (ToolRebalanceSketch)
DELEGATE_TOOL_RUN_RELEASE_EVENT (ToolRebalanceSketch)
DELEGATE_TOOL_RUN_CLOSE         (ToolRebalanceSketch)
