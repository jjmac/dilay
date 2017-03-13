/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <glm/glm.hpp>
#include "color.hpp"
#include "config.hpp"

#include "json-kvstore.hpp"

namespace {
  static constexpr int latestVersion = 5;
}

Config :: Config () 
  : store (new JsonKVStore("config"))
{
  this->restoreDefaults ();
}

#define THE_STORE static_cast<JsonKVStore*>(store)


Config :: ~Config ()
{
    delete THE_STORE;
}

#define CONFIG_ACCESSOR_IMPL(T)                                     \
void Config::get_impl (const std::string& path, T& v) const {       \
    v = THE_STORE->get <T> (path);                                  \
}                                                                   \
void Config::set_impl (const std::string& path, const T& value) {   \
    THE_STORE->set <T> (path, value);                               \
}

CONFIG_ACCESSOR_IMPL(float)
CONFIG_ACCESSOR_IMPL(int)
CONFIG_ACCESSOR_IMPL(Color)
CONFIG_ACCESSOR_IMPL(glm::vec3)

void Config::fromFile (const std::string& fileName) {
  THE_STORE->fromFile (fileName);
  this->update ();
}

void Config::toFile (const std::string& fileName) const {
  THE_STORE->toFile (fileName);
}

void Config::remove (const std::string& path) {
  THE_STORE->remove (path);
}

void Config :: restoreDefaults () {
  THE_STORE->reset ();

  this->set ("version", latestVersion);

  this->set ("editor/axis/color/normal",  Color (0.3f, 0.3f, 0.4f));
  this->set ("editor/axis/color/label",   Color (1.0f, 1.0f, 1.0f));
  this->set ("editor/axis/scaling",       glm::vec3 (0.01f, 0.3f , 0.01f));
  this->set ("editor/axis/arrowScaling", glm::vec3 (0.03f, 0.1f , 0.03f));

  this->set ("editor/floorPlane/color",      Color (0.3f, 0.3f, 0.4f));
  this->set ("editor/floorPlane/tileWidth", 1.0f);

  this->set ("editor/background", Color (0.1f, 0.1f, 0.2f));

  this->set ("editor/camera/nearClipping",   0.01f);
  this->set ("editor/camera/farClipping",    1000.0f);
  this->set ("editor/camera/rotationFactor", 1.0f);
  this->set ("editor/camera/movementFactor", 0.01f);
  this->set ("editor/camera/zoomInFactor",  0.9f);
  this->set ("editor/camera/fieldOfView",   45.0f);

  this->set ("editor/light/light1/direction",  glm::vec3 (-0.2f, 1.0f, 0.2f));
  this->set ("editor/light/light1/color",      Color (1.0f, 1.0f, 1.0f));
  this->set ("editor/light/light1/irradiance", 0.7f);

  this->set ("editor/light/light2/direction",  glm::vec3 (0.0f, 0.0f, 1.0f));
  this->set ("editor/light/light2/color",      Color (1.0f, 1.0f, 1.0f));
  this->set ("editor/light/light2/irradiance", 0.8f);

  this->set ("editor/mesh/color/normal",    Color (0.8f, 0.8f, 0.8f));
  this->set ("editor/mesh/color/wireframe", Color (0.3f, 0.3f, 0.3f));

  this->set ("editor/sketch/node/color",   Color (0.5f, 0.5f, 0.9f));
  this->set ("editor/sketch/bubble/color", Color (0.5f, 0.5f, 0.7f));
  this->set ("editor/sketch/sphere/color", Color (0.7f, 0.7f, 0.9f));

  this->set ("editor/tool/sculpt/detailFactor",       0.75f);
  this->set ("editor/tool/sculpt/stepWidthFactor",   0.1f);
  this->set ("editor/tool/sculpt/cursorColor",        Color (1.0f, 0.9f, 0.9f));
  this->set ("editor/tool/sculpt/maxAbsoluteRadius", 2.0f);
  this->set ("editor/tool/sculpt/mirror/width",        0.02f);
  this->set ("editor/tool/sculpt/mirror/color",        Color (0.8f, 0.8f, 0.8f));

  this->set ("editor/tool/sketchSpheres/cursorColor"     , Color (1.0f, 0.9f, 0.9f));
  this->set ("editor/tool/sketchSpheres/stepWidthFactor", 0.1f);

  this->set ("editor/undoDepth", 15);

  this->set ("window/initialWidth",  1024);
  this->set ("window/initialHeight", 768);
}

void Config :: update () {
  const int version = this->get <int> ("version");

  auto updateValue = [this] (const std::string& path, const auto& oldValue, const auto& newValue) {
    if (this->get <typename std::decay <decltype (oldValue)>::type> (path) == oldValue) {
      this->set (path, newValue);
    }
  };

  switch (version) {
    case 1: break;
    case 2: break;
    case 3:
      updateValue ("editor/undoDepth", 5, 15);
      break;

    case 4:
      this->remove ("editor/camera/gaze-point");
      this->remove ("editor/camera/eye-point");
      this->remove ("editor/camera/up");
      break;

    case latestVersion:
      return;

    default:
      this->restoreDefaults ();
      return;
  }
  this->set ("version", version + 1);
  this->update ();
}
