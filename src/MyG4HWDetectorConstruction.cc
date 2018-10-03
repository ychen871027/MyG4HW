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
  std::cout << "running MyG4HWDetectorConstruction..." << std::endl;
}

MyG4HWDetectorConstruction::~MyG4HWDetectorConstruction()
{
}

G4VPhysicalVolume* MyG4HWDetectorConstruction::Construct()
{

  auto AnaMan = MyG4HWAnalysis::Instance();
  fNVoxelX = AnaMan-> GetNoVoxelX();
  fNVoxelY = AnaMan-> GetNoVoxelY();
  fNVoxelZ = AnaMan-> GetNoVoxelZ();
  fVoxelXHalfOfX = AnaMan-> GetVoxelX();
  fVoxelXHalfOfY = AnaMan-> GetVoxelY();
  fVoxelXHalfOfZ = AnaMan-> GetVoxelZ();

  G4Material* G4air   = nullptr;
  G4Material* G4water = nullptr;

  G4Material* water = nullptr;
  G4Material* bone  = nullptr;
  G4Material* lung  = nullptr;

  G4NistManager* nist = G4NistManager::Instance();

  if (AnaMan-> GetMatType() == "g4air") {
    G4water = nist->FindOrBuildMaterial("G4_WATER");
    G4air   = nist->FindOrBuildMaterial("G4_AIR");

    water = nist->BuildMaterialWithNewDensity(      "G4waterWG4water",
                                                           "G4_WATER",
                                              G4water-> GetDensity());
    bone  = nist->BuildMaterialWithNewDensity(        "G4waterWbone",
                                                          "G4_WATER",
                                                        1.85*g/cm3 );
    lung  = nist->BuildMaterialWithNewDensity(        "G4waterWlung",
                                                          "G4_WATER",
                                                         0.26*g/cm3);

  } else if (AnaMan-> GetMatType() == "vg4water") {
    G4cout << "running from NIST G4WATER" << std::endl;
    G4water = nist->FindOrBuildMaterial("G4_WATER");
    G4Material* G4air_normal = nist->FindOrBuildMaterial("G4_AIR");

    G4air = nist->BuildMaterialWithNewDensity(             "G4airWG4water",
                                                                "G4_WATER",
                                              G4air_normal-> GetDensity());

    water = nist->BuildMaterialWithNewDensity(      "G4waterWG4water",
                                                           "G4_WATER",
                                              G4water-> GetDensity());

    bone  = nist->BuildMaterialWithNewDensity(        "G4waterWbone",
                                                          "G4_WATER",
                                                      1.85 * g/cm3 );

    lung  = nist->BuildMaterialWithNewDensity(        "G4waterWlung",
                                                          "G4_WATER",
                                                       0.26 * g/cm3);
                                                //G4air->GetDensity());

  } else if (AnaMan-> GetMatType() == "elewater") {
    G4cout << "running from element Water" << std::endl;
    G4Material* G4air_normal   = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* G4water_normal = nist->FindOrBuildMaterial("G4_WATER");
    G4double water_density =  G4water_normal->GetDensity();
    G4Element* elH = new G4Element("Hydrogen", "H", 1.0, 1.008 * g/mole);
    G4Element* elO = new G4Element("Oxygen", "O", 8.0, 16.00 * g/mole);

    G4air = new G4Material("air", G4air_normal->GetDensity(), 2);
    G4air->AddElement(elH,0.112);
    G4air->AddElement(elO,0.888);

    //water_density=0.26*g/cm3;//lung
    //water_density=1.85*g/cm3;//bone
    G4water = new G4Material("water", water_density, 2);
    G4water->AddElement(elH,0.112);
    G4water->AddElement(elO,0.888);

    water = new G4Material("waterWG4water", water_density, 2);
    water->AddElement(elH,0.112);
    water->AddElement(elO,0.888);
    bone = new G4Material( "waterWbone", 1.85*g/cm3, 2 );
    bone-> AddElement(elH,0.112);
    bone-> AddElement(elO,0.888);
    lung = new G4Material( "waterWlung", 0.26*g/cm3, 2 );
    lung-> AddElement(elH,0.112);
    lung-> AddElement(elO,0.888);

  }else{
    std::cout << "!!!Please configure your detector materials!!!" << std::endl;
  }

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

  fMaterials.clear();
  if (AnaMan->GetExDensity()) {
    std::cout << " importing the density from : " << AnaMan-> GetNameOfDenFile() << std::endl;
    std::ifstream findensity( AnaMan->GetNameOfDenFile() );
    for ( int iz=0; iz<fNVoxelZ; iz++) {
      for ( int iy=0; iy<fNVoxelY; iy++) {
        for ( int ix=0; ix<fNVoxelX; ix++) {
          int idx_id =  ix + iy*fNVoxelX + iz*fNVoxelX*fNVoxelY;
          int ixx;
          int iyy;
          int izz;
          double idensity;
          findensity >> ixx >> iyy >> izz >> idensity;
          G4String voxelMatname = "G4waterWdensity"+std::to_string(idx_id);

          //G4Material* voxel_density = nist->BuildMaterialWithNewDensity( voxelMatname,
          //                                                                      "G4_WATER",
          //                                                             idensity * (g/cm3));
       	  fMaterials.push_back(water);
       	  //fMaterials.push_back(voxel_density);
        }
      }
    }
  } else {
    for ( int iz=0; iz<fNVoxelZ; iz++) {
      for ( int iy=0; iy<fNVoxelY; iy++) {
        for ( int ix=0; ix<fNVoxelX; ix++) {
      	  //int idx_id =  ix + iy*fNVoxelX + iz*fNVoxelX*fNVoxelY;
      	  if (iz > 26 && iz <= 51) {
            if (AnaMan-> GetMidMat() == "bone") {
	      fMaterials.push_back(bone);
	    } else if (AnaMan-> GetMidMat() == "lung") {
	      fMaterials.push_back(lung);
	    } else {
      	      fMaterials.push_back(water);
	    }
	  } else {
      	    fMaterials.push_back(water);
	  }
        }
      }
    }
  }

  MyG4HWPhantomParameterisation* param
    = new MyG4HWPhantomParameterisation(voxelSize, fMaterials);
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
