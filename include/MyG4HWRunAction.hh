#ifndef MYG4HW_RUN_ACTION_H_
#define MYG4HW_RUN_ACTION_H_

#include "G4Timer.hh"
#include "G4UserRunAction.hh"

class G4Run;
class MyG4HWAnalysis;

class MyG4HWRunAction:public G4UserRunAction{
public:
  MyG4HWRunAction();
  virtual ~MyG4HWRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:
  G4Timer mtime;
};
#endif
