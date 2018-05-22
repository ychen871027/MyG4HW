#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "PrimaryGeneratorAction.hh"
#include "MyG4HWPrimaryGeneratorAction.hh"

namespace{
  G4ParticleGun* particleGun {nullptr};
  PrimaryGeneratorAction* pgAction {nullptr};
}

MyG4HWPrimaryGeneratorAction::MyG4HWPrimaryGeneratorAction()
  :G4VUserPrimaryGeneratorAction()
{
  G4int n_particle = 1;
  ::particleGun = new G4ParticleGun(n_particle);
  ::pgAction   = new PrimaryGeneratorAction(::particleGun);
}

MyG4HWPrimaryGeneratorAction::~MyG4HWPrimaryGeneratorAction()
{
  delete ::particleGun;
}

void MyG4HWPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  ::pgAction ->GeneratePrimaries(anEvent);
}
