#ifndef MYG4HW_PRIMARY_GENERATORACTION_H_
#define MYG4HW_PRIMARY_GENERATORACTION_H_

#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4ParticleGun;
class PrimaryGeneratorAction;

class MyG4HWPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
  MyG4HWPrimaryGeneratorAction();
  virtual ~MyG4HWPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

};

#endif
