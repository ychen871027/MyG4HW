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
  fno_trk=0;
  fno_trkE=0.;
  fno_stepn=0;
  fno_stepL=0.;
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
  //if ( edep == 0 ) return false;
  fno_step++;

  G4StepPoint* preStepPoint   = aStep->GetPreStepPoint();
  G4TouchableHandle touchable = preStepPoint->GetTouchableHandle();
  G4int    copyNo_x   = touchable->GetReplicaNumber(1);
  G4int    copyNo_y   = touchable->GetReplicaNumber(2);
  G4int    copyNo_z   = touchable->GetReplicaNumber(0);
  //G4int idx = copyNo_x + copyNo_y*61 +copyNo_z*61*61;
  //fVoxelSumDep[idx] += edep;

  //G4int idx = copyNo_z + copyNo_x*150 + copyNo_y*150*61;
  fVoxelSumDep[copyNo_x][copyNo_y][copyNo_z] += edep;
  fVoxelN[copyNo_x][copyNo_y][copyNo_z] += 1;

  G4ThreeVector pos_world = preStepPoint->GetPosition();
  //G4cout << " world(x,y,z)" << pos_world.x() << ", " << pos_world.y()
  //       <<", "<<pos_world.z()<< G4endl;
  auto AnaMan = MyG4HWAnalysis::Instance();
  if ( fabs(pos_world.z()) < 0.0001*mm && aStep->GetTrack()->GetParentID() == 0)
  {
    // std::cout << "testcheny:  " << pos_world.x() << "/"
    //           << pos_world.y() << "/" << pos_world.z() << std::endl;
    AnaMan-> Fill1DHist(3, pos_world.x(), 1.);
    AnaMan-> Fill1DHist(4, pos_world.y(), 1.);

  }
  if ( pos_world.z() < 0.0001*mm && pos_world.z() > -100*cm ){
    AnaMan-> Fill1DHist(5, pos_world.z(), 1.);
    if(aStep->GetTrack()->GetParentID() != 0)
    {
      fno_trk++;
      fno_trkE += edep;
    }
    if(aStep->GetTrack()->GetParentID() == 0)
      fno_stepn++;
      fno_stepL += aStep->GetStepLength();
  }
   AnaMan-> FillNtuple( pos_world.x(),pos_world.y(),pos_world.z(),
                       copyNo_x, copyNo_y, copyNo_z, edep,
                       aStep->GetTrack()->GetParentID(), aStep->GetTrack()->GetTrackID());

  return true;
}


void MyG4HWSD::EndOfEvent(G4HCofThisEvent*)
{
  auto AnaMan = MyG4HWAnalysis::Instance();
  AnaMan-> Fill1DHist(8, fno_trk, 1.);
  AnaMan-> Fill1DHist(6, fno_trkE, 1.);
  AnaMan-> Fill1DHist(7, fno_stepL/fno_stepn, 1.);
  G4int fNVoxelZ=150;
  G4int fNVoxelY=61;
  G4int fNVoxelX=61;
  for( G4int iz=0; iz<fNVoxelZ; iz++){
  for( G4int iy=0; iy<fNVoxelY; iy++){
    for( G4int ix=0; ix<fNVoxelX; ix++){
      //G4int idx_id = ix + iy*fNVoxelX + iz*fNVoxelX*fNVoxelY;
      //G4int idx_id = iz + ix*fNVoxelZ + iy*fNVoxelX*fNVoxelZ;
      AnaMan-> SetDosePerVoxel( ix, iy, iz, fVoxelSumDep[ix][iy][iz]/MeV);
      //if(fVoxelSumDep[idx_id]==0.)continue;
      if(fVoxelSumDep[ix][iy][iz]==0.)continue;
      // AnaMan-> FillNtuple(ix*(5*mm)/cm, iy*(5*mm)/cm, iz*(2*mm)/cm,
      //                     ix, iy, iz,fVoxelSumDep[ix][iy][iz]/MeV);
      //                    ix, iy, iz, fVoxelSumDep[idx_id]/MeV);
      //if (ix==30&&iy==30) AnaMan-> Fill1DHist(0, iz*(2*mm)/cm, fVoxelSumDep[idx_id]/MeV);
      if (ix==30&&iy==30) AnaMan-> Fill1DHist(0, iz*(2*mm)/cm, fVoxelSumDep[ix][iy][iz]/MeV);
      if (iz==15&&iy==30) AnaMan-> Fill1DHist(2, (ix-30)*(5*mm)/cm, fVoxelSumDep[ix][iy][iz]/MeV);

    }
  }
}
}
