//#include "MyG4HWEMOld.hh"
//#include "MyG4HWEM.hh"
//#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "MyG4HWPL.hh"
#include "G4VModularPhysicsList.hh"
#include "G4EmParameters.hh"
//MyG4HWPL::MyG4HWPL(): G4VUserPhysicsList()
MyG4HWPL::MyG4HWPL(): G4VModularPhysicsList()
{
  std::cout << "My PhysicsList will be running in " << __FILE__ << std::endl;

  int ver = 1;
  //SetDefaultCutValue( 1*CLHEP::cm );
  //SetDefaultCutValue( 1*CLHEP::cm );
  //defaultCutValue = 1.*CLHEP::cm;
  defaultCutValue = 1.*CLHEP::mm;
  //defaultCutValue = 1*CLHEP::cm;//0.7*CLHEP::mm;
  //defaultCutValue = 0.7*CLHEP::mm;
  SetVerboseLevel(ver);

  //RegisterPhysics( new MyG4HWEMOld(1) );
  //RegisterPhysics( new MyG4HWEM(ver) );
  //RegisterPhysics( new G4EmStandardPhysics(ver) );
  //RegisterPhysics( new G4EmStandardPhysics(ver) );
  RegisterPhysics( new G4EmStandardPhysics_option4(ver) );
  //RegisterPhysics(new StepLimiterBuilder());
  //G4EmParameters* param = G4EmParameters::Instance();
  //param->SetMaxEnergy(10*TeV);
  //G4ProductionCutsTable::GetProductionCutsTable()->
  //  SetEnergyRange(100*CLHEP::eV, 1*CLHEP::GeV);
  //param->SetMinEnergy(100*CLHEP::eV);
  //param->SetMaxEnergy(1*CLHEP::GeV);
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
  //if ( this-> verboseLevel > 1 ){
    //G4cout << "MyG4HWPL::SetCuts: ";
   SetCutsWithDefault();
// }
}
