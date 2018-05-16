
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4GenericPhysicsList.hh"

#include "MyG4HWDetectorConstruction.hh"
#include "MyG4HWActionInitialization.hh"
//#include "MyG4HWHandler.hh"
//#include "MyG4HWInsectVolume.hh"
#include "QGSP_BIC.hh"
#include "G4tgrMessenger.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Shielding.hh"

int main( int argc, char** argv){
  //new G4G4tgrMessenger;
  G4RunManager* runManager = new G4RunManager;
  G4cout<<"MyG4HW:: create runManager"<<G4endl;
  MyG4HWDetectorConstruction* theGeometry = 0;

  //MyG4HWHandler* myhandler = new MyG4HWHandler;
  //myhandler->CheckFileFormat();

  theGeometry = new MyG4HWDetectorConstruction();

  const G4int NVoxelX=61;
  const G4int NVoxelY=1;
  const G4int NVoxelZ=150;
  const G4double VoxelX=5.;//305.;//30.5*cm;
  const G4double VoxelY=5;//2.;//0.2*cm;
  const G4double VoxelZ=2;//300.;//30*cm;

  theGeometry->SetNoVoxelX(NVoxelX);
  theGeometry->SetNoVoxelY(NVoxelY);
  theGeometry->SetNoVoxelZ(NVoxelZ);
  theGeometry->SetVoxelX(VoxelX);
  theGeometry->SetVoxelY(VoxelY);
  theGeometry->SetVoxelZ(VoxelZ);

  runManager->SetUserInitialization( theGeometry );

  G4VModularPhysicsList* phys = new Shielding();
  runManager->SetUserInitialization( phys );

  runManager->SetUserInitialization( new MyG4HWActionInitialization() );

  runManager->Initialize();

//  new MyG4HWInsectVolume();

#ifdef G4VIS_USE
     G4VisManager* visManager = new G4VisExecutive;
     visManager->Initialize();
#endif

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  if (argc==1){
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive( argc, argv );
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");
#endif
      ui->SessionStart();
      delete ui;
#endif
  }else{
    G4String command  = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand( command + fileName );
  }

  delete runManager;
#ifdef G4VIS_USE
  delete visManager;
#endif

  //delete myhandler;

  return 0;
}
