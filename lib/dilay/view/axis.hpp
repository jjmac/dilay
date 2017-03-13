/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_AXIS
#define DILAY_VIEW_AXIS

#include "../globals.hpp"
#include "../configurable.hpp"
#include "../macro.hpp"

class Camera;
class Config;

class DILAY_LIB_EXPORT ViewAxis : public Configurable {
  public:
    DECLARE_BIG3 (ViewAxis, const Config&)

    void  render (Camera&);

  private:
    IMPLEMENTATION

    void runFromConfig (const Config&);
};

#endif
