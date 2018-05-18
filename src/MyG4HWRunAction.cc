#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWRunAction.hh"
#include "G4Run.hh"
#include "MyG4HWSD.hh"

MyG4HWRunAction::MyG4HWRunAction()
  :G4UserRunAction(),
   fAnaMan{nullptr},
   fPos_x(0.),
   fPos_y(0.),
   fPos_z(0.),
   fEdep(0.)
{
  fSDName.push_back( G4String("phantomSD") );
}

MyG4HWRunAction::~MyG4HWRunAction()
{
  fSDName.clear();
}

void MyG4HWRunAction::BeginOfRunAction( const G4Run* aRun)
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
