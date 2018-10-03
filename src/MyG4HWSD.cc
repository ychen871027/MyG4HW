#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWSD.hh"

MyG4HWSD::MyG4HWSD(G4String SDname)
  :G4VSensitiveDetector(SDname)
{
}

MyG4HWSD::~MyG4HWSD(){}

void MyG4HWSD::Initialize(G4HCofThisEvent*)
{
}

G4bool MyG4HWSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

  double edep = aStep-> GetTotalEnergyDeposit();
  if ( edep == 0 ) return false;

  G4StepPoint* preStepPoint   = aStep->GetPreStepPoint();
  G4TouchableHandle touchable = preStepPoint->GetTouchableHandle();
  G4int copyNo_x   = touchable->GetReplicaNumber(1);
  G4int copyNo_y   = touchable->GetReplicaNumber(2);
  G4int copyNo_z   = touchable->GetReplicaNumber(0);
  G4double voxel_density = touchable ->GetVolume() ->GetLogicalVolume()
                                     ->GetMaterial() ->GetDensity();
  auto AnaMan = MyG4HWAnalysis::Instance();
  AnaMan-> SetDosePerVoxel(copyNo_x, copyNo_y, copyNo_z, edep/CLHEP::MeV);
  AnaMan-> SetDensityPerVoxel(copyNo_x, copyNo_y, copyNo_z, voxel_density/(CLHEP::g/CLHEP::cm3));
  
  return true;
}

void MyG4HWSD::EndOfEvent(G4HCofThisEvent*)
{
}
