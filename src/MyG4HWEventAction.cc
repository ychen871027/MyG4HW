#include "G4Event.hh"
#include "MyG4HWEventAction.hh"

MyG4HWEventAction::MyG4HWEventAction() : G4UserEventAction()
{
}

MyG4HWEventAction::~MyG4HWEventAction()
{
}

void MyG4HWEventAction::BeginOfEventAction(const G4Event* evt)
{
  G4cout << "---------EventID: "<< evt->GetEventID() << "-----------" << G4endl;
}

void MyG4HWEventAction::EndOfEventAction(const G4Event*)
{
}
