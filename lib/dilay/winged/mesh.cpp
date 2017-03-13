/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include "../util.hpp"
#include "action/finalize.hpp"
#include "affected-faces.hpp"
#include "edge-map.hpp"
#include "hash.hpp"
#include "intersection.hpp"
#include "mesh-util.hpp"
#include "primitive/ray.hpp"
#include "primitive/triangle.hpp"
#include "winged/edge.hpp"
#include "winged/face.hpp"
#include "winged/face-intersection.hpp"
#include "winged/mesh.hpp"
#include "winged/vertex.hpp"


  WingedMesh::WingedMesh (unsigned int i)
    : _index  (i)
  {}

  bool WingedMesh::operator== (const WingedMesh& other) const {
    return this->_index == other.index ();
  }

  bool WingedMesh::operator!= (const WingedMesh& other) const {
    return ! this->operator== (other);
  }

  unsigned int WingedMesh::index  ()               const { return this->_index;          }
  glm::vec3    WingedMesh::vector (unsigned int i) const { return this->_mesh.vertex (i); }
  unsigned int WingedMesh::index  (unsigned int i) const { return this->_mesh.index  (i); }
  glm::vec3    WingedMesh::normal (unsigned int i) const { return this->_mesh.normal (i); }

  WingedVertex* WingedMesh::vertex (unsigned int i) {
    return this->_vertices.get (i);
  }

  WingedEdge* WingedMesh::edge (unsigned int i) {
    return this->_edges.get (i);
  }

  WingedFace* WingedMesh::face (unsigned int i) {
    return this->_faces.get (i);
  }

  WingedFace* WingedMesh::someDegeneratedFace () {
    return this->_octree.numDegeneratedElements () == 0
      ? nullptr
      : this->face (this->_octree.someDegeneratedElement ());
  }

  WingedVertex& WingedMesh::addVertex (const glm::vec3& pos) {
    WingedVertex& vertex = this->_vertices.emplaceBack ();

    if (vertex.index () == this->_mesh.numVertices ()) {
      this->_mesh.addVertex (pos);
    }
    else {
      assert (vertex.index () < this->_mesh.numVertices ());
      this->_mesh.setVertex (vertex.index (), pos);
    }
    return vertex;
  }

  WingedEdge& WingedMesh::addEdge () {
    return this->_edges.emplaceBack ();
  }

  void WingedMesh::addFaceToOctree (const WingedFace& face, const PrimTriangle& geometry) {
    if (geometry.isDegenerated ()) {
      this->_octree.addDegeneratedElement (face.index ());
    }
    else {
      this->_octree.addElement (face.index (), geometry.center (), geometry.maxDimExtent ());
    }
  }

  WingedFace& WingedMesh::addFace (const PrimTriangle& geometry) {
    WingedFace& face = this->_faces.emplaceBack ();

    this->addFaceToOctree (face, geometry);

    if (3 * face.index () == this->_mesh.numIndices ()) {
      this->_mesh.addIndex (Util::invalidIndex ());
      this->_mesh.addIndex (Util::invalidIndex ());
      this->_mesh.addIndex (Util::invalidIndex ());
    }
    return face;
  }

  void WingedMesh::setIndex (unsigned int index, unsigned int vertexIndex) {
    return this->_mesh.setIndex (index, vertexIndex);
  }

  void WingedMesh::setVertex (unsigned int index, const glm::vec3& v) {
    assert (this->_vertices.isFreeSLOW (index) == false);
    return this->_mesh.setVertex (index,v);
  }

  void WingedMesh::setNormal (unsigned int index, const glm::vec3& n) {
    assert (this->_vertices.isFreeSLOW (index) == false);
    return this->_mesh.setNormal (index,n);
  }

  const IndexOctree& WingedMesh::octree () const {
      return _octree;
  }

  const Mesh& WingedMesh::mesh () const {
      return _mesh;
  }

  void WingedMesh::deleteEdge (WingedEdge& edge) {
    this->_edges.deleteElement (edge);
  }

  void WingedMesh::deleteFace (WingedFace& face) {
    this->_octree.deleteElement (face.index ());
    this->_faces.deleteElement (face);
  }

  void WingedMesh::deleteVertex (WingedVertex& vertex) {
    this->_vertices.deleteElement (vertex);
  }

  void WingedMesh::realignFace (const WingedFace& face, const PrimTriangle& geometry) {
    this->_octree.deleteElement (face.index ());
    this->addFaceToOctree (face, geometry);
  }

  void WingedMesh::realignFace (const WingedFace& face) {
    this->realignFace (face.index (), face.triangle (*this));
  }

  void WingedMesh::realignAllFaces () {
    this->forEachFace ([this] (WingedFace& f) { 
      this->realignFace (f);
    });
  }

  void WingedMesh::sanitize () {
    this->_octree.deleteEmptyChildren ();
    this->_octree.shrinkRoot          ();
  }

  unsigned int WingedMesh::numVertices () const {
    return this->_vertices.numElements ();
  }

  unsigned int WingedMesh::numEdges () const {
    return this->_edges.numElements ();
  }

  unsigned int WingedMesh::numFaces () const {
    return this->_faces.numElements ();
  }

  unsigned int WingedMesh::numIndices () const {
    return this->_mesh.numIndices ();
  }

  bool WingedMesh::isEmpty () const {
    return this->numVertices () == 0
        && this->numFaces    () == 0
        && this->numIndices  () == 0;
  }

  Mesh WingedMesh::makePrunedMesh (std::vector <unsigned int>* newFaceIndices) const {
    Mesh prunedMesh (this->_mesh, false);

    if (this->_vertices.hasFreeIndices () || this->_faces.hasFreeIndices ()) {
      std::vector <unsigned int> newVertexIndices;

      newVertexIndices.resize    (this->_mesh.numVertices (), Util::invalidIndex ());
      prunedMesh.reserveVertices (this->_mesh.numVertices ());
      prunedMesh.reserveIndices  (this->_mesh.numIndices  ());

      if (newFaceIndices) {
        assert (this->_mesh.numIndices () % 3 == 0);

        newFaceIndices->clear ();
        newFaceIndices->resize (this->_mesh.numIndices () / 3, Util::invalidIndex ());
      }

      this->forEachConstVertex ([&prunedMesh, &newVertexIndices, this] (const WingedVertex& v) {
        const unsigned int newIndex = prunedMesh.addVertex ( v.position    (*this)
                                                           , v.savedNormal (*this) );
        newVertexIndices [v.index ()] = newIndex;
      });
      this->forEachConstFace ( [&prunedMesh, &newVertexIndices, &newFaceIndices]
                               (const WingedFace& f) 
      {
        const unsigned int newV1 = newVertexIndices [f.vertexRef (0).index ()];
        const unsigned int newV2 = newVertexIndices [f.vertexRef (1).index ()];
        const unsigned int newV3 = newVertexIndices [f.vertexRef (2).index ()];

        assert (newV1 != Util::invalidIndex ());
        assert (newV2 != Util::invalidIndex ());
        assert (newV3 != Util::invalidIndex ());

        const unsigned int newI = prunedMesh.addIndex (newV1);
                                  prunedMesh.addIndex (newV2);
                                  prunedMesh.addIndex (newV3);

        assert (std::div (newI, 3).rem == 0);
        if (newFaceIndices) {
          (*newFaceIndices)[f.index ()] = std::div (newI, 3).quot;
        }
      });
    }
    else {
      prunedMesh = this->_mesh;
    }
    return prunedMesh;
  }

  void WingedMesh::fromMesh (const Mesh& mesh, const PrimPlane* mirror) {
    EdgeMap <WingedEdge*> edgeMap;

    /** `findOrAddEdge (m,i1,i2,f)` searches an edge between vertices 
     * `i1` and `i2` in `m`.
     * If such an edge exists, `f` becomes its new right face.
     * Otherwise a new edge is added to `this` and `m`, with `f` being its left face.
     * The found (resp. created) edge is returned.
     */
    auto findOrAddEdge = [this, &edgeMap] ( unsigned int index1, unsigned int index2
                                          , WingedFace& face ) -> WingedEdge&
    {
      WingedEdge** result = edgeMap.find (index1, index2);

      if (result) {
        WingedEdge* existingEdge = *result;

        existingEdge->rightFace (&face);
        face.edge (existingEdge);

        return *existingEdge;
      }
      else {
        WingedVertex* v1    = this->vertex (index1);
        WingedVertex* v2    = this->vertex (index2);
        WingedEdge& newEdge = this->addEdge ();
          
        edgeMap.add (index1, index2, &newEdge);
        newEdge.vertex1  (v1);
        newEdge.vertex2  (v2);
        newEdge.leftFace (&face);

        v1-> edge (&newEdge);
        v2-> edge (&newEdge);
        face.edge (&newEdge);

        return newEdge;
      }
    };

    // mesh
    this->reset ();

    this->_mesh = bool (mirror) ? MeshUtil::mirror (mesh, *mirror)
                               : mesh;

    // octree
    glm::vec3 minVertex, maxVertex;
    this->_mesh.minMax (minVertex, maxVertex);

    const glm::vec3 center = (maxVertex + minVertex) * glm::vec3 (0.5f);
    const glm::vec3 delta  =  maxVertex - minVertex;
    const float     width  = glm::max (glm::max (delta.x, delta.y), delta.z);

    this->setupOctreeRoot (center, width);

    // vertices
    for (unsigned int i = 0; i < this->_mesh.numVertices (); i++) {
      this->_vertices.emplaceBack ();
    }

    // faces & edges
    assert (this->_mesh.numIndices () % 3 == 0);

    edgeMap.resize (this->_mesh.numVertices ());

    for (unsigned int i = 0; i < this->_mesh.numIndices (); i += 3) {
      unsigned int index1 = this->_mesh.index (i + 0);
      unsigned int index2 = this->_mesh.index (i + 1);
      unsigned int index3 = this->_mesh.index (i + 2);

      WingedFace& f = this->addFace (PrimTriangle (*this
                                                  , this->vertexRef (index1)
                                                  , this->vertexRef (index2)
                                                  , this->vertexRef (index3) ));

      WingedEdge& e1 = findOrAddEdge (index1, index2, f);
      WingedEdge& e2 = findOrAddEdge (index2, index3, f);
      WingedEdge& e3 = findOrAddEdge (index3, index1, f);

      e1.predecessor (f, &e3);
      e1.successor   (f, &e2);
      e2.predecessor (f, &e1);
      e2.successor   (f, &e3);
      e3.predecessor (f, &e2);
      e3.successor   (f, &e1);
    }

    if (this->_octree.numDegeneratedElements () > 0) {
      Action::collapseDegeneratedFaces (*this);
    }
    this->writeAllNormals ();
    this->bufferData      ();
  }

  void WingedMesh::writeAllIndices () {
    this->_faces.forEachElement ([this] (WingedFace& face) {
      face.writeIndices (*this);
    });
  }

  void WingedMesh::writeAllNormals () {
    this->_vertices.forEachElement ([this] (WingedVertex& v) {
      v.writeNormal (*this, v.interpolatedNormal (*this));
    });
  }

  void WingedMesh::bufferData  () {
    auto resetFreeFaceIndices = [this] () {
      if (this->numFaces () > 0) {
        WingedFace& someFace = this->_faces.front ();

        for (unsigned int index : this->_faces.freeIndices ()) {
          this->setIndex ((3 * index) + 0, this->index ((3 * someFace.index ()) + 0));
          this->setIndex ((3 * index) + 1, this->index ((3 * someFace.index ()) + 1));
          this->setIndex ((3 * index) + 2, this->index ((3 * someFace.index ()) + 2));
        }
      }
    };

    resetFreeFaceIndices  ();
    this->_mesh.bufferData ();
  }

  void WingedMesh::render (Camera& camera) {
    this->_mesh.render   (camera);
#ifdef DILAY_RENDER_OCTREE
    this->_octree.render (camera);
#endif
  }

  void WingedMesh::reset () {
    this->_mesh    .reset ();
    this->_vertices.reset ();
    this->_edges   .reset ();
    this->_faces   .reset ();
    this->_octree  .reset ();
  }

  void WingedMesh::mirror (const PrimPlane& plane) {
    this->fromMesh (this->makePrunedMesh (nullptr), &plane);
  }

  void WingedMesh::setupOctreeRoot (const glm::vec3& center, float width) {
    assert (this->_octree.hasRoot () == false);
    this->_octree.setupRoot (center,width);
  }

  const RenderMode& WingedMesh::renderMode () const { return this->_mesh.renderMode (); }
  RenderMode&       WingedMesh::renderMode ()       { return this->_mesh.renderMode (); }

  bool WingedMesh::intersects (const PrimRay& ray, WingedFaceIntersection& intersection) {
    this->_octree.intersects (ray, [this, &ray, &intersection] (unsigned int i) {
      WingedFace&        face = this->faceRef (i);
      const PrimTriangle tri  = face.triangle (*this);
      float              t;

      if (IntersectionUtil::intersects (ray, tri, &t)) {
        intersection.update (t, ray.pointAt (t), tri.normal (), *this, face);
      }
    });
    return intersection.isIntersection ();
  }

  bool WingedMesh::intersects (const PrimSphere& sphere, AffectedFaces& faces) {
    this->_octree.intersects (sphere, [this, &sphere, &faces] (unsigned int i) {
      WingedFace&        face = this->faceRef (i);
      const PrimTriangle tri  = face.triangle (*this);

      if (IntersectionUtil::intersects (sphere, tri)) {
        faces.insert (face);
      }
    });
    faces.commit ();
    return faces.isEmpty () == false;
  }

  void               WingedMesh::scale          (const glm::vec3& v)   { return this->_mesh.scale (v); }
  void               WingedMesh::scaling        (const glm::vec3& v)   { return this->_mesh.scaling (v); }
  glm::vec3          WingedMesh::scaling        () const               { return this->_mesh.scaling (); }
  void               WingedMesh::translate      (const glm::vec3& v)   { return this->_mesh.translate (v); }
  void               WingedMesh::position       (const glm::vec3& v)   { return this->_mesh.position (v); }
  glm::vec3          WingedMesh::position       () const               { return this->_mesh.position (); }
  void               WingedMesh::rotationMatrix (const glm::mat4x4& m) { return this->_mesh.rotationMatrix (m); }
  const glm::mat4x4& WingedMesh::rotationMatrix () const               { return this->_mesh.rotationMatrix (); }
  void               WingedMesh::rotationX      (float v)              { return this->_mesh.rotationX (v); }
  void               WingedMesh::rotationY      (float v)              { return this->_mesh.rotationY (v); }
  void               WingedMesh::rotationZ      (float v)              { return this->_mesh.rotationZ (v); }

  void WingedMesh::normalize () {
    this->_mesh.normalize ();
    this->_octree.reset ();

    this->forEachFace ([this] (WingedFace& face) { 
      this->addFaceToOctree (face, face.triangle (*this));
    });
  }

  glm::vec3 WingedMesh::center () const {
    return this->_mesh.center ();
  }

  const Color& WingedMesh::color () const {
    return this->_mesh.color ();
  }

  void WingedMesh::color (const Color& c) {
    this->_mesh.color (c);
  }

  const Color& WingedMesh::wireframeColor () const {
    return this->_mesh.wireframeColor ();
  }

  void WingedMesh::wireframeColor (const Color& c) {
    this->_mesh.wireframeColor (c);
  }

  void WingedMesh::forEachVertex (const std::function <void (WingedVertex&)>& f) {
    this->_vertices.forEachElement (f);
  }

  void WingedMesh::forEachConstVertex (const std::function <void (const WingedVertex&)>& f) const {
    this->_vertices.forEachConstElement (f);
  }

  void WingedMesh::forEachEdge (const std::function <void (WingedEdge&)>& f) {
    this->_edges.forEachElement (f);
  }

  void WingedMesh::forEachConstEdge (const std::function <void (const WingedEdge&)>& f) const {
    this->_edges.forEachConstElement (f);
  }

  void WingedMesh::forEachFace (const std::function <void (WingedFace&)>& f) {
    this->_faces.forEachElement (f);
  }

  void WingedMesh::forEachConstFace (const std::function <void (const WingedFace&)>& f) const {
    this->_faces.forEachConstElement (f);
  }
