#include "MyG4HWPhantomParameterisation.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

MyG4HWPhantomParameterisation::MyG4HWPhantomParameterisation()
: G4PhantomParameterisation(){
}

MyG4HWPhantomParameterisation::~MyG4HWPhantomParameterisation(){
}

/*
G4Material* MyG4HWPhantomParameterisation::ComputeMaterial(
            const G4int copyNo, G4VPhysicalVolume* physVol,
            const G4VTouchable*                            ){

  G4Material* mate = G4PhantomParameterisation::ComputeMaterial(
                     copyNo, physVol, 0 );
  if ( physVol ){
    G4String mateName = mate->GetName();
    std::string::size_type iuu = mateName.find("__");
    if( iuu != std::string::npos ){
      mateName = mateName.substr( 0, iuu );
    }
    std::map<G4String, G4VisAttributes*>::const_iterator ite =
     fColours.find(mateName);
    if ( ite != fColours.end() ){
      physVol->GetLogicalVolume()->SetVisAttributes( (*ite).second );
    }else{
      physVol->GetLogicalVolume()->SetVisAttributes(
                             (*(fColours.begin())).second);
    }
  }
  return mate;
}*/
