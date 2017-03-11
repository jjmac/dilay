/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_CONFIG_JSON_CONVERSION
#define DILAY_CONFIG_JSON_CONVERSION

#include <glm/fwd.hpp>
#include "json.hpp"

class Color;

namespace JsonConversion {
  bool fromDomElement (const nlohmann::json&, float&);
  bool fromDomElement (const nlohmann::json&, int&);
  bool fromDomElement (const nlohmann::json&, bool&);
  bool fromDomElement (const nlohmann::json&, glm::vec3&);
  bool fromDomElement (const nlohmann::json&, glm::ivec2&);
  bool fromDomElement (const nlohmann::json&, Color&);

  nlohmann::json& toDomElement (nlohmann::json&, const float&);
  nlohmann::json& toDomElement (nlohmann::json&, const int&);
  nlohmann::json& toDomElement (nlohmann::json&, const bool&);
  nlohmann::json& toDomElement (nlohmann::json&, const glm::vec3&);
  nlohmann::json& toDomElement (nlohmann::json&, const glm::ivec2&);
  nlohmann::json& toDomElement (nlohmann::json&, const Color&);
}

#endif
