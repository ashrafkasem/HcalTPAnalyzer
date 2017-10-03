//
//  HcalEvent.h
//

#ifndef ____HcalEvent__
#define ____HcalEvent__

#include <stdio.h>
#include <vector>
#include "TH2F.h"

#include "UserCode/HcalTPAnalyzer/interface/DataEvtSummaryHandler.h"
#include "DataFormats/Math/interface/deltaR.h"

//
class HcalObject_TP {
public :
 HcalObject_TP(Float_t et_):
  et(et_) {}
  
  void setEnergyInfo(Int_t ieta_, Int_t iphi_, Int_t soi_, Int_t adc_[10], Int_t fg_[7]) {
    ieta=ieta_;
    iphi=iphi_;
    soi=soi_;
    for (int i=0; i<10; i++) 
      adc[i]=adc_[i];
    for (int i=0; i<7; i++)   
      fg[i]=fg_[i];

  }
  
  void setTypeInfo(Int_t depth_, Int_t version_, Int_t npresamples_, Bool_t unzs_) { 
    depth=depth_;
    version=version_;
    npresamples=npresamples_;
    unzs=unzs_;
  }

  Float_t et;
  Int_t ieta, iphi;
  Int_t depth, version, npresamples;
  Bool_t unzs;

  Int_t soi;
  Int_t adc[10], fg[7];
  
};


typedef std::vector<HcalObject_TP>  HcalTriggerPrimitiveCollection;

//
struct HcalEvent_t {
  int run,event,lumi,bx;
  int nvtx;

  bool hasTrigger;
  
  HcalTriggerPrimitiveCollection hcaltps;

};



//
HcalEvent_t getHcalEventFrom(DataEvtSummary_t &ev);




#endif /* defined(____HcalEvent__) */
