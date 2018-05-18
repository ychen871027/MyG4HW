#ifndef MYG4HW_RUN_ACTION_H_
#define MYG4_HWRUN_ACTION_H_

#include "G4UserRunAction.hh"
#include "G4String.hh"

class G4Run;
class MyG4HWAnalysis;

class MyG4HWRunAction:public G4UserRunAction{
public:
  MyG4HWRunAction();
  virtual ~MyG4HWRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:
  MyG4HWAnalysis* fAnaMan;
  G4double fPos_x;
  G4double fPos_y;
  G4double fPos_z;
  G4double fEdep;
};
#endif
