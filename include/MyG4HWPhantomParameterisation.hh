#ifndef MYG4HW_PHANTOM_PARAMETERISATION_H_
#define MYG4HW_PHANTOM_PARAMETERISATION_H_

#include "G4ThreeVector.hh"
#include "G4VNestedParameterisation.hh"
//class G4VisAttributes;
//class G4VPhysicalVolume;
//class G4VTouchable;

class MyG4HWPhantomParameterisation : public G4VNestedParameterisation
{
public:
  MyG4HWPhantomParameterisation(const G4ThreeVector& voxelSize, std::vector<G4Material*>& mat);
  virtual ~MyG4HWPhantomParameterisation();

  virtual G4Material* ComputeMaterial(G4VPhysicalVolume* , const G4int ,
                                      const G4VTouchable* );

  void SetNoVoxel(G4int nx, G4int ny, G4int nz);
  virtual G4int GetNumberOfMaterials()const;
  virtual G4Material* GetMaterial(G4int i)const;
  virtual void ComputeTransformation(const G4int copyNo,
                             G4VPhysicalVolume* physVol ) const;

using G4VNestedParameterisation::ComputeMaterial;

private:
  G4int fnX;
  G4int fnY;
  G4int fnZ;
  G4double fdX;
  G4double fdY;
  G4double fdZ;
  std::vector<G4Material*> fMaterials;

};

#endif
