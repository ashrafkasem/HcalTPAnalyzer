#include <iostream>

#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "UserCode/HcalTPAnalyzer/interface/MacroUtils.h"
#include "UserCode/HcalTPAnalyzer/interface/HcalEvent.h"
#include "UserCode/HcalTPAnalyzer/interface/DataEvtSummaryHandler.h"
#include "UserCode/HcalTPAnalyzer/interface/SmartSelectionMonitor.h"


#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TMath.h"


using namespace std;


//  offline thresholds
const int adc_threshold_hbhe=1;
const int adc_threshold_hf=5;
const float tp_threshold=0.5;

int main(int argc, char* argv[])
{
    //##################################################################################
    //##########################    GLOBAL INITIALIZATION     ##########################
    //##################################################################################

    // check arguments
    if(argc<2) {
        std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
        exit(0);
    }

    // load framework libraries
    gSystem->Load( "libFWCoreFWLite" );
    //AutoLibraryLoader::enable();
    FWLiteEnabler::enable();

    // configure the process
    const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");

    bool isMC = runProcess.getParameter<bool>("isMC");
   
    TString dtag=runProcess.getParameter<std::string>("tag");
    TString suffix=runProcess.getParameter<std::string>("suffix");

    bool verbose = runProcess.getParameter<bool>("verbose");
    
    TString url = runProcess.getParameter<std::string>("input");
    TString outFileUrl( dtag ); //gSystem->BaseName(url));
    //    outFileUrl.ReplaceAll(".root","");
  
    TString outdir=runProcess.getParameter<std::string>("outdir");
    TString outUrl( outdir );
    gSystem->Exec("mkdir -p " + outUrl);


    TString outTxtUrl_final= outUrl + "/" + outFileUrl + "_FinalList.txt";
    FILE* outTxtFile_final = NULL;
    outTxtFile_final = fopen(outTxtUrl_final.Data(), "w");
    printf("TextFile URL = %s\n",outTxtUrl_final.Data());
    fprintf(outTxtFile_final,"run lumi event\n");

    // int fType(0);
    // if(url.Contains("ZeroBias")) fType=ZB;
    // if(url.Contains("JetHT"))  fType=JetHT;

    bool is2016(!isMC && url.Contains("283171"));
    
    bool isZeroBias(!isMC && url.Contains("ZeroBias"));
    bool isJetHTPD(!isMC && url.Contains("JetHT"));

    bool isMC_QCD = isMC && (string(url.Data()).find("QCD")  != string::npos);

    if (isZeroBias) printf("Data url = %s\n",url.Data());

    

    //tree info
    int evStart     = runProcess.getParameter<int>("evStart");
    int evEnd       = runProcess.getParameter<int>("evEnd");
    TString dirname = runProcess.getParameter<std::string>("dirName");

 
    
    //##################################################################################
    //##########################    INITIATING HISTOGRAMS     ##########################
    //##################################################################################

    SmartSelectionMonitor mon;
     
     // GEN level kinematics
    //  mon.addHistogram( new TH1F( "higgsMass",";m_{h} [GeV];Events",200,0.,600.) );
    mon.addHistogram( new TH2F("tp_occupancy",";TP ieta; TP iphi",84, -42, 42, 90, 0, 90) );
    
    mon.addHistogram( new TH1F("adc",";adc count;a.u.",250,0,250) );
    mon.addHistogram( new TH1F("soi",";uncompressed E_{T} (SOI);a.u.",250,0,250) );
    mon.addHistogram( new TH1F("npre",";# presamples;a.u.",10,-0.5,9.5) );

    mon.addHistogram( new TH1F("ntp",";#TPs;a.u.",800, -0.5, 799.5) );
    mon.addHistogram( new TH1F("etp",";#sum TPs E_{T};a.u.", 200, -0.5, 599.5) );
    
    mon.addHistogram( new TH2F("ntp_vs_nvtx",";#nvtx; Number of TPs",100,-0.5, 99.5, 800, -0.5, 799.5) );
    mon.addHistogram( new TH2F("etp_vs_nvtx",";#nvtx; Sum TPs E_{T}",100,-0.5, 99.5, 200, -0.5, 599.5) );
  
    // //for MC normalization (to 1/pb)
    // TH1F* Hcutflow = (TH1F*) mon.addHistogram( new TH1F ("cutflow" , "cutflow" ,6,0,6) ) ;
    
    // TH1F *h1 = (TH1F*) mon.addHistogram( new TH1F( "nleptons_raw", ";Lepton multiplicity;Events", 4,0,4) );
    // for(int ibin=1; ibin<=h1->GetXaxis()->GetNbins(); ibin++) {
    //     TString label("");
    // 	if(ibin==h1->GetXaxis()->GetNbins()) label +="#geq";
    // 	else                                label +="=";
    //     label += (ibin-1);
    //     h1->GetXaxis()->SetBinLabel(ibin,label);
    // }


    //##################################################################################
    //#############         GET READY FOR THE EVENT LOOP           #####################
    //##################################################################################

    //open the file and get events tree
    DataEvtSummaryHandler summaryHandler_;

    TFile *file = TFile::Open(url);
    printf("Looping on %s\n",url.Data());
    if(file==0) { return -1; printf("file is 0"); }
    
    if(file->IsZombie()) return -1;
    if( !summaryHandler_.attachToTree( (TTree *)file->Get(dirname) ) ) {
        file->Close();
        return -1;
    }


    //check run range to compute scale factor (if not all entries are used)
    const Int_t totalEntries= summaryHandler_.getEntries();
    float rescaleFactor( evEnd>0 ?  float(totalEntries)/float(evEnd-evStart) : -1 );
    if(evEnd<0 || evEnd>summaryHandler_.getEntries() ) evEnd=totalEntries;
    if(evStart > evEnd ) {
        file->Close();
        return -1;
    }

    //MC normalization (to 1/pb)
    double xsecWeight = 1.0;
    // float cnorm=1.0;
    // if (isMC) {
    //   double totalNumberofEvents;

    //   TH1F* nevtH = (TH1F *) file->Get("mainNtuplizer/nevents");
    //   totalNumberofEvents = nevtH->GetBinContent(1);
    //   TH1F* posH = (TH1F *) file->Get("mainNtuplizer/n_posevents");
    //   TH1F* negH = (TH1F *) file->Get("mainNtuplizer/n_negevents");
    //   if(posH && negH) cnorm = posH->GetBinContent(1) - negH->GetBinContent(1);
    //   if(rescaleFactor>0) cnorm /= rescaleFactor;
    //   printf("cnorm = %f\n",cnorm);
     
    //   xsecWeight=xsec/totalNumberofEvents;
    // }
    // Hcutflow->SetBinContent(1,cnorm);

    // //pileup weighting
    // TString PU_Central = runProcess.getParameter<std::string>("pu_central");
    // gSystem->ExpandPathName(PU_Central);
    // cout << "Loading PU weights Central: " << PU_Central << endl;
    // TFile *PU_Central_File = TFile::Open(PU_Central);
    // TH1F* weight_pileup_Central = (TH1F *) PU_Central_File->Get("pileup");
    
    
 

    //####################################################################################################################
    //###########################################           EVENT LOOP         ###########################################
    //####################################################################################################################


    // loop on all the events
    int treeStep = (evEnd-evStart)/50;
    if(treeStep==0)treeStep=1;

    printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
    printf("Scanning the ntuple :");

    for( int iev=evStart; iev<evEnd; iev++) {
        if((iev-evStart)%treeStep==0) {
	  printf("."); fflush(stdout);
        }

	if ( verbose ) printf("\n\n Event info %3d: \n",iev);

	
        //##############################################   EVENT LOOP STARTS   ##############################################
        //load the event content from tree
        summaryHandler_.getEntry(iev);
        DataEvtSummary_t &ev=summaryHandler_.getEvent();
        // if(!isMC && duplicatesChecker.isDuplicate( ev.run, ev.lumi, ev.event) ) {
        //     nDuplicates++;
        //     cout << "nDuplicates: " << nDuplicates << endl;
        //     continue;
        // }

	std::vector<TString> tags(1,"all");
	
      
        //systematical weight
        float weight = 1.0; //xsecWeight;
	if(isMC) weight *= xsecWeight; 
      
	

        // add PhysicsEvent_t class, get all tree to physics objects
        HcalEvent_t phys=getHcalEventFrom(ev);

	if (!phys.hasTrigger) continue;
	
        //#########################################################################
        //#####################      Objects Selection       ######################
        //#########################################################################

	HcalTriggerPrimitiveCollection &hcal_tps = phys.hcaltps;

	int ntp_hb_, ntp_he_, ntp_hf_;
	ntp_hb_=ntp_he_=ntp_hf_= 0;

	float sum_hb_, sum_he_, sum_hf_;
	sum_hb_=sum_he_=sum_hf_=0.;
	
	for (auto & it : hcal_tps) {

	  float tp_et=it.et;
	  if (is2016 && abs(it.ieta) > 29) tp_et /= 0.7;
	  // Cut on a minimun TP Et > 0.5			   
	  if (tp_et < tp_threshold)
	    continue;
	  
	  // if (it.soi < adc_threshold)
	  //   continue;

	  // adc count integrated
	  float adc_=0.;
	  for (int i=0; i<10; i++) {
	    adc_+=it.adc[i];
	  }
	  
	  // Remove HEP17 region
	  if (it.ieta<=29 && it.ieta>=17 && it.iphi>=63 && it.iphi<=66)
	    continue;
	  
	  // TP occupancy vs ieta
	  mon.fillHisto("tp_occupancy","tps",it.ieta,it.iphi,weight);
	  
	  // Number of TPs
	  if (abs(it.ieta) <= 16) {
	    ++ntp_hb_;
	    sum_hb_+= tp_et;
	    mon.fillHisto("adc","hb",adc_,weight);
	    mon.fillHisto("soi","hb",it.soi,weight);
	    if (phys.nvtx==25.) {
	      mon.fillHisto("adc","hb_vtx25",adc_,weight);
	      mon.fillHisto("soi","hb_vtx25",it.soi,weight);
	    }
	    mon.fillHisto("npre","hb",it.npresamples,weight);
	  } else if (abs(it.ieta) <= 29) {
	    ++ntp_he_;
	    sum_he_+=tp_et;
	    mon.fillHisto("adc","he",adc_,weight);
	    mon.fillHisto("soi","he",it.soi,weight);
	    if (phys.nvtx==25.) {
	      mon.fillHisto("adc","he_vtx25",adc_,weight);
	      mon.fillHisto("soi","he_vtx25",it.soi,weight);
	    }
	    mon.fillHisto("npre","he",it.npresamples,weight);
	  } else {
	    if (it.version==1) {
	      ++ntp_hf_;
	      sum_hf_+=tp_et;
	      mon.fillHisto("adc","hf",adc_,weight);
	      mon.fillHisto("soi","hf",it.soi,weight);
	      if (phys.nvtx==25.) {
		mon.fillHisto("adc","hf_vtx25",adc_,weight);
		mon.fillHisto("soi","hf_vtx25",it.soi,weight);
	      }
	      mon.fillHisto("npre","hf",it.npresamples,weight);
	    }
	  }
	  
	} // loop on hcal_tps
	
	if (phys.nvtx==25.) {
	  mon.fillHisto("ntp","hb_vtx25",ntp_hb_,weight);
	  mon.fillHisto("ntp","he_vtx25",ntp_he_,weight);
	  mon.fillHisto("ntp","hf_vtx25",ntp_hf_,weight);

	  mon.fillHisto("etp","hb_vtx25",sum_hb_,weight);
	  mon.fillHisto("etp","he_vtx25",sum_he_,weight);
	  mon.fillHisto("etp","hf_vtx25",sum_hf_,weight);
	}
	// TP occupancy / Sum ET vs : nvtx 
	mon.fillHisto("ntp_vs_nvtx","hb",phys.nvtx,ntp_hb_,weight);
	mon.fillHisto("etp_vs_nvtx","hb",phys.nvtx,sum_hb_,weight);

	mon.fillHisto("ntp_vs_nvtx","he",phys.nvtx,ntp_he_,weight);
	mon.fillHisto("etp_vs_nvtx","he",phys.nvtx,sum_he_,weight);

	mon.fillHisto("ntp_vs_nvtx","hf",phys.nvtx,ntp_hf_,weight);
	mon.fillHisto("etp_vs_nvtx","hf",phys.nvtx,sum_hf_,weight);

    } // loop on all events END


    printf("\n");
    file->Close();

    //##############################################
    //########     SAVING HISTO TO FILE     ########
    //##############################################
    //save control plots to file
    outUrl += "/";
    outUrl += outFileUrl + ".root";
    printf("Results saved in %s\n", outUrl.Data());

    //save all to the file
    TFile *ofile=TFile::Open(outUrl, "recreate");
    mon.Write();
    ofile->Close();

    if(outTxtFile_final)fclose(outTxtFile_final);
}

