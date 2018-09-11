#include "G4UnitsTable.hh"
#include <CLHEP/Units/SystemOfUnits.h>
#include "G4Event.hh"
#include "MyG4HWEventAction.hh"
#include "MyG4HWAnalysis.hh"

MyG4HWEventAction::MyG4HWEventAction() : G4UserEventAction()
{
}

MyG4HWEventAction::~MyG4HWEventAction()
{
}

void MyG4HWEventAction::BeginOfEventAction(const G4Event* evt)
{
  if ( evt->GetEventID()%1000==0 )
    G4cout << "---------EventID: "<< evt->GetEventID() << "-----------" << G4endl;
  fNtrk=0;
  fTotEdep=0.;
}

void MyG4HWEventAction::EndOfEventAction(const G4Event*)
{
  auto AnaMan = MyG4HWAnalysis::Instance();
  //std::cout << "fNtrk: " << fNtrk << std::endl;
  //if (fNtrk > 1) std::cout << " iamhere " << std::endl;
  AnaMan-> Fill1DHist(9, fTotEdep/CLHEP::MeV, 1.);
  AnaMan-> Fill1DHist(8, fNtrk, 1.);

}
