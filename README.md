# HcalTPAnalyzer
HCAL Trigger Primitives analyzer package

cmsrel CMSSW_9_2_7
cd CMSSW_9_2_7/src
cmsenv

git clone https://github.com/georgia14/HcalTPAnalyzer.git UserCode/HcalTPAnalyzer
scram b -j 4

cd UserCode/HcalTPAnalyzer
- cd test/ -> (CRAB3 submission for mainNtuplizer )
- cd macros/ -> ./submit.sh 1 ( runHcalAnalysis )
