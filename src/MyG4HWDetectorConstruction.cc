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
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* G4water = nist->FindOrBuildMaterial("G4_WATER");
 G4Material* G4air   = nist->FindOrBuildMaterial("G4_AIR");
 G4double air_density =  G4air->GetDensity();
 //
 //G4double air_density = 0.006 * G4water->GetDensity();

  std::cout << "G4water: density: " << G4water-> GetDensity() << " state: "
            << G4water-> GetState() << " temperature: " << G4water-> GetTemperature()
            << " pressure: " << G4water-> GetPressure() << " formula:"
            << G4water-> GetChemicalFormula() << std::endl;

  G4Material* G4air_normal  = nist->FindOrBuildMaterial("G4_AIR");
  // G4double air_density =  G4air_normal->GetDensity();
  // G4Element* elH = new G4Element( "Hydrogen", "H", 1.0, 1.008  * g/mole );
  //
  // G4Element* elO = new G4Element( "Oxygen", "O", 8.0, 16.00  * g/mole );
  //
  //
  // G4Material* G4air = new G4Material( "airWater", air_density, 2 );
  // G4air->AddElement(elH,0.112);
  // G4air->AddElement(elO,0.888);

  std::cout << "varwater: density: " << G4air-> GetDensity() << " state: "
            << G4air-> GetState() << " temperature: " << G4air-> GetTemperature()
            << " pressure: " << G4air-> GetPressure() << " formula:"
            << G4air-> GetChemicalFormula() << std::endl;


  /*G4Material* G4airsss    = new G4Material(
                                         "airtowaterwithairp",
                                        air_density,
                                        G4water,
                                        kStateGas,
                                        G4water-> GetTemperature(),
                                        G4water-> GetPressure());
  G4airsss-> GetIonisation()-> SetMeanExcitationEnergy(68.981*eV);
  G4airsss-> SetChemicalFormula(G4air-> GetChemicalFormula());
 */
 G4Material* G4airsss    = nist-> BuildMaterialWithNewDensity(
                                           "airtowaterwithairp",
                                                  "G4_WATER",
                                                 air_density);
  //G4Material* G4air   = nist->FindOrBuildMaterial("G4_Galactic");

  std::cout << "varG4water: density: " << G4airsss-> GetDensity() << " state: "
            << G4airsss-> GetState() << " temperature: " << G4airsss-> GetTemperature()
            << " pressure: " << G4airsss-> GetPressure() << " formula:"
            << G4airsss-> GetChemicalFormula() << std::endl;






  std::cout << "air density/water: "<< air_density << "?"
            << G4water->GetDensity() << std::endl;
  std::cout << "testcheny water Z/A/density: " << G4water ->GetNumberOfElements()
            << "/" << G4water ->GetNumberOfElements() << "/"
	    << G4water ->GetDensity()/(g/cm3) << " g/cm3"
            << std::endl;

  //std::cout << "testcheny air Z/A/density: " << G4air_normal ->GetNumberOfElements() << "/"
  //           << G4air_normal ->GetNumberOfElements() << "/" << G4air_normal ->GetDensity()/(g/cm3) << " g/cm3"
   //          << std::endl;

  //std::cout << "testcheny watervariable Z/A/density: " << G4air ->GetNumberOfElements() << "/"
  //          << G4air ->GetNumberOfElements() << "/" << G4air ->GetDensity()/(g/cm3) << " g/cm3"
  //          << std::endl;

  int NElements = G4air-> GetNumberOfElements();
  double matA = 0.;
  if ( NElements > 1 )
  {
    for (int ie = 0; ie < NElements; ie++)
    {
      double aOfElement = G4air-> GetElement(ie)-> GetA() / (g/mole);
      double massFraction = G4air-> GetFractionVector()[ie];
      matA += aOfElement * massFraction;
    }
  } else {
    matA += G4air->GetA() / (g/mole);
  }

  double matZ = 0.;
  if ( NElements > 1 )
  {
    for (int ie = 0; ie < NElements; ie++)
    {
      double zOfElement = G4air-> GetElement(ie)-> GetZ();
      double massFraction = G4air-> GetFractionVector()[ie];
      matZ += zOfElement * massFraction;
    }
  } else {
    matZ += G4air->GetA() / (g/mole);
  }

  int NElementsw = G4water-> GetNumberOfElements();
  double matwA = 0.;
  if ( NElementsw > 1 )
  {
    for (int ie = 0; ie < NElementsw; ie++)
    {
      double aOfElement = G4water-> GetElement(ie)-> GetA() / (g/mole);
      double massFraction = G4water-> GetFractionVector()[ie];
      matwA += aOfElement * massFraction;
    }
  } else {
    matwA += G4water->GetZ();
  }

  double matwZ = 0.;
  if ( NElementsw > 1 )
  {
    for (int ie = 0; ie < NElementsw; ie++)
    {
      double zOfElement = G4water-> GetElement(ie)-> GetZ();
      double massFraction = G4water-> GetFractionVector()[ie];
      matwZ += zOfElement * massFraction;
    }
  } else {
    matwZ += G4water->GetZ();
  }

   int NElementsn = G4air_normal-> GetNumberOfElements();
   double matnA = 0.;
   if ( NElementsn > 1 )
   {
     for (int ie = 0; ie < NElementsn; ie++)
     {
       double aOfElement = G4air_normal-> GetElement(ie)-> GetA() / (g/mole);
       double massFraction = G4air_normal-> GetFractionVector()[ie];
       matnA += aOfElement * massFraction;
     }
   } else {
     matnA += G4air_normal->GetZ();
   }

   double matnZ = 0.;
   if ( NElementsn > 1 )
   {
     for (int ie = 0; ie < NElementsn; ie++)
     {
       double zOfElement = G4air_normal-> GetElement(ie)-> GetZ();
       double massFraction = G4air_normal-> GetFractionVector()[ie];
       matnZ += zOfElement * massFraction;
     }
   } else {
     matnZ += G4air_normal->GetZ();
   }


  std::cout << __LINE__ << "test atomicA/Z: \nvariable water:" << matA << "/" << matZ
            << "\nG4 water: " << matwA << "/" << matwZ
            << "\nG4 Air: " << matnA << "/" << matnZ << std::endl;










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

   //G4double maxStep = 1.0 * cm;
   //auto stepLimit = new G4UserLimits(maxStep);
   //world_logic->SetUserLimits(stepLimit);
  return world_phys;
}
