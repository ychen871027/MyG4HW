#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVParameterised.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "MyG4HWPhantomParameterisation.hh"
#include "MyG4HWSD.hh"
#include "MyG4HWDetectorConstruction.hh"
#include "G4UnitsTable.hh"
#include "G4UserLimits.hh"
#include "MyG4HWAnalysis.hh"

MyG4HWDetectorConstruction::MyG4HWDetectorConstruction()
  : G4VUserDetectorConstruction(),
    fNVoxelX{0}, fNVoxelY(0), fNVoxelZ(0)
{
}

MyG4HWDetectorConstruction::~MyG4HWDetectorConstruction()
{
}

G4VPhysicalVolume* MyG4HWDetectorConstruction::Construct()
{

  auto AnaMan = MyG4HWAnalysis::Instance();

  G4Material* G4air   = nullptr;
  G4Material* G4water = nullptr;

  G4NistManager* nist = G4NistManager::Instance();

  if ( AnaMan-> GetMatType() == "g4air" )
  {
    G4cout << "running from NIST" << std::endl;
    G4water = nist->FindOrBuildMaterial("G4_WATER");
    G4air   = nist->FindOrBuildMaterial("G4_AIR");

  } else if ( AnaMan-> GetMatType() == "vg4water" )
  {
    G4cout << "running from NIST G4Water" << std::endl;
    G4water      = nist->FindOrBuildMaterial("G4_WATER");
    G4Material* G4air_normal = nist->FindOrBuildMaterial("G4_AIR");
    G4double air_density =  G4air_normal->GetDensity();

    G4air    = nist-> BuildMaterialWithNewDensity(
                                                  "G4_waterWairp",
                                                       "G4_WATER",
                                                     air_density);
  } else if ( AnaMan-> GetMatType() == "eleWater" )
  {
    G4cout << "running from elemnt Water" << std::endl;
    G4Material* G4air_normal = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* G4water_normal = nist->FindOrBuildMaterial("G4_WATER");
    G4double water_density =  G4water_normal->GetDensity();
    G4double air_density =  G4air_normal->GetDensity();
    G4Element* elH = new G4Element( "Hydrogen", "H", 1.0, 1.008  * g/mole );
    G4Element* elO = new G4Element( "Oxygen", "O", 8.0, 16.00  * g/mole );
    G4air = new G4Material( "airWwater", air_density, 2 );
    G4air->AddElement(elH,0.112);
    G4air->AddElement(elO,0.888);

    G4water = new G4Material( "water", water_density, 2 );
    G4water->AddElement(elH,0.112);
    G4water->AddElement(elO,0.888);
  }

  std::cout << "current air: density: " << G4air-> GetDensity() << " state: "
            << G4air-> GetState() << " temperature: " << G4air-> GetTemperature()
            << " pressure: " << G4air-> GetPressure() << " formula:"
            << G4air-> GetChemicalFormula() << std::endl;

  const G4double world_x = 300*mm;
  const G4double world_y = 300*mm;
  const G4double world_z = 1200*mm;

  auto world_solid = new G4Box("WorldSolid",
                               world_x,
                               world_y,
                               world_z);

  auto world_logic = new G4LogicalVolume(world_solid,
                                         G4air,
                                         "WorldLogical",
                                         0, 0, 0);

  auto world_phys  = new G4PVPlacement(0,
                                       G4ThreeVector(0, 0, 0),
                                       "World",
                                       world_logic,
                                       0,
                                       false,
                                       0);

// Mother Volume of water phantom
  auto phantom_solid = new G4Box("phantomVol", fNVoxelX*fVoxelXHalfOfX,
                                 fNVoxelY*fVoxelXHalfOfY,
                                 fNVoxelZ*fVoxelXHalfOfZ );
  auto phantom_logic = new G4LogicalVolume(phantom_solid,
                                           G4water,
                                           "phantomVol",
                                           0, 0, 0);

  G4double Offset_X = 0.;
  G4double Offset_Y = 0.;
  G4double Offset_Z = fNVoxelZ*fVoxelXHalfOfZ;
  G4ThreeVector posCentreVoxels(Offset_X, Offset_Y, Offset_Z);

  G4cout << "placing voxel container volume at " << posCentreVoxels << G4endl;

  new G4PVPlacement(0,
                    posCentreVoxels,
                    phantom_logic,
                    "phantomVol",
                    world_logic,
                    false,
                    1);

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

  MyG4HWPhantomParameterisation* param
    = new MyG4HWPhantomParameterisation(voxelSize);
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
  logYRep->SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));
  //logYRep->SetVisAttributes(G4VisAttributes(true, G4Colour(1.0,0.0,0.0)));
  logXRep->SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));
  //logXRep->SetVisAttributes(G4VisAttributes(true, G4Colour(0.,1.0,0.0)));
  logicVoxel->SetVisAttributes(new G4VisAttributes( G4VisAttributes::GetInvisible()));
  //logicVoxel->SetVisAttributes(G4VisAttributes(true, G4Colour(0.,0.0,1.0)));

  if ( AnaMan->GetStepFlag() > 0)
  {
   //G4double maxStep = 1.0 * cm;
   //G4double maxStep = 1.0 * cm;
   G4double maxStep = AnaMan->GetStepFlag() * cm;
   G4cout << " running steplimiter: " << maxStep/cm << " cm" << std::endl;
   auto stepLimit = new G4UserLimits(maxStep);
   world_logic->SetUserLimits(stepLimit);
  }
  return world_phys;
}
