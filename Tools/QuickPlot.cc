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
    m_directory = "leptonPlots";
    m_colors.push_back(kBlue-3);
    m_colors.push_back(kRed+2);
    m_colors.push_back(kAzure-2);
    m_colors.push_back(kGreen+2);
    m_colors.push_back(kViolet-6);
    m_colors.push_back(kOrange+7);
    m_colors.push_back(kCyan+2);
    m_colors.push_back(kPink-8);
    m_colors.push_back(kTeal+3);
    m_colors.push_back(kYellow+2);
}

void QuickPlot::plot(std::map<std::string, TH1*> histos, std::vector<std::string> histoNames, std::string p_title, std::string x_title, std::string y_title)
{
    printf("Running QuickPlot::plot() for %s\n", p_title.c_str());
    TCanvas* c1 = new TCanvas("c1","c1");
    
    //Create TLegend: TLegend(x1, y1, x2, y2)
    TLegend *leg = new TLegend(0.65, 0.50, 0.85, 0.70);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetLineWidth(1);
    leg->SetNColumns(1);
    leg->SetTextFont(42);
    
    // iterate over histograms and plot on same canvas 
    // use histoNames to maintain histo order
    int i = 0;
    //std::map<std::string, TH1*>::iterator it;
    for(auto& histoName : histoNames)
    //for(it=histos.begin(); it!=histos.end(); ++it)
    {
        // name will be for legend; h is histogram
        //std::string histoName = it->first;
        TH1* h = histos[histoName];

        leg->AddEntry(h, histoName.c_str(), "F");
        
        h->SetTitle(p_title.c_str());
        h->GetXaxis()->SetTitle(x_title.c_str());
        h->GetYaxis()->SetTitle(y_title.c_str());
        h->SetLineColor(m_colors[i % m_colors.size()]);
        
        // begin with tallest histogram so that it will set the range
        if(i==0)    h->Draw("hist E");
        else        h->Draw("hist E same");
        
        i += 1;
    }
    
    //plot legend
    leg->Draw("same");

    std::string fileName;
    c1->Modified();
    c1->Update();
    
    // save pdf
    fileName = m_directory + "/" + p_title + ".pdf";
    c1->SaveAs(fileName.c_str());
    // png is poor quality: use script to convert pdf to png
    // fileName = m_directory + "/" + p_title + ".png";
    // c1->SaveAs(fileName.c_str());
    
    // delete canvas to clean up dynamic memory
    if (c1) { c1->Close(); delete c1; c1 = 0; } 
    
}

int main()
{
    
    TFile *_file0 = TFile::Open("example.root");
   
    TH1* AllEvents_MET       = nullptr;
    TH1* AllEvents_HT        = nullptr;
    TH1* AllEvents_MT2       = nullptr;
    TH1* AllEvents_nJets     = nullptr;
    TH1* AllEvents_nBJets    = nullptr;
    TH1* AllEvents_nTops     = nullptr;
    TH1* AllEvents_nVertices = nullptr;

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

    TH1* DoubleLepton_MET       = nullptr;
    TH1* DoubleLepton_HT        = nullptr;
    TH1* DoubleLepton_MT2       = nullptr;
    TH1* DoubleLepton_nJets     = nullptr;
    TH1* DoubleLepton_nBJets    = nullptr;
    TH1* DoubleLepton_nTops     = nullptr;
    TH1* DoubleLepton_nVertices = nullptr;

    TH1* QCD_MET       = nullptr;
    TH1* QCD_HT        = nullptr;
    TH1* QCD_MT2       = nullptr;
    TH1* QCD_nJets     = nullptr;
    TH1* QCD_nBJets    = nullptr;
    TH1* QCD_nTops     = nullptr;
    TH1* QCD_nVertices = nullptr;

    TH1* ttbar_MET       = nullptr;
    TH1* ttbar_HT        = nullptr;
    TH1* ttbar_MT2       = nullptr;
    TH1* ttbar_nJets     = nullptr;
    TH1* ttbar_nBJets    = nullptr;
    TH1* ttbar_nTops     = nullptr;
    TH1* ttbar_nVertices = nullptr;

    _file0->GetObject("AllEvents_MET",       AllEvents_MET);
    _file0->GetObject("AllEvents_HT",        AllEvents_HT);
    _file0->GetObject("AllEvents_MT2",       AllEvents_MT2);
    _file0->GetObject("AllEvents_nJets",     AllEvents_nJets);
    _file0->GetObject("AllEvents_nBJets",    AllEvents_nBJets);
    _file0->GetObject("AllEvents_nTops",     AllEvents_nTops);
    _file0->GetObject("AllEvents_nVertices", AllEvents_nVertices);
    
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
    
    _file0->GetObject("DoubleLepton_MET",       DoubleLepton_MET);
    _file0->GetObject("DoubleLepton_HT",        DoubleLepton_HT);
    _file0->GetObject("DoubleLepton_MT2",       DoubleLepton_MT2);
    _file0->GetObject("DoubleLepton_nJets",     DoubleLepton_nJets);
    _file0->GetObject("DoubleLepton_nBJets",    DoubleLepton_nBJets);
    _file0->GetObject("DoubleLepton_nTops",     DoubleLepton_nTops);
    _file0->GetObject("DoubleLepton_nVertices", DoubleLepton_nVertices);
    
    _file0->GetObject("QCD_MET",       QCD_MET);
    _file0->GetObject("QCD_HT",        QCD_HT);
    _file0->GetObject("QCD_MT2",       QCD_MT2);
    _file0->GetObject("QCD_nJets",     QCD_nJets);
    _file0->GetObject("QCD_nBJets",    QCD_nBJets);
    _file0->GetObject("QCD_nTops",     QCD_nTops);
    _file0->GetObject("QCD_nVertices", QCD_nVertices);
    
    _file0->GetObject("ttbar_MET",       ttbar_MET);
    _file0->GetObject("ttbar_HT",        ttbar_HT);
    _file0->GetObject("ttbar_MT2",       ttbar_MT2);
    _file0->GetObject("ttbar_nJets",     ttbar_nJets);
    _file0->GetObject("ttbar_nBJets",    ttbar_nBJets);
    _file0->GetObject("ttbar_nTops",     ttbar_nTops);
    _file0->GetObject("ttbar_nVertices", ttbar_nVertices);
    
    std::map<std::string, TH1*> allEventsHistos;
    std::map<std::string, TH1*> baselineHistos;
    std::map<std::string, TH1*> singleLeptonHistos;
    std::map<std::string, TH1*> doubleLeptonHistos;
    std::map<std::string, TH1*> qcdHistos;
    std::map<std::string, TH1*> ttbarHistos;
    
    allEventsHistos.insert(std::pair<std::string, TH1*>("MET",        AllEvents_MET));
    allEventsHistos.insert(std::pair<std::string, TH1*>("HT",         AllEvents_HT));
    allEventsHistos.insert(std::pair<std::string, TH1*>("MT2",        AllEvents_MT2));
    allEventsHistos.insert(std::pair<std::string, TH1*>("nJets",      AllEvents_nJets));
    allEventsHistos.insert(std::pair<std::string, TH1*>("nBJets",     AllEvents_nBJets));
    allEventsHistos.insert(std::pair<std::string, TH1*>("nTops",      AllEvents_nTops));
    allEventsHistos.insert(std::pair<std::string, TH1*>("nVertices",  AllEvents_nVertices));

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

    doubleLeptonHistos.insert(std::pair<std::string, TH1*>("MET",        DoubleLepton_MET));
    doubleLeptonHistos.insert(std::pair<std::string, TH1*>("HT",         DoubleLepton_HT));
    doubleLeptonHistos.insert(std::pair<std::string, TH1*>("MT2",        DoubleLepton_MT2));
    doubleLeptonHistos.insert(std::pair<std::string, TH1*>("nJets",      DoubleLepton_nJets));
    doubleLeptonHistos.insert(std::pair<std::string, TH1*>("nBJets",     DoubleLepton_nBJets));
    doubleLeptonHistos.insert(std::pair<std::string, TH1*>("nTops",      DoubleLepton_nTops));
    doubleLeptonHistos.insert(std::pair<std::string, TH1*>("nVertices",  DoubleLepton_nVertices));

    qcdHistos.insert(std::pair<std::string, TH1*>("MET",        QCD_MET));
    qcdHistos.insert(std::pair<std::string, TH1*>("HT",         QCD_HT));
    qcdHistos.insert(std::pair<std::string, TH1*>("MT2",        QCD_MT2));
    qcdHistos.insert(std::pair<std::string, TH1*>("nJets",      QCD_nJets));
    qcdHistos.insert(std::pair<std::string, TH1*>("nBJets",     QCD_nBJets));
    qcdHistos.insert(std::pair<std::string, TH1*>("nTops",      QCD_nTops));
    qcdHistos.insert(std::pair<std::string, TH1*>("nVertices",  QCD_nVertices));

    ttbarHistos.insert(std::pair<std::string, TH1*>("MET",        ttbar_MET));
    ttbarHistos.insert(std::pair<std::string, TH1*>("HT",         ttbar_HT));
    ttbarHistos.insert(std::pair<std::string, TH1*>("MT2",        ttbar_MT2));
    ttbarHistos.insert(std::pair<std::string, TH1*>("nJets",      ttbar_nJets));
    ttbarHistos.insert(std::pair<std::string, TH1*>("nBJets",     ttbar_nBJets));
    ttbarHistos.insert(std::pair<std::string, TH1*>("nTops",      ttbar_nTops));
    ttbarHistos.insert(std::pair<std::string, TH1*>("nVertices",  ttbar_nVertices));

    std::map<std::string, TH1*>::iterator it;
    printf("------- AllEvents ----------\n");
    for(it=allEventsHistos.begin(); it!=allEventsHistos.end(); ++it)        printf("%-15s : 0x%x\n", it->first.c_str(), it->second);
    printf("------- Baseline -----------\n");
    for(it=baselineHistos.begin(); it!=baselineHistos.end(); ++it)          printf("%-15s : 0x%x\n", it->first.c_str(), it->second);
    printf("------- SingleLepton -------\n");
    for(it=singleLeptonHistos.begin(); it!=singleLeptonHistos.end(); ++it)  printf("%-15s : 0x%x\n", it->first.c_str(), it->second);
    printf("------- DoubleLepton -------\n");
    for(it=doubleLeptonHistos.begin(); it!=doubleLeptonHistos.end(); ++it)  printf("%-15s : 0x%x\n", it->first.c_str(), it->second);
    printf("------- QCD ----------------\n");
    for(it=qcdHistos.begin(); it!=qcdHistos.end(); ++it)                    printf("%-15s : 0x%x\n", it->first.c_str(), it->second);
    printf("------- ttbar ----------------\n");
    for(it=ttbarHistos.begin(); it!=ttbarHistos.end(); ++it)                printf("%-15s : 0x%x\n", it->first.c_str(), it->second);
    
    // order histograms in vector
    std::vector<std::string> histoNames;
    histoNames.push_back("AllEvents");
    histoNames.push_back("Baseline");
    histoNames.push_back("SingleLepton");
    histoNames.push_back("DoubleLepton");
    //histoNames.push_back("QCD");
    //histoNames.push_back("ttbar");
    // make one plot for each variable
    QuickPlot qp = QuickPlot();
    for(it=allEventsHistos.begin(); it!=allEventsHistos.end(); ++it)
    {
        std::string variable = it->first;
        std::map<std::string, TH1*> histoMap;
        
        // begin with tallest histogram so that it will set the range
        histoMap.insert(std::pair<std::string, TH1*>("AllEvents",       allEventsHistos[variable]));
        histoMap.insert(std::pair<std::string, TH1*>("Baseline",        baselineHistos[variable]));
        histoMap.insert(std::pair<std::string, TH1*>("SingleLepton",    singleLeptonHistos[variable]));
        histoMap.insert(std::pair<std::string, TH1*>("DoubleLepton",    doubleLeptonHistos[variable]));
        //histoMap.insert(std::pair<std::string, TH1*>("QCD",             qcdHistos[variable]));
        //histoMap.insert(std::pair<std::string, TH1*>("ttbar",           ttbarHistos[variable]));
        
        qp.plot(histoMap, histoNames, variable + "_Distribution", variable, "weighted events");
    }
    
}

