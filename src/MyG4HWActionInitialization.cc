#include "MyG4HWPrimaryGeneratorAction.hh"
#include "MyG4HWRunAction.hh"
#include "MyG4HWEventAction.hh"
#include "MyG4HWActionInitialization.hh"

MyG4HWActionInitialization::MyG4HWActionInitialization()
  :G4VUserActionInitialization()
{
}

MyG4HWActionInitialization::~MyG4HWActionInitialization()
{
}

void MyG4HWActionInitialization::Build() const
{
  SetUserAction(new MyG4HWPrimaryGeneratorAction);
  SetUserAction(new MyG4HWRunAction);
  SetUserAction(new MyG4HWEventAction);
}
