/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_CONFIG
#define DILAY_CONFIG

#include "globals.hpp"

#include <memory>
#include <string>
#include "color.hpp"
#include <glm/fwd.hpp>

#define CONFIG_ACCESSOR(T) \
void get_impl (const std::string& path, T& v) const ;          \
void set_impl (const std::string& path, const T& value);


class DILAY_LIB_EXPORT Config {
  public:   
    Config ();
    ~Config ();

    template <class T>
    const T& get (const std::string& path) const {
        T v;
        get_impl(path, v);
        return v;
    }

    template <class T>
    T getFrom (const std::string& path) const {
        return get(path);
    }

    template <class T>
    void set (const std::string& path, const T& value) {
        set_impl(path, value);
    }

    CONFIG_ACCESSOR(float)
    CONFIG_ACCESSOR(int)
    CONFIG_ACCESSOR(Color)
    CONFIG_ACCESSOR(glm::vec3)

    void fromFile (const std::string& fileName);
    void toFile (const std::string& fileName) const;

    void remove (const std::string& path);

    void restoreDefaults ();

  private:
    void update ();

    void* store;
};


class ConfigProxy {
  public:
    ConfigProxy (const Config& c, const std::string& p) 
      : _config (c)
      ,  prefix (p) 
    {
      assert (p.back () == '/');
    }

    ConfigProxy (const ConfigProxy& o, const std::string& path) 
      : ConfigProxy (o._config, o.prefix + path)
    {}

    const Config& config () const {
      return this->_config;
    }

    std::string key (const std::string& path) const {
      return this->prefix + path;
    }

    template <class T> 
    const T& get (const std::string& path) const { 
      return this->_config.get <T> (this->key (path));
    }

    template <class T> 
    T getFrom (const std::string& path) const {
      return this->_config.getFrom <T> (this->key (path));
    }

  private:
    const Config&     _config;
    const std::string  prefix;
};

#endif
