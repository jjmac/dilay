/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_WINGED_MESH
#define DILAY_WINGED_MESH

#include <functional>
#include <glm/fwd.hpp>
#include <vector>
#include "intrusive-list.hpp"
#include "macro.hpp"

class AffectedFaces;
class Camera;
class Color;
class IndexOctree;
class Mesh;
class PrimPlane;
class PrimRay;
class PrimSphere;
class PrimTriangle;
class RenderMode;
class WingedEdge;
class WingedFace;
class WingedFaceIntersection;
class WingedVertex;

class WingedMesh : public IntrusiveList <WingedMesh>::Item {
  public: 
    DECLARE_BIG3 (WingedMesh, unsigned int);

    bool               operator==          (const WingedMesh&) const;
    bool               operator!=          (const WingedMesh&) const;

    unsigned int       index               () const;
    glm::vec3          vector              (unsigned int) const;
    unsigned int       index               (unsigned int) const;
    glm::vec3          normal              (unsigned int) const;
    WingedVertex*      vertex              (unsigned int);
    WingedEdge*        edge                (unsigned int);
    WingedFace*        face                (unsigned int);
    WingedFace*        someDegeneratedFace ();
    WingedVertex&      addVertex           (const glm::vec3&);
    WingedEdge&        addEdge             ();
    WingedFace&        addFace             (const PrimTriangle&);
    void               setIndex            (unsigned int, unsigned int);
    void               setVertex           (unsigned int, const glm::vec3&);
    void               setNormal           (unsigned int, const glm::vec3&);

    const IndexOctree& octree              () const;
    const Mesh&        mesh                () const;

    void               deleteEdge          (WingedEdge&);
    void               deleteFace          (WingedFace&);
    void               deleteVertex        (WingedVertex&);

    void               realignFace         (const WingedFace&, const PrimTriangle&);
    void               realignFace         (const WingedFace&);
    void               realignAllFaces     ();
    void               sanitize            ();

    unsigned int       numVertices         () const;
    unsigned int       numEdges            () const;
    unsigned int       numFaces            () const;
    unsigned int       numIndices          () const;
    bool               isEmpty             () const;

    Mesh               makePrunedMesh      (std::vector <unsigned int>* = nullptr) const;
    void               fromMesh            (const Mesh&, const PrimPlane* = nullptr);
    void               writeAllIndices     (); 
    void               writeAllNormals     (); 
    void               bufferData          ();
    void               render              (Camera&);
    void               reset               ();
    void               mirror              (const PrimPlane&);
    void               setupOctreeRoot     (const glm::vec3&, float);
    const RenderMode&  renderMode          () const;
    RenderMode&        renderMode          ();
    
    bool               intersects          (const PrimRay&, WingedFaceIntersection&);
    bool               intersects          (const PrimSphere&, AffectedFaces&);

    void               scale               (const glm::vec3&);
    void               scaling             (const glm::vec3&);
    glm::vec3          scaling             () const;
    void               translate           (const glm::vec3&);
    void               position            (const glm::vec3&);
    glm::vec3          position            () const;
    void               rotationMatrix      (const glm::mat4x4&);
    const glm::mat4x4& rotationMatrix      () const;
    void               rotationX           (float);
    void               rotationY           (float);
    void               rotationZ           (float);
    void               normalize           ();
    glm::vec3          center              () const;
    const Color&       color               () const;
    void               color               (const Color&);
    const Color&       wireframeColor      () const;
    void               wireframeColor      (const Color&);

    void               forEachVertex       (const std::function <void (WingedVertex&)>&);
    void               forEachConstVertex  (const std::function <void (const WingedVertex&)>&) const;
    void               forEachEdge         (const std::function <void (WingedEdge&)>&);
    void               forEachConstEdge    (const std::function <void (const WingedEdge&)>&) const;
    void               forEachFace         (const std::function <void (WingedFace&)>&);
    void               forEachConstFace    (const std::function <void (const WingedFace&)>&) const;

    SAFE_REF1 (WingedVertex, vertex, unsigned int)
    SAFE_REF1 (WingedEdge  , edge  , unsigned int)
    SAFE_REF1 (WingedFace  , face  , unsigned int)
  private:
    IMPLEMENTATION
};

#endif
