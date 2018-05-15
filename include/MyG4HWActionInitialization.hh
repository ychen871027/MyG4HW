#ifndef MyG4HWActionInitialization_h
#define MyG4HWActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class MyG4HWActionInitialization : public G4VUserActionInitialization{
public:
  MyG4HWActionInitialization();
  ~MyG4HWActionInitialization();

public:
  virtual void Build() const;

};
#endif
