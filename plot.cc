{
  gROOT->Reset();
  TFile* infile = TFile::Open("MyG4HWDepInfo.root");
  TCanvas* edep = new TCanvas("Edep", "Energy desposit");
  edep->Divide(1,2);
  edep->cd(1);
  TTree*  Tdep  = (TTree*)infile->Get("MyG4HW");
  TH1D*   sdep  = new TH1D("sdep","sdep",150,0.,30.);
  Tdep->Draw("Pos_z>>sdep","(Pos_x<=0.5&&Pos_x>=-0.5)*Edep");
  sdep->Draw();
  edep->cd(2);
  TH1D*   sxdep  = new TH1D("sxdep","sxdep",150,-15.,15.);
  Tdep->Draw("Pos_x>>sxdep","(Pos_z<7.6)*Edep");
  sxdep->Draw();
  edep->Draw();
}
