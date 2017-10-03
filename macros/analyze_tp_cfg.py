import FWCore.ParameterSet.Config as cms

process = cms.Process("AnalysisProc")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

import PhysicsTools.PythonAnalysis.LumiList as LumiList
LumiList.LumiList().getVLuminosityBlockRange()

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('') )

runProcess = cms.PSet(
    input = cms.string("/eos/cms/store/user/georgia/ZeroBias/crab_ZeroBias_run302472/170926_142640/0000/analyze_tps_11.root"),
    outdir = cms.string("/afs/cern.ch/work/g/georgia/HCALTrigger/CMSSW_9_2_7/src/UserCode/HcalTPAnalyzer/macros/"),
    tag = cms.string("ZeroBias"),
    dirName = cms.string("mainTPAnalyzer/tpdata"),
    isMC = cms.bool(False),
    verbose = cms.bool(False),
    xsec = cms.double(1.0),
    suffix = cms.string(""), 
    evStart = cms.int32(0),
    evEnd = cms.int32(-1) 
)
