#ifndef MYG4HW_ACTION_INITIALIZATION_H_
#define MYG4HW_ACTION_INITIALIZATION_H_

#include "G4VUserActionInitialization.hh"

class MyG4HWActionInitialization : public G4VUserActionInitialization{
public:
  MyG4HWActionInitialization();
  virtual ~MyG4HWActionInitialization();

public:
  virtual void Build() const;

};
#endif
