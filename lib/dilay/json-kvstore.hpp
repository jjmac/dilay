/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_JSON_JsonKVStore
#define DILAY_JSON_JsonKVStore

#include <string>
#include "macro.hpp"

class JsonKVStore {
  public:   
    DECLARE_BIG2 (JsonKVStore, const std::string&)

    template <class T> const T& get     (const std::string&) const;  
    template <class T>       T  getFrom (const std::string&) const;  
    template <class T> const T& get     (const std::string&, const T&) const;  
    template <class T>       T  getFrom (const std::string&, const T&) const;  
    template <class T> void     set     (const std::string&, const T&);  

    void fromFile (const std::string&);
    void toFile   (const std::string&) const;
    void remove   (const std::string&);
    void reset    ();

  private:
    IMPLEMENTATION
};

#endif
