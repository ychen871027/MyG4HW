#ifndef MyG4HWSD_h
#define MyG4HWSD_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class MyG4HWSD : public G4VSensitiveDetector{
public:
  MyG4HWSD(G4String SDname);
  ~MyG4HWSD();

  virtual void Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
  virtual void   EndOfEvent(G4HCofThisEvent*);
private:
  G4double sum_edep;
  G4int    no_step;
};

#endif
