//
//  HcalEvent.cc
//

#include "UserCode/HcalTPAnalyzer/interface/HcalEvent.h"

using namespace std;


//
HcalEvent_t getHcalEventFrom(DataEvtSummary_t &ev)
{
    HcalEvent_t phys;

    phys.run=ev.run;
    phys.event=ev.event;
    phys.lumi=ev.lumi;
    phys.bx=ev.bx;

    phys.nvtx = ev.nvtx;

    phys.hasTrigger = ev.hasTrigger;


    // Hcal Trigger PRimitives
    size_t ntp(0);
    for(Int_t i=0; i<ev.ntp; i++) {
      phys.hcaltps.push_back(ev.tp_et_[i]);
      phys.hcaltps[i].setEnergyInfo(ev.tp_ieta_[i], ev.tp_iphi_[i], ev.tp_soi_[i], ev.tp_adc_[i],  ev.tp_fg_[i]);
      phys.hcaltps[i].setTypeInfo(ev.tp_depth_[i], ev.tp_version_[i], ev.tp_npresamples_[i], ev.tp_unzs_[i]);
      ntp++;
    }


    return phys;
}


