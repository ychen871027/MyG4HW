#include "G4EmParameters.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4VModularPhysicsList.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWPL.hh"

MyG4HWPL::MyG4HWPL(): G4VModularPhysicsList()
{
  std::cout << "My PhysicsList will be running in " << __FILE__ << std::endl;

  auto AnaMan = MyG4HWAnalysis::Instance();
  int ver = 1;
  defaultCutValue = 1.*CLHEP::mm;
  defaultCutValue = AnaMan-> GetCutValue() * CLHEP::mm;
  SetVerboseLevel(ver);

  if (AnaMan-> GetPLname() == "G4EmStandardPhysics_option4") {
    RegisterPhysics( new G4EmStandardPhysics_option4(ver) );
    std::cout << "EM_option 4 registered" << std::endl;
  } else if (AnaMan-> GetPLname() == "G4EmStandardPhysics_option3") {
    RegisterPhysics( new G4EmStandardPhysics_option3(ver) );
    std::cout << "EM_option 3 registered" << std::endl;
  } else {
    std::cout << "need to add physics list" << std::endl;
  }

}

MyG4HWPL::~MyG4HWPL()
{
}

void MyG4HWPL:: ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();
}

void MyG4HWPL:: ConstructProcess()
{
  G4VModularPhysicsList:: ConstructProcess();

  G4EmParameters* param =  G4EmParameters:: Instance();
  param->SetVerbose(1);

}

void MyG4HWPL::SetCuts()
{
   SetCutsWithDefault();
}
