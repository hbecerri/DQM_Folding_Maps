#include <TH2.h>
#include <TH2F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLatex.h>
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"

void Folding_map(string var_eta, string var_phi, string cut, Double_t max = 0.0, Float_t x_bin = 0.0, Float_t y_bin = 0.0, Float_t f_bin = 0.0)
{

  TChain *chreco = new TChain("demo/ttree","");
  chreco->Add("*.root/demo/ttree");
  TTree *treereco = (TTree*) chreco;

  Long64_t nentries = treereco->GetEntries();
  cout << "Reco: " << nentries << endl;
  TH2F *Track = new TH2F("Track", "Track", 10, -3, 3, 10, -1.6, 1.6);
  TH2F *Track_Eta = new TH2F("Track_Eta", "", 10, -3, 3, 10, 0, 3.2);
  TH2F *Track_Eta2 = new TH2F("Track_Eta2", "", 10, -3, 3, 10, -1.6, 1.6);
  TH2F *Track_Eta_Phi_P = new TH2F("Track_Eta_Phi_P", "Track_Eta_Phi_P", 10, -3, 3, 10, 0, 3.2);
  TH2F *Track_Eta_Phi_N = new TH2F("Track_Eta_Phi_N", "Track_Eta_Phi_N", 10, -3, 3, 10, -3.2, 0);
  TH2F *Rearrange_N = new TH2F("Rearrange_N", "Rearrange_N", 10, -3, 3, 10, 0, 3.2);
  TH2F *Rearrange_N2 = new TH2F("Rearrange_N2", "Rearrange_N2", 10, -3, 3, 10, -1.6, 1.6);

  string cutpt = Form("(%s) >= (%f)",cut.c_str(),max);
  treereco->Project("T",Form("%s:%s",var_eta.c_str(), var_phi.c_str()),cutpt.c_str());
  treereco->Project("Track_Eta2",Form("%s:%s",var_eta.c_str(), var_phi.c_str()),cutpt.c_str());
  treereco->Project("Track_Eta_Phi_P",Form("%s:%s",var_eta.c_str(), var_phi.c_str()),cutpt.c_str());
  treereco->Project("Track_Eta_Phi_N",Form("%s:%s",var_eta.c_str(), var_phi.c_str()),cutpt.c_str());
  treereco->Project("Track_Eta_Phi_P_N",Form("%s:%s",var_eta.c_str(), var_phi.c_str()),cutpt.c_str());
    
  int nx = Track_Eta_Phi_N->GetXaxis()->GetNbins();
  int ny = Track_Eta_Phi_N->GetYaxis()->GetNbins();
  for (int i=1;i<=nx;i++) {    
  for (int j=1;j<=ny;j++) {
     
  double c1 =  Track_Eta_Phi_N->GetBinContent(i,j);
  Rearrange_N->SetBinContent(i,11-j,c1);
      
  }
  } 

  TCanvas *C = new TCanvas("My Canvas","",1200,900);
  C->Divide(1,2);
  C->cd(1);

  double c2 = Track_Eta_Phi_P->GetBinContent(x_bin,y_bin);
  double c1 = Rearrange_N->GetBinContent(x_bin,y_bin);

  if (c2<c1)  {
  double c3 = (1/c1)*sqrt(c2*(1-(c2/c1)));
  Track_Eta->Divide(Track_Eta_Phi_P,Rearrange_N,1.0,1.0);
  double d1 = Track_Eta->GetBinContent(x_bin,y_bin);
  ofstream myout("Method1.txt", ios::app);
  myout << max << "\t"  << d1 << "\t" << c3 << endl;
  }

  if (c2>=c1) {

  double c3 = (1/c2)*sqrt(c1*(1-(c1/c2)));
  Track_Eta->Divide(Rearrange_N,Track_Eta_Phi_P,1.0,1.0);
  double d1 = Track_Eta->GetBinContent(x_bin,y_bin);
  ofstream myout("Method1.txt", ios::app);
  myout << max << "\t"  << d1 << "\t" << c3 << endl;
  }

  if (max==0) {
    gStyle->SetOptStat(0);
    Track_Eta->Draw("colz");
    Track_Eta->SetTitle("Track_Eta_Phi Efficiency_Map");
    Track_Eta->GetXaxis()->SetTitle("Track #eta");
    Track_Eta->GetYaxis()->SetTitle("Track #phi");
    Track_Eta->GetZaxis()->SetTitle("Ratio #phi_{[0,#pi]}");
      
  }
/*
    TLatex latex;

    latex.SetTextSize(0.07);
    latex.SetTextFont(61);
    latex.DrawLatex(-3.0,3.25,"CMS");

    latex.SetTextSize(0.04);
    latex.SetTextFont(52);
    latex.DrawLatex(-2.3,3.25,"Preliminary");

    latex.SetTextSize(0.042);
    latex.SetTextFont(42);
    latex.DrawLatex(1.75,3.25,"#bf{#bf{470 pb^{-1} (13TeV)}}");
*/
      
      
      C->cd(2);
      
    int mx = Track_Eta_Phi_P->GetXaxis()->GetNbins();
    int my = Track_Eta_Phi_N->GetYaxis()->GetNbins();
    for (int i=1;i<=mx;i++) {
        for (int j=1;j<=5;j++) {
            double m1 =  Track_Eta_Phi_N->GetBinContent(i,j);
            Rearrange_N2->SetBinContent(i,6-j,m1);
        }
    }
    
    for (int i=1;i<=mx;i++) {
        for (int j=6;j<=my;j++) {
            double n1 =  Track_Eta_Phi_P->GetBinContent(i,j);
            Rearrange_N2->SetBinContent(i,16-j,n1);
        }
    }
  

    double e2 = Track_Eta2->GetBinContent(x_bin,f_bin);
    double e1 = Rearrange_N2->GetBinContent(x_bin,f_bin);
    if (e2<e1) {
        double e3 = (1/e1)*sqrt(e2*(1-(e2/e1)));
        Track->Divide(Track_Eta2,Rearrange_N2,1.0,1.0);
        double f1 = Track->GetBinContent(x_bin,f_bin);
        ofstream myout("Method2.txt", ios::app);
        myout << max << "\t"  << f1 << "\t" << e3 << endl;
    }
    
    if (e2>=e1) {
        double e3 = (1/e2)*sqrt(e1*(1-(e1/e2)));
        Track->Divide(Rearrange_N2,Track_Eta2,1.0,1.0);
        double f1 = Track->GetBinContent(x_bin,f_bin);
        ofstream myout("Method2.txt", ios::app);
        myout << max << "\t"  << f1 << "\t" << e3 << endl;
    }
    
    if (max==0) {
        gStyle->SetOptStat(0);
        Track->Draw("colz");
        Track->SetTitle("");
        Track->GetXaxis()->SetTitle("Track #eta");
        Track->GetYaxis()->SetTitle("Track #phi");
        Track->GetZaxis()->SetTitle("Ratio #phi_{[- #frac{#pi}{2},#frac{#pi}{2}]}");
        
        C->Print("Folded_tracking_map.pdf");
    }

  
}  

