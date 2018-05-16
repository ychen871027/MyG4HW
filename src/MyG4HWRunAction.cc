#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "MyG4HWRunAction.hh"
#include "G4Run.hh"
#include "MyG4HWRun.hh"

MyG4HWRunAction::MyG4HWRunAction():G4UserRunAction(){
  fSDName.push_back( G4String("phantomSD") );
}

G4Run* MyG4HWRunAction::GenerateRun(){
  return fDcmrun = new MyG4HWRun(fSDName);
}
MyG4HWRunAction::~MyG4HWRunAction(){
  fSDName.clear();
}

void MyG4HWRunAction::BeginOfRunAction( const G4Run* aRun){

  #ifdef G4VERBOSE
  G4cout << "### RunID: "<< aRun->GetRunID() << " start." << G4endl;
  #endif
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

void MyG4HWRunAction::EndOfRunAction( const G4Run* aRun ){
  G4int nofEvts = aRun->GetNumberOfEvent();
  if (nofEvts == 0 ) return;


  double tot_dose = 0.;
  const MyG4HWRun* rRun = static_cast<const MyG4HWRun*>(aRun);
  for (G4int i = 0; i< (G4int)fSDName.size(); i++){
    G4THitsMap<G4double>* DoseDeposit =
      rRun->GetHitsMap( fSDName[i] + "/DoseDeposit");
    if ( DoseDeposit && DoseDeposit->GetMap()->size()!=0 ){
      std::map<G4int, G4double*>::iterator itr = DoseDeposit->GetMap()->begin();
      for(; itr != DoseDeposit->GetMap()->end(); itr++){
        tot_dose += *(itr->second);

        #ifdef G4VERBOSE
        G4cout<<"dose of voxel: "<<itr->first<<"/"<<std::setprecision(6)
              << *(itr->second)/CLHEP::gray << "Gy"<<G4endl;
        #endif

      }
    }
  }


  G4cout << "\n----EndOfRun----"<<"\n the run was "<<nofEvts<< G4endl;
  G4cout << "Total Dose: \t" << tot_dose/CLHEP::gray<<" Gy"<<G4endl;
  G4cout << "Finished : EndOfRunAction "<<aRun->GetRunID()<<G4endl;
}
