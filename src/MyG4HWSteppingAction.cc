#include "G4SystemOfUnits.hh"
#include "G4SteppingManager.hh"
#include "MyG4HWSteppingAction.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWEventAction.hh"

MyG4HWSteppingAction::MyG4HWSteppingAction(MyG4HWEventAction* event)
:G4UserSteppingAction(),
  fEvent(event)
{
}

MyG4HWSteppingAction:: ~MyG4HWSteppingAction()
{
}

void MyG4HWSteppingAction::UserSteppingAction( const G4Step* aStep )
{
  //if (aStep->GetTrack()->GetPosition().z()>=0) aStep->GetTrack()->SetTrackStatus(fStopAndKill);
  //G4double edep = aStep-> GetTotalEnergyDeposit();
  //if ( edep == 0 ) return;
  //std::cout << "running:: " << __FILE__ << std::endl;

  G4String particleName = aStep-> GetTrack()-> GetDefinition()
                               -> GetParticleName();
  G4StepPoint* preStepPoint = aStep-> GetPreStepPoint();

  G4ThreeVector pos_world = preStepPoint-> GetPosition();

  //std::cout << edep << " world(x, y, z): " << pos_world.x() / mm << "/"
  //          << pos_world.y() / mm << "/" << pos_world.z() / mm << std::endl;

  auto AnaMan = MyG4HWAnalysis::Instance();

  //bool outputflag = false;
  //if ( pos_world.z() < 0.0*mm && pos_world.z() >= -100*cm
  //if ( pos_world.z() == 0.0*mm && aStep->GetTrack()->GetParentID() == 0)

  if (AnaMan->GetVerbose()) {
    std::ofstream ofs;
    ofs.open (AnaMan->MakeDetailTrackINFO(), std::ios::app);

    ofs << aStep->GetTrack()->GetPosition().x()/mm << " "
        << aStep->GetTrack()->GetPosition().y()/mm << " "
        << aStep->GetTrack()->GetPosition().z()/mm << " "
        << aStep->GetTrack()->GetKineticEnergy()/MeV   << " "
        << aStep->GetTotalEnergyDeposit()/MeV << " "
        << aStep->GetStepLength()/mm <<" "
        << aStep->GetTrack()->GetTrackLength()/mm << " "
        << aStep->GetTrack()->GetCurrentStepNumber()<< " "
        << aStep->GetTrack()->GetTrackID() << " "
        << aStep->GetTrack()->GetParentID() << " "
        << aStep->GetTrack()-> GetDefinition()->GetParticleName() << " "
        << aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()
        << std::endl;
    ofs.close();
  }
  //std::cout << "track position(x,y,z) " << aStep->GetTrack()->GetPosition().x() << "/" << aStep->GetTrack()->GetPosition().y() << "/" << aStep->GetTrack()->GetPosition().z() << std::endl;
  if (fabs(aStep->GetTrack()->GetPosition().z()) < 0.000001*mm && aStep->GetTrack()->GetParentID() == 0) {
    //fEvent->CountNumOfTrack();
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
    AnaMan-> Fill1DHist(10, 20*MeV-aStep->GetTrack()->GetKineticEnergy()/MeV,1.);
    //std::cout << "dedx: " << 20*MeV-aStep->GetTrack()->GetKineticEnergy()/MeV << std::endl;
  }

  if (pos_world.z() < 0.*mm && pos_world.z() >= -100*cm) {
    if (aStep->GetTrack()->GetParentID() == 0) {
      AnaMan-> Fill1DHist(5, aStep-> GetStepLength()/mm, 1.);
      AnaMan-> Fill1DHist(7, aStep-> GetTrack()->GetTrackLength()/mm, 1.);
    } else {
      AnaMan-> Fill1DHist(6, aStep-> GetTotalEnergyDeposit()/MeV, 1.);
      //std::cout << aStep-> GetTotalEnergyDeposit()/MeV << std::endl;
      fEvent->CountNumOfTrack();
      fEvent->CountEdepOfTrack(aStep-> GetTotalEnergyDeposit());
    }
  }

}
