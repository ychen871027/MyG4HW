#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "MyG4HWRunAction.hh"
#include "G4Run.hh"
#include "MyG4HWSD.hh"
#include "MyG4HWAnalysis.hh"
MyG4HWRunAction::MyG4HWRunAction():G4UserRunAction(){
  fSDName.push_back( G4String("phantomSD") );
  G4AnalysisManager* anaMag = G4AnalysisManager::Instance();
  anaMag->SetFileName("MyG4HW");
  anaMag->CreateH1("dedxvsZ","dedxvsZ", 300,0,30);
  anaMag->CreateNtuple("MyG4HW", "dedx postion");
  anaMag->CreateNtupleDColumn("pos_x");
  anaMag->CreateNtupleDColumn("pos_y");
  anaMag->CreateNtupleDColumn("pos_z");
  anaMag->CreateNtupleDColumn("sdep");
  anaMag->FinishNtuple();
}

MyG4HWRunAction::~MyG4HWRunAction(){
  fSDName.clear();
}

void MyG4HWRunAction::BeginOfRunAction( const G4Run* aRun){
  G4AnalysisManager* anaMag = G4AnalysisManager::Instance();
  #ifdef G4VERBOSE
  G4cout << "### RunID: "<< aRun->GetRunID() << " start." << G4endl;
  #endif
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  anaMag->OpenFile();
}

void MyG4HWRunAction::EndOfRunAction( const G4Run* aRun ){
  G4AnalysisManager* anaMag = G4AnalysisManager::Instance();
  G4int nofEvts = aRun->GetNumberOfEvent();
  if (nofEvts == 0 ) return;
  anaMag->Write();
  anaMag->CloseFile();
}
