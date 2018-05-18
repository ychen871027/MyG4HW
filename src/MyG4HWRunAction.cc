#include "G4RunManager.hh"
#include "G4Run.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWRunAction.hh"

MyG4HWRunAction::MyG4HWRunAction()
  :G4UserRunAction(),
   fAnaMan{nullptr},
   fPos_x(0.),
   fPos_y(0.),
   fPos_z(0.),
   fEdep(0.)
{
}

MyG4HWRunAction::~MyG4HWRunAction(){}

void MyG4HWRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### RunID: "<< aRun->GetRunID() << " start." << G4endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  fAnaMan = MyG4HWAnalysis::Instance();
  fAnaMan->BookTreeAndHist();
}

void MyG4HWRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int nofEvts = aRun->GetNumberOfEvent();
  if (nofEvts == 0) return;

  fAnaMan->SaveFile();
}
