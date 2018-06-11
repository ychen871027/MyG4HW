#ifndef MYG4HW_PHYSICS_LIST_H_
#define MYG4HW_PHYSICS_LIST_H_

#include "G4VUserPhysicsList.hh"

class MyG4HWPhysicsList: public G4VUserPhysicsList
{
  public:
    MyG4HWPhysicsList();
    ~MyG4HWPhysicsList();
  protected:
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();

    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();

    void ConstructGeneral();
    void ConstructEM();
    void AddStepMax();
};

#endif
