#ifndef macroutils_h
#define macroutils_h

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/Common/interface/MergeableCounter.h"


//Load here all the dataformat that we will need
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/RegexMatch.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "DataFormats/Math/interface/deltaPhi.h"

//#include "UserCode/bsmhiggs_fwk/interface/PatUtils.h"


#include <istream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <vector>
#include "TVector3.h"
#include "TMath.h"
#include "TGraph.h"
#include <Math/VectorUtil.h>


typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;


namespace utils
{
  namespace cmssw
  {
   

    //get impact parameter for a track
    template<class T>
    std::pair<bool,Measurement1D> getImpactParameter(const T &trkRef, reco::VertexRef &vtx, const edm::EventSetup &iSetup, bool is3d=true)
      {
	edm::ESHandle<TransientTrackBuilder> trackBuilder;
	iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);
	reco::TransientTrack tt = trackBuilder->build(trkRef.get());
	if(is3d) return IPTools::absoluteImpactParameter3D(tt, *vtx);
	else     return IPTools::absoluteTransverseImpactParameter(tt, *vtx);
      }

    //
    template<class T>
    float getArcCos(T &a,T &b)
      {
	TVector3 mom1(a.px(),a.py(),a.pz());
	TVector3 mom2(b.px(),b.py(),b.pz());
	float cosine = mom1.Dot(mom2)/(mom1.Mag()*mom2.Mag());
	float arcCosine = acos(cosine);
	return arcCosine;
      }

   
    typedef std::vector<TGraph *> PuShifter_t;
    enum PuShifterTypes {PUDOWN,PUUP};
    utils::cmssw::PuShifter_t getPUshifters(std::vector< float > &Lumi_distr, float puUnc);
    Float_t getEffectiveArea(int id, float eta,TString isoSum="");
//    double relIso(llvvLepton lep, double rho);


  inline float deltaPhi(float phi1, float phi2) { 
     float result = phi1 - phi2;
     while (result > float(M_PI)) result -= float(2*M_PI);
     while (result <= -float(M_PI)) result += float(2*M_PI);
     return result;
  }

// FWLITE CODE

  // loop on all the lumi blocks for an EDM file in order to count the number of events that are in a sample
  // this is useful to determine how to normalize the events (compute weight)
  /* double getTotalNumberOfEvents(std::vector<std::string>& urls, bool fast=false, bool weightSum=false); */

  /* unsigned long getMergeableCounterValue(const std::vector<std::string>& urls, std::string counter); */
  /* double getMCPileupDistributionAndTotalEventFromMiniAOD(std::vector<std::string>& urls, unsigned int Npu, std::vector<float>& mcpileup); */
  /* void getMCPileupDistributionFromMiniAOD(std::vector<std::string>& urls, unsigned int Npu, std::vector<float>& mcpileup); */
  /* bool isGoodVertex(reco::Vertex& vtx); */
  /* void getMCPileupDistributionFromMiniAODtemp(std::vector<std::string>& urls, unsigned int Npu, std::vector<float>& mcpileup); */
  /* void getMCPileupDistributionFromMiniAOD(fwlite::ChainEvent& ev, unsigned int Npu, std::vector<float>& mcpileup); */
  /* void getMCPileupDistribution(fwlite::ChainEvent& ev, unsigned int Npu, std::vector<float>& mcpileup); */
  /* void getPileupNormalization(std::vector<float>& mcpileup, double* PUNorm, edm::LumiReWeighting* LumiWeights, utils::cmssw::PuShifter_t PuShifters); */

  bool passTriggerPatternsAndGetName(edm::TriggerResultsByName& tr, std::string& pathName, std::string pattern);
  bool passTriggerPatterns(edm::TriggerResultsByName& tr, std::string pattern);
  bool passTriggerPatterns(edm::TriggerResultsByName& tr, std::string pattern1, std::string pattern2, std::string pattern3="", std::string pattern4="");
  bool passTriggerPatterns(edm::TriggerResultsByName& tr, std::vector<std::string>& patterns);


  // inline bool sort_CandidatesByPt(const pat::GenericParticle &a, const pat::GenericParticle &b)  { return a.pt()>b.pt(); }
}


};


#endif
