
void reso()
{
  TF1* afc = new TF1("ATLAScalo","sqrt(0.5*0.5/x+0.03*0.03)",0.,500.);
  TF1* aft = new TF1("ATLAStrack","sqrt(5e-04*5e-04*x*x+0.01*0.01)",0.,500.);
  TF1* cfc = new TF1("CMScalo","sqrt(1./x+0.05*0.05)",0.,500.);
  TF1* cft = new TF1("CMStrack","sqrt(1.5e-04*5e-04*x*x+0.005*0.005)",0.,500.);

  TCanvas* a = new TCanvas("ATLAS","ATLAS");
  afc->SetLineColor(kRed);
  afc->Draw();
  aft->SetLineColor(kBlue);
  aft->Draw("same");
  a->Print("ATLAS.pdf");
  cfc->SetLineColor(kBlack);
  cfc->Draw("same");
  cft->SetLineColor(kGreen);
  cft->Draw("same");
  a->Print("ATLAS+CMS.pdf");
}
