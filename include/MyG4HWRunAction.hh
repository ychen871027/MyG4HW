#ifndef MyG4HWRunAction_h
#define MyG4HWRunAction_h 1

#include "G4UserRunAction.hh"
#include <vector>
#include "G4String.hh"

class G4Run;


class MyG4HWRunAction:public G4UserRunAction{
public:
  MyG4HWRunAction();
  ~MyG4HWRunAction();
  virtual void BeginOfRunAction( const G4Run* );
  virtual void EndOfRunAction( const G4Run* );

private:
  std::vector<G4String> fSDName;
};
#endif
