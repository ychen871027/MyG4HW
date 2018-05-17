#include "MyG4HWSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "MyG4HWAnalysis.hh"
#include "G4NistManager.hh"
#include <math.h>
using CLHEP::cm;
using CLHEP::cm3;
using CLHEP::kg;
using CLHEP::g;

MyG4HWSD::MyG4HWSD(G4String SDname):G4VSensitiveDetector(SDname){}

MyG4HWSD::~MyG4HWSD(){}

void MyG4HWSD::Initialize(G4HCofThisEvent*){
  sum_edep=0.;
  no_step=0;
}

G4bool MyG4HWSD::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  G4AnalysisManager* anaMag = G4AnalysisManager::Instance();

  G4double edep =  aStep->GetTotalEnergyDeposit();
  sum_edep = sum_edep + edep;

  G4double VoxelX=5.;//unit cm
  G4double VoxelY=5.;
  G4double VoxelZ=2.;

  G4NistManager* nist = G4NistManager::Instance();
  G4Material* Water = nist->FindOrBuildMaterial( "G4_WATER" );
  G4double density=Water->GetDensity();
  //G4cout<<" ............... water density: "<<density<<"/"<<density*cm3/g<<G4endl;
  G4double emass= VoxelX * VoxelY * VoxelZ * density;
  //G4cout<<".....mass "<<emass/g<<"/"<<emass/kg<<G4endl;
  emass = emass/kg;
  G4double edose = edep/emass; //unit: MeV/kg
  G4cout<<"........"<<edep<<"/"<<emass<<"/"<<edose<<G4endl;
  edose = edose/(6.2415*pow(10,12));//unit:Gy

  G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  if(edep==0)return false;
  no_step = no_step+1;

  G4cout<<" each track particle "<<no_step<<" particle: "<<particleName<<G4endl;

  G4StepPoint* preStepPoint=aStep->GetPreStepPoint();
  G4TouchableHandle touchable = preStepPoint->GetTouchableHandle();

  G4ThreeVector pos_world = preStepPoint->GetPosition();
  G4ThreeVector pos_local = touchable->GetHistory()->GetTopTransform().TransformPoint(pos_world);

  G4String sdName = touchable->GetVolume()->GetName();
  G4int  copyNo   = touchable->GetCopyNumber();

  G4cout<<"SD: "<<sdName<<" copyNo: "<<copyNo<<" edep: "<< edep
  <<" edose: "<<edose<< " sum_edep: "<<sum_edep<<G4endl;
#ifdef G4VERBOS
  G4cout<<" world(x,y,z)"<< pos_world.x()<<", "<<pos_world.y()<<", "<<pos_world.z()<<G4endl;
  G4cout<<" local(x,y,z)"<< pos_local.x()<<", "<<pos_local.y()<<", "<<pos_local.z()<<G4endl;
#endif
  anaMag->FillH1(0, pos_world.z()/cm);
  anaMag->FillNtupleDColumn(0, pos_world.x()/cm);
  anaMag->FillNtupleDColumn(1, pos_world.y()/cm);
  anaMag->FillNtupleDColumn(2, pos_world.z()/cm);
  anaMag->FillNtupleDColumn(3, edep);
  anaMag->FillNtupleDColumn(4, edose);
  anaMag->AddNtupleRow();

  return true;
}


void MyG4HWSD::EndOfEvent(G4HCofThisEvent* ){
  //G4AnalysisManager* anaMag = G4AnalysisManager::Instance();
  G4cout<<"---EndOfEvent sum_edep= "<<sum_edep<<G4endl;
}
