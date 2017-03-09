/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_TOOLS
#define DILAY_TOOLS

#include "tool.hpp"
#include "tool/sculpt.hpp"
#include "tool/util/movement.hpp"
#include "sketch/mesh.hpp"

DECLARE_TOOL2 (ToolMoveMesh, "move", DECLARE_TOOL_RUN_MOVE_EVENT
                                    DECLARE_TOOL_RUN_PRESS_EVENT
                                    DECLARE_TOOL_RUN_RELEASE_EVENT,
                                    MovementConstraint constraint() const;
                                    void constraint(MovementConstraint c);
              )

DECLARE_TOOL (ToolDeleteMesh, "delete-mesh", DECLARE_TOOL_RUN_RELEASE_EVENT)

DECLARE_TOOL (ToolNewMesh, "new-mesh", DECLARE_TOOL_RUN_INITIALIZE)

DECLARE_TOOL_SCULPT (ToolSculptCarve  , "sculpt/carve",
                     DECLARE_TOOL_SCULPT_PARAM(float, intensity)
                     DECLARE_TOOL_SCULPT_PARAM(bool , invert)
                     DECLARE_TOOL_SCULPT_PARAM(bool , inflate)
                     )
DECLARE_TOOL_SCULPT (ToolSculptDrag   , "sculpt/drag",
                     DECLARE_TOOL_SCULPT_PARAM(float, smoothness)
                     DECLARE_TOOL_SCULPT_PARAM(bool, discardBackfaces)
                     DECLARE_TOOL_SCULPT_PARAM(MovementConstraint, constraint)
                     )
DECLARE_TOOL_SCULPT (ToolSculptGrab   , "sculpt/grab",
					 DECLARE_TOOL_SCULPT_PARAM(bool, discardBackfaces)
					 DECLARE_TOOL_SCULPT_PARAM(MovementConstraint, constraint)
					 )
DECLARE_TOOL_SCULPT (ToolSculptSmooth , "sculpt/smooth",
					 DECLARE_TOOL_SCULPT_PARAM(float, intensity)
					 DECLARE_TOOL_SCULPT_PARAM(bool, relaxOnly)
					 )
DECLARE_TOOL_SCULPT (ToolSculptFlatten, "sculpt/flatten",
					 DECLARE_TOOL_SCULPT_PARAM(float, intensity)
					 )
DECLARE_TOOL_SCULPT (ToolSculptCrease , "sculpt/crease",
					 DECLARE_TOOL_SCULPT_PARAM(float, intensity)
					 DECLARE_TOOL_SCULPT_PARAM(bool , invert)
					 )
DECLARE_TOOL_SCULPT (ToolSculptPinch  , "sculpt/pinch",
					 DECLARE_TOOL_SCULPT_PARAM(bool , invert)
					 )
DECLARE_TOOL_SCULPT (ToolSculptReduce , "sculpt/reduce",
					 DECLARE_TOOL_SCULPT_PARAM(float, intensity)
					 )

DECLARE_TOOL (ToolNewSketch, "new-sketch", DECLARE_TOOL_RUN_INITIALIZE)

DECLARE_TOOL2 (ToolModifySketch, "modify-sketch", DECLARE_TOOL_RUN_MOVE_EVENT
                                                  DECLARE_TOOL_RUN_PRESS_EVENT
                                                  DECLARE_TOOL_RUN_RELEASE_EVENT,
                                                  MovementConstraint constraint() const;
                                                  void constraint(MovementConstraint c);
                                                  void syncMirror();
                                                  bool transformChildren() const;
                                                  void transformChildren(bool);
                                                  bool snap() const;
                                                  void snap(bool);
                                                  int snapWidth() const;
                                                  void snapWidth(int);
               )

enum class DeleteSketchMode { DeleteSketch, DeleteNode, DeleteSpheres };
DECLARE_TOOL2 (ToolDeleteSketch, "delete-sketch", DECLARE_TOOL_RUN_RELEASE_EVENT,
                                                  bool deleteChildren() const;
                                                  void deleteChildren(bool);
                                                  DeleteSketchMode mode() const;
                                                  void mode(DeleteSketchMode);
               )

DECLARE_TOOL (ToolRebalanceSketch, "rebalance-sketch", DECLARE_TOOL_RUN_RELEASE_EVENT
                                                       DECLARE_TOOL_RUN_CLOSE )

DECLARE_TOOL2(ToolConvertSketch, "convert-sketch", DECLARE_TOOL_RUN_RELEASE_EVENT,
                                                   float getMinResolution() const;
                                                   float getResolution()    const;
                                                   void  setResolution(float);
                                                   float getMaxResolution() const;
                                                   bool getMoveToCenter() const;
                                                   void setMoveToCenter(bool b);
                                                   bool getSmoothMesh() const;
                                                   void setSmoothMesh(bool b);
)

DECLARE_TOOL2 (ToolSketchSpheres, "sketch-spheres", DECLARE_TOOL_RUN_INITIALIZE
                                                   DECLARE_TOOL_RUN_RENDER
                                                   DECLARE_TOOL_RUN_MOVE_EVENT
                                                   DECLARE_TOOL_RUN_PRESS_EVENT
                                                   DECLARE_TOOL_RUN_RELEASE_EVENT
                                                   DECLARE_TOOL_RUN_MOUSE_WHEEL_EVENT
                                                   DECLARE_TOOL_RUN_CURSOR_UPDATE
                                                   DECLARE_TOOL_RUN_FROM_CONFIG,
                                                   void syncMirror();
                                                   float radius()    const;
                                                   void  radius(float);
                                                   float height()    const;
                                                   void  height(float);
                                                   SketchPathSmoothEffect smoothEffect() const;
                                                   void smoothEffect(SketchPathSmoothEffect);

)
#endif
