#include "G4HCofThisEvent.hh"
#include "G4NistManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWSD.hh"

MyG4HWSD::MyG4HWSD(G4String SDname)
  :G4VSensitiveDetector(SDname)
{

}

MyG4HWSD::~MyG4HWSD(){}

void MyG4HWSD::Initialize(G4HCofThisEvent*)
{
  fsum_edep=0.;
  fno_step=0;
  //fVoxelSumDep = new G4double[61*61*150*2];
  //for (G4int i=0; i<(61*61*150*2); ++i) fVoxelSumDep[i]=0.;
  for (G4int i=0; i<61; ++i)
  {
    for (G4int j=0; j<61; ++j)
    {
      for (G4int k=0; k<150; ++k) {
        fVoxelSumDep[i][j][k]=0.;
        fVoxelN[i][j][k]=0;
      }
    }
  }

}

G4bool MyG4HWSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

  double edep =  aStep-> GetTotalEnergyDeposit();
  fsum_edep = fsum_edep + edep;

  G4String particleName = aStep->GetTrack()-> GetDefinition()-> GetParticleName();
  if ( edep == 0 ) return false;
  fno_step++;

  G4StepPoint* preStepPoint   = aStep->GetPreStepPoint();
  G4TouchableHandle touchable = preStepPoint->GetTouchableHandle();
  G4int    copyNo_x   = touchable->GetReplicaNumber(1);
  G4int    copyNo_y   = touchable->GetReplicaNumber(2);
  G4int    copyNo_z   = touchable->GetReplicaNumber(0);
  //G4int idx = copyNo_x + copyNo_y*61 +copyNo_z*61*61;
  //fVoxelSumDep[idx] += edep;
  G4int idx = copyNo_z + copyNo_x*150 + copyNo_y*150*61;
  fVoxelSumDep[copyNo_x][copyNo_y][copyNo_z] += edep;
  fVoxelN[copyNo_x][copyNo_y][copyNo_z] += 1;

  return true;
}


void MyG4HWSD::EndOfEvent(G4HCofThisEvent*)
{
  auto AnaMan = MyG4HWAnalysis::Instance();
  G4int fNVoxelZ=150;
  G4int fNVoxelY=61;
  G4int fNVoxelX=61;
  for( G4int iz=0; iz<fNVoxelZ; iz++){
  for( G4int iy=0; iy<fNVoxelY; iy++){
    for( G4int ix=0; ix<fNVoxelX; ix++){
      //G4int idx_id = ix + iy*fNVoxelX + iz*fNVoxelX*fNVoxelY;
      G4int idx_id = iz + ix*fNVoxelZ + iy*fNVoxelX*fNVoxelZ;
      AnaMan-> SetDosePerVoxel( ix, iy, iz, fVoxelSumDep[ix][iy][iz]/MeV);
      //if(fVoxelSumDep[idx_id]==0.)continue;
      if(fVoxelSumDep[ix][iy][iz]==0.)continue;
      AnaMan-> FillNtuple(ix*(5*mm)/cm, iy*(5*mm)/cm, iz*(2*mm)/cm,
                          ix, iy, iz,fVoxelSumDep[ix][iy][iz]/MeV);
      //                    ix, iy, iz, fVoxelSumDep[idx_id]/MeV);
      //if (ix==30&&iy==30) AnaMan-> Fill1DHist(0, iz*(2*mm)/cm, fVoxelSumDep[idx_id]/MeV);
      if (ix==30&&iy==30) AnaMan-> Fill1DHist(0, iz*(2*mm)/cm, fVoxelSumDep[ix][iy][iz]/MeV);
      if (iz==15&&iy==30) AnaMan-> Fill1DHist(2, (ix-30)*(5*mm)/cm, fVoxelSumDep[ix][iy][iz]/MeV);

    }
  }
}
}
