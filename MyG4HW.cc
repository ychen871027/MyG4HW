
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4GenericPhysicsList.hh"
//#include "Shielding.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4SystemOfUnits.hh"
#include "MyG4HWDetectorConstruction.hh"
#include "MyG4HWActionInitialization.hh"
#include "QGSP_BIC.hh"
//#include "MyG4HWPhysicsList.hh"
//#include "ExN02PhysicsList.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWPL.hh"
#include "physicslist_cug4.h"
//#include "G4StepLimiterPhysics.hh"

int main( int argc, char** argv)
{

  G4RunManager* runManager = new G4RunManager;
  G4cout << "MyG4HW:: create runManager" << G4endl;
  G4long myseed = 1234;
  myseed  = atoi(argv[3]);
  G4Random::setTheEngine(new CLHEP::MTwistEngine);
  //G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(myseed);
  MyG4HWDetectorConstruction* theGeometry = 0;

  theGeometry = new MyG4HWDetectorConstruction();

  const G4int NVoxelX=61;
  const G4int NVoxelY=61;
  const G4int NVoxelZ=150;
  const G4double VoxelX=5.*mm;
  const G4double VoxelY=5.*mm;
  const G4double VoxelZ=2.*mm;

  theGeometry->SetNoVoxelX(NVoxelX);
  theGeometry->SetNoVoxelY(NVoxelY);
  theGeometry->SetNoVoxelZ(NVoxelZ);
  theGeometry->SetVoxelX(VoxelX);
  theGeometry->SetVoxelY(VoxelY);
  theGeometry->SetVoxelZ(VoxelZ);

  runManager->SetUserInitialization(theGeometry);

  auto AnaMan = MyG4HWAnalysis::Instance();
  if ( argc > 2 ) {
    AnaMan-> SetSeedNum(atoi(argv[2]));
  } else {
    AnaMan-> SetSeedNum(-1);
  }
  std::cout << "SaveRootFileName: " << argv[2] << std::endl;
  AnaMan-> SetNoVoxelX(NVoxelX);
  AnaMan-> SetNoVoxelY(NVoxelY);
  AnaMan-> SetNoVoxelZ(NVoxelZ);
  AnaMan-> SetVoxelX(VoxelX);
  AnaMan-> SetVoxelY(VoxelY);
  AnaMan-> SetVoxelZ(VoxelZ);
  AnaMan-> SetBeamType(argv[4]);
  AnaMan-> SetMatType(argv[5]);
  AnaMan-> SetStepFlag(atof(argv[6]));
  AnaMan-> SetCutValue(atof(argv[7]));
  std::cout << "running " << argv[4] << " material: " << argv[5] << std::endl;

  //G4VModularPhysicsList* phys = new QGSP_BIC();
  //phys->SetDefaultCutValue( 1.*CLHEP::mm );
  //G4VModularPhysicsList* phys = new Shielding();
  //G4VUserPhysicsList* phys = new MyG4HWPhysicsList;
  //G4VUserPhysicsList* phys = new ExN02PhysicsList;
  //G4VModularPhysicsList* phys = new MyG4HWPL();
  G4VUserPhysicsList* phys = new cug4::PhysicsListCuG4();

  //if ( atof(argv[6])>0 )
  //phys->RegisterPhysics(new G4StepLimiterPhysics());

  runManager->SetUserInitialization(phys);

  runManager->SetUserInitialization(new MyG4HWActionInitialization());

  runManager->Initialize();

#ifdef G4VIS_USE
     G4VisManager* visManager = new G4VisExecutive;
     visManager->Initialize();
#endif

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  if (argc==1){
      G4UIExecutive* ui = new G4UIExecutive( argc, argv );
      ui->SessionStart();
      delete ui;
  }else{
      G4String command  = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand( command + fileName );
  }

  delete runManager;
#ifdef G4VIS_USE
  delete visManager;
#endif

  return 0;
}
