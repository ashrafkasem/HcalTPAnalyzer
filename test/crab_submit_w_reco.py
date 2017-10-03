from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'ZeroBias_run302674'
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'analyze_tp_main.py'
config.Data.inputDataset = '/ZeroBias/Run2017D-PromptReco-v1/RECO'
#'/JetHT/Run2017C-PromptReco-v3/RECO'
config.Data.useParent = True
config.Data.runRange = '302674'
config.Data.lumiMask = 'lumimask.json'
#'/afs/cern.ch/user/d/deguio/public/Certification/Cert_13TeV_2017_HCAL_DCS_GOOD.txt'
#'/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/DCSOnly/json_DCSONLY.txt'
config.Data.splitting = 'LumiBased'
config.Data.ignoreLocality = True
config.Data.unitsPerJob = 5
config.Data.totalUnits = -1 
config.Data.publication = False

config.Site.storageSite = 'T2_CH_CERN'
config.Site.whitelist = ['T2_CH_CERN','T2_US_UCSD'] 
