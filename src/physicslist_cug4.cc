/*============================================================================
  CUGEANT4 - CUDA Geant4 Project
  Copyright 2012 [[@copyright]]

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include <cassert>
#include "G4ComptonScattering.hh"
#include "G4Electron.hh"
#include "G4EmProcessOptions.hh"
#include "G4Gamma.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4PhysicsListHelper.hh"
#include "G4Positron.hh"
#include "G4ProcessManager.hh"
#include "G4eMultipleScattering.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eIonisation.hh"
#include "G4eplusAnnihilation.hh"
#include "G4SystemOfUnits.hh"
#include "G4MscStepLimitType.hh"
#include "G4Proton.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4StepLimiter.hh"
//#include "cugeant4/connector/physicslist_cug4.h"
#include "physicslist_cug4.h"
//#include "config.h"

namespace cug4 {

// --------------------------------------------------------------------------
PhysicsListCuG4::PhysicsListCuG4()
{
  //defaultCutValue = 1.0 * km;
  defaultCutValue = 1.0 * mm;
  SetVerboseLevel(1);
}

// --------------------------------------------------------------------------
PhysicsListCuG4::~PhysicsListCuG4()
{
}

// --------------------------------------------------------------------------
void PhysicsListCuG4::ConstructParticle()
{
  G4Gamma::GammaDefinition();
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4Proton::ProtonDefinition();
}

// --------------------------------------------------------------------------
void PhysicsListCuG4::ConstructProcess()
{
  AddTransportation();

  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  assert( ph != NULL );
  //auto theParticleIterator=GetParticleIterator();
  theParticleIterator-> reset();

  // multiple scattering for e-
  G4eMultipleScattering *em_msc = new G4eMultipleScattering();
  em_msc->SetStepLimitType(fMinimal);

  // multiple scattering for e+
  G4eMultipleScattering *ep_msc = new G4eMultipleScattering();
  ep_msc->SetStepLimitType(fMinimal);

  // multiple scattering for protons
  // G4hMultipleScattering *p_msc = new G4hMultipleScattering();
  // p_msc->SetStepLimitType(fMinimal);

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator-> value();
    G4String particle_name = particle-> GetParticleName();
    if (particle_name == "gamma") {
      ph-> RegisterProcess(new G4PhotoElectricEffect, particle);
      ph-> RegisterProcess(new G4ComptonScattering,   particle);
      ph-> RegisterProcess(new G4GammaConversion,     particle);
    } else if (particle_name == "e-") {
      ph-> RegisterProcess(em_msc, particle);
      ph-> RegisterProcess(new G4eIonisation,         particle);
      ph-> RegisterProcess(new G4eBremsstrahlung,     particle);
      //ph->RegisterProcess(new G4StepLimiter(), particle);
    } else if (particle_name == "e+") {
      ph-> RegisterProcess(ep_msc, particle);
      ph-> RegisterProcess(new G4eIonisation,         particle);
      ph-> RegisterProcess(new G4eBremsstrahlung,     particle);
      ph-> RegisterProcess(new G4eplusAnnihilation,   particle);
    // } else if (particle_name == "proton") {
    //   ph-> RegisterProcess(p_msc, particle);
    //   ph-> RegisterProcess(new G4hIonisation,         particle);
    }
  }
  G4EmProcessOptions em_options;
  em_options.SetVerbose(0);
}

// --------------------------------------------------------------------------
void PhysicsListCuG4::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}

// --------------------------------------------------------------------------
void PhysicsListCuG4::SetVerbose(int level)
{
  G4EmProcessOptions em_options;
  em_options.SetVerbose(level);
}

} // namespace
