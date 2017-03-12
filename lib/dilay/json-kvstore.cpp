/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */

#include <json.hpp>
#include <glm/glm.hpp>
#include <unordered_map>
#include "color.hpp"
#include "json-kvstore.hpp"
#include "util.hpp"
#include "variant.hpp"
#include "json-conversion.hpp"
#include <string>
#include <list>
#include <fstream>

using nlohmann::json;

struct JsonKVStore::Impl {
  typedef Variant <float,int,bool,glm::vec3,glm::ivec2,Color> Value;
  typedef std::unordered_map <std::string, Value>             Map;

  const std::string root;
        Map         map;

  Impl (const std::string& r) 
    : root (r)
  {
    assert (this->root.find ('/') == std::string::npos);
  }

  std::string path (const std::string& suffix) const {
    assert (suffix.back  () != '/');

    if (suffix.front () == '/') {
      assert (suffix.find ("/" + this->root + "/") == 0);
      return suffix;
    }
    else {
      return "/" + this->root + "/" + suffix;
    }
  }

  template <class T>
  const T& get (const std::string& p) const {

    Map::const_iterator value = this->map.find (this->path (p));

    if (value == this->map.end ()) {
      throw (std::runtime_error ("Can not find path '" + this->path (p) + "' in kv-store"));
    }
    else {
      return value->second.get <T> ();
    }
  }

  template <class T>
  const T& get (const std::string& p, const T& defaultV) const {
    Map::const_iterator value = this->map.find (this->path (p));

    if (value == this->map.end ()) {
      return defaultV;
    }
    else {
      return value->second.get <T> ();
    }
  }

  template <class T>
  void set (const std::string& p, const T& t) {
    this->remove (p);

    Value value;
    value.set <T> (t);
    this->map.emplace (this->path (p), value);
  }

  void fromFile (const std::string& fileName) {
    Util::withCLocale <void> ([this, &fileName] () {
        std::ifstream istrm(fileName);

        if (istrm.good()) {
          json doc;
          try {
            istrm >> doc;
            this->loadNode ("", doc);
          }
          catch (std::runtime_error& e) {
            throw (std::runtime_error
                ("Error while parsing kv-store file '" + fileName + "': " + e.what ()));
          }
        }
        else {
          throw (std::runtime_error ("Can not load kv-store file '" + fileName + "'"));
        }
    });
  }

  void loadNode (const std::string& prefix, const json& node) {
    // iterate the array
    for (json::const_iterator it = node.begin(); it != node.end(); ++it) {

      if (it->is_object()) {
          const json& element = *it;
          Value       value;

          if (element.find("type") == element.end()) {
            this->loadNode (prefix + "/" + it.key(), element);
          }
          else {
              std::string attribute = element["type"];
              if (attribute == "float") {
                this->loadElement <float> (prefix, it.key(), element);
              }
              else if (attribute == "integer") {
                this->loadElement <int> (prefix, it.key(), element);
              }
              else if (attribute == "boolean") {
                this->loadElement <bool> (prefix, it.key(), element);
              }
              else if (attribute == "vector3f") {
                this->loadElement <glm::vec3> (prefix, it.key(), element);
              }
              else if (attribute == "vector2i") {
                this->loadElement <glm::ivec2> (prefix, it.key(), element);
              }
              else if (attribute == "color") {
                this->loadElement <Color> (prefix, it.key(), element);
              }
              else {
                throw (std::runtime_error
                  ( "invalid type '" + attribute + "' in" + element.dump(2))
                );
              }
            }
      }
      else {
        throw (std::runtime_error 
          ( "invalid node (" + it->dump() + ")")
          );
      }
    }
  }

  template <typename T>
  bool loadElement (const std::string& prefix, const std::string& suffix, const json& element) {
          T           t;
    const bool        ok     = JsonConversion::fromDomElement (element, t);
    const std::string key    = prefix + "/" + suffix;

    if (ok) {
      json elt;
      JsonConversion::toDomElement(elt, t);
      this->set <T> (key, t);
    }
    else {
      throw (std::runtime_error 
          ( "can not parse value of key '" + key + "' " + element.dump(2))
      );
    }
    return ok;
  }

  void toFile (const std::string& fileName) const {
    Util::withCLocale <void> ([this, &fileName] () {
        nlohmann::json doc;

      for (auto& c : this->map) {
        const std::string& key   = c.first;
        const Value&       value = c.second;

        addChild(key, value, doc);
      }
      if (!doc.is_null ()) {
          std::ofstream ostrm(fileName);

          if (ostrm.good()) {
              ostrm << std::setw(4) << doc;                      // text output
          }
          else {
            throw (std::runtime_error ("Can not save kv-store file '" + fileName + "'"));
          }

      }
    });
  }

  void addChild(const std::string& path, const Value& value, json& doc) const
  {
      assert(!path.empty());
      size_t next_sep = path.find('/');
      if (next_sep == -1) {
          doc[path] = {};
          json& parent = doc[path];
          if (value.is <int> ()) {
            JsonConversion::toDomElement (parent, value.get <int> ());
          }
          else if (value.is <float> ()) {
            JsonConversion::toDomElement (parent, value.get <float> ());
          }
          else if (value.is <bool> ()) {
            JsonConversion::toDomElement (parent, value.get <bool> ());
          }
          else if (value.is <glm::vec3> ()) {
            JsonConversion::toDomElement (parent, value.get <glm::vec3> ());
          }
          else if (value.is <glm::ivec2> ()) {
            JsonConversion::toDomElement (parent, value.get <glm::ivec2> ());
          }
          else if (value.is <Color> ()) {
            JsonConversion::toDomElement (parent, value.get <Color> ());
          }
          else
            DILAY_IMPOSSIBLE

      }
      else if (next_sep == 0) {
          addChild(path.substr(1), value, doc);
      }
      else {
          std::string childKey = path.substr(0, next_sep);
          if (doc.find(childKey) == doc.end()) {
              doc[childKey] = {};
          }
          addChild(path.substr(next_sep+1), value, doc[childKey]);
      }

  }

  void appendAsDomChild ( json& doc, json& parent,
                          std::list < std::string> & path, const Value& value ) const
  {
    if (path.empty ()) {
      assert (parent.is_object());
      if (value.is <float> ()) {
        JsonConversion::toDomElement (parent, value.get <float> ());
      }
      else if (value.is <int> ()) {
        JsonConversion::toDomElement (parent, value.get <int> ());
      }
      else if (value.is <bool> ()) {
        JsonConversion::toDomElement (parent, value.get <bool> ());
      }
      else if (value.is <glm::vec3> ()) {
        JsonConversion::toDomElement (parent, value.get <glm::vec3> ());
      }
      else if (value.is <glm::ivec2> ()) {
        JsonConversion::toDomElement (parent, value.get <glm::ivec2> ());
      }
      else if (value.is <Color> ()) {
        JsonConversion::toDomElement (parent, value.get <Color> ());
      }
      else
        DILAY_IMPOSSIBLE
    }
    else {
      const std::string& head  = path.front();
      json* child = nullptr;
      if (parent.find(head) == parent.end()) {
          if (parent.is_null ()) {
              doc[head] = {};
              child = &doc[head];
          } else {
              parent[head] = {};
              child = &parent[head];
          }
      }
      else {
          child = &parent[head];
      }
      assert (child->find("type") == child->end());

      path.pop_front();
      this->appendAsDomChild (doc, *child, path, value);
    }
  }

  void remove (const std::string& p) {
    this->map.erase (this->path (p));
  }

  void reset () {
    this->map.clear ();
  }
};

DELEGATE1_BIG2  (JsonKVStore, const std::string&)
DELEGATE1       (void, JsonKVStore, fromFile, const std::string&);
DELEGATE1_CONST (void, JsonKVStore, toFile, const std::string&);
DELEGATE1       (void, JsonKVStore, remove, const std::string&);
DELEGATE        (void, JsonKVStore, reset);

template <class T>
const T& JsonKVStore :: get (const std::string& path) const {
  return this->impl->get<T> (path);
}

template <class T>
const T& JsonKVStore :: get (const std::string& path, const T& defaultV) const {
  return this->impl->get<T> (path, defaultV);
}

template <class T>
void JsonKVStore :: set (const std::string& path, const T& value) {
  return this->impl->set<T> (path, value);
}

template const float&      JsonKVStore :: get<float>      (const std::string&) const;
template const float&      JsonKVStore :: get<float>      (const std::string&, const float&) const;
template void              JsonKVStore :: set<float>      (const std::string&, const float&);
template const int&        JsonKVStore :: get<int>        (const std::string&) const;
template const int&        JsonKVStore :: get<int>        (const std::string&, const int&) const;
template void              JsonKVStore :: set<int>        (const std::string&, const int&);
template const bool&       JsonKVStore :: get<bool>       (const std::string&) const;
template const bool&       JsonKVStore :: get<bool>       (const std::string&, const bool&) const;
template void              JsonKVStore :: set<bool>       (const std::string&, const bool&);
template const Color&      JsonKVStore :: get<Color>      (const std::string&) const;
template const Color&      JsonKVStore :: get<Color>      (const std::string&, const Color&) const;
template void              JsonKVStore :: set<Color>      (const std::string&, const Color&);
template const glm::vec3&  JsonKVStore :: get<glm::vec3>  (const std::string&) const;
template const glm::vec3&  JsonKVStore :: get<glm::vec3>  (const std::string&, const glm::vec3&) const;
template void              JsonKVStore :: set<glm::vec3>  (const std::string&, const glm::vec3&);
template const glm::ivec2& JsonKVStore :: get<glm::ivec2> (const std::string&) const;
template const glm::ivec2& JsonKVStore :: get<glm::ivec2> (const std::string&, const glm::ivec2&) const;
template void              JsonKVStore :: set<glm::ivec2> (const std::string&, const glm::ivec2&);
