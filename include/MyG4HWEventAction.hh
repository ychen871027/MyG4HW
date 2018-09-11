#ifndef MYG4HW_EVENT_ACTION_H_
#define MYG4HW_EVENT_ACTION_H_

#include "G4UserEventAction.hh"

class G4Event;

class MyG4HWEventAction : public G4UserEventAction
{
public:
  MyG4HWEventAction();
  ~MyG4HWEventAction();

  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event* anEvent);
  void CountNumOfTrack();
  double CountEdepOfTrack(double edep);
private:
  int fNtrk;
  double fTotEdep;

};

inline void MyG4HWEventAction::CountNumOfTrack()
{
  fNtrk++;
}
inline double MyG4HWEventAction::CountEdepOfTrack(double edep)
{
  fTotEdep = fTotEdep + edep;
}
#endif
