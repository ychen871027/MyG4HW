#include "G4Run.hh"
#include "G4RunManager.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWRunAction.hh"

MyG4HWRunAction::MyG4HWRunAction()
  :G4UserRunAction()
{
}

MyG4HWRunAction::~MyG4HWRunAction()
{
}

void MyG4HWRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### RunID: "<< aRun->GetRunID() << " start." << G4endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  int ntot = aRun-> GetNumberOfEventToBeProcessed();
  auto AnaMan = MyG4HWAnalysis::Instance();
  AnaMan-> BookTreeAndHist();
  AnaMan-> SetTotNum( ntot );
}

void MyG4HWRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int nofEvts = aRun->GetNumberOfEvent();
  if (nofEvts == 0) return;

  auto AnaMan = MyG4HWAnalysis::Instance();
  for (int ix=0; ix < 61; ix++ )
  {
    for (int iy=0; iy < 61; iy++ ){
      for (int iz=0; iz < 150; iz++ ){
        if (AnaMan->GetDosePerVoxel(ix, iy, iz)>0)std::cout << "X:Y:Z:dep:: "
          <<ix<<"-"<<iy<<"-"<<iz<<"-"<< AnaMan->GetDosePerVoxel(ix, iy, iz) << std::endl;
      }
    }
  }
  AnaMan->SaveFile();

  int totev = AnaMan-> GetTotNum();
  G4String fileName = "result_dep";
  fileName = fileName + AnaMan-> GetBeamType() + AnaMan-> GetMatType() + ".";
  if ( AnaMan-> GetSeedNum() > 0 ) {
    fileName += "dat.";
    fileName += std::to_string( AnaMan-> GetSeedNum() );
  } else {
    fileName += "dat";
  }
  int fNVoxelX = AnaMan-> GetNoVoxelX();
  int fNVoxelY = AnaMan-> GetNoVoxelY();
  int fNVoxelZ = AnaMan-> GetNoVoxelZ();

  double fVoxelXX = AnaMan-> GetVoxelX();
  double fVoxelXY = AnaMan-> GetVoxelY();
  double fVoxelXZ = AnaMan-> GetVoxelZ();

  G4String rootout = getenv("ROOTOUT");
  fileName =  rootout + "/" + fileName;
  std::cout << __LINE__ << "outputrootfile: " << fileName << std::endl;

  std::ofstream  file(fileName);
  file << "### Total number of events processed" << G4endl;
  file << totev << G4endl;
  file << "### Voxel dimension (x, y, z)" << G4endl;
  file << fNVoxelX << " " << fNVoxelY << " " << fNVoxelZ << G4endl;
  file << "### Total voxle number" << G4endl;
  file << fNVoxelX*fNVoxelY*fNVoxelZ << G4endl;
  file << "### Voxle size (mm)" << G4endl;
  float vox_x = fVoxelXX;
  float vox_y = fVoxelXY;
  float vox_z = fVoxelXZ;
  file << vox_x << " " << vox_y << " " << vox_z << G4endl;
  file << "### Voxel lower limit(mm)" << G4endl;
  float vox_dx = -0.5 * fVoxelXX * fNVoxelX;
  float vox_dy = -0.5 * fVoxelXY * fNVoxelY;
  float vox_dz = 0.*CLHEP::mm;
  file << vox_dx << " " << vox_dy << " " << vox_dz << G4endl;
  float vox_ux = 0.5 * fVoxelXX * fNVoxelX;
  float vox_uy = 0.5 * fVoxelXY * fNVoxelY;
  float vox_uz = fVoxelXZ * fNVoxelZ;
  file << "### Voxel upper limit(mm)" << G4endl;
  file << vox_ux << " " << vox_uy << " " << vox_uz << G4endl;
  file << "### Voxel index (x, y, z), Density scale, Energy deposit (MeV)" << G4endl;
  for (int iz = 0; iz < fNVoxelZ; iz++){
    for (int  iy = 0; iy < fNVoxelY; iy++){
      for (int  ix = 0; ix < fNVoxelX; ix++){
        float idensity=1.0;
        file << ix << " "<<iy<<" "<<iz<<" "<<idensity
             <<" "<< AnaMan-> GetDosePerVoxel(ix, iy, iz)/CLHEP::MeV << G4endl;
      }
    }
  }
  file.close();

}
