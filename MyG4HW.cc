
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

int main( int argc, char** argv)
{

  G4RunManager* runManager = new G4RunManager;
  G4cout << "MyG4HW:: create runManager" << G4endl;
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

  G4VModularPhysicsList* phys = new QGSP_BIC();
  //G4VModularPhysicsList* phys = new Shielding();
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
