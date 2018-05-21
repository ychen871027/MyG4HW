#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "MyG4HWPrimaryGeneratorAction.hh"

MyG4HWPrimaryGeneratorAction::MyG4HWPrimaryGeneratorAction()
  :G4VUserPrimaryGeneratorAction(),
   fParticleGun{nullptr}
{
  //G4int n_particle = 1;
  fParticleGun = new G4GeneralParticleSource();
  //fParticleGun = new G4ParticleGun(n_particle);
}

MyG4HWPrimaryGeneratorAction::~MyG4HWPrimaryGeneratorAction()
{
  delete fParticleGun;
}

void MyG4HWPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  //G4String particleName;
  //G4ParticleDefinition* particle
  //  =particleTable->FindParticle( particleName="e-" );
  //=particleTable->FindParticle( particleName="gamma" );
  //fParticleGun->SetParticleDefinition( particle );
  //fParticleGun->SetParticleMomentumDirection( G4ThreeVector(0.,0.,1.) );
  //fParticleGun->SetParticlePosition( G4ThreeVector(0.,0.,-100.*cm) );
  //fParticleGun->SetParticleEnergy( 20.*MeV );
  //fParticleGun->SetParticleEnergy( 6.*MeV );
  fParticleGun->GeneratePrimaryVertex( anEvent );
}
