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
  G4ParticleDefinition* particle;

  bool pencilBeam=true;
  if (pencilBeam==true) {
    particle = particleTable-> FindParticle( particleName="e-" );
    ::particleGun -> SetParticleDefinition( particle );
    ::particleGun -> SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    ::particleGun -> SetParticleEnergy( 20.*MeV );

  }else{
    particle = particleTable-> FindParticle( particleName="gamma" );
    ::particleGun -> SetParticleDefinition( particle );

    const double fSSD = 100.*cm;
    const double fieldX = 10.*cm;
    const double fieldY = 10.*cm;
    double dr = sqrt( sqr(fieldX/2.) + sqr(fieldY/2.) );
    double sinAlpha = dr /fSSD;
    double cosAlpha = sqrt(1. - sqr(sinAlpha));
    double dcos = 0.;

    double dsin, dphi, z;
    double x = DBL_MAX;
    double y = DBL_MAX;
    double xmax, ymax;
    xmax = fieldX/2./fSSD;
    ymax = fieldY/2./fSSD;

    while (!(abs(x)<xmax && abs(y)<ymax)) {
      dcos = G4RandFlat::shoot(cosAlpha, 1.0);
      dsin = sqrt(1.-sqr(dcos));
      dphi = G4RandFlat::shoot(0., CLHEP::twopi);

      x=cos(dphi)*dsin*dcos;
      y=sin(dphi)*dsin*dcos;

    }
    z=dcos;

    ::particleGun -> SetParticleMomentumDirection(G4ThreeVector(x, y, z));
    ::particleGun ->SetParticleEnergy( 6.*MeV );
  }

  ::particleGun ->SetParticlePosition( G4ThreeVector(0.*cm,0.*cm,-100.*cm) );
  ::particleGun ->GeneratePrimaryVertex( anEvent );
}
