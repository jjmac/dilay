/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_TOOL_MOVE_CAMERA
#define DILAY_TOOL_MOVE_CAMERA

#include <glm/fwd.hpp>
#include "../configurable.hpp"
#include "../macro.hpp"

class Config;
class State;
class ViewWheelEvent;
class ViewPointingEvent;

class DILAY_LIB_EXPORT ToolMoveCamera : public Configurable {
  public:
    DECLARE_BIG3 (ToolMoveCamera, const Config&)

    void snap           (State&, bool);
    void resetGazePoint (State&);

    void moveEvent      (State&, const ViewPointingEvent&);
    void pressEvent     (State&, const ViewPointingEvent&);
	void wheelEvent     (State&, const ViewWheelEvent&);

    void zoom           (State&, int delta);
    void rotate         (State&, const glm::ivec2& delta);
    void translate      (State&, const glm::ivec2& delta);
    void moveTo         (State&, const glm::ivec2& delta);

  private:
    IMPLEMENTATION

    void runFromConfig (const Config&);
};

#endif
