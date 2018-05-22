#include "Randomize.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "PrimaryGeneratorAction.hh"
#include "MyG4HWPrimaryGeneratorAction.hh"

namespace{
  G4ParticleGun* particleGun {nullptr};
}

PrimaryGeneratorAction::PrimaryGeneratorAction(G4ParticleGun* gun)
{
  ::particleGun = gun;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete ::particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    =particleTable->FindParticle( particleName="e-" );
  //=particleTable->FindParticle( particleName="gamma" );
  ::particleGun ->SetParticleDefinition( particle );

  double RnmX     = 5 * (G4UniformRand()*2.0-1.0) *cm;
  double RnmY     = 5 * (G4UniformRand()*2.0-1.0) *cm;
  double RnmZ     = 100 *cm;
  double RnmXY    = sqrt(RnmX * RnmX + RnmY * RnmY);
  double RnmXYZ   = sqrt(RnmX * RnmX + RnmY * RnmY + RnmZ * RnmZ);
  double sinAlpha = RnmXY / RnmXYZ;
  double cosAlpha = RnmZ  / RnmXYZ;
  double sinPsi   = RnmY  / RnmXY ;
  double cosPsi   = RnmX  / RnmXY ;
  G4cout << "RnmX: " << RnmX << " RnmY: " << RnmY << " RnmZ: "<< RnmZ << G4endl;
  ::particleGun ->SetParticleMomentumDirection(
    G4ThreeVector(sinAlpha*cosPsi, sinAlpha*sinPsi, cosAlpha));
    //  G4ThreeVector(0, 0, -1));
  ::particleGun ->SetParticlePosition( G4ThreeVector(0.*cm,0.*cm,-100.*cm) );
  //::particleGun ->SetParticleEnergy( 6.*MeV );
  ::particleGun ->SetParticleEnergy( 20.*MeV );
  //fParticleGun->SetParticleEnergy( 6.*MeV );
  ::particleGun ->GeneratePrimaryVertex( anEvent );
}
