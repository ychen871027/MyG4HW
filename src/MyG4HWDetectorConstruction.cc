#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4UIcommand.hh"
#include "G4PhysicalConstants.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSDoseDeposit3D.hh"
#include "MyG4HWSD.hh"
#include "MyG4HWDetectorConstruction.hh"
#include "MyG4HWPhantomParameterisation.hh"

MyG4HWDetectorConstruction::MyG4HWDetectorConstruction()
  :G4VUserDetectorConstruction(),
   fAir{nullptr},
   fWater{nullptr},
   fWorld_solid{nullptr},
   fWorld_logic{nullptr},
   fWorld_phys{nullptr},
   fContainer_solid{nullptr},
   fContainer_logic{nullptr},
   fContainer_phys{nullptr},
   fNVoxelX(0),
   fNVoxelY(0),
   fNVoxelZ(0)
{}

MyG4HWDetectorConstruction::~MyG4HWDetectorConstruction(){}

G4VPhysicalVolume* MyG4HWDetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();
  fAir   = nist->FindOrBuildMaterial( "G4_AIR" );
  fWater = nist->FindOrBuildMaterial( "G4_WATER" );
  G4double world_x = 300;//0.5*m;
  G4double world_y = 10;//0.5*m;
  G4double world_z = 1200;//1.5*m;

  fWorld_solid = new G4Box("WorldSolid",
                           world_x,
                           world_y,
                           world_z);

  fWorld_logic = new G4LogicalVolume(fWorld_solid,
                                     fAir,
                                     "WorldLogical",
                                     0, 0, 0 );

  fWorld_logic->SetVisAttributes(G4VisAttributes(true, G4Colour(0.0, 0.0,1.0)));

  fWorld_phys  = new G4PVPlacement( 0,
                                    G4ThreeVector(0, 0, 0),
                                    "World",
                                    fWorld_logic,
                                    0,
                                    false,
                                    0 );

  ConstructPhantomContainer();
  ConstructPhantom();

  return fWorld_phys;
}

void MyG4HWDetectorConstruction::ConstructPhantomContainer(){

  G4cout << " fNVoxelX: " << fNVoxelX << " fVoxelXHalfOfX: " << fVoxelXHalfOfX
  << G4endl;
  G4cout << " fNVoxelY: " << fNVoxelY << " fVoxelXHalfOfY: " << fVoxelXHalfOfY
  << G4endl;
  G4cout << " fNVoxelZ: " << fNVoxelZ << " fVoxelXHalfOfZ: " << fVoxelXHalfOfZ
  << G4endl;

  fContainer_solid = new G4Box( "phantomContainer", fNVoxelX*fVoxelXHalfOfX,
                                fNVoxelY*fVoxelXHalfOfY,
                                fNVoxelZ*fVoxelXHalfOfZ );
  fContainer_logic = new G4LogicalVolume( fContainer_solid,
                                          fWater,
                                          "phantomContainer",
                                          0, 0, 0 );
  fContainer_logic->SetVisAttributes(G4VisAttributes(true, G4Colour(1.0,0.0,0.0)));

  G4double Offset_X = 0.;
  G4double Offset_Y = 0.;
  G4double offset_Z = fNVoxelZ*fVoxelXHalfOfZ;
  G4ThreeVector posCentreVoxels( Offset_X, Offset_Y, offset_Z );

  G4cout << "placing voxel container volume at " << posCentreVoxels << G4endl;

  fContainer_phys = new G4PVPlacement( 0,
                                       posCentreVoxels,
                                       fContainer_logic,
                                       "phantomContainer",
                                       fWorld_logic,
                                       false,
                                       1 );

  }

void MyG4HWDetectorConstruction::ConstructPhantom()
{
  G4cout<<"MyG4HWDetectorConstruction::ConstructPhantom"<<G4endl;

  fMateIDs = new size_t[fNVoxelX*fNVoxelY*fNVoxelZ];
  fMaterials.clear();
  for( G4int iz=0; iz<fNVoxelZ; iz++){
    for( G4int iy=0; iy<fNVoxelY; iy++){
      for( G4int ix=0; ix<fNVoxelX; ix++){
        G4int idx_id = ix + iy*fNVoxelX + iz*fNVoxelX*fNVoxelY;
        fMateIDs[idx_id] = idx_id;
        fMaterials.push_back(fWater);
      }
    }
  }

  G4VSolid* solYRep = new G4Box("RepY", fNVoxelX*fVoxelXHalfOfX,
                                fVoxelXHalfOfY,
                                fNVoxelZ*fVoxelXHalfOfZ);
  G4LogicalVolume* logYRep = new G4LogicalVolume(solYRep, fWater, "RepY");
  new G4PVReplica("RepY", logYRep, fContainer_logic,
                  kYAxis, fNVoxelY, fVoxelXHalfOfY*2);
  //logYRep->SetVisAttributes( new G4VisAttributes(G4VisAttributes::GetInvisible()));
  logYRep->SetVisAttributes(G4VisAttributes(true, G4Colour(1.0,0.0,0.0)));

  G4String xRepName("RepX");
  G4VSolid* solXRep = new G4Box("RepX", fVoxelXHalfOfX,
                                fVoxelXHalfOfY, fNVoxelZ*fVoxelXHalfOfZ);
  G4LogicalVolume* logXRep = new G4LogicalVolume(solXRep, fWater, xRepName);
  new G4PVReplica( xRepName, logXRep, logYRep, kXAxis, fNVoxelX, fVoxelXHalfOfX*2);
  //logXRep->SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));
  logXRep->SetVisAttributes(G4VisAttributes(true, G4Colour(0.,1.0,0.0)));

  G4VSolid* solVoxel =  new G4Box("phantom", fVoxelXHalfOfX, fVoxelXHalfOfY, fVoxelXHalfOfZ);
  G4LogicalVolume* logicVoxel = new G4LogicalVolume(solVoxel, fWater, "phantom");
  //logicVoxel->SetVisAttributes( new G4VisAttributes( G4VisAttributes::GetInvisible()));
  logicVoxel->SetVisAttributes(G4VisAttributes(true, G4Colour(0.,0.0,1.0)));
  G4ThreeVector voxelSize(fVoxelXHalfOfX, fVoxelXHalfOfY, fVoxelXHalfOfZ);
  MyG4HWPhantomParameterisation* param = new MyG4HWPhantomParameterisation(voxelSize, fMaterials);
  param->SetMaterialIndices( fMateIDs );
  param->SetNoVoxel( fNVoxelX, fNVoxelY, fNVoxelZ );
  new G4PVParameterised( "phatom", logicVoxel, logXRep, kZAxis, fNVoxelZ, param );

  //creat and add the SensitiveDetector for voxel;
  MyG4HWSD* aSD = new MyG4HWSD("MyG4HWSD");
  logicVoxel->SetSensitiveDetector(aSD);
  G4SDManager::GetSDMpointer()->AddNewDetector(aSD);

}
