#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "MyG4HWPhysicsList.hh"

MyG4HWPhysicsList::MyG4HWPhysicsList(): G4VUserPhysicsList()
{
  defaultCutValue = 1.0*cm;
  SetVerboseLevel(1);
}

MyG4HWPhysicsList:: ~MyG4HWPhysicsList()
{
}

void MyG4HWPhysicsList::ConstructParticle()
{
  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
}

void MyG4HWPhysicsList::ConstructBosons()
{
  G4Gamma::GammaDefinition();
}

void MyG4HWPhysicsList::ConstructLeptons()
{
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
}

void MyG4HWPhysicsList::ConstructMesons()
{
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
}

void MyG4HWPhysicsList::ConstructBaryons()
{
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
}

void MyG4HWPhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  ConstructGeneral();
  AddStepMax();
}

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"
#include "G4ionIonisation.hh"

void MyG4HWPhysicsList::ConstructEM()
{
  auto particleIterator = GetParticleIterator();
  particleIterator-> reset();
  while ( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator-> value();
    G4ProcessManager* pmanager     = particle-> GetProcessManager();
    G4String particleName          = particle-> GetParticleName();

    if ( particleName == "gamma" ){
      pmanager-> AddDiscreteProcess( new G4PhotoElectricEffect );
      pmanager-> AddDiscreteProcess( new G4ComptonScattering );
      pmanager-> AddDiscreteProcess( new G4GammaConversion );
    } else if ( particleName == "e-" ){
      pmanager-> AddProcess( new G4eMultipleScattering, -1, 1, 1 );
      pmanager-> AddProcess( new G4eIonisation, -1, 2, 2 );
      pmanager-> AddProcess( new G4eBremsstrahlung, -1, 3, 3 );
    } else if ( particleName == "e+" ){
      pmanager-> AddProcess( new G4eMultipleScattering, -1, 1, 1 );
      pmanager-> AddProcess( new G4eIonisation, -1, 2, 2 );
      pmanager-> AddProcess( new G4eBremsstrahlung, -1, 3, 3 );
      pmanager-> AddProcess( new G4eplusAnnihilation, 0, -1, 4 );
    } else if ( particleName == "proton" || particleName == "pi-" ||
                particleName == "pi+" ){
      pmanager-> AddProcess( new G4hMultipleScattering, -1, 1, 1);
      pmanager-> AddProcess( new G4hIonisation, -1, 2, 2);
      pmanager-> AddProcess( new G4hBremsstrahlung, -1, 3, 3);
      pmanager-> AddProcess( new G4hPairProduction, -1, 4, 4);
    } else if ( particleName == "GenericIon" ){
      pmanager-> AddProcess( new G4hMultipleScattering, -1, 1, 1 );
      pmanager-> AddProcess( new G4ionIonisation, -1, 2, 2 );
    } else if ( (!particle-> IsShortLived()) &&
                (particle-> GetPDGCharge() != 0.0 ) &&
                (particle-> GetParticleName() != "chargedgeantino") ){
      pmanager-> AddProcess( new G4hMultipleScattering, -1, 1, 1 );
      pmanager-> AddProcess( new G4hIonisation, -1, 2, 2 );
    }
  }
}

#include "G4Decay.hh"

void MyG4HWPhysicsList:: ConstructGeneral()
{
  G4Decay* theDecayProcess = new G4Decay();
  auto particleIterator    = GetParticleIterator();
  particleIterator-> reset();
  while ( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator-> value();
    G4ProcessManager* pmanager     = particle-> GetProcessManager();
    if ( theDecayProcess-> IsApplicable(*particle) ){
      pmanager-> AddProcess( theDecayProcess );
      pmanager-> SetProcessOrdering( theDecayProcess, idxPostStep );
      pmanager-> SetProcessOrdering( theDecayProcess, idxAtRest );
    }
  }
}

#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

void MyG4HWPhysicsList:: AddStepMax()
{
  G4StepLimiter* stepLimiter = new G4StepLimiter();
  auto particleIterator      = GetParticleIterator();
  particleIterator-> reset();
  while ( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator-> value();
    G4ProcessManager* pmanager     = particle-> GetProcessManager();
    if (particle-> GetPDGCharge() != 0.0 ){
      pmanager-> AddDiscreteProcess( stepLimiter );
    }
  }
}

void MyG4HWPhysicsList::SetCuts()
{
  SetCutsWithDefault();
  if ( verboseLevel>0 ) DumpCutValuesTable();
}
