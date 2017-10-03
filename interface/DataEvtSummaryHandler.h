#ifndef dataevtsummaryhandler_h
#define dataevtsummaryhandler_h

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <string.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <set>
#include <cmath>

#include "Math/LorentzVector.h"
#include "TMath.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TTree.h"
#include "TLorentzVector.h"

#endif

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;


//#define MAXSB 2
#define MAXTPS 5000
//#define MAXHETPS 500
//#define MAXHFTPS 1200

const Int_t ADCMAX = 10;
static const int FGCOUNT = 7;


struct DataEvtSummary_t {

  int run, lumi, bx;
  long long int event;

  Bool_t hasTrigger;
  //int triggerType;

  //primary vertex
  int nvtx;

  // Hcal TPs

  int ntp;
  int tp_ieta_[MAXTPS], tp_iphi_[MAXTPS];
  int tp_depth_[MAXTPS], tp_version_[MAXTPS];
  int tp_npresamples_[MAXTPS];
  bool tp_unzs_[MAXTPS];

  int tp_soi_[MAXTPS];
  int tp_adc_[MAXTPS][ADCMAX];
  int tp_fg_[MAXTPS][FGCOUNT];
  float tp_et_[MAXTPS];
  /*
  int nhetp;
  int hetp_ieta[MAXHETPS], hetp_iphi[MAXHETPS];
  int hetp_depth[MAXHETPS], hetp_version[MAXHETPS];
  int hetp_soi[MAXHETPS];
  int hetp_fg0[MAXHETPS], hetp_fg1[MAXHETPS];
  float hetp_et[MAXHETPS];

  int nhftp;
  int hftp_ieta[MAXHFTPS], hftp_iphi[MAXHFTPS];
  int hftp_depth[MAXHFTPS], hftp_version[MAXHFTPS];
  int hftp_soi[MAXHFTPS];
  int hftp_fg0[MAXHFTPS], hftp_fg1[MAXHFTPS];
  float hftp_et[MAXHFTPS];
  */
};

class DataEvtSummaryHandler {
public:
    //
    DataEvtSummaryHandler();
    ~DataEvtSummaryHandler();

    //current event
    DataEvtSummary_t evSummary_;
    DataEvtSummary_t &getEvent() {
        return evSummary_;
    }

    //write mode
    bool initTree(TTree *t);
    void fillTree();

    //read mode
    bool attachToTree(TTree *t);
    int getEntries() { return (t_ ? t_->GetEntriesFast() : 0); }
    void getEntry(int ientry) {
    	resetStruct();
    	if(t_) t_->GetEntry(ientry);
    }

    void resetStruct();

private:
    //the tree
    TTree *t_;
};

#endif
