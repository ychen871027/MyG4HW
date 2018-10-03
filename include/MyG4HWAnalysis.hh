#ifndef MYG4HW_ANALYSIS_H_
#define MYG4HW_ANALYSIS_H_

#include "G4MscStepLimitType.hh"
class TFile;
class TTree;
class TH1D;

const G4int kMaxHist=20;

class MyG4HWAnalysis{
public:
  MyG4HWAnalysis();
  ~MyG4HWAnalysis();
  static MyG4HWAnalysis* Instance();

  void SetVerbose(bool val);
  bool GetVerbose();
  void SetOutPutPrefix(G4String val);
  G4String GetOutPutPrefix();
  void SetConfVoxel(G4String voxelconfig);
  G4String GetVoxelMatType();
  G4String GetPLname();
  G4String MakeDetailTrackINFO();
  bool GetExDensity();
  void SetDensityFlag(bool val);
  G4String GetNameOfDenFile();
  void SetSeedNum(G4int idxN);
  int GetSeedNum();
  void SetTotNum(int ntot);
  int GetTotNum();
  void SetBeamEnergy(G4double beamE);
  void SetDosePerVoxel( int ix, int iy, int iz, double edep);
  double GetDosePerVoxel( int ix, int iy, int iz );
  void SetDensityPerVoxel( int ix, int iy, int iz, double density);
  double GetDensityPerVoxel( int ix, int iy, int iz );

  void SetNoVoxelX(G4int val);
  void SetNoVoxelY(G4int val);
  void SetNoVoxelZ(G4int val);
  void SetVoxelX(G4double val);
  void SetVoxelY(G4double val);
  void SetVoxelZ(G4double val);
  void SetBeamName(G4String val);
  G4String GetBeamName();
  void SetXrayEnergy(G4String val);
  G4String GetXrayEnergy();
  void SetMidMat(G4String val);
  G4String GetMidMat();
  void SetBeamType(G4String val);
  G4String GetBeamType();
  void SetMatType(G4String val);
  G4String GetMatType();
  void SetCutValue(G4double val);
  G4double GetCutValue();
  void SetStepFlag(G4double val);
  G4double GetStepFlag();
  void SetMSCStepAlg(G4String val);
  G4MscStepLimitType GetMSCStepAlg();

  int GetNoVoxelX();
  int GetNoVoxelY();
  int GetNoVoxelZ();
  double GetVoxelX();
  double GetVoxelY();
  double GetVoxelZ();

  void BookTreeAndHist();
  void SaveFile();

  void Fill1DHist(G4int idx, G4double ibin_x, G4double iwet);

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
  G4double*    fVoxelSumDept;
  G4double*    fdensityPervox;
  //G4double    fVoxelSumDept[61][61][150];
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
  G4String fmscstepalg;
  G4String fbeamname;
  G4String fxrayE;
  G4String fMidMat;
  G4String fVoxMatType;
  G4String fPLname;
  bool fExDensity;
  G4String fdensitylist;
  bool fverbose;
  G4String ofilenamefordetail;
  G4String outputprefix;
};

inline void MyG4HWAnalysis::SetOutPutPrefix(G4String val)
{
  outputprefix = val;
}
inline G4String MyG4HWAnalysis::GetOutPutPrefix()
{
  return outputprefix;
}
inline G4String MyG4HWAnalysis::MakeDetailTrackINFO()
{
  return ofilenamefordetail;
}
inline void MyG4HWAnalysis::SetVerbose(bool val)
{
  fverbose=val;
}
inline bool MyG4HWAnalysis::GetVerbose()
{
  return fverbose;
}
inline G4String MyG4HWAnalysis::GetNameOfDenFile()
{
  return fdensitylist;
}

inline bool MyG4HWAnalysis::GetExDensity()
{
  return fExDensity;
}
inline void MyG4HWAnalysis::SetDensityFlag(bool val)
{
  fExDensity = val;
}
inline G4String MyG4HWAnalysis::GetPLname()
{
  return fPLname;
}
inline G4String MyG4HWAnalysis::GetVoxelMatType()
{
  return fVoxMatType;
}
inline void MyG4HWAnalysis::SetMidMat(G4String val)
{
  fMidMat=val;
}


inline G4String MyG4HWAnalysis::GetMidMat()
{
  return fMidMat;
}

inline void MyG4HWAnalysis::SetStepFlag(G4double val)
{
  fstepflag=val;
}

inline G4double MyG4HWAnalysis::GetStepFlag()
{
  return fstepflag;
}

inline void MyG4HWAnalysis::SetMSCStepAlg(G4String val)
{
  fmscstepalg=val;
}

inline G4MscStepLimitType MyG4HWAnalysis::GetMSCStepAlg()
{
  if(strcmp(fmscstepalg, "fMinimal")==0){
    return fMinimal;
  } else if (strcmp(fmscstepalg, "fUseSafety")==0) {
    return fUseSafety;
  } else if (strcmp(fmscstepalg, "fUseSafetyPlus")==0) {
    return fUseSafetyPlus;
  } else if (strcmp(fmscstepalg, "fUseDistanceToBoundary")==0) {
    return fUseDistanceToBoundary;
  } else{
    std::cout << "you don't select MSC step alg, will use fMinimal" << std::endl;
    return fMinimal;
  } 
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

inline void MyG4HWAnalysis::SetBeamName(G4String val)
{
  fbeamname=val;
}

inline G4String MyG4HWAnalysis::GetBeamName()
{
  return fbeamname;
}


inline void MyG4HWAnalysis::SetXrayEnergy(G4String val)
{
  fxrayE=val;
}

inline G4String MyG4HWAnalysis::GetXrayEnergy()
{
  return fxrayE;
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
  return fvX *CLHEP::mm ;
}
inline double MyG4HWAnalysis::GetVoxelY()
{
  return fvY *CLHEP::mm ;
}
inline double MyG4HWAnalysis::GetVoxelZ()
{
  return fvZ *CLHEP::mm ;
}

#endif
