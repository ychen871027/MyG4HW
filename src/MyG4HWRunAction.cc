#include "G4RunManager.hh"
#include "G4Run.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWRunAction.hh"

MyG4HWRunAction::MyG4HWRunAction()
  :G4UserRunAction()
{
}

MyG4HWRunAction::~MyG4HWRunAction(){}

void MyG4HWRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### RunID: "<< aRun->GetRunID() << " start." << G4endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  auto AnaMan = MyG4HWAnalysis::Instance();
  AnaMan->BookTreeAndHist();
}

void MyG4HWRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int nofEvts = aRun->GetNumberOfEvent();
  if (nofEvts == 0) return;

  auto AnaMan = MyG4HWAnalysis::Instance();
  AnaMan->SaveFile();
}
