#include "UserCode/HcalTPAnalyzer/interface/DataEvtSummaryHandler.h"

using namespace std;

//
DataEvtSummaryHandler::DataEvtSummaryHandler()
{
}

//
bool DataEvtSummaryHandler::initTree(TTree *t)
{
    if(t==0) return false;
    t_ = t;

    //event info
    t_->Branch("run",        	&evSummary_.run,            "run/I");
    t_->Branch("lumi",       	&evSummary_.lumi,           "lumi/I");
    t_->Branch("bx",       	&evSummary_.bx,             "bx/I");
    t_->Branch("event",      	&evSummary_.event,          "event/L");
    //primary vertex
    t_->Branch("nvtx",          &evSummary_.nvtx,            "nvtx/I");

    t_->Branch("hasTrigger", &evSummary_.hasTrigger, "hasTrigger/O");

    // HBHE, HF
    t_->Branch("ntp",           &evSummary_.ntp,         "ntp/I");
    t_->Branch("tp_ieta_",           &evSummary_.tp_ieta_,         "tp_ieta_[ntp]/I");
    t_->Branch("tp_iphi_",           &evSummary_.tp_iphi_,         "tp_iphi_[ntp]/I");
    t_->Branch("tp_depth_",          &evSummary_.tp_depth_,         "tp_depth_[ntp]/I");
    t_->Branch("tp_version_",        &evSummary_.tp_version_,       "tp_version_[ntp]/I");

    t_->Branch("tp_npresamples_",    &evSummary_.tp_npresamples_,   "tp_npresamples_[ntp]/I");
    t_->Branch("tp_unzs_",          &evSummary_.tp_unzs_,           "tp_unzs_[ntp]/O");

    t_->Branch("tp_soi_",           &evSummary_.tp_soi_,         "tp_soi_[ntp]/I");
    t_->Branch("tp_et_",           &evSummary_.tp_et_,         "tp_et_[ntp]/F");

    t_->Branch("tp_fg_",           &evSummary_.tp_fg_,         "tp_fg_[ntp][7]/I");
    t_->Branch("tp_adc_",           &evSummary_.tp_adc_,         "tp_adc_[ntp][10]/I");

    return true;
}

//
bool DataEvtSummaryHandler::attachToTree(TTree *t)
{
    if(t==0) return false;
    t_ = t;


    //event info
    t_->SetBranchAddress("run",             &evSummary_.run);
    t_->SetBranchAddress("lumi",            &evSummary_.lumi);
    t_->SetBranchAddress("bx",            &evSummary_.bx);
    t_->SetBranchAddress("event",           &evSummary_.event);

    t_->SetBranchAddress("hasTrigger", &evSummary_.hasTrigger);

    //primary vertex
    t_->SetBranchAddress("nvtx", &evSummary_.nvtx);
   
 // HBHE, HF
    t_->SetBranchAddress("ntp",           &evSummary_.ntp);
    t_->SetBranchAddress("tp_ieta_",           &evSummary_.tp_ieta_);
    t_->SetBranchAddress("tp_iphi_",           &evSummary_.tp_iphi_);
    t_->SetBranchAddress("tp_depth_",          &evSummary_.tp_depth_);
    t_->SetBranchAddress("tp_version_",        &evSummary_.tp_version_);

    t_->SetBranchAddress("tp_npresamples_",   &evSummary_.tp_npresamples_);
    t_->SetBranchAddress("tp_unzs_",         &evSummary_.tp_unzs_);

    t_->SetBranchAddress("tp_soi_",           &evSummary_.tp_soi_);
    t_->SetBranchAddress("tp_et_",           &evSummary_.tp_et_);

    t_->SetBranchAddress("tp_fg_",           &evSummary_.tp_fg_);
    t_->SetBranchAddress("tp_adc_",           &evSummary_.tp_adc_);  

    return true;
}


//
void DataEvtSummaryHandler::resetStruct()
{
   
    evSummary_.run=0;
    evSummary_.lumi=0;
    evSummary_.bx=0;
    evSummary_.event=0;
    
    evSummary_.nvtx=0;

    evSummary_.ntp=0;
    //    evSummary_.nhetp=0;
    // evSummary_.nhftp=0;

}

//
void DataEvtSummaryHandler::fillTree()
{
    if(t_) t_->Fill();
}

//
DataEvtSummaryHandler::~DataEvtSummaryHandler()
{
}
