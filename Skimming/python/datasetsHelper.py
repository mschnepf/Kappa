
# -*- coding: utf-8 -*-
from Kappa.Skimming.datasets import *

def getProcess(nickname):
	for string in Data:
		if string in nickname:
			return "Data"
	for string in DYJets:
		if string in nickname:
			return "DYJets"
	for string in TTJets:
		if string in nickname:
			return "TTJets"
	for string in Dibosons:
		if string in nickname:
			return "Dibosons"
	for string in WJets:
		if string in nickname:
			return "WJets"
	for string in SM_GluGluToHToTauTau:
		if string in nickname:
			return "SM_GluGluToHToTauTau"
	for string in SM_VBFHToTauTau:
		if string in nickname:
			return "SM_VBFHToTauTau"
	for string in SM_WH_ZH_TTH_HToTauTau:
		if string in nickname:
			return "SM_WH_ZH_TTH_HToTauTau"

def getIsEmbedded(nickname):
	# Todo together with implementation of embedded samples in datasets.py
	return False

def getJetMultiplicity(nickname):
	for string in DYJets:
		if string in nickname:
			if string[2] in ["1", "2", "3", "4"]:
				return int(string[2])
			else:
				return 0
	return 0

def getRunPeriod(nickname):
	for string in Data:
		if string in nickname:
			posLeft = nickname.find("_Run") + 4
			posRight = posLeft + nickname[posLeft:].find("_")
			return nickname[posLeft:posRight]
	return "0"

def getGenerator(nickname): ########todo
	if getProcess(nickname) == "Data": return "None"
	generators = ["madgraph_tauola", "pythia_tauola", "powheg_tauola", "powheg_pythia", "pythia", "madgraph"]
	for generator in generators:
		if nickname.find(generator) > -1: return generator
	raise NameError("Generator information for nickname " + nickname + " could not be determined!")

def getPuScenario(nickname, centerOfMassEnergy):
	if getProcess(nickname) == "Data": return "None"
	dataSetName = datasets[nickname]["dataset"][centerOfMassEnergy]
	posLeft = dataSetName.find("PU_")
	posRight = posLeft + dataSetName[posLeft+3:].find("_")
	return dataSetName[posLeft:posRight+3]

def getDatasetName(nickname, centerOfMassEnergy):
	dataSetName = datasets[nickname]["dataset"][centerOfMassEnergy]
	return dataSetName

def getProductionCampaignGlobalTag(nickname, centerOfMassEnergy):
	dataSetName = datasets[nickname]["dataset"][centerOfMassEnergy]
	if getProcess(nickname) == "Data": return "None"
	posLeft = dataSetName.find("_START") +1
	posRight = posLeft + dataSetName[posLeft+5:].find("-")
	return dataSetName[posLeft:posRight+5]

def isData(nickname):
	return (getProcess(nickname) == "Data")