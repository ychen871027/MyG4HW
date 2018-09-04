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
  if (aStep->GetTrack()->GetPosition().z()>0) aStep->GetTrack()->SetTrackStatus(fStopAndKill);

  G4double edep = aStep-> GetTotalEnergyDeposit();
  G4String particleName = aStep-> GetTrack()-> GetDefinition()
                               -> GetParticleName();
  G4StepPoint* preStepPoint = aStep-> GetPreStepPoint();

  G4ThreeVector pos_world = preStepPoint-> GetPosition();

  //std::cout << edep << " world(x, y, z): " << pos_world.x() / mm << "/"
  //          << pos_world.y() / mm << "/" << pos_world.z() / mm << std::endl;

  auto AnaMan = MyG4HWAnalysis::Instance();

  bool outputflag = false;
  
  //if ( pos_world.z() < 0.0*mm && pos_world.z() >= -100*cm
  //if ( pos_world.z() == 0.0*mm && aStep->GetTrack()->GetParentID() == 0)
  //std::cout << "track position(x,y,z) " << aStep->GetTrack()->GetPosition().x() << "/" << aStep->GetTrack()->GetPosition().y() << "/" << aStep->GetTrack()->GetPosition().z() << std::endl;
  if ( fabs(aStep->GetTrack()->GetPosition().z()) < 0.000001*mm && aStep->GetTrack()->GetParentID() == 0)
  {
    // if (fabs(pos_world.x()) > 0.0){
    //   std::cout << aStep->GetDeltaPosition().x()/mm << "/"
    //             << aStep->GetDeltaPosition().y()/mm << "/"
    //             << aStep->GetDeltaPosition().z()/mm << std::endl;
    //
    //   std::cout << aStep->GetTrack()->GetMomentumDirection().x()/mm << "/"
    //             << aStep->GetTrack()->GetMomentumDirection().y()/mm << "/"
    //             << aStep->GetTrack()->GetMomentumDirection().z()/mm << std::endl;
    //   std::cout << aStep->GetTrack()->GetGlobalTime() /ns
    //             << "/" << aStep->GetTrack()->GetLocalTime() /ns << std::endl;
    //   std::cout << "ssbefore arrived at the surface:  " << pos_world.x() << "/"
    //            << pos_world.y() << "/" << pos_world.z() << std::endl;
    // }
    AnaMan-> Fill1DHist(3, pos_world.x(), 1.);
    AnaMan-> Fill1DHist(4, pos_world.y(), 1.);
  }

  if ( pos_world.z() < 0.*mm && pos_world.z() >= -100*cm ){
  //if ( pos_world.z() < 0.0001*mm && pos_world.z() >= -100*cm ){
    //AnaMan-> Fill1DHist(5, pos_world.z(), 1.);
    //if (aStep-> GetStepLength()/mm < 5*mm){
     //std::cout << "before arrived at the surface:  " << pos_world.x() << "/"
     //          << pos_world.y() << "/" << pos_world.z() << "/" << aStep-> GetStepLength()/mm<< std::endl;
     //std::cout << "track position(x,y,z) " << aStep->GetTrack()->GetPosition().x() << "/" << aStep->GetTrack()->GetPosition().y() << "/" << aStep->GetTrack()->GetPosition().z() << std::endl;
    // }
    //if (aStep-> GetStepLength()/mm > 1100*mm) std::cout << "poute " << std::endl;
    if(aStep->GetTrack()->GetParentID() == 0){
      AnaMan-> Fill1DHist(5, aStep-> GetStepLength()/mm, 1.);
      AnaMan-> Fill1DHist(7, aStep-> GetTrack()->GetTrackLength()/mm, 1.);
    }else{
      AnaMan-> Fill1DHist(6, aStep-> GetTotalEnergyDeposit()/MeV, 1.);
    }
  }

}
