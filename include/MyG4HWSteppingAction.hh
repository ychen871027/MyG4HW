#ifndef MYG4HW_STEPPINGACTION_H_
#define MYG4HW_STEPPINGACTION_H_

#include "G4UserSteppingAction.hh"

class MyG4HWSteppingAction : public G4UserSteppingAction
{
  public:
    MyG4HWSteppingAction();
    virtual ~MyG4HWSteppingAction();

    virtual void UserSteppingAction( const G4Step* );
  
  //private:
};

#endif
