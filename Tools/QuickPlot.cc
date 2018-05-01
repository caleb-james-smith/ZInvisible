// QuickPlot.cc
#include "QuickPlot.h"

#include "TChain.h"
#include "TFile.h"
#include "TLegend.h"
#include "TCanvas.h"

#include <iostream>
#include <stdio.h>

QuickPlot::QuickPlot()
{
    m_directory = "singleLeptonPlots";
    m_colors.push_back(kBlue-3);
    m_colors.push_back(kRed+2);
    m_colors.push_back(kAzure-2);
    m_colors.push_back(kGreen+2);
    m_colors.push_back(kViolet-6);
}

void QuickPlot::plot(std::map<std::string, TH1*> histos, std::string p_title, std::string x_title, std::string y_title)
{
    printf("Running QuickPlot::plot() for %s\n", p_title.c_str());
    TCanvas* c1 = new TCanvas("c1","c1");
    
    //Create TLegend: TLegend(x1, y1, x2, y2)
    TLegend *leg = new TLegend(0.65, 0.60, 0.85, 0.70);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetLineWidth(1);
    leg->SetNColumns(1);
    leg->SetTextFont(42);
    
    // iterate over histograms and plot on same canvas 
    int i = 0;
    std::map<std::string, TH1*>::iterator it;
    for(it=histos.begin(); it!=histos.end(); ++it)
    {
        // name will be for legend; h is histogram
        std::string name = it->first;
        TH1* h = it->second;

        leg->AddEntry(h, name.c_str(), "F");
        
        h->SetTitle(p_title.c_str());
        h->GetXaxis()->SetTitle(x_title.c_str());
        h->GetYaxis()->SetTitle(y_title.c_str());
        h->SetLineColor(m_colors[i % m_colors.size()]);
        
        if(i==0)    h->Draw("hist E");
        else        h->Draw("hist E same");
        
        i += 1;
    }
    
    //plot legend
    leg->Draw("same");

    std::string fileName;
    c1->Modified();
    c1->Update();
    
    fileName = m_directory + "/" + p_title + ".pdf";
    c1->SaveAs(fileName.c_str());
    fileName = m_directory + "/" + p_title + ".png";
    c1->SaveAs(fileName.c_str());
    
    // delete canvas to clean up dynamic memory
    if (c1) { c1->Close(); delete c1; c1 = 0; } 
    
}

int main()
{
    
    TFile *_file0 = TFile::Open("example.root");
   
    TH1* Baseline_MET       = nullptr;
    TH1* Baseline_HT        = nullptr;
    TH1* Baseline_MT2       = nullptr;
    TH1* Baseline_nJets     = nullptr;
    TH1* Baseline_nBJets    = nullptr;
    TH1* Baseline_nTops     = nullptr;
    TH1* Baseline_nVertices = nullptr;

    TH1* SingleLepton_MET       = nullptr;
    TH1* SingleLepton_HT        = nullptr;
    TH1* SingleLepton_MT2       = nullptr;
    TH1* SingleLepton_nJets     = nullptr;
    TH1* SingleLepton_nBJets    = nullptr;
    TH1* SingleLepton_nTops     = nullptr;
    TH1* SingleLepton_nVertices = nullptr;

    _file0->GetObject("Baseline_MET",       Baseline_MET);
    _file0->GetObject("Baseline_HT",        Baseline_HT);
    _file0->GetObject("Baseline_MT2",       Baseline_MT2);
    _file0->GetObject("Baseline_nJets",     Baseline_nJets);
    _file0->GetObject("Baseline_nBJets",    Baseline_nBJets);
    _file0->GetObject("Baseline_nTops",     Baseline_nTops);
    _file0->GetObject("Baseline_nVertices", Baseline_nVertices);
    
    _file0->GetObject("SingleLepton_MET",       SingleLepton_MET);
    _file0->GetObject("SingleLepton_HT",        SingleLepton_HT);
    _file0->GetObject("SingleLepton_MT2",       SingleLepton_MT2);
    _file0->GetObject("SingleLepton_nJets",     SingleLepton_nJets);
    _file0->GetObject("SingleLepton_nBJets",    SingleLepton_nBJets);
    _file0->GetObject("SingleLepton_nTops",     SingleLepton_nTops);
    _file0->GetObject("SingleLepton_nVertices", SingleLepton_nVertices);
    
    std::map<std::string, TH1*> baselineHistos;
    std::map<std::string, TH1*> singleLeptonHistos;
    
    baselineHistos.insert(std::pair<std::string, TH1*>("MET",        Baseline_MET));
    baselineHistos.insert(std::pair<std::string, TH1*>("HT",         Baseline_HT));
    baselineHistos.insert(std::pair<std::string, TH1*>("MT2",        Baseline_MT2));
    baselineHistos.insert(std::pair<std::string, TH1*>("nJets",      Baseline_nJets));
    baselineHistos.insert(std::pair<std::string, TH1*>("nBJets",     Baseline_nBJets));
    baselineHistos.insert(std::pair<std::string, TH1*>("nTops",      Baseline_nTops));
    baselineHistos.insert(std::pair<std::string, TH1*>("nVertices",  Baseline_nVertices));

    singleLeptonHistos.insert(std::pair<std::string, TH1*>("MET",        SingleLepton_MET));
    singleLeptonHistos.insert(std::pair<std::string, TH1*>("HT",         SingleLepton_HT));
    singleLeptonHistos.insert(std::pair<std::string, TH1*>("MT2",        SingleLepton_MT2));
    singleLeptonHistos.insert(std::pair<std::string, TH1*>("nJets",      SingleLepton_nJets));
    singleLeptonHistos.insert(std::pair<std::string, TH1*>("nBJets",     SingleLepton_nBJets));
    singleLeptonHistos.insert(std::pair<std::string, TH1*>("nTops",      SingleLepton_nTops));
    singleLeptonHistos.insert(std::pair<std::string, TH1*>("nVertices",  SingleLepton_nVertices));

    std::map<std::string, TH1*>::iterator it;
    printf("------- Baseline -----------\n");
    for(it=baselineHistos.begin(); it!=baselineHistos.end(); ++it)          printf("%-15s : 0x%x\n", it->first.c_str(), it->second);
    printf("------- SingleLepton -------\n");
    for(it=singleLeptonHistos.begin(); it!=singleLeptonHistos.end(); ++it)  printf("%-15s : 0x%x\n", it->first.c_str(), it->second);
    
    // make one plot for each variable
    QuickPlot qp = QuickPlot();
    for(it=baselineHistos.begin(); it!=baselineHistos.end(); ++it)
    {
        std::map<std::string, TH1*> histoMap;
        histoMap.insert(std::pair<std::string, TH1*>("Baseline", it->second));
        histoMap.insert(std::pair<std::string, TH1*>("SingleLepton", singleLeptonHistos[it->first]));
        qp.plot(histoMap, it->first + "_Distribution", it->first, "events");
    }
    
    
    //for(auto& histo : singleLeptonHistos)   std::cout << "this should be nonzero: " << histo << std::endl;

    //QuickPlot qp = QuickPlot();
    //qp.plot(baselineHistos, "Baseline");

    // Baseline->SetLineColor(kRed+1);
    
    // TCanvas* c1 = new TCanvas("c1","c1");
    // Baseline->Draw("hist E");
    // c1->Modified();
    // c1->Update();
    // c1->SaveAs("MET.pdf");

    // TCanvas* c2 = new TCanvas("c2","c2");
    // c2->Modified();
    // c2->Update();

}


