#ifndef MyG4HWPhantomParameterisation_h
#define MyG4HWPhantomParameterisation_h 1

#include <map>
#include "G4PhantomParameterisation.hh"

class G4VisAttributes;

class MyG4HWPhantomParameterisation : public G4PhantomParameterisation{
public:
  MyG4HWPhantomParameterisation();
  ~MyG4HWPhantomParameterisation();
  /*virtual G4Material* ComputeMaterial( const G4int repNo,
                                       G4VPhysicalVolume* currentVol,
                                       const G4VTouchable* parentTouch=0 );*/
private:
  std::map<G4String, G4VisAttributes*> fColours;

};

#endif
