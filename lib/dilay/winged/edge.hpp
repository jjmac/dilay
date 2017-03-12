/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_WINGED_EDGE
#define DILAY_WINGED_EDGE

#include <glm/fwd.hpp>
#include "intrusive-list.hpp"
#include "macro.hpp"

class WingedVertex;
class WingedFace;
class WingedMesh;

class WingedEdge : public IntrusiveList <WingedEdge>::Item {
  public:
    WingedEdge (unsigned int);
    WingedEdge (const WingedEdge&)  = delete;
    WingedEdge (      WingedEdge&&) = default;

    bool            operator==       (const WingedEdge&) const;
    bool            operator!=       (const WingedEdge&) const;

    unsigned int    index            () const { return this->_index; }
    WingedVertex*   vertex1          () const { return this->_vertex1; }
    WingedVertex*   vertex2          () const { return this->_vertex2; }
    WingedFace*     leftFace         () const { return this->_leftFace; }
    WingedFace*     rightFace        () const { return this->_rightFace; }
    WingedEdge*     leftPredecessor  () const { return this->_leftPredecessor; }
    WingedEdge*     leftSuccessor    () const { return this->_leftSuccessor; }
    WingedEdge*     rightPredecessor () const { return this->_rightPredecessor; }
    WingedEdge*     rightSuccessor   () const { return this->_rightSuccessor; }

    bool            isLeftFace       (const WingedFace&)   const;
    bool            isRightFace      (const WingedFace&)   const;
    bool            isVertex1        (const WingedVertex&) const;
    bool            isVertex2        (const WingedVertex&) const;

    WingedVertex*   firstVertex      (const WingedFace&)   const;
    WingedVertex*   secondVertex     (const WingedFace&)   const;
    WingedEdge*     predecessor      (const WingedFace&)   const;
    WingedEdge*     successor        (const WingedFace&)   const;
    WingedFace*     otherFace        (const WingedFace&)   const;
    WingedVertex*   otherVertex      (const WingedVertex&) const;

    void            setGeometry      ( WingedVertex*, WingedVertex*
                                     , WingedFace*, WingedFace*
                                     , WingedEdge*, WingedEdge*
                                     , WingedEdge*, WingedEdge* );

    void            vertex1          (WingedVertex* v) { this->_vertex1          = v; }
    void            vertex2          (WingedVertex* v) { this->_vertex2          = v; }
    void            leftFace         (WingedFace* f)   { this->_leftFace         = f; }
    void            rightFace        (WingedFace* f)   { this->_rightFace        = f; }
    void            leftPredecessor  (WingedEdge* e)   { this->_leftPredecessor  = e; }
    void            leftSuccessor    (WingedEdge* e)   { this->_leftSuccessor    = e; }
    void            rightPredecessor (WingedEdge* e)   { this->_rightPredecessor = e; }
    void            rightSuccessor   (WingedEdge* e)   { this->_rightSuccessor   = e; }

    void            firstVertex      (const WingedFace&, WingedVertex*);
    void            secondVertex     (const WingedFace&, WingedVertex*);
    void            vertex           (const WingedVertex&, WingedVertex*);
    void            face             (const WingedFace&, WingedFace*);
    void            predecessor      (const WingedFace&, WingedEdge*);
    void            successor        (const WingedFace&, WingedEdge*);

    glm::vec3       vector           (const WingedMesh&) const;
    glm::vec3       vector           (const WingedMesh&, const WingedVertex&) const;
    float           length           (const WingedMesh&) const;
    float           lengthSqr        (const WingedMesh&) const;
    WingedEdge*     successor        (const WingedFace&, unsigned int) const;
    WingedEdge*     predecessor      (const WingedFace&, unsigned int) const;
    WingedVertex*   vertex           (const WingedFace&, unsigned int) const;
    glm::vec3       middle           (const WingedMesh&) const;
    WingedEdge*     adjacentSibling  (const WingedMesh&, const WingedVertex&) const;

    SAFE_REF_CONST  (WingedVertex, vertex1)
    SAFE_REF_CONST  (WingedVertex, vertex2)
    SAFE_REF_CONST  (WingedFace  , leftFace)
    SAFE_REF_CONST  (WingedFace  , rightFace)
    SAFE_REF_CONST  (WingedEdge  , leftPredecessor)
    SAFE_REF_CONST  (WingedEdge  , leftSuccessor)
    SAFE_REF_CONST  (WingedEdge  , rightPredecessor)
    SAFE_REF_CONST  (WingedEdge  , rightSuccessor)

    SAFE_REF1_CONST (WingedVertex, firstVertex, const WingedFace&)
    SAFE_REF1_CONST (WingedVertex, secondVertex, const WingedFace&)
    SAFE_REF1_CONST (WingedEdge  , predecessor, const WingedFace&)
    SAFE_REF1_CONST (WingedEdge  , successor, const WingedFace&)
    SAFE_REF1_CONST (WingedFace  , otherFace, const WingedFace&)
    SAFE_REF1_CONST (WingedVertex, otherVertex, const WingedVertex&)

    SAFE_REF2_CONST (WingedEdge  , successor, const WingedFace&, unsigned int)
    SAFE_REF2_CONST (WingedEdge  , predecessor, const WingedFace&, unsigned int)
    SAFE_REF2_CONST (WingedVertex, vertex, const WingedFace&, unsigned int)

  private:
    const unsigned int _index;

    WingedVertex*      _vertex1;
    WingedVertex*      _vertex2;

    WingedFace*        _leftFace;
    WingedFace*        _rightFace;

    WingedEdge*        _leftPredecessor;
    WingedEdge*        _leftSuccessor;

    WingedEdge*        _rightPredecessor;
    WingedEdge*        _rightSuccessor;
};

#endif
