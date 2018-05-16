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
#include "MyG4HWDetectorConstruction.hh"
#include "MyG4HWPhantomParameterisation.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSDoseDeposit3D.hh"
MyG4HWDetectorConstruction::MyG4HWDetectorConstruction()
:G4VUserDetectorConstruction(),
fAir(0),
fWater(0),
fWorld_solid(0),
fWorld_logic(0),
fWorld_phys(0),
fContainer_solid(0),
fContainer_logic(0),
fContainer_phys(0),
fNVoxelX(0),
fNVoxelY(0),
fNVoxelZ(0)
{}

MyG4HWDetectorConstruction::~MyG4HWDetectorConstruction(){}

G4VPhysicalVolume* MyG4HWDetectorConstruction::Construct(){

  G4NistManager* nist = G4NistManager::Instance();
  fAir   = nist->FindOrBuildMaterial( "G4_AIR" );
  fWater = nist->FindOrBuildMaterial( "G4_WATER" );
  G4double world_x=300;//0.5*m;
  G4double world_y=10;//0.5*m;
  G4double world_z=1200;//1.5*m;

  fWorld_solid = new G4Box( "WorldSolid",
                            world_x,
                            world_y,
                            world_z );
  fWorld_logic = new G4LogicalVolume( fWorld_solid,
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

void MyG4HWDetectorConstruction::ConstructPhantom(){

  G4cout << "MyG4HWDetectorConstruction::ConstructPhantom" << G4endl;
  MyG4HWPhantomParameterisation* param =
    new MyG4HWPhantomParameterisation();
  param->SetVoxelDimensions( fVoxelXHalfOfX, fVoxelXHalfOfY, fVoxelXHalfOfZ );
  param->SetNoVoxel( fNVoxelX, fNVoxelY, fNVoxelZ );

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

  param->SetMaterials( fMaterials );
  param->SetMaterialIndices( fMateIDs );

  G4Box* voxel_solid =
    new G4Box( "Voxel", fVoxelXHalfOfX, fVoxelXHalfOfY, fVoxelXHalfOfZ );
  G4LogicalVolume* voxel_logic =
    new G4LogicalVolume( voxel_solid, fWater, "VoxelLogical", 0, 0, 0 );
  //voxel_logic->SetVisAttributes(
  //             new G4VisAttributes( G4VisAttributes::GetInvisible()));
  voxel_logic->SetVisAttributes(G4VisAttributes(true, G4Colour(0.0,1.0,0.0)));
  param->BuildContainerSolid( fContainer_phys );
  param->CheckVoxelsFillContainer( fContainer_solid->GetXHalfLength(),
                                   fContainer_solid->GetYHalfLength(),
                                   fContainer_solid->GetZHalfLength()
                                 );

  G4PVParameterised * phantom_phys =
    new G4PVParameterised( "phatom", voxel_logic, fContainer_logic, kXAxis,
                           fNVoxelX*fNVoxelY*fNVoxelZ, param );

  phantom_phys->SetRegularStructureId(1);

  SetScorer( voxel_logic );
}

void MyG4HWDetectorConstruction::SetScorer( G4LogicalVolume* voxel_logic ){
  G4cout << "\t SET SCORER: " << voxel_logic->GetName() << G4endl;
  fScorers.insert( voxel_logic );
}

void MyG4HWDetectorConstruction::ConstructSDandField(){
  G4cout << "\t construct SD " << G4endl;
  G4String concreteSDName = "phantomSD";
  std::vector<G4String> scorer_names;
  scorer_names.push_back( concreteSDName );
  G4MultiFunctionalDetector* MFDet =
    new G4MultiFunctionalDetector( concreteSDName );
  G4SDManager::GetSDMpointer()->AddNewDetector( MFDet );
  G4VPrimitiveScorer* dosedep =
    new G4PSDoseDeposit3D( "DoseDeposit", fNVoxelX, fNVoxelY, fNVoxelZ );
  MFDet->RegisterPrimitive(dosedep);

  for(std::set<G4LogicalVolume*>::iterator ite = fScorers.begin();
      ite != fScorers.end(); ++ite ){
        SetSensitiveDetector( *ite, MFDet );
      }
}
