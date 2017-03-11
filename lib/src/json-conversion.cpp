/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <glm/glm.hpp>
#include "color.hpp"
#include "json-conversion.hpp"
#include "json.hpp"

using nlohmann::json;
namespace {
bool fromDomElementNoCheck (const json& e, float& v) {
  bool ok = false;
  if (e.is_number()) {
      v = e.get<float>();
      ok = true;
  }
  return ok;
}

bool fromDomElementNoCheck (const json& e, int& v) {
  bool ok = false;
  if (e.is_number_integer()) {
      v = e.get<int>();
      ok = true;
  }
  return ok;
}

bool fromDomElementNoCheck (const json& e, bool& v) {
  bool ok = false;
  if (e.is_boolean()) {
      v = e.get<bool>();
      ok = true;
  }
  return ok;
}
}

bool JsonConversion :: fromDomElement (const json& e, float& v) {
  assert (e["type"].get<std::string>() == "float");
  bool ok = false;
  if (e.find("value") != e.end()) {
      ok = fromDomElementNoCheck(e["value"], v);
  }
  return ok;
}

bool JsonConversion :: fromDomElement (const json& e, int& v) {
  assert (e["type"].get<std::string> () == "integer");
  bool ok = false;
  if (e.find("value") != e.end()) {
      ok = fromDomElementNoCheck(e["value"], v);
  }
  return ok;
}

bool JsonConversion :: fromDomElement (const json& e, bool& v) {
  assert (e["type"].get<std::string>() == "boolean");
  bool ok = false;
  if (e.find("value") != e.end()) {
      ok = fromDomElementNoCheck(e["value"], v);
  }
  return ok;
}

bool JsonConversion :: fromDomElement (const json& e, glm::vec3& v) {
  assert (e["type"].get<std::string>() == "vector3f");
  assert (e.find("x") != e.end());
  assert (e.find("y") != e.end());
  assert (e.find("z") != e.end());


  return  fromDomElementNoCheck(e["x"], v.x)
        & fromDomElementNoCheck (e["y"], v.y)
        & fromDomElementNoCheck (e["z"], v.z);

}

bool JsonConversion :: fromDomElement (const json& e, glm::ivec2& v) {
  assert (e["type"].get<std::string>() == "vector2i");
  assert (e.find("x") != e.end());
  assert (e.find("y") != e.end());

  return fromDomElementNoCheck(e["x"], v.x)
       & fromDomElementNoCheck (e["y"], v.y);
}

bool JsonConversion :: fromDomElement (const json& e, Color& v) {
  assert (e["type"].get<std::string>() == "color");
  assert (e.find("r") != e.end());
  assert (e.find("g") != e.end());
  assert (e.find("b") != e.end());

  float r, g, b = -1000.f;
  bool ok = fromDomElementNoCheck(e["r"], r)
          & fromDomElementNoCheck(e["g"], g)
          & fromDomElementNoCheck(e["b"], b);

  v.rgb(r, g, b);
  return ok;
}

json& JsonConversion :: toDomElement ( json& elem, const float& v) {
  elem["type"] = "float";
  elem["value"] = v;
  return elem;
}

json& JsonConversion :: toDomElement ( json& elem, const int& v) {
    elem["type"] = "integer";
    elem["value"] = v;
    return elem;
}

json& JsonConversion :: toDomElement ( json& elem, const bool& v) {
    elem["type"] = "boolean";
    elem["value"] = v;
  return elem;
}

json& JsonConversion :: toDomElement ( json& elem, const glm::vec3& v) {
  elem["type"] = "vector3f";
  elem["x"] = v.x;
  elem["y"] = v.y;
  elem["z"] = v.z;
  return elem;
}

json& JsonConversion :: toDomElement ( json& elem, const glm::ivec2& v) {
    elem["type"] = "vector2i";
    elem["x"] = v.x;
    elem["y"] = v.y;
  return elem;
}

json& JsonConversion :: toDomElement ( json& elem, const Color& v) {
    elem["type"] = "color";
    elem["r"] = v.r();
    elem["g"] = v.g();
    elem["b"] = v.b();
  return elem;
}
