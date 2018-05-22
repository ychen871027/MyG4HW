#ifndef MYG4HW_ANALYSIS_H_
#define MYG4HW_ANALYSIS_H_

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
  void FillNtuple(G4double pos_x, G4double pos_y, G4double pos_z,
                  G4int vox_id_x, G4int vox_id_y, G4int vox_id_z,
                  G4double edep);

private:
  static MyG4HWAnalysis* fInstance;
  TFile* fRootFile;
  TTree* fNtuple;
  TH1D*  fHist[kMaxHist];

  G4double fPos_x;
  G4double fPos_y;
  G4double fPos_z;
  G4int    fVoxID_X;
  G4int    fVoxID_Y;
  G4int    fVoxID_Z;
  G4double fEdep;
};
#endif
