#!/bin/sh

_GT="106X_mcRun2_asymptotic_preVFP_v8"
_NEVT=100
_IS_DATA=false
_GRID_PATH_PREFIX="root://xrootd-cms.infn.it:1094/"
_FILE="/store/mc/RunIISummer19UL16MiniAODAPV/ZZ_TuneCP5_13TeV-pythia8/MINIAODSIM/106X_mcRun2_asymptotic_preVFP_v8-v2/20000/F37CF2E7-DAC0-5A42-95AD-BBB5714758AA.root"
_DIR="test/${_GT}"

mkdir -p $_DIR

echo "##############################################################################"
echo "STARTING cmsRun !!!!"
echo "##############################################################################"

cmsRun ../kappaSkim_default_cfg.py inputFiles=${_GRID_PATH_PREFIX}${_FILE} \
                        globalTag=${_GT} \
                        isData=${_IS_DATA} \
                        outputFile=testKappaSkim_out_${_GT}.root \
                        maxEvents=${_NEVT} \
                        dumpPython=1 2>&1 | tee cout_${_GT}.log

echo "##############################################################################"
echo "cmsRun terminated !!!!"
echo "##############################################################################"

if [ -f cout_$_GT.log ]; then
    mv cout_$_GT.log $_DIR/
fi

if [ -f infos.log ]; then
    mv infos.log $_DIR/
fi

if [ -f debugs.log ]; then
    mv debugs.log $_DIR/
fi

if [ -f testKappaSkim_out_${_GT}_numEvent${_NEVT}.root ]; then
    mv testKappaSkim_out_${_GT}_numEvent${_NEVT}.root $_DIR/
fi

