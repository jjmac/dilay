/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "cache.hpp"
#include "dimension.hpp"
#include "scene.hpp"
#include "sketch/mesh.hpp"
#include "sketch/node-intersection.hpp"
#include "sketch/path-intersection.hpp"
#include "state.hpp"
#include "tools.hpp"
#include "util.hpp"
#include "view/pointing-event.hpp"
#include "view/properties.hpp"
#include "view/tool-tip.hpp"
#include "view/util.hpp"

namespace {
  enum class Mode { DeleteSketch, DeleteNode, DeleteSpheres };

  int fromMode (DeleteSketchMode m) {
    switch (m) {
      case DeleteSketchMode::DeleteSketch:  return 0;
      case DeleteSketchMode::DeleteNode:    return 1;
      case DeleteSketchMode::DeleteSpheres: return 2;
      default:                  DILAY_IMPOSSIBLE;
    }
  }

  DeleteSketchMode toMode (int m) {
    switch (m) {
      case  0: return DeleteSketchMode::DeleteSketch;
      case  1: return DeleteSketchMode::DeleteNode;
      case  2: return DeleteSketchMode::DeleteSpheres;
      default: DILAY_IMPOSSIBLE;
    }
  }
}

struct ToolDeleteSketch::Impl {
  ToolDeleteSketch* self;
  DeleteSketchMode  mode;
  bool              deleteChildren;

  Impl (ToolDeleteSketch* s) 
    : self           (s)
    , mode           (toMode (s->cache ().get <int> ("mode", fromMode (DeleteSketchMode::DeleteNode))))
	, deleteChildren (s->cache ().get <bool> ("deleteChildren", false))
  {
    this->self->renderMirror (false);

    this->setupToolTip    ();
  }

  void setupToolTip () {
    ViewToolTip toolTip;
    toolTip.add (ViewToolTip::MouseEvent::Left, QObject::tr ("Delete selection"));
    this->self->showToolTip (toolTip);
  }

  ToolResponse runReleaseEvent (const ViewPointingEvent& e) {
    if (e.primaryButton ()) {
      switch (this->mode) {
        case DeleteSketchMode::DeleteSketch: {
          SketchMeshIntersection intersection;
          if (this->self->intersectsScene (e, intersection)) {
            this->self->snapshotSketchMeshes ();
            this->self->state ().scene ().deleteMesh (intersection.mesh ());
          }
          return ToolResponse::Redraw;
        }
        case DeleteSketchMode::DeleteNode: {
          SketchNodeIntersection intersection;
          if (this->self->intersectsScene (e, intersection)) {
            this->self->snapshotSketchMeshes ();
            intersection.mesh ().deleteNode ( intersection.node ()
                                            , this->deleteChildren
                                            , this->self->mirrorDimension () );
            if (intersection.mesh ().isEmpty ()) {
              this->self->state ().scene ().deleteMesh (intersection.mesh ());
            }
          }
          return ToolResponse::Redraw;
        }
        case DeleteSketchMode::DeleteSpheres: {
          SketchPathIntersection intersection;
          if (this->self->intersectsScene (e, intersection)) {
            this->self->snapshotSketchMeshes ();
            intersection.mesh ().deletePath ( intersection.path ()
                                            , this->self->mirrorDimension () );
            if (intersection.mesh ().isEmpty ()) {
              this->self->state ().scene ().deleteMesh (intersection.mesh ());
            }
          }
          return ToolResponse::Redraw;
        }
      }
    }
    return ToolResponse::None;
  }
};

DELEGATE_TOOL                   (ToolDeleteSketch)
DELEGATE_TOOL_RUN_RELEASE_EVENT (ToolDeleteSketch)

bool ToolDeleteSketch::deleteChildren() const
{
    return impl->deleteChildren;
}
void ToolDeleteSketch::deleteChildren(bool m)
{
    impl->deleteChildren = m;
    cache ().set ("deleteChildren", m);

}

DeleteSketchMode ToolDeleteSketch::mode() const
{
    return impl->mode;
}
void ToolDeleteSketch::mode(DeleteSketchMode m)
{
    impl->mode = m;
    cache ().set ("mode", fromMode (m));
}
