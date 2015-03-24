
#ifndef KAPPA_TRACKPAIRPRODUCER_H
#define KAPPA_TRACKPAIRPRODUCER_H

#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>

#include <DataFormats/TrackReco/interface/Track.h>
#include <TrackingTools/TransientTrack/interface/TransientTrackBuilder.h>
#include <TrackingTools/PatternTools/interface/TwoTrackMinimumDistance.h>
#include <RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h>

#include "KBaseMultiProducer.h"


class KTrackPairProducer : public KBaseMultiProducer<edm::View<reco::Track>, KTrackPairs>
{

public:
	KTrackPairProducer(const edm::ParameterSet &cfg, TTree *_event_tree, TTree *_run_tree) :
		KBaseMultiProducer<edm::View<reco::Track>, KTrackPairs>(cfg, _event_tree, _run_tree, getLabel()),
		electronsTag(cfg.getParameter<edm::InputTag>("electrons")),
		muonsTag(cfg.getParameter<edm::InputTag>("muons"))
	{
	}

	static const std::string getLabel() { return "TrackPair"; }

	virtual bool onRun(edm::Run const &run, edm::EventSetup const &setup)
	{
		setup.get<TransientTrackRecord>().get("TransientTrackBuilder", this->transientTrackBuilder);
		return true;
	}

	virtual void clearProduct(OutputType &output)
	{
		output.clear();
	}

	virtual void fillProduct(const InputType &in, OutputType &out,
		const std::string &name, const edm::InputTag *tag, const edm::ParameterSet &pset)
	{
		std::cout << "hier 1" << std::endl;
		// get electron and muon collections
		if (electronsTag.label() != "")
		{
			cEvent->getByLabel(electronsTag, electrons);
		}
		if (muonsTag.label() != "")
		{
			cEvent->getByLabel(muonsTag, muons);
		}
		std::cout << "hier 2" << std::endl;
		
		// loop over electrons
		for (edm::View<pat::Electron>::const_iterator particle1 = electrons->begin(); particle1 < electrons->end(); ++particle1)
		{
			for (edm::View<pat::Electron>::const_iterator particle2 = particle1 + 1; particle2 < electrons->end(); ++particle2)
			{
				std::pair<KTrackPair, bool> trackPair = getTrackPair(particle1, particle2);
				if (trackPair.second)
				{
					out.push_back(trackPair.first);
				}
			}
		}
		std::cout << "hier 3" << std::endl;
		
		// loop over muons
		for (edm::View<reco::Muon>::const_iterator particle1 = muons->begin(); particle1 < muons->end(); ++particle1)
		{
			for (edm::View<reco::Muon>::const_iterator particle2 = particle1 + 1; particle2 < muons->end(); ++particle2)
			{
				std::pair<KTrackPair, bool> trackPair = getTrackPair(particle1, particle2);
				if (trackPair.second)
				{
					out.push_back(trackPair.first);
				}
			}
		}
		std::cout << "hier 4" << std::endl;
		
		// loop over electrons and muons
		for (edm::View<pat::Electron>::const_iterator particle1 = electrons->begin(); particle1 < electrons->end(); ++particle1)
		{
			for (edm::View<reco::Muon>::const_iterator particle2 = muons->begin(); particle2 < muons->end(); ++particle2)
			{
				std::pair<KTrackPair, bool> trackPair = getTrackPair(particle1, particle2);
				if (trackPair.second)
				{
					out.push_back(trackPair.first);
				}
			}
		}
		std::cout << "hier 5" << std::endl;
		
		/*
		//Creating a KinematicParticleFactory
		KinematicParticleFactoryFromTransientTrack particleFactory;
		
		for (InputType::const_iterator muon1 = in.begin(); muon1 < in.end(); ++muon1)
		{
			
			reco::TransientTrack transientTrack1 = this->transientTrackBuilder->build(muon1->innerTrack());
			FreeTrajectoryState freeTrajectoryState1 = transientTrack1.impactPointTSCP().theState();
			
			ParticleMass particleMass1 = muon1->mass();
			float particleMassSigma1 = particleMass1 * 1.e-6;
			float chi1 = 0.0; //initial chi2 before kinematic fits.
			float ndf1 = 0.0; //initial ndf before kinematic fits.
			RefCountedKinematicParticle particle1 = particleFactory.particle(transientTrack1, particleMass1, chi1, ndf1, particleMassSigma1);
			
			for (InputType::const_iterator muon2 = muon1 + 1; muon2 < in.end(); ++muon2)
			{
				reco::TransientTrack transientTrack2 = this->transientTrackBuilder->build(muon2->innerTrack());
				FreeTrajectoryState freeTrajectoryState2 = transientTrack2.impactPointTSCP().theState();
				
				ParticleMass particleMass2 = muon2->mass();
				float particleMassSigma2 = particleMass2 * 1.e-6;
				float chi2 = 0.0; //initial chi2 before kinematic fits.
				float ndf2 = 0.0; //initial ndf before kinematic fits.
				RefCountedKinematicParticle particle2 = particleFactory.particle(transientTrack2, particleMass2, chi2, ndf2, particleMassSigma2);
				
				TwoTrackMinimumDistance twoTrackMinimumDistance;
				twoTrackMinimumDistance.calculate(freeTrajectoryState1, freeTrajectoryState2);
				if (twoTrackMinimumDistance.status())
				{
					std::pair<GlobalPoint, GlobalPoint> pcas = twoTrackMinimumDistance.points();
					Vector3DBase<float, GlobalTag> dcaVector = pcas.second - pcas.first;
					ROOT::Math::SVector<double, 3> dcaVectorConverted(
							dcaVector.x(),
							dcaVector.y(),
							dcaVector.z()
					);
					
					auto totCov = particle1->stateAtPoint(pcas.first).kinematicParametersError().matrix() +
					              particle2->stateAtPoint(pcas.second).kinematicParametersError().matrix();
					ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > totCovConverted;
					for (int i = 0; i < 3; ++i)
					{
						for (int j = 0; j < 3; ++j)
						{
							totCovConverted(i, j) = totCov(i, j);
						}
					}
					
					double dca3D = ROOT::Math::Mag(dcaVectorConverted); // twoTrackMinimumDistance.distance();
					double dca3DError = sqrt(ROOT::Math::Similarity(totCovConverted, dcaVectorConverted)) / dca3D;
					
					ROOT::Math::SVector<double, 3> transverseDcaVectorConverted = dcaVectorConverted;
					transverseDcaVectorConverted(2) = 0.0;
					double dca2D = ROOT::Math::Mag(transverseDcaVectorConverted);
					double dca2DError = sqrt(ROOT::Math::Similarity(totCovConverted, transverseDcaVectorConverted)) / dca2D;
					
					KTrackPair trackPair;
					trackPair.dca3D = dca3D;
					trackPair.dca3DError = dca3DError;
					trackPair.dca2D = dca2D;
					trackPair.dca2DError = dca2DError;
					out.push_back(trackPair);
				}
			}
		}
		*/
	}

private:
	edm::InputTag electronsTag;
	edm::InputTag muonsTag;
	
	edm::ESHandle<TransientTrackBuilder> transientTrackBuilder;
	
	edm::Handle<edm::View<pat::Electron> > electrons;
	edm::Handle<edm::View<reco::Muon> > muons;
	
	template<class T1, class T2>
	std::pair<KTrackPair, bool> getTrackPair(T1 particle1, T2 particle2)
	{
		KinematicParticleFactoryFromTransientTrack particleFactory;
		
		reco::TransientTrack transientTrack1 = this->transientTrackBuilder->build(particle1->track()); // innerTrack()
		FreeTrajectoryState freeTrajectoryState1 = transientTrack1.impactPointTSCP().theState();
		ParticleMass particleMass1 = particle1->mass();
		float particleMassSigma1 = particleMass1 * 1.e-6;
		float chi1 = 0.0; //initial chi2 before kinematic fits.
		float ndf1 = 0.0; //initial ndf before kinematic fits.
		RefCountedKinematicParticle kinParticle1 = particleFactory.particle(transientTrack1, particleMass1, chi1, ndf1, particleMassSigma1);
		
		reco::TransientTrack transientTrack2 = this->transientTrackBuilder->build(particle2->track()); // innerTrack()
		FreeTrajectoryState freeTrajectoryState2 = transientTrack2.impactPointTSCP().theState();
		ParticleMass particleMass2 = particle2->mass();
		float particleMassSigma2 = particleMass2 * 1.e-6;
		float chi2 = 0.0; //initial chi2 before kinematic fits.
		float ndf2 = 0.0; //initial ndf before kinematic fits.
		RefCountedKinematicParticle kinParticle2 = particleFactory.particle(transientTrack2, particleMass2, chi2, ndf2, particleMassSigma2);
		
		TwoTrackMinimumDistance twoTrackMinimumDistance;
		twoTrackMinimumDistance.calculate(freeTrajectoryState1, freeTrajectoryState2);
		if (twoTrackMinimumDistance.status())
		{
			std::pair<GlobalPoint, GlobalPoint> pcas = twoTrackMinimumDistance.points();
			Vector3DBase<float, GlobalTag> dcaVector = pcas.second - pcas.first;
			ROOT::Math::SVector<double, 3> dcaVectorConverted(
					dcaVector.x(),
					dcaVector.y(),
					dcaVector.z()
			);
			
			auto totCov = kinParticle1->stateAtPoint(pcas.first).kinematicParametersError().matrix() +
			              kinParticle2->stateAtPoint(pcas.second).kinematicParametersError().matrix();
			ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > totCovConverted;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					totCovConverted(i, j) = totCov(i, j);
				}
			}
			
			double dca3D = ROOT::Math::Mag(dcaVectorConverted); // twoTrackMinimumDistance.distance();
			double dca3DError = sqrt(ROOT::Math::Similarity(totCovConverted, dcaVectorConverted)) / dca3D;
			
			ROOT::Math::SVector<double, 3> transverseDcaVectorConverted = dcaVectorConverted;
			transverseDcaVectorConverted(2) = 0.0;
			double dca2D = ROOT::Math::Mag(transverseDcaVectorConverted);
			double dca2DError = sqrt(ROOT::Math::Similarity(totCovConverted, transverseDcaVectorConverted)) / dca2D;
			
			KTrackPair trackPair;
			//trackPair.hashLepton1 = KLepton::getHash(particle1->Pt(), particle1->Eta(), particle1->Phi(), particle1->mass(), particle1->charge())
			//trackPair.hashLepton2 = KLepton::getHash(particle2->Pt(), particle2->Eta(), particle2->Phi(), particle2->mass(), particle2->charge())
			trackPair.dca3D = dca3D;
			trackPair.dca3DError = dca3DError;
			trackPair.dca2D = dca2D;
			trackPair.dca2DError = dca2DError;
			std::cout << "dca3D = " << trackPair.dca3D << " +/- " << trackPair.dca3DError << ", "
			          << "dca2D = " << trackPair.dca2D << " +/- " << trackPair.dca2DError;
			return std::pair<KTrackPair, bool>(trackPair, true);
		}
		else
		{
			return std::pair<KTrackPair, bool>(KTrackPair(), false);
		}
	}
};

#endif
