#include <fstream>
#include "G4UnitsTable.hh"
#include <CLHEP/Units/SystemOfUnits.h>
#include "G4Event.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWEventAction.hh"

MyG4HWEventAction::MyG4HWEventAction() : G4UserEventAction()
{
}

MyG4HWEventAction::~MyG4HWEventAction()
{
}

void MyG4HWEventAction::BeginOfEventAction(const G4Event* evt)
{
  fNtrk=0;
  fTotEdep=0.;
  mtime.Start();
  
  auto AnaMan = MyG4HWAnalysis::Instance();
  if (AnaMan-> GetVerbose()) {
    std::ofstream ofs;
    ofs.open(AnaMan->MakeDetailTrackINFO(), std::ios::app);
    ofs << "Event " << evt->GetEventID() << std::endl;
    ofs.close();
  }
}

void MyG4HWEventAction::EndOfEventAction(const G4Event* evt)
{
  auto AnaMan = MyG4HWAnalysis::Instance();
  AnaMan-> Fill1DHist(9, fTotEdep/CLHEP::MeV, 1.);
  AnaMan-> Fill1DHist(8, fNtrk, 1.);

  if (evt->GetEventID()%200000 == 0) {
    std::cout << evt->GetEventID() << " events finished." << std::endl;
    mtime.Stop();
    std::cout << "Time: " << G4BestUnit(mtime.GetSystemElapsed(), "Time") << std::endl;
  }

}
