#include "G4Run.hh"
#include "G4RunManager.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWRunAction.hh"
#include "G4UnitsTable.hh"

MyG4HWRunAction::MyG4HWRunAction()
  :G4UserRunAction()
{
}

MyG4HWRunAction::~MyG4HWRunAction()
{
}

void MyG4HWRunAction::BeginOfRunAction(const G4Run* aRun)
{
  std::cout << "### RunID: "<< aRun->GetRunID() << " start." << std::endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  int ntot = aRun-> GetNumberOfEventToBeProcessed();
  auto AnaMan = MyG4HWAnalysis::Instance();
  AnaMan-> BookTreeAndHist();
  AnaMan-> SetTotNum( ntot );
  if (AnaMan-> GetVerbose()) {
    std::ofstream ofs;
    ofs.open (AnaMan->MakeDetailTrackINFO(), std::ios::app);
    ofs << "RunID " << aRun->GetRunID() << std::endl;
    ofs.close();
  }
  mtime.Start();
}

void MyG4HWRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int nofEvts = aRun->GetNumberOfEvent();
  mtime.Stop();
  std::cout << "EPS: " << G4BestUnit(mtime.GetSystemElapsed()/nofEvts, "Time") << std::endl;
  if (nofEvts == 0) return;

  auto AnaMan = MyG4HWAnalysis::Instance();
  int totev = AnaMan-> GetTotNum();
  std::cout << "tot: " << nofEvts << "/" << totev << std::endl;
  G4String fileName = "ResultDep_" + AnaMan->GetOutPutPrefix() + ".";
  if (AnaMan-> GetSeedNum() > 0) {
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
  std::cout << "outputdosefile: " << fileName << std::endl;

  std::ofstream  file(fileName);
  file << "### Total number of events processed" << std::endl;
  file << totev << std::endl;
  file << "### Voxel dimension (x, y, z)" << std::endl;
  file << fNVoxelX << " " << fNVoxelY << " " << fNVoxelZ << std::endl;
  file << "### Total voxle number" << std::endl;
  file << fNVoxelX*fNVoxelY*fNVoxelZ << std::endl;
  file << "### Voxle size (mm)" << std::endl;
  float vox_x = fVoxelXX;
  float vox_y = fVoxelXY;
  float vox_z = fVoxelXZ;
  file << vox_x << " " << vox_y << " " << vox_z << std::endl;
  file << "### Voxel lower limit(mm)" << std::endl;
  float vox_dx = -0.5 * fVoxelXX * fNVoxelX;
  float vox_dy = -0.5 * fVoxelXY * fNVoxelY;
  float vox_dz = 0.*CLHEP::mm;
  file << vox_dx << " " << vox_dy << " " << vox_dz << std::endl;
  float vox_ux = 0.5 * fVoxelXX * fNVoxelX;
  float vox_uy = 0.5 * fVoxelXY * fNVoxelY;
  float vox_uz = fVoxelXZ * fNVoxelZ;
  file << "### Voxel upper limit(mm)" << std::endl;
  file << vox_ux << " " << vox_uy << " " << vox_uz << std::endl;
  file << "### Voxel index (x, y, z), Density scale, Energy deposit (MeV)" << std::endl;
  for (int iz = 0; iz < fNVoxelZ; iz++) {
    for (int  iy = 0; iy < fNVoxelY; iy++) {
      for (int  ix = 0; ix < fNVoxelX; ix++) {
        float idensity =
          AnaMan-> GetDensityPerVoxel(ix, iy, iz)/(CLHEP::g/CLHEP::cm3);

          file << ix << " "<<iy<<" "<<iz<<" "<<idensity
               <<" "<< AnaMan-> GetDosePerVoxel(ix, iy, iz)/CLHEP::MeV
               << std::endl;
      }
    }
  }

  file.close();
  AnaMan->SaveFile();
}
