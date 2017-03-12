/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_ACTION_FINALIZE
#define DILAY_ACTION_FINALIZE

class AffectedFaces;
class WingedMesh;

namespace Action {

  void collapseDegeneratedFaces (WingedMesh&);
  void collapseDegeneratedFaces (WingedMesh&, AffectedFaces&);
  void finalize                 (WingedMesh&, AffectedFaces&);
}

#endif
