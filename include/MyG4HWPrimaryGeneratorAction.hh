#ifndef MyG4HWPrimaryGeneratorAction_h
#define MyG4HWPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class MyG4HWPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{
public:
  MyG4HWPrimaryGeneratorAction();
  ~MyG4HWPrimaryGeneratorAction();
  virtual void GeneratePrimaries( G4Event* );

private:
  G4ParticleGun* fParticleGun;
};

#endif
