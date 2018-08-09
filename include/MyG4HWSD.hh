
#ifndef MYG4HW_SD_H_
#define MYG4HW_SD_H_

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class MyG4HWSD : public G4VSensitiveDetector{
public:
  MyG4HWSD(G4String SDname);
  virtual ~MyG4HWSD();

  virtual void   Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
  virtual void   EndOfEvent(G4HCofThisEvent*);

//protected:
//  G4int fNVoxelX;
//  G4int fNVoxelY;
//  G4int fNVoxelZ;
//  G4double fVoxelXX;
//  G4double fVoxelXY;
//  G4double fVoxelXZ;

private:
  G4double    fsum_edep;
  G4int       fno_step;
  G4int       fno_trk;
  G4double    fno_trkE;
  G4int       fno_stepn;
  G4double    fno_stepL;
  G4double    fdepth_z;
  //G4double    fVoxelSumDep[61*61*150*2];
  G4double    fVoxelSumDep[61][61][150];
  G4int    fVoxelN[61][61][150];
};

#endif
