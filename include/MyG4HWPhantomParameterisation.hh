#ifndef MYG4HW_PHANTOM_PARAMETERISATION_H_
#define MYG4HW_PHANTOM_PARAMETERISATION_H_

#include <vector>
#include "G4ThreeVector.hh"
#include "G4VNestedParameterisation.hh"


class MyG4HWPhantomParameterisation : public G4VNestedParameterisation {
public:
  MyG4HWPhantomParameterisation(const G4ThreeVector& voxelSize,
                                std::vector<G4Material*>& mat);
  virtual ~MyG4HWPhantomParameterisation();

  virtual G4Material* ComputeMaterial(G4VPhysicalVolume* , const G4int iz,
                              const G4VTouchable* parentTouch);

  void SetNoVoxel(G4int nx, G4int ny, G4int nz);
  G4int GetMaterialIndex( G4int copyNo )const;
  virtual G4int GetNumberOfMaterials()const;
  virtual G4Material* GetMaterial(G4int i)const;
  virtual void ComputeTransformation(const G4int copyNo,
                             G4VPhysicalVolume* physVol ) const;
  //virtual void ComputeDimensions(G4Box& box, const G4int,
  //                       const G4VPhysicalVolume*) const;

//private:  // Dummy declarations to get rid of warnings ...
/*
virtual void ComputeDimensions (G4Trd&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Trap&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Cons&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Sphere&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Ellipsoid&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Orb&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Torus&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Para&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Hype&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Tubs&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Polycone&, const G4int,
                        const G4VPhysicalVolume*) const {}
virtual void ComputeDimensions (G4Polyhedra&, const G4int,
                        const G4VPhysicalVolume*) const {}
*/

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
