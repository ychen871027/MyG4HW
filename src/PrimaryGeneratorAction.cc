#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "PrimaryGeneratorAction.hh"
#include "MyG4HWPrimaryGeneratorAction.hh"
#include "MyG4HWAnalysis.hh"

namespace{
  G4ParticleGun* particleGun {nullptr};
}

PrimaryGeneratorAction::PrimaryGeneratorAction(G4ParticleGun* gun)
{
  ::particleGun = gun;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete ::particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle;
  //std::cout << "run GeneratePrimaries " << std::endl;
  auto AnaMan = MyG4HWAnalysis::Instance();
  //bool pencilBeam = false;
  bool pencilBeam = true;//false;
  G4String pName = AnaMan-> GetBeamName();
  pencilBeam = ( (AnaMan-> GetBeamType() == "pencil")? true : false );
  if (pencilBeam == true) {
    //particle = particleTable-> FindParticle( particleName="e-" );
    particle = particleTable-> FindParticle( particleName=pName );
    //particle = particleTable-> FindParticle( particleName="gamma" );
    ::particleGun -> SetParticleDefinition( particle );
    ::particleGun -> SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    //::particleGun -> SetParticleEnergy( 6.*MeV );
    double deK = atof(AnaMan-> GetXrayEnergy());
    
    ::particleGun -> SetParticleEnergy( deK *MeV );
    //::particleGun -> SetParticleEnergy( 20.*MeV );
    //std::cout << " you are running " << particle->GetParticleName() << " "
    //          << ::particleGun ->GetParticleEnergy() << " pencil beam " << std::endl;

    AnaMan-> Fill1DHist(1, deK, 1);
  }else{
    //std::cout << "you are running broad beam " << std::endl;
    particle = particleTable-> FindParticle( particleName=pName );
    ::particleGun -> SetParticleDefinition( particle );

    // photon energy spectrum
    const double kEbin = 250.*keV;
    int Nbin = 0;
    std::vector<double> vecpro;
    vecpro.clear();
    if( strcmp(AnaMan-> GetXrayEnergy(), "6MV") == 0 ){
      //std::cout << "runnging 6MV" << std::endl;
      // 6 MV
      enum { kSize6 = 29 };
      Nbin=kSize6;
      const double vec_rprob6[kSize6] = {
        0.,
        9.4485E-06, 9.5786E-06, 8.0766E-06, 6.8650E-06, 5.8836E-06, 5.0212E-06,
        4.2913E-06, 3.7020E-06, 3.2493E-06, 2.7831E-06, 2.4103E-06, 2.1283E-06,
        1.8404E-06, 1.5743E-06, 1.4005E-06, 1.2077E-06, 1.0442E-06, 8.6051E-07,
        7.4537E-07, 5.7857E-07, 4.3973E-07, 2.8741E-07, 1.2629E-07, 7.6060E-09,
        7.6060E-19, 7.6060E-19, 7.6060E-19, 7.6060E-24
      };
      for(int ik=0;ik<Nbin;ik++)vecpro.push_back(vec_rprob6[ik]);
    }else if( strcmp(AnaMan-> GetXrayEnergy(), "18MV") == 0 ){
      // 18 MV
      //std::cout << "runnging 18MV" << std::endl;
      enum { kSize18 = 76 };
      Nbin=kSize18;
      const double vec_rprob18[kSize18] = {
         0.,
         4.0337E-06, 9.5959E-06, 1.2065E-05, 1.3781E-05, 1.4319E-05, 1.4062E-05,
         1.3924E-05, 1.3086E-05, 1.2041E-05, 1.1453E-05, 1.0889E-05, 1.0112E-05,
         9.3359E-06, 8.7616E-06, 8.1366E-06, 7.6597E-06, 7.1010E-06, 6.7447E-06,
         6.2360E-06, 5.8474E-06, 5.4393E-06, 5.2179E-06, 4.8653E-06, 4.6058E-06,
         4.2906E-06, 4.0800E-06, 3.8501E-06, 3.6913E-06, 3.6273E-06, 3.2497E-06,
         3.1424E-06, 2.8892E-06, 2.8118E-06, 2.6369E-06, 2.5523E-06, 2.3753E-06,
         2.2205E-06, 2.1544E-06, 2.0812E-06, 2.0213E-06, 1.9223E-06, 1.7353E-06,
         1.7610E-06, 1.6670E-06, 1.5800E-06, 1.4863E-06, 1.3896E-06, 1.4569E-06,
         1.2893E-06, 1.2828E-06, 1.1637E-06, 1.0773E-06, 1.0395E-06, 1.0556E-06,
         9.6263E-07, 9.1664E-07, 8.9720E-07, 8.7556E-07, 7.7567E-07, 7.4345E-07,
         6.8121E-07, 6.3706E-07, 5.4163E-07, 5.6762E-07, 4.8476E-07, 4.3953E-07,
         4.0895E-07, 3.2791E-07, 2.8358E-07, 2.1152E-07, 1.3752E-07, 6.5072E-08,
         1.3752E-07, 6.5072E-08, 6.5072E-13
      };
      for(int ik=0;ik<Nbin;ik++)vecpro.push_back(vec_rprob18[ik]);
    } else {
      std::cout << "you need to set the beam energy " << std::endl;
    }

    //for(int ik=0;ik<Nbin;ik++)std::cout << ik << "/ssss " << vecpro[ik] << std::endl;
    const double fSSD = 100.*cm;
    const double fieldX = 10.*cm;
    const double fieldY = 10.*cm;
    double dr = sqrt( sqr(fieldX/2.) + sqr(fieldY/2.) );
    double sinAlpha = dr /fSSD;
    double cosAlpha = sqrt(1. - sqr(sinAlpha));
    double dcos = 0.;

    double dsin, dphi, z;
    double x = DBL_MAX;
    double y = DBL_MAX;
    double xmax, ymax;
    xmax = fieldX/2./fSSD;
    ymax = fieldY/2./fSSD;

    if (std::abs(x)<xmax && std::abs(y)<ymax)
    {
      //std::cout << "runnnntrue" << std::endl;
      //std::cout << std::abs(x) << "/" << xmax << std::endl;
    }
    while (!(std::abs(x)<xmax && std::abs(y)<ymax)) {
      dcos = G4RandFlat::shoot(cosAlpha, 1.0);
      dsin = sqrt(1.-sqr(dcos));
      dphi = G4RandFlat::shoot(0., CLHEP::twopi);

      x=cos(dphi)*dsin*dcos;
      y=sin(dphi)*dsin*dcos;

    }
    z=dcos;

    //std::cout << "generating the energy starting "<< x << "/" << y <<
    //          "/" << z << "/" << xmax << "/" << ymax << std::endl;
    //std::cout << sizeof(vecpro) << "/" << Nbin << std::endl;
    double deK = 0.;
    int iEbin = 0;
    double sumkEbin = 0.;
    //for(int ii=0; ii<Nbin; ii++) std::cout << "sum "<<sumkEbin << "/" << kEbin
    //                                       << "/" << vecpro[ii] << std::endl;
    for(int ii=0; ii<Nbin; ii++) sumkEbin += kEbin * vecpro[ii];
    while ( iEbin < Nbin){
      iEbin = Nbin * G4UniformRand();
      double slopeA = (vecpro[iEbin+1] - vecpro[iEbin]) / kEbin;
      double spoint = kEbin * G4UniformRand();
      double sppro  = vecpro[iEbin] + slopeA * spoint;

      double sumEbin1 = 0.;
      for(int ii=0; ii < iEbin; ii++) sumEbin1 += kEbin * vecpro[ii] / sumkEbin;
      //double sumEbin2 = sumEbin1 + kEbin * vec_rprob6[iEbin+1] / sumkEbin;
      double kBin = 0.;
      kBin = sumEbin1 + 0.5 * spoint * (vecpro[iEbin] + sppro) / sumkEbin;
      
      //std::cout <<sumkEbin << " kkkkk " << kBin << "/" << iEbin << "/" << Nbin << std::endl;
      //if (sumEbin1 <= kBin && kBin <= sumEbin2) {
      if ( kBin <= G4UniformRand() ) {
      //if ( kBin >= G4UniformRand() ) {
        deK  = kEbin*iEbin + spoint;// + G4UniformRand()*(kEbin);
        break;
      }
    }

    AnaMan-> Fill1DHist(1, deK, 1);
    vecpro.clear();

    //if (iEbin==1)
    //if( deK < 250.*keV )
    //std::cout << "generating the energy: " << deK << "iEbin" << iEbin << std::endl;
    //::particleGun ->SetParticleEnergy( 6.*MeV );
    ::particleGun -> SetParticleMomentumDirection(G4ThreeVector(x, y, z));
    ::particleGun ->SetParticleEnergy( deK );

    //std::cout << " you are running " << particle->GetParticleName() << " "
    //          << ::particleGun ->GetParticleEnergy() << " broad beam " << std::endl;
  }

  ::particleGun ->SetParticlePosition( G4ThreeVector(0.*cm,0.*cm,-100.*cm) );
  ::particleGun ->GeneratePrimaryVertex( anEvent );
}
