#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "MyG4HWPhantomParameterisation.hh"
#include "MyG4HWSD.hh"
#include "MyG4HWDetectorConstruction.hh"

MyG4HWDetectorConstruction::MyG4HWDetectorConstruction()
  :G4VUserDetectorConstruction(),
   fNVoxelX(0),
   fNVoxelY(0),
   fNVoxelZ(0)
{}

MyG4HWDetectorConstruction::~MyG4HWDetectorConstruction(){}

G4VPhysicalVolume* MyG4HWDetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* G4air   = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* G4water = nist->FindOrBuildMaterial("G4_WATER");

  G4double world_x = 300;//0.5*m;
  G4double world_y = 10;//0.1*m;
  G4double world_z = 1200;//1.2*m;

  auto world_solid = new G4Box("WorldSolid",
                               world_x*mm,
                               world_y*mm,
                               world_z*mm);

  auto world_logic = new G4LogicalVolume(world_solid,
                                         G4air,
                                         "WorldLogical",
                                          0, 0, 0 );

  auto world_phys  = new G4PVPlacement(0,
                                       G4ThreeVector(0, 0, 0),
                                       "World",
                                       world_logic,
                                       0,
                                       false,
                                       0 );

// Mother Volume of water phantom
  auto phantom_solid = new G4Box("phantomVol", fNVoxelX*fVoxelXHalfOfX,
                                 fNVoxelY*fVoxelXHalfOfY,
                                 fNVoxelZ*fVoxelXHalfOfZ );
  auto phantom_logic = new G4LogicalVolume(phantom_solid,
                                           G4water,
                                           "phantomVol",
                                           0, 0, 0 );

  G4double Offset_X = 0.;
  G4double Offset_Y = 0.;
  G4double offset_Z = fNVoxelZ*fVoxelXHalfOfZ;
  G4ThreeVector posCentreVoxels( Offset_X, Offset_Y, offset_Z );

  G4cout << "placing voxel container volume at " << posCentreVoxels << G4endl;

  //auto phantom_phys =
  new G4PVPlacement(0,
                    posCentreVoxels,
                    phantom_logic,
                    "phantomVol",
                    world_logic,
                    false,
                    1 );

  //Replication for water phantom and create logical volume for voxel
  G4VSolid* solYRep = new G4Box("RepY", fNVoxelX*fVoxelXHalfOfX,
                                fVoxelXHalfOfY,
                                fNVoxelZ*fVoxelXHalfOfZ);
  G4LogicalVolume* logYRep = new G4LogicalVolume(solYRep, G4water, "RepY");
  new G4PVReplica("RepY", logYRep, phantom_logic,
                  kYAxis, fNVoxelY, fVoxelXHalfOfY*2);

  G4VSolid* solXRep = new G4Box("RepX", fVoxelXHalfOfX,
                              fVoxelXHalfOfY, fNVoxelZ*fVoxelXHalfOfZ);
  G4LogicalVolume* logXRep = new G4LogicalVolume(solXRep, G4water, "RepX");
  new G4PVReplica("RepX", logXRep, logYRep, kXAxis, fNVoxelX, fVoxelXHalfOfX*2);

  G4VSolid* solVoxel
    =  new G4Box("phantom", fVoxelXHalfOfX, fVoxelXHalfOfY, fVoxelXHalfOfZ);
  G4LogicalVolume* logicVoxel
    = new G4LogicalVolume(solVoxel, G4water, "phantom");

  G4ThreeVector voxelSize(fVoxelXHalfOfX, fVoxelXHalfOfY, fVoxelXHalfOfZ);

  std::vector<G4Material*> phantomMat(1, G4water);

  MyG4HWPhantomParameterisation* param
    = new MyG4HWPhantomParameterisation(voxelSize, phantomMat);
  param->SetNoVoxel( fNVoxelX, fNVoxelY, fNVoxelZ );

  new G4PVParameterised("phatom",
                        logicVoxel,
                        logXRep,
                        kZAxis,
                        fNVoxelZ,
                        param);

  //creat and add the SensitiveDetector for voxel;
  MyG4HWSD* aSD = new MyG4HWSD("MyG4HWSD");
  logicVoxel->SetSensitiveDetector(aSD);
  G4SDManager::GetSDMpointer()->AddNewDetector(aSD);

  //Vis for geometry
  world_logic->SetVisAttributes(G4VisAttributes(true, G4Colour(0.0, 0.0,1.0)));
  phantom_logic->SetVisAttributes(G4VisAttributes(true, G4Colour(1.0,0.0,0.0)));
  //logYRep->SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));
  logYRep->SetVisAttributes(G4VisAttributes(true, G4Colour(1.0,0.0,0.0)));
  //logXRep->SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));
  logXRep->SetVisAttributes(G4VisAttributes(true, G4Colour(0.,1.0,0.0)));
  //logicVoxel->SetVisAttributes(new G4VisAttributes( G4VisAttributes::GetInvisible()));
  logicVoxel->SetVisAttributes(G4VisAttributes(true, G4Colour(0.,0.0,1.0)));

  return world_phys;
}
