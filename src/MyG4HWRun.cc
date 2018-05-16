#include "MyG4HWRun.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"

MyG4HWRun::MyG4HWRun() : G4Run(){}

MyG4HWRun::MyG4HWRun(const std::vector<G4String> mfdName) : G4Run(){
  ConstructMFD(mfdName);
}

MyG4HWRun::~MyG4HWRun(){
  G4int Nmap = fRunMap.size();
  for (G4int i=0; i<Nmap; i++){
    if(fRunMap[i]) fRunMap[i]->clear();
  }
  fCollName.clear();
  fCollID.clear();
  fRunMap.clear();
}

void MyG4HWRun::ConstructMFD(const std::vector<G4String>& mfdName){
  G4cout<<"DEBUG::MyG4HWRun::ConstructMFD "<<mfdName.size()<<G4endl;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4int Nmfd = mfdName.size();
  for (G4int idet=0; idet<Nmfd; idet++){
    G4String detName = mfdName[idet];
    G4MultiFunctionalDetector* mfd =
    (G4MultiFunctionalDetector*)(SDman->FindSensitiveDetector(detName));
    if (mfd){
      for(G4int icol=0; icol<mfd->GetNumberOfPrimitives(); icol++){
        G4VPrimitiveScorer* scorer = mfd->GetPrimitive(icol);
        G4String collName=scorer->GetName();
        G4String fullcollName=detName+"/"+collName;
        G4int collID=SDman->GetCollectionID(fullcollName);
        if( collID>=0){
          G4cout<<"fullcollName: "<<fullcollName<<" id "<<collID<<G4endl;
          fCollName.push_back(fullcollName);
          fCollID.push_back(collID);
          fRunMap.push_back(new G4THitsMap<G4double>(detName, collName));
        }else{
          G4cout<<" collection name: "<<fullcollName<<" not found!!"<<G4endl;
        }
      }
    }
  }
}

void MyG4HWRun::RecordEvent(const G4Event* aEv){
  numberOfEvent++;
  G4HCofThisEvent* HCE = aEv->GetHCofThisEvent();
  if( !HCE ) return;

  G4int Ncol = fCollID.size();
  for (G4int i=0; i<Ncol; i++){
    G4THitsMap<G4double>* EvtMap = 0;
    if( fCollID[i]>=0 ){
      EvtMap=static_cast<G4THitsMap<G4double>*>(HCE->GetHC(fCollID[i]));
    }else{
      G4cout<<" Error EvtMap not found "<<i<<G4endl;
    }
    if(EvtMap){
      *fRunMap[i] += *EvtMap;
    }
  }

  G4Run::RecordEvent(aEv);
}

G4THitsMap<G4double>* MyG4HWRun::GetHitsMap( const G4String& detName,
                                             const G4String& colName) const
{
  G4String fullName = detName + "/" +colName;
  return GetHitsMap(fullName);
}

G4THitsMap<G4double>* MyG4HWRun::GetHitsMap( const G4String& fullName )const
{
  G4int Ncol = fCollName.size();
  G4cout<<"DEBUG::fCollName size: "<<Ncol<<" fullName: "<<fullName.c_str()<<G4endl;
  for(G4int i=0; i<Ncol; i++){
    if(fCollName[i]==fullName){
      return fRunMap[i];
    }
  }
  G4Exception("MyG4HWRun", fullName.c_str(), JustWarning,
              "GetHitsMap failed to locate the requested hitsmap");
  return NULL;
}
