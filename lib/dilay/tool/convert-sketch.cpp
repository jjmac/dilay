/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "action/sculpt.hpp"
#include "cache.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "sketch/conversion.hpp"
#include "sketch/mesh.hpp"
#include "sketch/mesh-intersection.hpp"
#include "state.hpp"
#include "tools.hpp"
#include "view/pointing-event.hpp"
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
    , moveToCenter  (s->cache ().get <bool>  ("moveToCenter", true))
    , smoothMesh    (s->cache ().get <bool>  ("smoothMesh", true))
  {
    this->self->renderMirror (false);
  }

  void runReleaseEvent (const ViewPointingEvent& e) {
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
        this->self->state().setStatus(EngineStatus::Redraw);
      }
    }
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
    cache ().set ("moveToCenter", b);
}

bool ToolConvertSketch::getSmoothMesh() const
{
    return impl->smoothMesh;
}
void ToolConvertSketch::setSmoothMesh(bool b)
{
    impl->smoothMesh = b;
    cache ().set ("smoothMesh", b);
}

