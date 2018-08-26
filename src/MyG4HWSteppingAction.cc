#include "G4SystemOfUnits.hh"
#include "G4SteppingManager.hh"
#include "MyG4HWSteppingAction.hh"
#include "MyG4HWAnalysis.hh"

MyG4HWSteppingAction::MyG4HWSteppingAction()
{
}

MyG4HWSteppingAction:: ~MyG4HWSteppingAction()
{
}

void MyG4HWSteppingAction::UserSteppingAction( const G4Step* aStep )
{
  //std::cout << "running:: " << __FILE__ << std::endl;
  G4double edep = aStep-> GetTotalEnergyDeposit();
  G4String particleName = aStep-> GetTrack()-> GetDefinition()
                               -> GetParticleName();
  G4StepPoint* preStepPoint = aStep-> GetPreStepPoint();

  G4ThreeVector pos_world = preStepPoint-> GetPosition();

  //std::cout << edep << " world(x, y, z): " << pos_world.x() / mm << "/" 
  //          << pos_world.y() / mm << "/" << pos_world.z() / mm << std::endl;
  
  auto AnaMan = MyG4HWAnalysis::Instance();

  if ( fabs(pos_world.z()) < 0.0001*mm && aStep->GetTrack()->GetParentID() == 0)
  {
    // std::cout << "before arrived at the surface:  " << pos_world.x() << "/"
    //           << pos_world.y() << "/" << pos_world.z() << std::endl;
    AnaMan-> Fill1DHist(3, pos_world.x(), 1.);
    AnaMan-> Fill1DHist(4, pos_world.y(), 1.);
  }

  if ( pos_world.z() < 0.0001*mm && pos_world.z() > -100*cm ){
    //AnaMan-> Fill1DHist(5, pos_world.z(), 1.);
    if(aStep->GetTrack()->GetParentID() == 0){
      AnaMan-> Fill1DHist(5, aStep-> GetStepLength()/mm, 1.);
      AnaMan-> Fill1DHist(7, aStep-> GetTrack()->GetTrackLength()/mm, 1.);
    }
  }

}
