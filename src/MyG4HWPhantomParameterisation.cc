#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4Box.hh"
#include "MyG4HWPhantomParameterisation.hh"

MyG4HWPhantomParameterisation::MyG4HWPhantomParameterisation(
                               const G4ThreeVector& voxelSize,
                                std::vector<G4Material*>& mat)
  :fdX(voxelSize.x()), fdY(voxelSize.y()), fdZ(voxelSize.z()),
   fMaterials(mat)
{
}

MyG4HWPhantomParameterisation::~MyG4HWPhantomParameterisation()
{
}

void MyG4HWPhantomParameterisation::SetNoVoxel( G4int nx,
                                                G4int ny,
                                                G4int nz)
{
  fnX = nx;
  fnY = ny;
  fnZ = nz;
}

G4Material* MyG4HWPhantomParameterisation::ComputeMaterial(
                                G4VPhysicalVolume*, G4int,
                                const G4VTouchable* parentTouch)
{
  if (parentTouch==0) return fMaterials[0];

  G4Material* mate = 0;
  mate = fMaterials[0];

  return mate;
}

G4int MyG4HWPhantomParameterisation::GetNumberOfMaterials()const
{
  return fMaterials.size();
}

G4Material* MyG4HWPhantomParameterisation::GetMaterial(G4int i)const
{
  return fMaterials[i];
}

void MyG4HWPhantomParameterisation::ComputeTransformation(
                                       const G4int copyNo,
                                       G4VPhysicalVolume* physVol) const
{
  physVol->SetTranslation(G4ThreeVector(0.,
                                         0.,
                            (2.*static_cast<double>(copyNo)+1.)*fdZ-fdZ*fnZ));
}

void MyG4HWPhantomParameterisation::ComputeDimensions(
                                            G4Box& box,
                                            const G4int,
                                            const G4VPhysicalVolume*)const
{
  box.SetXHalfLength(fdX);
  box.SetYHalfLength(fdY);
  box.SetZHalfLength(fdZ);
}
