#include "G4UnitsTable.hh"
#include <CLHEP/Units/SystemOfUnits.h>
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
#include "MyG4HWAnalysis.hh"

MyG4HWAnalysis* MyG4HWAnalysis::fInstance=nullptr;

MyG4HWAnalysis* MyG4HWAnalysis::Instance()
{
  if (!fInstance) fInstance = new MyG4HWAnalysis;
  return fInstance;
}

MyG4HWAnalysis::MyG4HWAnalysis()
  : fRootFile{nullptr},
    fNtuple{nullptr},
    fPos_x(0.),
    fPos_y(0.),
    fPos_z(0.),
    fVoxID_X(0),
    fVoxID_Y(0),
    fVoxID_Z(0),
    fEdep(0.)
{
   for(G4int i=0; i<kMaxHist; ++i) fHist[i] = nullptr;
   fInstance = this;
}

MyG4HWAnalysis::~MyG4HWAnalysis()
{
  if (fRootFile) delete fRootFile;
  fInstance = nullptr;
}

void MyG4HWAnalysis::BookTreeAndHist()
{
  G4String fileName = "MyG4HWDepInfo.electron";
  //G4String fileName = "MyG4HWDepInfo.gamma";
  if ( fSeedNum > 0 ) {
    fileName += ".root.";
    fileName += std::to_string( fSeedNum );
  } else {
    fileName += ".root";
  }
  std::cout << "ana outname: " << fileName << std::endl;

  fRootFile = new TFile(fileName, "RECREATE");
  if (!fRootFile) {
    G4cout << "can not create output root file" << G4endl;
    return;
  }

  fNtuple = new TTree("MyG4HW","MyG4HW");
  fNtuple->Branch("Pos_x",    &fPos_x,   "Pos_x/D");
  fNtuple->Branch("Pos_y",    &fPos_y,   "Pos_y/D");
  fNtuple->Branch("Pos_z",    &fPos_z,   "Pos_z/D");
  fNtuple->Branch("Vox_ID_X", &fVoxID_X, "Vox_ID_X/I");
  fNtuple->Branch("Vox_ID_Y", &fVoxID_Y, "Vox_ID_Y/I");
  fNtuple->Branch("Vox_ID_Z", &fVoxID_Z, "Vox_ID_Z/I");
  fNtuple->Branch("Edep",     &fEdep,    "Edep/D");
  fHist[0] = new TH1D("PositionZ", "PostitionZ", 150, 0., 30*CLHEP::cm/CLHEP::cm);
  fHist[1] = new TH1D("BEnergy", "BEnergy", 240, 0., 6*CLHEP::MeV);
  fHist[2] = new TH1D("PositionX", "PostitionX", 150, -15.*CLHEP::cm/CLHEP::cm,
                      15.*CLHEP::cm/CLHEP::cm);

  for (G4int i=0; i<61; ++i)
  {
    for (G4int j=0; j<61; ++j)
    {
      for (G4int k=0; k<150; ++k)
      {
        fVoxelSumDept[i][j][k]=0.;
      }
    }

  }

}

void MyG4HWAnalysis:: SetDosePerVoxel( int ix, int iy, int iz, double edep)
{
  fVoxelSumDept[ix][iy][iz] += edep;
}

double MyG4HWAnalysis:: GetDosePerVoxel(int ix, int iy, int iz)
{
  return fVoxelSumDept[ix][iy][iz];
}

void MyG4HWAnalysis::SaveFile()
{
  if (!fRootFile) return;
  fRootFile->Write();
  fRootFile->Close();
  G4cout << "\n Histograms and ntuples are save.\n" << G4endl;
}

void MyG4HWAnalysis::Fill1DHist(G4int idx, G4double ibin_x, G4double iwet)
{
  if (idx >= kMaxHist) {
    G4cout << "The histogram is not created yet." << G4endl;
    return;
  }
  if (fHist[idx]) fHist[idx]->Fill(ibin_x, iwet);
}

void MyG4HWAnalysis::FillNtuple(
                                G4double pos_x, G4double pos_y, G4double pos_z,
                                G4int vox_id_x, G4int vox_id_y, G4int vox_id_z,
                                G4double edep)
{
  fPos_x   = pos_x;
  fPos_y   = pos_y;
  fPos_z   = pos_z;
  fVoxID_X = vox_id_x;
  fVoxID_Y = vox_id_y;
  fVoxID_Z = vox_id_z;
  fEdep    = edep;

  fNtuple->Fill();
}
