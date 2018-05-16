#ifndef MyG4HWDetectorConstruction_h
#define MyG4HWDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include <set>

class G4Material;
class G4Box;
class G4LogicalVolume;

class MyG4HWDetectorConstruction : public G4VUserDetectorConstruction{
public:
  MyG4HWDetectorConstruction();
  ~MyG4HWDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();

public:
  void SetNoVoxelX( G4int val );
  void SetNoVoxelY( G4int val );
  void SetNoVoxelZ( G4int val );
  void SetVoxelX( G4double val);
  void SetVoxelY( G4double val);
  void SetVoxelZ( G4double val);

protected:
  void ConstructPhantomContainer();
  void ConstructPhantom();
  void SetScorer( G4LogicalVolume* voxel_logic );
  virtual void ConstructSDandField();

protected:
  G4Material* fAir;
  G4Material* fWater;

  G4Box* fWorld_solid;
  G4LogicalVolume* fWorld_logic;
  G4VPhysicalVolume* fWorld_phys;

  G4Box* fContainer_solid;
  G4LogicalVolume* fContainer_logic;
  G4VPhysicalVolume* fContainer_phys;

  std::vector<G4Material*> fMaterials;
  size_t* fMateIDs;
  G4int fNVoxelX, fNVoxelY, fNVoxelZ;
  G4double fVoxelXHalfOfX, fVoxelXHalfOfY, fVoxelXHalfOfZ;
  std::set<G4LogicalVolume*> fScorers;
};

inline void MyG4HWDetectorConstruction::SetNoVoxelX( G4int val ){fNVoxelX=val;}
inline void MyG4HWDetectorConstruction::SetNoVoxelY( G4int val ){fNVoxelY=val;}
inline void MyG4HWDetectorConstruction::SetNoVoxelZ( G4int val ){fNVoxelZ=val;}
inline void MyG4HWDetectorConstruction::SetVoxelX( G4double val){fVoxelXHalfOfX=val/2.;}
inline void MyG4HWDetectorConstruction::SetVoxelY( G4double val){fVoxelXHalfOfY=val/2.;}
inline void MyG4HWDetectorConstruction::SetVoxelZ( G4double val){fVoxelXHalfOfZ=val/2.;}
#endif
