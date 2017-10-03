#include "UserCode/HcalTPAnalyzer/interface/MacroUtils.h"
#include "TH1F.h"
#include "TSystem.h"

namespace utils
{
  namespace cmssw
  {
    //
   
    //
    PuShifter_t getPUshifters(std::vector< float > &Lumi_distr, float puUnc)
    {
      Int_t NBins = Lumi_distr.size();
      TH1F *pu=new TH1F("putmp","",NBins,-0.5,float(NBins)-0.5);
      TH1F *puup=(TH1F *)pu->Clone("puuptmp");
      TH1F *pudown=(TH1F *)pu->Clone("pudowntmp");
      for(size_t i=0; i<Lumi_distr.size(); i++)  pu->SetBinContent(i+1,Lumi_distr[i]);

      for(int ibin=1; ibin<=pu->GetXaxis()->GetNbins(); ibin++)
			{
	  		Double_t xval=pu->GetBinCenter(ibin);
	  		TGraph *gr = new TGraph;
	  		for(int ishift=-3; ishift<3; ishift++)
	    	{
	      	if(ibin+ishift<0) continue;
	      if(ibin+ishift>pu->GetXaxis()->GetNbins()) continue;
	      
	      gr->SetPoint(gr->GetN(),xval+ishift,pu->GetBinContent(ibin+ishift));
	    }
	  if(gr->GetN()>1)
	    {
	      Double_t newval(gr->Eval(xval*(1+puUnc)));
	      pudown->SetBinContent(ibin,newval>0?newval:0.0);
	      newval=gr->Eval(xval*(1-puUnc));
	      puup->SetBinContent(ibin,newval>0?newval:0.0);
	    }
	  delete gr;
	}
      puup->Scale(pu->Integral()/puup->Integral());
      pudown->Scale(pu->Integral()/pudown->Integral());
      std::cout << "getPUshifts will shift average PU by " << puup->GetMean()-pu->GetMean() << " / " << pudown->GetMean()-pu->GetMean() << std::endl; 
      
      puup->Divide(pu);    TGraph *puupWgt = new TGraph(puup);
      pudown->Divide(pu);  TGraph *pudownWgt = new TGraph(pudown);
      delete puup;
      delete pudown;  
      delete pu;
      
      PuShifter_t res(2);
      res[PUDOWN] = pudownWgt;
      res[PUUP]   = puupWgt;
      return res;
    }
  

  bool isGoodVertex(reco::Vertex& vtx)
  {

    if(vtx.chi2()==0 && vtx.ndof()==0) return false; // Corresponds to the AOD method vtx->isFake()  

    if(vtx.ndof() < 4)            return false;
    if(abs(vtx.z())>24.)          return false;
    if(vtx.position().Rho() >2.0) return false;
    // else
    return true;
  }
  

  void getPileupNormalization(std::vector<float>& mcpileup, double* PUNorm, edm::LumiReWeighting* LumiWeights, utils::cmssw::PuShifter_t PuShifters){
    PUNorm[0]=0; PUNorm[1]=0; PUNorm[2]=0;
    double NEvents=0;
    for(unsigned int i=0;i<mcpileup.size();i++){
      NEvents+=mcpileup[i];
      double puWeight = LumiWeights->weight((int)i);
      PUNorm[0]+=mcpileup[i]*puWeight;
      PUNorm[1]+=mcpileup[i]*puWeight*PuShifters[utils::cmssw::PUDOWN]->Eval(i);
      PUNorm[2]+=mcpileup[i]*puWeight*PuShifters[utils::cmssw::PUUP  ]->Eval(i);
    }
    PUNorm[0]/=NEvents;
    PUNorm[1]/=NEvents;
    PUNorm[2]/=NEvents;
  }



  bool passTriggerPatternsAndGetName(edm::TriggerResultsByName& tr, std::string& pathName, std::string pattern){
     if(edm::is_glob(pattern)){
        std::vector< std::vector<std::string>::const_iterator > matches = edm::regexMatch(tr.triggerNames(), pattern);
        for(size_t t=0;t<matches.size();t++){
           if(tr.accept( matches[t]->c_str() ) ){pathName = *matches[t]; return true;}
        }
     }else{
        if(tr.accept( pattern.c_str() ) ) { pathName = pattern; return true;}
     }
     return false;
  }


  bool passTriggerPatterns(edm::TriggerResultsByName& tr, std::string pattern){
     if(edm::is_glob(pattern)){
        std::vector< std::vector<std::string>::const_iterator > matches = edm::regexMatch(tr.triggerNames(), pattern);
        for(size_t t=0;t<matches.size();t++){
           if(tr.accept( matches[t]->c_str() ) )return true;
        }
     }else{
        if(tr.accept( pattern.c_str() ) ) return true;
     }
     return false;
  }

  bool passTriggerPatterns(edm::TriggerResultsByName& tr, std::string pattern1, std::string pattern2, std::string pattern3, std::string pattern4){
     if(pattern1!="" && passTriggerPatterns(tr, pattern1))return true;
     if(pattern2!="" && passTriggerPatterns(tr, pattern2))return true;
     if(pattern3!="" && passTriggerPatterns(tr, pattern3))return true;
     if(pattern4!="" && passTriggerPatterns(tr, pattern4))return true;
     return false;
  }

  bool passTriggerPatterns(edm::TriggerResultsByName& tr, std::vector<std::string>& patterns){
     for(size_t p=0;p<patterns.size();p++){
        if(passTriggerPatterns(tr, patterns[p]))return true;
     }
     return false;
  }

  }
  
}
