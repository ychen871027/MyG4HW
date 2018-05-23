#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
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

  double fieldX     = 5 * (G4UniformRand()*2.0-1.0) *cm;
  double fieldY     = 5 * (G4UniformRand()*2.0-1.0) *cm;
  double SSD_Z      = 100 *cm;
  double fieldXY    = std::sqrt( sqr(fieldX)  + sqr(fieldY) );
  double dR         = std::sqrt( sqr(fieldXY) + sqr(SSD_Z)  );
  double sinAlpha   = fieldXY / dR;
  double cosAlpha   = SSD_Z   / dR;
  double sinPsi     = fieldY  / fieldXY ;
  double cosPsi     = fieldX  / fieldXY ;

  G4cout << "Field_X: " << fieldX /cm << " Field_Y: " << fieldY /cm
         << " SSD: "<< SSD_Z /cm << G4endl;

  ::particleGun ->SetParticleMomentumDirection(
    G4ThreeVector(sinAlpha*cosPsi, sinAlpha*sinPsi, cosAlpha));
    //G4ThreeVector(0., 0., 1.));
  ::particleGun ->SetParticlePosition( G4ThreeVector(0.*cm,0.*cm,-100.*cm) );
  //::particleGun ->SetParticleEnergy( 6.*MeV );
  ::particleGun ->SetParticleEnergy( 20.*MeV );
  //fParticleGun->SetParticleEnergy( 6.*MeV );
  ::particleGun ->GeneratePrimaryVertex( anEvent );
}
