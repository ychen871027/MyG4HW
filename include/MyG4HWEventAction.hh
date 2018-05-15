#ifndef MyG4HWEventAction_h
#define MyG4HWEventAction_h 1

#include "G4UserEventAction.hh"

class G4Event;

class MyG4HWEventAction : public G4UserEventAction{
public:
  MyG4HWEventAction();
  ~MyG4HWEventAction();

  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfEventAction( const G4Event* anEvent);
};

#endif
