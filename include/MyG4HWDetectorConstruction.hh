#ifndef MYG4HW_DETECTOR_CONSTRUCTION2_H_
#define MYG4HW_DETECTOR_CONSTRUCTION2_H_

#include "G4VUserDetectorConstruction.hh"

class MyG4HWDetectorConstruction : public G4VUserDetectorConstruction{
public:
  MyG4HWDetectorConstruction();
  virtual ~MyG4HWDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();

  void SetNoVoxelX(G4int val);
  void SetNoVoxelY(G4int val);
  void SetNoVoxelZ(G4int val);
  void SetVoxelX(G4double val);
  void SetVoxelY(G4double val);
  void SetVoxelZ(G4double val);

protected:
  G4int fNVoxelX;
  G4int fNVoxelY;
  G4int fNVoxelZ;
  G4double fVoxelXHalfOfX;
  G4double fVoxelXHalfOfY;
  G4double fVoxelXHalfOfZ;
  std::vector<G4Material*> fMaterials;
};

inline void MyG4HWDetectorConstruction::SetNoVoxelX(G4int val) {fNVoxelX=val;}
inline void MyG4HWDetectorConstruction::SetNoVoxelY(G4int val) {fNVoxelY=val;}
inline void MyG4HWDetectorConstruction::SetNoVoxelZ(G4int val) {fNVoxelZ=val;}
inline void MyG4HWDetectorConstruction::SetVoxelX(G4double val)
{
  fVoxelXHalfOfX=val/2.;
}
inline void MyG4HWDetectorConstruction::SetVoxelY(G4double val)
{
  fVoxelXHalfOfY=val/2.;
}
inline void MyG4HWDetectorConstruction::SetVoxelZ(G4double val)
{
  fVoxelXHalfOfZ=val/2.;
}

#endif
