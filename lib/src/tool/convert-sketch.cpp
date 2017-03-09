/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QCheckBox>
#include "action/sculpt.hpp"
#include "cache.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "sketch/conversion.hpp"
#include "sketch/mesh.hpp"
#include "sketch/mesh-intersection.hpp"
#include "state.hpp"
#include "tools.hpp"
#include "view/double-slider.hpp"
#include "view/pointing-event.hpp"
#include "view/properties.hpp"
#include "view/tool-tip.hpp"
#include "view/util.hpp"
#include "winged/mesh.hpp"

namespace {
  glm::vec3 computeCenter (const SketchMesh& mesh) {
    if (mesh.tree ().hasRoot ()) {
      return mesh.tree ().root ().data ().center ();
    }
    else {
      glm::vec3 min, max;
      mesh.minMax (min, max);
      return ((min + max) * 0.5f);
    }
  }
}

struct ToolConvertSketch::Impl {
  ToolConvertSketch* self;
  const float        minResolution;
  const float        maxResolution;
  float              resolution;
  bool               moveToCenter;
  bool               smoothMesh;

  Impl (ToolConvertSketch* s)
    : self          (s)
    , minResolution (0.01f)
    , maxResolution (0.1f)
    , resolution    (s->cache ().get <float> ("resolution", 0.06))
    , moveToCenter  (s->cache ().get <bool>  ("move-to-center", true))
    , smoothMesh    (s->cache ().get <bool>  ("smooth-mesh", true))
  {
    this->self->renderMirror (false);

    this->setupProperties ();
    this->setupToolTip    ();
  }

  void setupProperties () {
    ViewTwoColumnGrid& properties = this->self->properties ().body ();

    ViewDoubleSlider& resolutionEdit = ViewUtil::slider (2, this->minResolution
                                                          , this->resolution
                                                          , this->maxResolution);
    ViewUtil::connect (resolutionEdit, [this] (float r) {
      this->resolution = r;
      this->self->cache ().set ("resolution", r);
    });
    properties.addStacked (QObject::tr ("Resolution"), resolutionEdit);

    QCheckBox& moveToCenterEdit = ViewUtil::checkBox ( QObject::tr ("Move to center")
                                                     , this->moveToCenter );
    ViewUtil::connect (moveToCenterEdit, [this] (bool m) {
      this->moveToCenter = m;
      this->self->cache ().set ("move-to-center", m);
    });
    properties.add (moveToCenterEdit);

    QCheckBox& smoothMeshEdit = ViewUtil::checkBox ( QObject::tr ("Smooth mesh")
                                                   , this->smoothMesh );
    ViewUtil::connect (smoothMeshEdit, [this] (bool s) {
      this->smoothMesh = s;
      this->self->cache ().set ("smooth-mesh", s);
    });
    properties.add (smoothMeshEdit);
  }

  void setupToolTip () {
    ViewToolTip toolTip;
    toolTip.add (ViewToolTip::MouseEvent::Left, QObject::tr ("Convert selection"));
    this->self->showToolTip (toolTip);
  }

  ToolResponse runReleaseEvent (const ViewPointingEvent& e) {
    if (e.primaryButton ()) {
      SketchMeshIntersection intersection;
      if (this->self->intersectsScene (e, intersection)) {
        SketchMesh& sMesh = intersection.mesh ();
        glm::vec3 center = computeCenter (sMesh);

        this->self->snapshotAll ();
        sMesh.optimizePaths ();

        Mesh mesh = SketchConversion::convert ( sMesh
                                              , this->maxResolution + this->minResolution
                                                                    - this->resolution );
        WingedMesh& wMesh = this->self->state ().scene ()
                                                .newWingedMesh ( this->self->state ().config ()
                                                               , mesh );
        if (this->moveToCenter) {
          wMesh.translate (-center);
          wMesh.normalize ();
          wMesh.bufferData ();
        }
        if (this->smoothMesh) {
          Action::smoothMesh (wMesh);
        }
        this->self->state ().scene ().deleteMesh (sMesh);
        return ToolResponse::Redraw;
      }
    }
    return ToolResponse::None;
  }
};

DELEGATE_TOOL                   (ToolConvertSketch)
DELEGATE_TOOL_RUN_RELEASE_EVENT (ToolConvertSketch)

float ToolConvertSketch::getMinResolution() const
{
    return impl->minResolution;
}
float ToolConvertSketch::getResolution()    const
{
    return impl->resolution;
}
void  ToolConvertSketch::setResolution(float r)
{
    impl->resolution = r;
    cache ().set ("resolution", r);
}

float ToolConvertSketch::getMaxResolution() const
{
    return impl->maxResolution;
}
bool ToolConvertSketch::getMoveToCenter() const
{
    return impl->moveToCenter;
}

void ToolConvertSketch::setMoveToCenter(bool b)
{
    impl->moveToCenter = b;
    cache ().set ("move-to-center", b);
}

bool ToolConvertSketch::getSmoothMesh() const
{
    return impl->smoothMesh;
}
void ToolConvertSketch::setSmoothMesh(bool b)
{
    impl->smoothMesh = b;
    cache ().set ("smooth-mesh", b);
}

