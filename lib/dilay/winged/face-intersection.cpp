/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "winged/face-intersection.hpp"

  WingedFaceIntersection::WingedFaceIntersection() {}

  void WingedFaceIntersection::update ( float d, const glm::vec3& p, const glm::vec3& n
              , WingedMesh& mesh, WingedFace& face) 
  {
    if (Intersection::update (d,p,n)) {
      this->_mesh = &mesh;
      this->_face = &face;
    }
  }

  WingedMesh& WingedFaceIntersection::mesh () const {
    assert (this->isIntersection ());
    assert (this->_mesh);
    return *this->_mesh;
  }

  WingedFace& WingedFaceIntersection::face () const {
    assert (this->isIntersection ());
    assert (this->_face);
    return *this->_face;
  }

