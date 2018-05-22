#ifndef PRIMARY_GENERATOR_ACTION_H_
#define PRIMARY_GENERATOR_ACTION_H_

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction(G4ParticleGun*);
  virtual ~PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*);

};

#endif
