#ifndef MYG4HW_PL_H_
#define MYG4HW_PL_H_

#include "G4VModularPhysicsList.hh"
//#include "G4VUserPhysicsList.hh"
//class MyG4HWPL: public G4VUserPhysicsList
class MyG4HWPL: public G4VModularPhysicsList
{
public:
  MyG4HWPL();//G4int ver = 1);
  virtual ~MyG4HWPL();
  void ConstructParticle();
  void ConstructProcess();

  virtual void SetCuts();
//private:
//  MyG4HWPL & operator = ( const MyG4HWPL &right );
//  MyG4HWPL( const MyG4HWPL& );
//private:
//  MyG4HWPL(MyG4HWPL &);
//  MyG4HWPL & operator=( const MyG4HWPL &right);

};

#endif
