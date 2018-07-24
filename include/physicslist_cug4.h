/*============================================================================
  CUGEANT4 - CUDA Geant4 Project
  Copyright 2012 [[@copyright]]

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef PHYSICS_LIST_CUG4_H_
#define PHYSICS_LIST_CUG4_H_

#include "G4VUserPhysicsList.hh"

namespace cug4 {

class PhysicsListCuG4 : public G4VUserPhysicsList {
public:
  PhysicsListCuG4();
  ~PhysicsListCuG4();

  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void SetCuts();

  void SetVerbose(int level);

};

} // namespace

#endif
