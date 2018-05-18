#include "G4HCofThisEvent.hh"
#include "G4NistManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include <math.h>
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
  fdepth_z=0.;
}

G4bool MyG4HWSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

  double edep =  aStep-> GetTotalEnergyDeposit();
  fsum_edep = fsum_edep + edep;

  double VoxelX=5.;//unit cm
  double VoxelY=5.;
  double VoxelZ=2.;

  G4NistManager* nist = G4NistManager::Instance();
  G4Material* Water = nist->FindOrBuildMaterial( "G4_WATER" );
  G4double density = Water->GetDensity();
  //G4cout<<" ............... water density: "<<density<<"/"<<density*cm3/g<<G4endl;
  G4double emass= VoxelX * VoxelY * VoxelZ * density;
  //G4cout<<".....mass "<<emass/g<<"/"<<emass/kg<<G4endl;

  emass = emass/kg;
  G4double edose = edep/emass; //unit: MeV/kg
  std::cout << "........" <<edep << "/" <<emass << "/" << edose << G4endl;
  edose = edose/(6.2415*pow(10,12));//unit:Gy

  G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  if (edep==0) return false;
  fno_step = fno_step+1;

  G4cout << "each track particle " << fno_step << " particle: "
         << particleName <<G4endl;

  G4StepPoint* preStepPoint   = aStep->GetPreStepPoint();
  G4TouchableHandle touchable = preStepPoint->GetTouchableHandle();

  G4ThreeVector pos_world = preStepPoint->GetPosition();
  G4ThreeVector pos_local
      = touchable->GetHistory()->GetTopTransform().TransformPoint(pos_world);

  G4String sdName = touchable->GetVolume()->GetName();
  G4int  copyNo   = touchable->GetCopyNumber();

  G4cout << "SD: " << sdName << " copyNo: " << copyNo << " edep: " << edep
         << " edose: "<< edose << " sum_edep: " << fsum_edep << G4endl;

  G4cout << " world(x,y,z)" << pos_world.x() << ", " << pos_world.y()
         <<", "<<pos_world.z()<<G4endl;

  G4cout << " local(x,y,z)" << pos_local.x() << ", " << pos_local.y()
         <<", "<<pos_local.z()<<G4endl;

  fdepth_z = pos_world.z()/cm;

  auto fAnaMan = MyG4HWAnalysis::Instance();
  fAnaMan->FillNtuple(pos_world.x()/cm, pos_world.y()/cm,
                      pos_world.z()/cm, edep);
  fAnaMan->Fill1DHist(0, pos_world.z(), edep);

  return true;
}


void MyG4HWSD::EndOfEvent(G4HCofThisEvent*)
{
  //G4AnalysisManager* anaMag = G4AnalysisManager::Instance();
  G4cout << "---EndOfEvent sum_edep= " << fsum_edep << "/" << fdepth_z << G4endl;
  //anaMag->FillH1(0, fdepth_z, fsum_edep);
}
