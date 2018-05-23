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
}

G4bool MyG4HWSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

  double edep =  aStep-> GetTotalEnergyDeposit();
  fsum_edep = fsum_edep + edep;

  G4String particleName = aStep->GetTrack()-> GetDefinition()-> GetParticleName();
  if ( edep == 0 ) return false;
  fno_step++;

  //G4cout << "Each track particle " << fno_step << " particle: "
  //       << particleName <<G4endl;

  G4StepPoint* preStepPoint   = aStep->GetPreStepPoint();
  G4TouchableHandle touchable = preStepPoint->GetTouchableHandle();

  G4ThreeVector pos_world = preStepPoint->GetPosition();

  G4String sdName     = touchable->GetVolume()-> GetName();
  G4int    copyNo_x   = touchable->GetReplicaNumber(1);
  G4int    copyNo_y   = touchable->GetReplicaNumber(2);
  G4int    copyNo_z   = touchable->GetReplicaNumber(0);

  //G4cout << "SD: " << sdName << " edep: " << edep << G4endl;

  //G4cout << " world(x,y,z)" << pos_world.x()/cm << ", "
  //       << pos_world.y()/cm << ", " << pos_world.z()/cm
  //       << " copyNo(z,x,y) " << touchable->GetReplicaNumber(0)
  //       << "/" << touchable->GetReplicaNumber(1)
  //       << "/" << touchable->GetReplicaNumber(2) << G4endl;


  auto AnaMan = MyG4HWAnalysis::Instance();
  AnaMan-> FillNtuple(pos_world.x()/cm, pos_world.y()/cm, pos_world.z()/cm,
                      copyNo_x, copyNo_y, copyNo_z, edep/MeV);
  AnaMan-> Fill1DHist(0, pos_world.z()/cm, edep/MeV);

  return true;
}


void MyG4HWSD::EndOfEvent(G4HCofThisEvent*)
{
}
