#ifndef MyG4HWRun_h
#define MyG4HWRun_h 1
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4THitsMap.hh"
#include <vector>

class MyG4HWRun : public G4Run{
public:
  MyG4HWRun();
  MyG4HWRun( const std::vector<G4String> mfdName);
  ~MyG4HWRun();

public:
  virtual void RecordEvent(const G4Event* );
  G4int GetNumberOfHitsMap() const{return fRunMap.size();};
  G4THitsMap<G4double>* GetHitsMap( G4int i) const {return fRunMap[i];};
  G4THitsMap<G4double>* GetHitsMap( const G4String& detName,
                                    const G4String& colName)const;
  G4THitsMap<G4double>* GetHitsMap( const G4String& fullName)const;
  void ConstructMFD( const std::vector<G4String>& );

private:
  std::vector<G4String> fCollName;
  std::vector<G4int> fCollID;
  std::vector<G4THitsMap<G4double>*> fRunMap;

};
#endif
