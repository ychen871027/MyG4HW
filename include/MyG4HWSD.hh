#ifndef MYG4HW_SD_H_
#define MYG4HW_SD_H_

#include "G4VSensitiveDetector.hh"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class MyG4HWSD : public G4VSensitiveDetector{
public:
  MyG4HWSD(G4String SDname);
  virtual ~MyG4HWSD();

  virtual void   Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
  virtual void   EndOfEvent(G4HCofThisEvent*);

};

#endif
