/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_COLOR
#define DILAY_COLOR

#include "globals.hpp"

#include <glm/fwd.hpp>
#include <iosfwd>

class DILAY_LIB_EXPORT Color {
  public: 
             Color ();
             Color (float);
             Color (float,float,float);
             Color (float,float,float,float);
    explicit Color (const glm::vec3&);
    explicit Color (const glm::vec4&);

    // copies and scales a color using `scale`
    Color (const Color&, float);

    static Color Black ();
    static Color White ();
    static Color Red   ();
    static Color Green ();
    static Color Blue  ();

    float r          () const;
    float g          () const;
    float b          () const;
    float opacity    () const;

    void  rgb(float, float, float);
    void  r          (float);
    void  g          (float);
    void  b          (float);
    void  opacity    (float);

    // `scale` does not scale opacity
    void  scale      (float);

    glm::vec3 vec3   () const;
    glm::vec4 vec4   () const;

  private:
    float _r;
    float _g;
    float _b;
    float _opacity;
};

std::ostream& operator<<(std::ostream&, const Color&);

#endif
