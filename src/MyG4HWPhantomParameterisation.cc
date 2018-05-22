#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "MyG4HWPhantomParameterisation.hh"

namespace {
  G4Material* water {nullptr};
}

MyG4HWPhantomParameterisation::MyG4HWPhantomParameterisation(
                               const G4ThreeVector& voxelSize
                               )
  :fdX(voxelSize.x()), fdY(voxelSize.y()), fdZ(voxelSize.z())
{
  G4NistManager* nist = G4NistManager::Instance();
  ::water = nist->FindOrBuildMaterial( "G4_WATER" );
}

MyG4HWPhantomParameterisation::~MyG4HWPhantomParameterisation()
{
}

void MyG4HWPhantomParameterisation::SetNoVoxel(
    G4int nx, G4int ny, G4int nz)
{
  fnX = nx;
  fnY = ny;
  fnZ = nz;
}

G4Material* MyG4HWPhantomParameterisation::ComputeMaterial(
    G4VPhysicalVolume*, const G4int, const G4VTouchable*)
{
  return ::water;
}

G4int MyG4HWPhantomParameterisation::GetNumberOfMaterials() const
{
  return 1;
}

G4Material* MyG4HWPhantomParameterisation::GetMaterial( G4int  ) const
{
  return ::water;
}

void MyG4HWPhantomParameterisation::ComputeTransformation(
    const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  physVol->SetTranslation(
      G4ThreeVector(0., 0., (2.*copyNo + 1.) * fdZ - fdZ * fnZ));
}
