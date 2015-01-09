Todo list {#todo}
=================

## Important

1. to be fixed in the C++ code:
    - dz, dxy in KMuon is not filled correctly (IPTools) http://www-ekp.physik.uni-karlsruhe.de/~berger/kappa/struct_k_muon.html#a62be02f7980e717c78fee69e0457389a -> fix it
2. to be fixed/checked in the configuration
    - is the JetID, JetPUID, btag, electron MVA-ID, tau discriminators configured correctly?
    - configure pt thresholds as agreed upon in a meeting (~ 8GeV Leptons, 10 GeV Jets)
    - use snippet by Andrew to select genParticles (only ME + tau decay chain)
    - remove 'PFCandidates' from active (most important point for smaller skims)
3. test the code:
    - check all variables before and after the skimming changes with some events
    - do a comparison with Artus (advantage: can compare in the same ntuple output format value by value)
    - check the lepton isolation values and their usability for delta beta corrections


## Other todos

Frage:
- was ist binValue?
- warum TBronch
- warum hxx und nicht in KDebug.cpp?

List:
- renaming IDs/Ids
- Make debug output reflect new data format
- check everything:
  - Muon ID vars Iso vars rest
  - Tau  ID vars Iso vars rest
  - Ele  ID vars Iso vars rest
  - Jets ID vars Iso?? rest
  - MET
  - Track

- versions:
  - CMSSW 5.3.14 (old)
  - CMSSW 5.3.22 (new 2012)
  - CMSSW 7.0.7_patch1  (CSA)
  - CMSSW 7.XX (Phsy14)
  - MiniAOD
- Rewrite TauProducer
- add scripts to KappaTools
- install for skimming
- usage for analysis
  - see KappaTools

- No producer for?
- clean up Producer headers
- remove template from BasicTau
- move lepton in extra file
- Filters
- guidelines: C++11?, must compile with make and scram, 
- timing: run/lumi/event time per prod. CMSSW (rot) Kappa (blau, stacked) pro event
- Kappa tag from git
- Profile: off, auto, on (auto=if cmssw w
- no ints? short or long?
- Recipe: QG, PUJetID, JetID, MVAMET
- Kappa Tag
- list of files that can be run if present
- hltNames: Electron
- unified status scheme following HepMC: 0-4+11->7
- KLeptonProducer
- CMSSW Jet ID als configurable binary ID
- Handles everywhere!
- automatic eIDs

## Documentation
- add todos
- KappaTools doc?
- compare jet und tagged jet in size
- HEPMCProducer raus in eigenene ehader
- Tower im plural?
- Triggerobjects als vector?
- ../../Producers/interface/KGenParticleProducer.h:#define KAPPA_PARTONPRODUCER_H
../../Producers/interface/KHCALNoiseSummaryProducer.h:#define KAPPA_HCALNOISEPRODUCER_H
../../Producers/interface/KLorentzProducer.h:#define KAPPA_LORENTZPRODUCER_H -> LV
../../Producers/interface/KPFCandidateProducer.h:#define KAPPA_PFCandidatePRODUCER_H
../../Producers/interface/KTreeMetadataProducer.h:#define KAPPA_TREE_METADATAPRODUCER_H
- classes + UP in test -> py
- Jet producer must derive from BasicJet producer!
- get json
- reweighting
- 

# CMSSW
[CMSSW on github](https://github.com/cms-sw/cmssw)
[CMSSW Doxygen](https://cmssdt.cern.ch/SDT/doxygen/)
[CMSSW LXR]()
[grid-control]()
[CSA14](https://twiki.cern.ch/twiki/bin/viewauth/CMS/CSA14)
[SCRAM](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideScram)
[EDM](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideEDMGetDataFromEvent)



Jets
----
pt > 5 GeV for Jet studies
Tagged

Reduce to 2 tags: qg, b and PUIDs


PFCandidates
------------
pt > 10 GeV?
Only within DR 0.5


GenParticles
------------

Reduce number of objects
------------------------
- Electrons: pt > 10 GeV saves
- Muons: pt > 10 GeV
- Jets: pt > 10 GeV
- PFCandidates pt > 10 GeV, DR(lepton) < 0.5
