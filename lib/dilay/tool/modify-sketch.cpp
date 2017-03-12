/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "cache.hpp"
#include "mirror.hpp"
#include "primitive/plane.hpp"
#include "scene.hpp"
#include "sketch/bone-intersection.hpp"
#include "sketch/mesh.hpp"
#include "sketch/node-intersection.hpp"
#include "state.hpp"
#include "tool/util/movement.hpp"
#include "tool/util/scaling.hpp"
#include "tools.hpp"
#include "view/pointing-event.hpp"

struct ToolModifySketch::Impl {
  ToolModifySketch* self;
  SketchMesh*       mesh;
  SketchNode*       node;
  SketchNode*       parent;
  ToolUtilMovement  movement;
  ToolUtilScaling   scaling;
  bool              transformChildren;
  bool              snap;
  int               snapWidth;

  Impl (ToolModifySketch* s)
    : self              (s)
    , mesh              (nullptr)
    , node              (nullptr)
    , parent            (nullptr)
    , movement          ( s->state ().camera ()
                        , s->cache ().getFrom <MovementConstraint>
                            ("constraint", MovementConstraint::PrimaryPlane) )
    , scaling           (s->state ().camera ())
    , transformChildren (s->cache ().get <bool> ("transformChildren", false))
    , snap              (s->cache ().get <bool> ("snap", true))
    , snapWidth         (s->cache ().get <int> ("snapWidth", 5))
  {
    this->self->renderMirror (false);
  }

  ToolResponse runMoveEvent (const ViewPointingEvent& e) {
    if (e.primaryButton () && this->node) {
      if (e.modifiers () == KeyboardModifiers::ShiftModifier) {
        if (this->scaling.move (e)) {
          if (this->parent) {
            this->mesh->scale ( *this->parent, this->scaling.factor ()
                              , false, this->self->mirrorDimension () );
          }
          this->mesh->scale ( *this->node, this->scaling.factor ()
                            , this->transformChildren, this->self->mirrorDimension () );
        }
      }
      else if (this->movement.move (e, false)) {
        if (this->parent) {
          this->mesh->move (*this->parent, this->movement.delta ()
                           , false, this->self->mirrorDimension () );
        }
        this->mesh->move (*this->node, this->movement.delta ()
                         , this->transformChildren, this->self->mirrorDimension () );
      }
      return ToolResponse::Redraw;
    }
    else {
      return ToolResponse::None;
    }
  }

  ToolResponse runPressEvent (const ViewPointingEvent& e) {

    auto handleNodeIntersection = [this, &e] (SketchNodeIntersection &intersection) {
      this->self->snapshotSketchMeshes ();

      this->movement.resetPosition ( intersection.position ());
      this->scaling .resetPosition ( intersection.node ().data ().center ()
                                   , intersection.position () );

      this->mesh   = &intersection.mesh ();
      this->parent = nullptr;

      if (e.modifiers () == KeyboardModifiers::ControlModifier) {
        SketchNode& iNode = intersection.node ();

        const float radius = iNode.numChildren () > 0
                           ? iNode.lastChild ().data ().radius ()
                           : iNode.data ().radius ();

        this->node = &this->mesh->addChild ( iNode, iNode.data ().center ()
                                           , radius, this->self->mirrorDimension () );
      }
      else {
        this->node = &intersection.node ();
      }
    };

    auto handleBoneIntersection = [this, &e] (SketchBoneIntersection &intersection) {
      this->self->snapshotSketchMeshes ();

      this->movement.resetPosition ( intersection.position ());
      this->scaling .resetPosition ( intersection.projectedPosition ()
                                   , intersection.position () );

      this->mesh = &intersection.mesh ();

      if (e.modifiers () == KeyboardModifiers::ControlModifier) {
        const float radius = glm::distance ( intersection.projectedPosition ()
                                           , intersection.position () );

        this->parent = nullptr;
        this->node   = &this->mesh->addParent ( intersection.child ()
                                              , intersection.projectedPosition ()
                                              , radius, this->self->mirrorDimension () );
      }
      else {
        this->node   = &intersection.child  ();
        this->parent = &intersection.parent ();
      }
    };

    if (e.primaryButton ()) {
      SketchNodeIntersection nodeIntersection;
      SketchBoneIntersection boneIntersection;

      if (this->self->intersectsScene (e, nodeIntersection)) {
        handleNodeIntersection (nodeIntersection);
      }
      else if (this->self->intersectsScene (e, boneIntersection)) {
        handleBoneIntersection (boneIntersection);
      }
    }
    return ToolResponse::None;
  }

  ToolResponse runReleaseEvent (const ViewPointingEvent& e) {
    bool redraw = false;

    if (e.primaryButton ()) {
      if (this->snap && this->mesh && this->self->hasMirror ()) {
        PrimPlane mirrorPlane = this->mesh->mirrorPlane (*this->self->mirrorDimension ());

        const auto isSnappable = [this, &mirrorPlane] (const SketchNode& node) -> bool {
          return mirrorPlane.absDistance (node.data ().center ()) <=
                 this->snapWidth * this->self->mirror ().width ();
        };

        if (this->node && isSnappable (*this->node)) {
          this->mesh->snap (*this->node, *this->self->mirrorDimension ());
          redraw = true;
        }
        if (this->parent && isSnappable (*this->parent)) {
          this->mesh->snap (*this->parent, *this->self->mirrorDimension ());
          redraw = true;
        }
      }
      this->mesh   = nullptr;
      this->node   = nullptr;
      this->parent = nullptr;
    }
    return redraw ? ToolResponse::Redraw : ToolResponse::None;
  }
};

DELEGATE_TOOL                   (ToolModifySketch)
DELEGATE_TOOL_RUN_MOVE_EVENT    (ToolModifySketch)
DELEGATE_TOOL_RUN_PRESS_EVENT   (ToolModifySketch)
DELEGATE_TOOL_RUN_RELEASE_EVENT (ToolModifySketch)

MovementConstraint ToolModifySketch::constraint() const
{
    return impl->movement.constraint();
}
void ToolModifySketch::constraint(MovementConstraint c)
{
    impl->movement.constraint(c);
    cache ().set ("constraint", impl->movement.constraint ());
}

void ToolModifySketch::syncMirror()
{
    mirrorWingedMeshes ();
    updateGlWidget ();
}

bool ToolModifySketch::transformChildren() const
{
    return impl->transformChildren;
}
void ToolModifySketch::transformChildren(bool m)
{
    impl->transformChildren = m;
    impl->self->cache ().set ("transformChildren", m);
}

bool ToolModifySketch::snap() const
{
    return impl->snap;
}
void ToolModifySketch::snap(bool m)
{
    impl->snap = m;
    impl->self->cache ().set ("snap", m);
}

int ToolModifySketch::snapWidth() const
{
    return impl->snapWidth;
}
void ToolModifySketch::snapWidth(int w)
{
    impl->snapWidth = w;
    impl->self->cache ().set ("snapWidth", w);
}
