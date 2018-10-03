#include "G4UnitsTable.hh"
#include <CLHEP/Units/SystemOfUnits.h>
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
#include "MyG4HWAnalysis.hh"
#include <fstream>

MyG4HWAnalysis* MyG4HWAnalysis::fInstance=nullptr;

MyG4HWAnalysis* MyG4HWAnalysis::Instance()
{
  if (!fInstance) fInstance = new MyG4HWAnalysis;
  return fInstance;
}

MyG4HWAnalysis::MyG4HWAnalysis()
  : fRootFile{nullptr},
    fNtuple{nullptr}
{
   for(G4int i=0; i<kMaxHist; ++i) fHist[i] = nullptr;
   fInstance = this;
}

MyG4HWAnalysis::~MyG4HWAnalysis()
{
  if (fRootFile) delete fRootFile;
  fInstance = nullptr;
  if (fNtuple) delete fNtuple;
}

void MyG4HWAnalysis::SetConfVoxel(G4String voxelconfig)
{
  std::ifstream fin(voxelconfig);
  fin >> fPLname;
  std::cout << " physics list name: " << fPLname << std::endl;
  fin >> fVoxMatType;
  std::cout << " voxel material type: " << fVoxMatType << std::endl;
  fin >> fNvX >> fNvY >> fNvZ;
  std::cout << "voxelNum(x,y,z): " << fNvX << "/" << fNvY << "/" << fNvZ << std::endl;
  fin >> fvX >> fvY >> fvZ;
  std::cout << "voxelsize(x,y,z)[mm]: " << fvX << "/" << fvY << "/" << fvZ << std::endl;
  fin >> fbeam;
  std::cout << "beamtype: " << fbeam << std::endl;
  fin >> fmat;
  std::cout << "MatType: " << fmat << std::endl;
  fin >> fstepflag;
  std::cout << "StepFlag: " << fstepflag << std::endl;
  fin >> fcutvalue;
  std::cout << "CutValue: " << fcutvalue << std::endl;
  fin >> fmscstepalg;
  std::cout << "MSCStepAlg: " << fmscstepalg << std::endl;
  fin >> fbeamname;
  std::cout << "BeamName: " << fbeamname << std::endl;
  fin >> fxrayE;
  std::cout << "Xray Energy: " << fxrayE << std::endl;
  fin >> fMidMat;
  std::cout << "Mid material: " << fMidMat << std::endl;
  if (GetExDensity()) {
    fin >> fdensitylist;
    std::cout << "density list name: " << fdensitylist << std::endl;
  }
}
void MyG4HWAnalysis::BookTreeAndHist()
{

  fHist[0] = new TH1D("PositionZ", "PostitionZ", 150, 0., 30*CLHEP::cm/CLHEP::cm);
  if( strcmp(GetXrayEnergy(), "6MV") == 0 ){
    fHist[1] = new TH1D("BEnergy", "BEnergy", 240, 0., 6*CLHEP::MeV);
  }else if( strcmp(GetXrayEnergy(), "6MV") == 0 ){
    fHist[1] = new TH1D("BEnergy", "BEnergy", 720, 0., 18*CLHEP::MeV);
  }else{
    fHist[1] = new TH1D("BEnergy", "BEnergy", 4000, 0., 100*CLHEP::MeV);
  }

  fHist[2] = new TH1D("PositionX", "PostitionX", 150, -15.*CLHEP::cm/CLHEP::cm,
                      15.*CLHEP::cm/CLHEP::cm);
  fHist[3] = new TH1D("sxdep", "sxdep", 600, -300.*CLHEP::mm/CLHEP::mm, 300.*CLHEP::mm/CLHEP::mm);
  fHist[4] = new TH1D("sydep", "sydep", 600, -300.*CLHEP::mm/CLHEP::mm, 300.*CLHEP::mm/CLHEP::mm);
  fHist[5] = new TH1D("szdep", "szdep", 1500, 0., 1500.*CLHEP::mm/CLHEP::mm);
  fHist[6] = new TH1D("szdepe", "szdepe", 500, 0., 5.*CLHEP::MeV/CLHEP::MeV);
  fHist[7] = new TH1D("trklength", "trklength", 1500, 0., 1500.*CLHEP::mm/CLHEP::mm);
  fHist[8] = new TH1D("trkNum", "trkNum", 100, 0., 100.);
  fHist[9] = new TH1D("salldepe", "salldepe", 500, 0., 5.*CLHEP::MeV/CLHEP::MeV);
  fHist[10] = new TH1D("dedxE", "dedxE", 50, 0., 5.*CLHEP::MeV/CLHEP::MeV);

  fVoxelSumDept  = new G4double[fNvX * fNvY * fNvZ];
  fdensityPervox = new G4double[fNvX * fNvY * fNvZ];
  for (G4int iz=0; iz<fNvZ; ++iz)
  {
    for (G4int iy=0; iy<fNvY; ++iy)
    {
      for (G4int ix=0; ix<fNvX; ++ix)
      {
      	int idx_id =  ix + iy*fNvX + iz*fNvX*fNvY;
        fVoxelSumDept[idx_id]=0.;
        fdensityPervox[idx_id]=0.;
      }
    }
  }
  if (fverbose) {
    std::ofstream ofs;
    ofilenamefordetail = G4String(getenv("ROOTOUT")) + "/" + "DetailedTrackINFO_"
                         + outputprefix + ".dat";
    ofs.open (ofilenamefordetail);
    ofs << ".....Starting to record all track info...." << std::endl;
    ofs.close();
  }
}

void MyG4HWAnalysis::SetDensityPerVoxel( int ix, int iy, int iz, double density)
{
  int idx_id =  ix + iy*fNvX + iz*fNvX*fNvY;
  fdensityPervox[idx_id] = density;
}

double MyG4HWAnalysis::GetDensityPerVoxel( int ix, int iy, int iz)
{
  int idx_id =  ix + iy*fNvX + iz*fNvX*fNvY;
  return fdensityPervox[idx_id];
}


void MyG4HWAnalysis::SetDosePerVoxel( int ix, int iy, int iz, double edep)
{
  int idx_id =  ix + iy*fNvX + iz*fNvX*fNvY;
  fVoxelSumDept[idx_id] += edep;
}

double MyG4HWAnalysis::GetDosePerVoxel(int ix, int iy, int iz)
{
  int idx_id =  ix + iy*fNvX + iz*fNvX*fNvY;
  return fVoxelSumDept[idx_id];
}

void MyG4HWAnalysis::SaveFile()
{
  G4String fileName = "MyG4HWHistogram.";
  fileName = fileName + outputprefix + ".";
  //fileName = fileName + GetBeamType() + GetMatType() + ".";
  if ( fSeedNum > 0 ) {
    fileName += "root.";
    fileName += std::to_string( fSeedNum );
  } else {
    fileName += "root";
  }

  G4String rootout = getenv("ROOTOUT");
  fileName =  rootout + "/" + fileName;
  std::cout << ">>> Outputrootfile <<<\n";
  std::cout << fileName << std::endl;
  fRootFile = new TFile(fileName, "RECREATE");
  if (!fRootFile) {
    G4cout << "can not create output root file" << G4endl;
    return;
  }

  for (int ih=0; ih<kMaxHist; ih++ ) {
    if (fHist[ih]==nullptr) continue;
    fHist[ih]->Write("",TObject::kOverwrite);
  }

  fRootFile->Write();
  fRootFile->Close();
  delete[] fVoxelSumDept;
  delete[] fdensityPervox;
}

void MyG4HWAnalysis::Fill1DHist(G4int idx, G4double ibin_x, G4double iwet)
{
  if (idx >= kMaxHist) {
    G4cout << "The histogram is not created yet." << G4endl;
    return;
  }
  if (fHist[idx]) fHist[idx]->Fill(ibin_x, iwet);
}
