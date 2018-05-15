#include "MyG4HWPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"

MyG4HWPrimaryGeneratorAction::MyG4HWPrimaryGeneratorAction()
:G4VUserPrimaryGeneratorAction(),
 fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun( n_particle );
}

MyG4HWPrimaryGeneratorAction::~MyG4HWPrimaryGeneratorAction(){

  delete fParticleGun;

}

void MyG4HWPrimaryGeneratorAction::GeneratePrimaries( G4Event* anEvent){
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    =particleTable->FindParticle( particleName="e-" );
  fParticleGun->SetParticleDefinition( particle );
  fParticleGun->SetParticleMomentumDirection( G4ThreeVector(0.,0.,1.) );
  fParticleGun->SetParticlePosition( G4ThreeVector(0.,0.,-100.*cm) );
  fParticleGun->SetParticleEnergy( 20.*MeV );
  fParticleGun->GeneratePrimaryVertex( anEvent );

}