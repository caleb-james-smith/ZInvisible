#include "../../SusyAnaTools/Tools/NTupleReader.h"
#include "../../SusyAnaTools/Tools/samples.h"
#include "../../SusyAnaTools/Tools/SATException.h"
#include "derivedTupleVariables.h"
#include "baselineDef.h"
#include "BTagCorrector.h"
#include "TTbarCorrector.h"
#include "ISRCorrector.h"
#include "PileupWeights.h"
#include "customize.h"

#include "TopTaggerResults.h"
#include "Constituent.h"

#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>

#include "math.h"

#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TFile.h"

void stripRoot(std::string &path)
{
    int dot = path.rfind(".root");
    if (dot != std::string::npos)
    {
        path.resize(dot);
    }
}

double SF_13TeV(double top_pt){

    return exp(0.0615-0.0005*top_pt);

}

bool filterEvents(NTupleReader& tr)
{
    const std::vector<TLorentzVector>& jetsLVec = tr.getVec<TLorentzVector>("jetsLVec");
    const double& met = tr.getVar<double>("met");

    return jetsLVec.size() >= 4;//met > 250;
}

class HistoContainer
{
private:
    std::vector<TH1*> histos;
    std::string csName_;

    template<typename H, typename... Args>
    H* bookHisto(const std::string& name, Args... args)
    {
        H* hptr = new H((csName_ + name).c_str(), (csName_ + name).c_str(), args...);
        hptr->Sumw2();
        histos.push_back(static_cast<TH1*>(hptr));
        return hptr;
    }

public:
    TH1* hMET;
    TH1* hHT;
    TH1* hMT2;
    TH1* hNJets;
    TH1* hNBJets;
    TH1* hNTops;
    TH1* hNVertices;

    HistoContainer(const std::string& csName = "") : csName_(csName)
    {
        if(csName_.size() > 0) csName_ += "_";

        hMET       = bookHisto<TH1D>("MET",100,0,2000);
        hHT        = bookHisto<TH1D>("HT", 100,0,2000);
        hMT2       = bookHisto<TH1D>("MT2",100,0,2000);
        hNJets     = bookHisto<TH1D>("nJets", 21,-0.5,20.5);
        hNBJets    = bookHisto<TH1D>("nBJets",21,-0.5,20.5);
        hNTops     = bookHisto<TH1D>("nTops", 21,-0.5,20.5);
        hNVertices = bookHisto<TH1D>("nVertices",61,-0.5, 60.5);

    }

    void fill(const NTupleReader& tr, const double& eWeight, TRandom* trand)
    {
        const double& met               = tr.getVar<double>("met");                         // MET
        const double& metphi            = tr.getVar<double>("metphi");
        const double& ht                = tr.getVar<double>("HTTopTag");                    // HT
        const double& mt2               = tr.getVar<double>("best_had_brJet_MT2TopTag");    // MT2
        const int&    vtxSize           = tr.getVar<int>("vtxSize");
        const int&    cntCSVS           = tr.getVar<int>("cntCSVSTopTag");                  // number of bottom jets
        const int&    nTops             = tr.getVar<int>("nTopCandSortedCntTopTag");        // number of top quarks
        const int&    cntNJetsPt30Eta24 = tr.getVar<int>("cntNJetsPt30Eta24TopTag");        // number of jets

        hMET->Fill(met, eWeight);
        hHT->Fill(ht, eWeight);
        hMT2->Fill(mt2, eWeight);
        hNJets->Fill(cntNJetsPt30Eta24, eWeight);
        hNBJets->Fill(cntCSVS, eWeight);
        hNTops->Fill(nTops, eWeight);
        hNVertices->Fill(vtxSize,eWeight);
        
    }

    void save(TFile *f)
    {
        f->cd();

        for(TH1* hist : histos) hist->Write();
    }
};

int main(int argc, char* argv[])
{

    std::string jetVecLabel           = "jetsLVec";

    int opt;
    int option_index = 0;

    static struct option long_options[] = {
        {"condor",             no_argument, 0, 'c'},
        {"TTbar weight",       no_argument, 0, 't'},
        {"no event weighting", no_argument, 0, 'd'},
        {"dataSets",     required_argument, 0, 'D'},
        {"numFiles",     required_argument, 0, 'N'},
        {"startFile",    required_argument, 0, 'M'},
        {"numEvts",      required_argument, 0, 'E'},
        {"output",       required_argument, 0, 'O'}
    };

    bool runOnCondor = false, enableTTbar = false, doWgt = true;
    int nFiles = -1, startFile = 0, nEvts = -1;
    std::string dataSets = "Signal_T2tt_mStop850_mLSP100", filename = "example.root";

    while((opt = getopt_long(argc, argv, "ctdD:N:M:E:O:", long_options, &option_index)) != -1)
    {
        switch(opt)
        {
        case 'c':
            runOnCondor = true;
            std::cout << "Configured for condor compatibility." << std::endl;
            break;

        case 't':
            enableTTbar = true;
            std::cout << "Enabled TTbar event weighting." << std::endl;
            break;

        case 'd':
            doWgt = false;
            std::cout << "No Event weighting." << std::endl;
            break;

        case 'D':
            dataSets = optarg;
            std::cout << "Running over the " << dataSets << " data sets." << std::endl;
            break;

        case 'N':
            nFiles = int(atoi(optarg));
            std::cout << "Running over " << nFiles << " files." << std::endl;
            break;

        case 'M':
            startFile = int(atoi(optarg));
            std::cout << "Starting on file #" << startFile << std::endl;
            break;

        case 'E':
            nEvts = int(atoi(optarg));
            std::cout << "Events: " << nEvts << std::endl;
            break;

        case 'O':
            filename = optarg;
            std::cout << "Filename: " << filename << std::endl;
        }
    }

    std::string sampleloc = AnaSamples::fileDir;
    //if running on condor override all optional settings
    if(runOnCondor)
    {
        char thistFile[128];
        stripRoot(filename);
        sprintf(thistFile, "%s_%s_%d.root", filename.c_str(), dataSets.c_str(), startFile);
        filename = thistFile;
        std::cout << "Filename modified for use with condor: " << filename << std::endl;
        sampleloc = "condor";
    }

    TH1::AddDirectory(false);

    bool savefile = true;
    if(filename == "-"){
        savefile = false;
        std::cout << "Histogram file will not be saved." << std::endl;
    }

    std::cout << "Sample location: " << sampleloc << std::endl;

    AnaSamples::SampleSet        ss(sampleloc, AnaSamples::luminosity);
    AnaSamples::SampleCollection sc(ss);

    if(dataSets.find("Data") != std::string::npos){
       std::cout << "This looks like a data n-tuple. No weighting will be applied." << std::endl;
       doWgt = false;
    }

    if(dataSets.find("TT") != std::string::npos){
       std::cout << "This looks like a TTbar sample. Applying TTbar weighting" << std::endl;
       enableTTbar = true;
    }

    std::cout << "Dataset: " << dataSets << std::endl;

    int events = 0, pevents = 0;

    HistoContainer histsBaseline("Baseline");
    HistoContainer histsSingleLepton("SingleLepton");
    HistoContainer histsQCD("QCD");
    HistoContainer histsTTbar("ttbar");

    TRandom* trand = new TRandom3();

    try
    {
        //for(auto& fs : sc[dataSets])
        auto& fs = ss[dataSets];
        {
            TChain *t = new TChain(fs.treePath.c_str());
            fs.addFilesToChain(t, startFile, nFiles);

            std::cout << "File: " << fs.filePath << std::endl;
            std::cout << "Tree: " << fs.treePath << std::endl;
            //std::cout << "sigma*lumi: " << fs.getWeight() << std::endl;
            
            BaselineVessel myBLVtoptag(*static_cast<NTupleReader*>(nullptr), "TopTag", "");
            BaselineVessel myBLVlostlepton(*static_cast<NTupleReader*>(nullptr), "lostlept", "");
            //plotterFunctions::PrepareTopVars prepareTopVars;
            plotterFunctions::TriggerInfo triggerInfo(false, false);
            
            BTagCorrector bTagCorrector("allINone_bTagEff.root", "", false);
            TTbarCorrector ttbarCorrector(false, "");
            ISRCorrector ISRcorrector("allINone_ISRJets.root","","");   
            Pileup_Sys pileup("PileupHistograms_0121_69p2mb_pm4p6.root");

            NTupleReader tr(t);
            tr.registerFunction(filterEvents);
            tr.registerFunction(myBLVtoptag);
            tr.registerFunction(myBLVlostlepton);
            //tr.registerFunction(prepareTopVars);
            tr.registerFunction(triggerInfo);
            tr.registerFunction(bTagCorrector);
            tr.registerFunction(ttbarCorrector);
            tr.registerFunction(ISRcorrector);
            tr.registerFunction(pileup);

            double fileWgt = fs.getWeight();

            const int printInterval = 1000;
            int printNumber = 0;
            while(tr.getNextEvent())
            {
                events++;
              
//                tr.printTupleMembers();
//                return 0;

                if(nEvts > 0 && tr.getEvtNum() > nEvts) break;
                if(tr.getEvtNum() / printInterval > printNumber) 
                {
                    printNumber = tr.getEvtNum() / printInterval;
                    std::cout << "Event #: " << printNumber * printInterval << std::endl;
                }

                const double& met    = tr.getVar<double>("met");
                const double& metphi = tr.getVar<double>("metphi");

                const bool&   passNoiseEventFilter = tr.getVar<bool>("passNoiseEventFilterTopTag");
                const bool&   passBaseline         = tr.getVar<bool>("passBaselineTopTag");
                const bool&   passLeptonVeto       = tr.getVar<bool>("passLeptVetoTopTag");
                const bool&   passBJets            = tr.getVar<bool>("passBJetsTopTag");
                const bool&   passnJets            = tr.getVar<bool>("passnJetsTopTag");
                const bool&   passdPhis            = tr.getVar<bool>("passdPhisTopTag");
                const double& ht                   = tr.getVar<double>("HTTopTag");
                
                const bool&   passSingleLepton     = tr.getVar<bool>("passBaselineSingleLeptonlostlept");

                const bool&   passMuTrigger        = tr.getVar<bool>("passMuTrigger");
                const bool&   passElecTrigger      = tr.getVar<bool>("passElecTrigger");
                const bool&   passMETMHTTrigger    = tr.getVar<bool>("passMETMHTTrigger");
                const bool&   passSearchTrigger    = tr.getVar<bool>("passSearchTrigger");
                const bool&   passHighHtTrigger    = tr.getVar<bool>("passHighHtTrigger");
                const bool&   passPhotonTrigger    = tr.getVar<bool>("passPhotonTrigger");

                const double isData = !tr.checkBranch("genDecayLVec");

                double eWeight = fileWgt;

                if(doWgt)
                {
                    const double& puWF               = tr.getVar<double>("_PUweightFactor");
                    const double& bTagWF             = tr.getVar<double>("bTagSF_EventWeightSimple_Central");
                    if(enableTTbar)
                    {
                        const double& ttbarWF        = tr.getVar<double>("TTbarWF");         
                        eWeight *= ttbarWF;
                    }
                    const double& triggerWF          = tr.getVar<double>("TriggerEffMC");

                    eWeight *= puWF * bTagWF * triggerWF;

                }

                int cntNJetsPt30Eta24 = AnaFunctions::countJets(tr.getVec<TLorentzVector>(jetVecLabel), AnaConsts::pt30Eta24Arr);


                //Baseline
                if( passBaseline )
                {
                    histsBaseline.fill(tr, eWeight, trand);
                }

                //Single Lepton
                if( passSingleLepton )
                {
                    histsSingleLepton.fill(tr, eWeight, trand);
                }

                //High HT QCD control sample
                if( (!isData || passHighHtTrigger)
                    && passNoiseEventFilter 
                    && passLeptonVeto
                    && passnJets
                    && (ht > 1000)
                    )
                {
                    histsQCD.fill(tr, eWeight, trand);
                }

                //semileptonic ttbar enriched control sample
                if( (!isData || passSearchTrigger)
                    && passNoiseEventFilter 
                    && passBJets
                    && passnJets
                    && passdPhis
                    && (ht > 300)
                    && (met > 250)
                    )
                {
                    histsTTbar.fill(tr, eWeight, trand);
                }
            }
        }
    }
    catch(const std::string e)
    {
        std::cout << e << std::endl;
        return 0;
    }
    catch(const TTException e)
    {
        std::cout << e << std::endl;
        return 0;
    }
    catch(const SATException e)
    {
        std::cout << e << std::endl;
        return 0;
    }

    std::cout << "Processed " << events << " events. " << pevents << " passed selection." << std::endl;

    if(savefile)
    {
        std::cout << "Saving root file..." << std::endl;

        TFile *f = new TFile(filename.c_str(),"RECREATE");
        if(f->IsZombie())
        {
            std::cout << "Cannot create " << filename << std::endl;
            throw "File is zombie";
        }

        histsBaseline.save(f);
        histsSingleLepton.save(f);
        histsQCD.save(f);
        histsTTbar.save(f);

        f->Write();
        f->Close();
    }
}
