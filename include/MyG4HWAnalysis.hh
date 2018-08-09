#ifndef MYG4HW_ANALYSIS_H_
#define MYG4HW_ANALYSIS_H_

class TFile;
class TTree;
class TH1D;

const G4int kMaxHist=20;

class MyG4HWAnalysis{
public:
  MyG4HWAnalysis();
  ~MyG4HWAnalysis();
  static MyG4HWAnalysis* Instance();

  void SetSeedNum(G4int idxN);
  int GetSeedNum();
  void SetTotNum(int ntot);
  int GetTotNum();
  void SetBeamEnergy(G4double beamE);
  void SetDosePerVoxel( int ix, int iy, int iz, double edep);
  double GetDosePerVoxel( int ix, int iy, int iz );

  void SetNoVoxelX(G4int val);
  void SetNoVoxelY(G4int val);
  void SetNoVoxelZ(G4int val);
  void SetVoxelX(G4double val);
  void SetVoxelY(G4double val);
  void SetVoxelZ(G4double val);
  void SetBeamType(G4String val);
  G4String GetBeamType();
  void SetMatType(G4String val);
  G4String GetMatType();
  void SetCutValue(G4double val);
  G4double GetCutValue();
  void SetStepFlag(G4double val);
  G4double GetStepFlag();

  int GetNoVoxelX();
  int GetNoVoxelY();
  int GetNoVoxelZ();
  double GetVoxelX();
  double GetVoxelY();
  double GetVoxelZ();

  void BookTreeAndHist();
  void SaveFile();

  void Fill1DHist(G4int idx, G4double ibin_x, G4double iwet);
  void FillNtuple(G4double pos_x, G4double pos_y, G4double pos_z,
                  G4int vox_id_x, G4int vox_id_y, G4int vox_id_z,
                  G4double edep, G4int trkp_ID, G4int trk_ID);

private:
  static MyG4HWAnalysis* fInstance;

  int       fNtot;
  G4String  fOutname;
  int       fSeedNum;
  TFile* fRootFile;
  TTree* fNtuple;
  TH1D*  fHist[kMaxHist];

  G4double fPos_x;
  G4double fPos_y;
  G4double fPos_z;
  G4int    fVoxID_X;
  G4int    fVoxID_Y;
  G4int    fVoxID_Z;
  G4int    fTrkp_ID;
  G4int    fTrk_ID;
  G4double fEdep;
  G4double fBeamE;
  G4double    fVoxelSumDept[61][61][150];
  G4int    fNvX;
  G4int    fNvY;
  G4int    fNvZ;
  G4double fvX;
  G4double fvY;
  G4double fvZ;
  G4double fstepflag;
  G4double fcutvalue;
  G4String fbeam;
  G4String fmat;

};

inline void MyG4HWAnalysis::SetStepFlag(G4double val)
{
  fstepflag=val;
}

inline G4double MyG4HWAnalysis::GetStepFlag()
{
  return fstepflag;
}

inline void MyG4HWAnalysis::SetCutValue(G4double val)
{
  fcutvalue=val;
}

inline G4double MyG4HWAnalysis::GetCutValue()
{
  return fcutvalue;
}

inline void MyG4HWAnalysis::SetBeamType(G4String val)
{
  fbeam=val;
}

inline G4String MyG4HWAnalysis::GetBeamType()
{
  return fbeam;
}

inline void MyG4HWAnalysis::SetMatType(G4String val)
{
  fmat=val;
}

inline G4String MyG4HWAnalysis::GetMatType()
{
  return fmat;
}

inline void MyG4HWAnalysis::SetSeedNum(int idxN)
{
  fSeedNum=idxN;
}
inline int MyG4HWAnalysis::GetSeedNum()
{
  return fSeedNum;
}
inline void MyG4HWAnalysis::SetTotNum(int ntot)
{
  fNtot=ntot;
}
inline int MyG4HWAnalysis::GetTotNum()
{
  return fNtot;
}
inline void MyG4HWAnalysis::SetBeamEnergy(G4double beamE)
{
  fBeamE=beamE;
}
inline void MyG4HWAnalysis::SetNoVoxelX(G4int val)
{
  fNvX = val;
}
inline void MyG4HWAnalysis::SetNoVoxelY(G4int val)
{
  fNvY = val;
}
inline void MyG4HWAnalysis::SetNoVoxelZ(G4int val)
{
  fNvZ = val;
}
inline void MyG4HWAnalysis::SetVoxelX(G4double val)
{
  fvX  = val;
}
inline void MyG4HWAnalysis::SetVoxelY(G4double val)
{
  fvY  = val;
}
inline void MyG4HWAnalysis::SetVoxelZ(G4double val)
{
  fvZ  = val;
}

inline int MyG4HWAnalysis::GetNoVoxelX()
{
  return fNvX;
}
inline int MyG4HWAnalysis::GetNoVoxelY()
{
  return fNvY;
}
inline int MyG4HWAnalysis::GetNoVoxelZ()
{
  return fNvZ;
}
inline double MyG4HWAnalysis::GetVoxelX()
{
  return fvX ;
}
inline double MyG4HWAnalysis::GetVoxelY()
{
  return fvY ;
}
inline double MyG4HWAnalysis::GetVoxelZ()
{
  return fvZ ;
}














#endif
