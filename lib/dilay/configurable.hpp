/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_CONFIGURABLE
#define DILAY_CONFIGURABLE

#include "globals.hpp"


class Config;
class ConfigProxy;

class DILAY_LIB_EXPORT Configurable {
  public:
    void fromConfig (const Config&);

  private:
    virtual void runFromConfig (const Config&) = 0;
};

class DILAY_LIB_EXPORT ProxyConfigurable {
  public:
    void fromConfig (const ConfigProxy&);

  private:
    virtual void runFromConfig (const ConfigProxy&) = 0;
};

#endif
