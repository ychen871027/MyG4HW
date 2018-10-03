#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4SystemOfUnits.hh"
#include "MyG4HWDetectorConstruction.hh"
#include "MyG4HWActionInitialization.hh"
#include "MyG4HWAnalysis.hh"
#include "MyG4HWPL.hh"
#include "physicslist_cug4.h"
#include <getopt.h>

void showhelp()
{
  std::cout << "MyG4HW option" << std::endl;
  std::cout << "usage: " << std::endl;
  std::cout << "./MyG4HW [options]....." << std::endl;
  std::cout << "    -h, --help              show this message." << std::endl;
  std::cout << "    -v, --verbose           show verbose message." << std::endl;
  std::cout << "    -i, --macrofile         set macro file." << std::endl;
  std::cout << "    -s, --seed Number       set seed number." << std::endl;
  std::cout << "    -o, --outputfilename    set Output name." << std::endl;
  std::cout << "    -c, --configurefile     set voxel configure file." << std::endl;
  std::cout << "    -m, --EMstandard        set flag to use EM standard PL." << std::endl;
  std::cout << "    -x, --ImortDensityList  set flag to run density list." << std::endl;
  std::cout << std::endl;
}


int main( int argc, char** argv)
{ 

  char* outputenv = getenv("ROOTOUT");
  if ( outputenv == NULL ) {
    std::cout << "Please set env[ROOTOUT]: " << getenv("ROOTOUT") << std::endl;
    std::exit(EXIT_FAILURE);
  } else{
    std::cout << "env ROOTOUT: " << getenv("ROOTOUT") << std::endl;
  }
  auto AnaMan = MyG4HWAnalysis::Instance();
  bool qhelp = false;
  bool qverbose = false;
  bool qsm = false;
  bool qdensity = false;
  G4String macfileName;
  G4String outputName;
  G4String confvoxel;
  G4long myseed = 1234;
  
  struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"verbose", no_argument, NULL, 'v'},
    {"macrofile", required_argument, NULL, 'i'},
    {"seedNum", required_argument, NULL, 's'},
    {"outname", required_argument, NULL, 'o'},
    {"configure", required_argument, NULL, 'c'},
    {"EMstandard", no_argument, NULL, 'm'},
    {"DensityList", no_argument, NULL, 'x'},
    {NULL, 0, NULL, 0}
  };

  int c;
  while (1) {
    int option_index = -1;
    c = getopt_long(argc, argv, "hvi:s:o:c:mx", long_options, &option_index);
    if (c == -1) break;
    switch (c) {
    case 'h' :
      qhelp = true;
      break;
    case 'v' :
      qverbose = true;
      break;
    case 'i' :
      macfileName = optarg;
      break;
    case 's' :
      myseed = atoi(optarg);
      break;
    case 'o' : 
      outputName = optarg;
      break;
    case 'c' :
      confvoxel = optarg;
      break;
    case 'm' :
      qsm = true;
      break;
    case 'x' :
      qdensity = true;
      break;
    default:
      std::exit(EXIT_FAILURE);
      break;
    }
  }

  if ( qhelp ) {
    showhelp();
    return 0;
  }
  
  if ( qverbose ) {
    std::cout << "running stepping class " << std::endl;
    AnaMan-> SetVerbose(qverbose);
  }
  std::cout << "The configure filename: " << confvoxel << std::endl;
  std::cout << "You are running the mac: " << macfileName << std::endl;
  std::cout << "Output File's name is set: " << outputName << std::endl;
  G4cout << "MyG4HW:: create runManager, the seed: " << myseed << G4endl;
  
  G4RunManager* runManager = new G4RunManager;
  G4Random::setTheEngine(new CLHEP::MTwistEngine);
  //G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(myseed);
  AnaMan-> SetDensityFlag(qdensity);
  AnaMan-> SetConfVoxel(confvoxel);
  AnaMan-> SetOutPutPrefix(outputName);
  MyG4HWDetectorConstruction* theGeometry = 0;
  theGeometry = new MyG4HWDetectorConstruction();

  runManager->SetUserInitialization(theGeometry);

  if (qsm) {
    G4VModularPhysicsList* phys = new MyG4HWPL();
    runManager->SetUserInitialization(phys);
  } else {
    G4VUserPhysicsList* phys = new cug4::PhysicsListCuG4();
    runManager->SetUserInitialization(phys);
  }

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
      fileName = macfileName;
      std::cout << "You are running the mac: " << macfileName << std::endl;
      UImanager->ApplyCommand( command + fileName );
  }

  delete runManager;
#ifdef G4VIS_USE
  delete visManager;
#endif

  return 0;
}
