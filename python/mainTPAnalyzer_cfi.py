import FWCore.ParameterSet.Config as cms

from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.StandardSequences.Eras import eras

process = cms.Process('PLOT', eras.Run2_2017)

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("SimCalorimetry.Configuration.hcalDigiSequence_cff")
process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')

process.mainTPAnalyzer = cms.EDAnalyzer("mainTPAnalyzer",                                      
                                        triggerPrimitives=cms.InputTag("hcalDigis", "", ""),
                                        vtxToken=cms.untracked.VInputTag("offlinePrimaryVertices","","RECO"), 
                                        ##trigger
                                        bits = cms.InputTag("TriggerResults","","HLT")
)

