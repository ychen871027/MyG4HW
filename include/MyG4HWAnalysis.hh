#ifndef MYG4HW_ANALYSIS_H_
#define MYG4HW_ANALYSIS_H_
#include <CLHEP/Units/SystemOfUnits.h>
#include "G4UnitsTable.hh"

class TFile;
class TTree;
class TH1D;

const G4int kMaxHist=2;

class MyG4HWAnalysis{
public:
  MyG4HWAnalysis();
  ~MyG4HWAnalysis();
  static MyG4HWAnalysis* Instance();

  void BookTreeAndHist();
  void SaveFile();

  void Fill1DHist(G4int idx, G4double ibin_x, G4double iwet);
  void FillNtuple(G4double pos_x, G4double pos_y,
                  G4double pos_z, G4double edep);

private:
  static MyG4HWAnalysis* fInstance;
  TFile* fRootFile;
  TTree* fNtuple;
  TH1D*  fHist[kMaxHist];

  G4double fPos_x;
  G4double fPos_y;
  G4double fPos_z;
  G4double fEdep;
};
#endif
