#!/usr/bin/env bash

#--------------------------------------------------
# Global Code 
#--------------------------------------------------

step=$1   #variable that store the analysis step to run

#Additional arguments to take into account
arguments=''; for var in "${@:2}"; do arguments=$arguments" "$var; done
#arguments='crab3'; for var in "${@:2}"; do arguments=$arguments" "$var; done
if [[ $# -ge 4 ]]; then echo "Additional arguments will be considered: "$arguments ;fi 

#--------------------------------------------------
# Global Variables
#--------------------------------------------------

SUFFIX=_2017_10_02

MAINDIR=$CMSSW_BASE/src/UserCode/HcalTPAnalyzer/macros
JSON=$MAINDIR/samples.json
RESULTSDIR=$MAINDIR/results$SUFFIX
PLOTSDIR=$MAINDIR/plots${SUFFIX}

NTPL_INPUT=ZeroBias
RUNLOG=$RESULTSDIR/LOGFILES/runSelection.log

queue='cmscaf1nd'

################################################# STEPS between 0 and 1
if [[ $step == 0 ]]; then   
        #analysis cleanup
    echo "Really delete directory "$RESULTSDIR" ?" 
    echo "ALL DATA WILL BE LOST! [N/y]?"
    read answer
    if [[ $answer == "y" ]];
    then
	echo "CLEANING UP..."
	rm -rdf $RESULTSDIR $PLOTSDIR LSFJOB_* core.* *.sh.e* *.sh.o*
    fi
fi #end of step0

if [[ $step == 1 ]]; then  
    echo "JOB SUBMISSION for Hcal TP Analysis"
    echo "Input: " $JSON
    echo "Output: " $RESULTSDIR
    runLocalAnalysisOverSamples.py -e runHcalAnalysis -g $RUNLOG -j $JSON -d $NTPL_INPUT -o $RESULTSDIR -c $MAINDIR/analyze_tp_cfg.py.templ -p "@verbose=False" -s $queue 
fi