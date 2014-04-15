#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include "subdivision-butterfly.hpp"
#include "winged/vertex.hpp"
#include "winged/edge.hpp"
#include "winged/face.hpp"
#include "winged/mesh.hpp"
#include "adjacent-iterator.hpp"

typedef std::vector <glm::vec3> Adjacents;
glm::vec3 subdivide (const glm::vec3&, const Adjacents&, const glm::vec3&, const Adjacents&);
Adjacents adjacents (const WingedMesh&, WingedEdge&, const WingedVertex&);

glm::vec3 SubdivisionButterfly::subdivideEdge (const WingedMesh& mesh, WingedEdge& edge) {
  WingedVertex& v1            = edge.vertex1Ref ();
  WingedVertex& v2            = edge.vertex2Ref ();
  Adjacents     a1            = adjacents (mesh, edge, v1);
  Adjacents     a2            = adjacents (mesh, edge, v2);

  return subdivide (v1.vertex (mesh), a1, v2.vertex (mesh), a2);
}

glm::vec3 subdivK6            (const Adjacents&, const Adjacents&);
glm::vec3 subdivK             (const glm::vec3&, const Adjacents&);
glm::vec3 subdivExtraordinary (const Adjacents&, const Adjacents&);

glm::vec3 subdivide ( const glm::vec3& v1, const Adjacents& a1
                    , const glm::vec3& v2, const Adjacents& a2) {
  if (a1.size () == 6 && a2.size () == 6)
    return subdivK6 (a1,a2);
  else if (a1.size () == 6 && a2.size () != 6)
    return subdivK (v2, a2);
  else if (a1.size () != 6 && a2.size () == 6)
    return subdivK (v1, a1);
  else {
    return subdivExtraordinary (a1,a2);
  }
}

Adjacents adjacents (const WingedMesh& mesh, WingedEdge& edge, const WingedVertex& vertex) {
  // traverses edge's siblings until it finds a vertex with of a lower-or-equal level
  std::function < glm::vec3 (const WingedEdge&, const WingedVertex&, int) > traverse =
    [&mesh, &traverse] 
    (const WingedEdge& e, const WingedVertex& o, int vertexGradient) -> glm::vec3 {

      if (vertexGradient <= 0) {
        return o.vertex (mesh);
      }
      else {
        WingedEdge* sibling = e.adjacentSibling (o);
        assert (sibling);

        return traverse (*sibling, sibling->otherVertexRef (o)
                        , vertexGradient + sibling->gradientAlong (o));
      }
  };

  Adjacents adjacents;
  for (auto it = vertex.adjacentEdgeIterator (edge,true); it.isValid (); it.next ()) {
    WingedEdge&   e = it.element ();
    WingedVertex& a = e.otherVertexRef (vertex);
    const int start = edge.gradientAlong (vertex) > 0 ? -edge.gradientAlong (vertex) : 0;
    const int first = e.gradientAlong (vertex);

    adjacents.push_back (traverse (e, a, start + first));
  }
  return adjacents;
}

glm::vec3 subdivK6 (const Adjacents& a1, const Adjacents& a2) {

  return (0.5f    * a1[0]) + (0.5f    * a2[0])
       + (0.125f  * a1[1]) + (0.125f  * a2[1])
       - (0.0625f * a1[2]) - (0.0625f * a2[2])
       - (0.0625f * a1[4]) - (0.0625f * a2[4]);
}

glm::vec3 subdivK (const glm::vec3& center, const Adjacents& a) {
  glm::vec3 v (0.0f,0.0f,0.0f);

  if (a.size () == 3) {
    v = (float (5/12) * (a[0] - center))
      - (float (1/12) * (a[1] - center))
      - (float (1/12) * (a[2] - center));
  }
  else if (a.size () == 4) {
    v = (0.375f * (a[0] - center))
      - (0.125f * (a[2] - center));
  }
  else {
    float K = float (a.size ());

    for (unsigned int i = 0; i < a.size (); i++) {
      float j   = float (i);
      float s_j = ( 0.25f 
                  +         cos ( 2.0f * M_PI * j / K ) 
                  + (0.5f * cos ( 4.0f * M_PI * j / K ))
                  ) / K;

      v = v + (s_j * (a[i] - center));
    }
  }
  return v + center;
}

glm::vec3 subdivExtraordinary (const Adjacents& a1, const Adjacents& a2) {

  return 0.5f * (subdivK (a2[0], a1) + subdivK (a1[0], a2));
}