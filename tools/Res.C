void Res(const char* filename)
{
   TFile* f = new TFile(Form("%s.root", filename));
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(10);

   TCanvas* c = new TCanvas(Form("c_%s", filename),filename, 0, 0, 1600, 800);
   c->Divide(3,1);
   TLatex latex;
   latex.SetTextSize(0.035);
   latex.SetTextAlign(12);
   double tres, eres, hres, pX,pY,pZ;
   TTree *t = (TTree*)f->Get("Event_0");
   t->SetBranchAddress("tracker_res",&tres);
   t->SetBranchAddress("emcal_res",&eres);
   t->SetBranchAddress("hcal_res",&hres);
   t->SetBranchAddress("MC_pX",&pX);
   t->SetBranchAddress("MC_pY",&pY);
   t->SetBranchAddress("MC_pZ",&pZ);
   t->GetEntry(0);
   c->cd(1);
   TH1D* hP = (TH1D*)f->Get("Pdiff");
   hP->Fit("gaus");
   hP->Draw();
   hP->SetTitle(Form("%s: %s", filename, hP->GetTitle()));
   hP->GetXaxis()->SetRangeUser(-1000,1000);
   if(hP->GetEntries())
   {
      latex.DrawLatexNDC(0.15,0.85 ,Form("#color[4]{resolution: %.1f%}",tres*100 ));
      latex.DrawLatexNDC(0.15,0.8 ,Form("#color[4]{p = %.0f GeV}",sqrt(pX*pX+pY*pY+pZ*pZ)/1.e3 ));
   }

   c->cd(2);
   TH1D* hEE = (TH1D*)f->Get("EMCalEdiff");
   hEE->Fit("gaus");
   hEE->Draw();
   hEE->SetTitle(Form("%s: %s", filename, hEE->GetTitle()));
   hEE->GetXaxis()->SetRangeUser(-200,200);
   if(hEE->GetEntries())
   {
      latex.DrawLatexNDC(0.15,0.85,Form("#color[4]{resolution: %.1f%}",eres*100 ));
      latex.DrawLatexNDC(0.15,0.8 ,Form("#color[4]{p = %.0f GeV}",sqrt(pX*pX+pY*pY+pZ*pZ)/1.e3 ));
   }
   c->cd(3);
   TH1D* hEH = (TH1D*)f->Get("HCalEdiff");
   hEH->Fit("gaus");
   hEH->Draw();
   hEH->SetTitle(Form("%s: %s", filename, hEH->GetTitle()));
   hEH->GetXaxis()->SetRangeUser(-2500,2500);
   if(hEH->GetEntries())
   {
      latex.DrawLatexNDC(0.15,0.85,Form("#color[4]{resolution: %.1f%}",hres*100 ));
      latex.DrawLatexNDC(0.15,0.8 ,Form("#color[4]{p = %.0f GeV}",sqrt(pX*pX+pY*pY+pZ*pZ)/1.e3 ));
   }
}
