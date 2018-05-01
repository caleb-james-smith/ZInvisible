#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"

int main()
{
    printf("Time to plot.\n");
    
    TFile *_file0 = TFile::Open("example.root");
    
    TH1* Baseline_Tau32 = nullptr;
    TH1* QCD_Tau32      = nullptr;
    TH1* ttbar_Tau32    = nullptr;
    TH1* Baseline_Tau21 = nullptr;
    TH1* QCD_Tau21      = nullptr;
    TH1* ttbar_Tau21    = nullptr;
    
    _file0->GetObject("Baseline_Tau32", Baseline_Tau32);
    _file0->GetObject("QCD_Tau32", QCD_Tau32);
    _file0->GetObject("ttbar_Tau32", ttbar_Tau32);
    _file0->GetObject("Baseline_Tau21", Baseline_Tau21);
    _file0->GetObject("QCD_Tau21", QCD_Tau21);
    _file0->GetObject("ttbar_Tau21", ttbar_Tau21);
    
    std::cout << Baseline_Tau32 << std::endl;
    std::cout << QCD_Tau32 << std::endl;
    std::cout << ttbar_Tau32 << std::endl;
    std::cout << Baseline_Tau21 << std::endl;
    std::cout << QCD_Tau21 << std::endl;
    std::cout << ttbar_Tau21 << std::endl;

    Baseline_Tau32->SetLineColor(kRed+1);
    QCD_Tau32->SetLineColor(kBlue+1);
    ttbar_Tau32->SetLineColor(kGreen+1);
    Baseline_Tau21->SetLineColor(kRed+1);
    QCD_Tau21->SetLineColor(kBlue+1);
    ttbar_Tau21->SetLineColor(kGreen+1);
    
    TCanvas* c1 = new TCanvas("c1","c1");
    Baseline_Tau32->Draw("hist E");
    QCD_Tau32->Draw("hist E same");
    ttbar_Tau32->Draw("hist E same");
    c1->Modified();
    c1->Update();
    c1->SaveAs("Tau32.pdf");

    TCanvas* c2 = new TCanvas("c2","c2");
    Baseline_Tau21->Draw("hist E");
    QCD_Tau21->Draw("hist E same");
    ttbar_Tau21->Draw("hist E same");
    c2->Modified();
    c2->Update();
    c2->SaveAs("Tau21.pdf");

}



