import FWCore.ParameterSet.Config as cms

from UserCode.HcalTPAnalyzer.mainTPAnalyzer_cfi import *

from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.StandardSequences.Eras import eras

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#process.GlobalTag = GlobalTag(process.GlobalTag, '92X_dataRun2_Prompt_v8', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '92X_dataRun2_HLT_v5', '')  

#process.load("CondCore.CondDB.CondDB_cfi")
#CondCore.DBCommon.CondDBSetup_cfi")

#process.prod = cms.EDFilter("HcalDumpConditions",
#                            dump = cms.untracked.vstring('LutMetadata'),
#                            outFilePrefix = cms.untracked.string('DumpCond')
#                            )

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(10))

process.source = cms.Source("PoolSource",
                            skipBadFiles=cms.untracked.bool(True),
                            fileNames=cms.untracked.vstring(),
                            secondaryFileNames=cms.untracked.vstring(),
#                            firstRun=cms.untracked.uint32(260627),
                            inputCommands=cms.untracked.vstring(
        'keep *',
        'drop CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters_*_RECO'
#        'drop *_*_*_HLT',
        #'keep FEDRawDataCollection_*_*_*'
        )
                            )


process.source.fileNames.extend([

#'/store/data/Run2017C/ZeroBias/RECO/PromptReco-v3/000/300/742/00000/0A0FD9E0-C97E-E711-B146-02163E01420B.root'

'/store/data/Run2017C/JetHT/RECO/PromptReco-v3/000/300/742/00000/14F94A59-007E-E711-950E-02163E01359C.root'

#'/store/data/Run2017C/ZeroBias0/RECO/PromptReco-v1/000/299/542/00000/8C7C1870-CE70-E711-9B07-02163E014485.root'
#'/store/data/Run2017B/ZeroBias/RECO/PromptReco-v1/000/297/469/00000/1C06F39D-B85A-E711-B8B3-02163E01A5DF.root'

])

process.source.secondaryFileNames.extend([

#'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/742/00000/08E49EBD-0A7D-E711-AA2E-02163E01A708.root',
#'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/742/00000/52124013-6F7C-E711-8E6B-02163E01192A.root',
#'/store/data/Run2017C/ZeroBias/RAW/v1/000/300/742/00000/86AD2554-007D-E711-B28F-02163E019BE5.root'

'/store/data/Run2017C/JetHT/RAW/v1/000/300/742/00000/2209CC13-947C-E711-A07E-02163E01A5F5.root',
'/store/data/Run2017C/JetHT/RAW/v1/000/300/742/00000/4C933352-947C-E711-BDAA-02163E01476C.root',
'/store/data/Run2017C/JetHT/RAW/v1/000/300/742/00000/529031DE-937C-E711-9E6B-02163E019C9A.root',
'/store/data/Run2017C/JetHT/RAW/v1/000/300/742/00000/801C0578-947C-E711-89BE-02163E012305.root',
'/store/data/Run2017C/JetHT/RAW/v1/000/300/742/00000/920C65DF-937C-E711-A441-02163E01A32E.root',
'/store/data/Run2017C/JetHT/RAW/v1/000/300/742/00000/E69726DA-937C-E711-BA2B-02163E01A3B7.root'


#'/store/data/Run2017C/ZeroBias0/RAW/v1/000/299/542/00000/E4F860B2-FE6D-E711-A049-02163E01A5B8.root'
#'/store/data/Run2017B/ZeroBias/RAW/v1/000/297/469/00000/F04F22DC-2459-E711-B1C7-02163E019D7B.root'

])

process.TFileService = cms.Service("TFileService",
                                   closeFileFasO=cms.untracked.bool(True),
                                   fileName=cms.string('analyze_tps.root'))

process.GlobalTag.toGet = cms.VPSet(
  cms.PSet(record = cms.string("HcalElectronicsMapRcd"),
           tag = cms.string("HcalElectronicsMap_2017plan1_v3.0_data"),
           connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
          )
)

#process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.mainTPAnalyzer.dtag = cms.string("data")
process.mainTPAnalyzer.isData = cms.bool(True)
process.mainTPAnalyzer.verbose = cms.bool(False)
process.mainTPAnalyzer.doReco = cms.bool(True)
process.mainTPAnalyzer.maxVtx = cms.uint32(100)
process.mainTPAnalyzer.threshold = cms.untracked.double(0.5)

process.p = cms.Path( process.hcalDigis * process.mainTPAnalyzer )

