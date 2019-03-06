#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "RooPlot.h"
#include "TH1D.h"

void Plot_ratio(){

    TGraphErrors graph1("./Method1.txt","%lg %lg %lg");
    TGraphErrors graph0("./Method2.txt","%lg %lg %lg");

    auto  mycanvas = new TCanvas();

    graph1.SetTitle("");
    graph1.GetXaxis()->SetTitle("p_{T} [GeV]");
    graph1.GetYaxis()->SetTitle("Ratio");
    graph1.GetYaxis()->SetRangeUser(0,1.2);
    graph1.SetLineColor(kBlue);
    graph1.SetMarkerColor(kBlue);
    graph1.SetMarkerStyle(33);
    graph1.Draw("ap");

    graph0.SetLineColor(kRed);
    graph0.SetMarkerColor(kRed);
    graph0.SetMarkerStyle(33);
    graph0.Draw("same p");

    TLegend leg(0.1,0.8,0.55,0.9,"Symmetry");
    leg.AddEntry(&graph0,"Folding along the line from - #frac{#pi}{2}  to #frac{#pi}{2}");
    leg.AddEntry(&graph1,"Folding along the line from 0 to #pi");

    leg.DrawClone("Same p");
    mycanvas->Print("Ratio.pdf");

}

int main(){
    Plot_ratio();
    }
