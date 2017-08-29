// -*- C++ -*-
//
// Package:    TriggerTools/AdvCnC
// Class:      AdvCnC
// 
/**\class AdvCnC AdvCnC.cc TriggerTools/AdvCnC/plugins/AdvCnC.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Colin James Jacob
//         Created:  Fri, 30 Jun 2017 08:48:48 GMT
//
//

// C++ includes

// CMSSW includes
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/Candidate/interface/Candidate.h"

// ROOT includes
#include "Math/SMatrix.h"
#include "Math/VectorUtil.h"
#include "TVector3.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include <TMath.h>
#include <TMatrixD.h>
#include <TROOT.h>

// personal includes
#include "MatchElectrons.h"
#include "ZCands.h"
#include "Histograms.h"

using namespace std;
using namespace reco;
using namespace edm;
using namespace pat;


//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class AdvCnC : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit AdvCnC(const edm::ParameterSet&);
      ~AdvCnC();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
  EDGetTokenT<ElectronCollection> electrons_;
  EDGetTokenT<TriggerObjectStandAloneCollection> objects_;
  EDGetTokenT<TriggerResults> results_;

  string triggerName, pathFilterLeg1, pathFilterLeg2, refTrigName, refFilter;

  TH3D* EtaEtaMPass;
  TH3D* EtaEtaMFail;

  TH3D* EtaEtaMPass_noPtCut;
  TH3D* EtaEtaMFail_noPtCut;

  TH3D* ZPass;
  TH3D* ZFail;

  TH3D* ZPass_noPtCut;
  TH3D* ZFail_noPtCut;

  TH3D* EtaEtaMPass_justEles;
  TH3D* EtaEtaMFail_justEles;

  TH3D* ZPass_justEles;
  TH3D* ZFail_justEles;

  TH3D* EtaEtaZPt;
  TH3D* EtaEtaZPz;
  TH3D* EtaEtaZP;

  TH3D* EtaEtaMPass_SingleLeg1;
  TH3D* EtaEtaMFail_SingleLeg1;
  TH3D* EtaEtaMPass_SingleLeg2;
  TH3D* EtaEtaMFail_SingleLeg2;

  TH1D* MPass;
  TH1D* MFail;

  TH1D* MPass_noPtCut;
  TH1D* MFail_noPtCut;

  TH1D* nEventEles;
  TH1D* nGoodEles;
  TH1D* nElesAbovePt;
  TH1D* nElesMedAndPt;
  TH1D* nElesMatched;
  TH1D* nTrigObjsFilt;
  TH1D* nTrigObjsPath;
  TH1D* nZs;
  TH1D* nMatchedZs;

  TH1D* nEvents;
  TH1D* nTriggerFound;
  TH1D* nTriggerRun;
  TH1D* nTriggerAccept;

  size_t triggerIndex;

  unsigned long nTrigFound, nTrigRun, nTrigAccept;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
AdvCnC::AdvCnC(const edm::ParameterSet& iConfig)
  : electrons_(consumes<ElectronCollection>(iConfig.getParameter<InputTag>("electrons")))
  , objects_(consumes<TriggerObjectStandAloneCollection>(iConfig.getParameter<InputTag>("objects")))
  , results_(consumes<TriggerResults>(iConfig.getParameter<InputTag>("results")))
{
   //now do what ever initialization is needed
   usesResource("TFileService");

   triggerName = iConfig.getUntrackedParameter<string>("pathName", "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");
   pathFilterLeg1 = iConfig.getUntrackedParameter<string>("pathFilterLeg1");
   pathFilterLeg2 = iConfig.getUntrackedParameter<string>("pathFilterLeg2");
   refTrigName = iConfig.getUntrackedParameter<string>("refPath");
   refFilter = iConfig.getUntrackedParameter<string>("refFilter");

   nTrigFound = 0UL; nTrigRun = 0UL; nTrigAccept = 0UL;
}


AdvCnC::~AdvCnC()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
AdvCnC::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  nEvents->Fill(1);

  Handle<ElectronCollection> electrons;
  iEvent.getByToken(electrons_, electrons);
  nEventEles->Fill(electrons->size());

  Handle<TriggerObjectStandAloneCollection> objects;
  iEvent.getByToken(objects_, objects);

  Handle<TriggerResults> results;
  iEvent.getByToken(results_, results);

  const TriggerNames TrigNames = iEvent.triggerNames(*results);

  size_t nTrigs = results->size();
  for ( triggerIndex = 0U; triggerIndex < nTrigs; ++triggerIndex ) {
    TString trigName = TrigNames.triggerName(triggerIndex);
    if ( trigName.Contains(triggerName) ) {
      break;
    }
  }
  if ( triggerIndex == nTrigs ) { // above loop finished without finding triggerName
    return;
  }

  vector<int> EMtypes; EMtypes.push_back(82); EMtypes.push_back(92);
  vector<TriggerObjectStandAlone> EMObjects = FilterObjectsByType(*objects, EMtypes);

  vector<string> filters; filters.push_back(pathFilterLeg1); filters.push_back(pathFilterLeg2);
  vector<TriggerObjectStandAlone> filterObjs = ObjectsWithAFilter(EMObjects, TrigNames, filters);
  vector<TriggerObjectStandAlone> pathObjs = ObjectsWithPath(EMObjects, TrigNames, triggerName+"*");

  nTrigObjsFilt->Fill(filterObjs.size());
  nTrigObjsPath->Fill(pathObjs.size());

  vector<Electron> goodElectrons = GoodElectrons(*electrons);
  vector<Electron> ptCutElectrons = ElectronsAbovePtThreshold(goodElectrons);
  vector<Electron> mediumIDptCutEles = ApplyMediumID(ptCutElectrons);

  nGoodEles->Fill(goodElectrons.size());
  nElesAbovePt->Fill(ptCutElectrons.size());
  nElesMedAndPt->Fill(mediumIDptCutEles.size());

  ++nTrigFound;
  nTriggerFound->Fill(1);
  if ( results->wasrun(triggerIndex) ) {
    ++nTrigRun;
    nTriggerRun->Fill(1);
    if ( results->accept(triggerIndex) ) {
      ++nTrigAccept;
      nTriggerAccept->Fill(1);

      vector<ZBoson> zCandsPtCutEles = ZCands(mediumIDptCutEles);
      nZs->Fill(zCandsPtCutEles.size());
      int nZsWithMatches = 0;
      for ( ZBoson z : zCandsPtCutEles ) { 
	z.MatchDaughters(filterObjs); 
	if ( z.DaughtersMatch ) { ++nZsWithMatches;
	  ZPass->Fill(z.Z.Pt(), z.Z.Eta(), z.Z.M());
	  EtaEtaMPass->Fill(z.Daughters.at(0).Eta(), z.Daughters.at(1).Eta(), z.Z.M());
	  MPass->Fill(z.Z.M());
	}
	else {
	  ZFail->Fill(z.Z.Pt(), z.Z.Eta(), z.Z.M());
	  EtaEtaMFail->Fill(z.Daughters.at(0).Eta(), z.Daughters.at(1).Eta(), z.Z.M());
	  MFail->Fill(z.Z.M());
	}
      } // loop over z's
      nMatchedZs->Fill(nZsWithMatches);

      vector<ZBoson> zCands = ZCands(goodElectrons);
      for ( ZBoson z : zCands ) { 
	z.MatchDaughters(filterObjs); 
	if ( z.DaughtersMatch ) {
	  ZPass_noPtCut->Fill(z.Z.Pt(), z.Z.Eta(), z.Z.M());
	  EtaEtaMPass_noPtCut->Fill(z.Daughters.at(0).Eta(), z.Daughters.at(1).Eta(), z.Z.M());
	  MPass_noPtCut->Fill(z.Z.M());
	}
	else {
	  ZFail_noPtCut->Fill(z.Z.Pt(), z.Z.Eta(), z.Z.M());
	  EtaEtaMFail_noPtCut->Fill(z.Daughters.at(0).Eta(), z.Daughters.at(1).Eta(), z.Z.M());
	  MFail_noPtCut->Fill(z.Z.M());
	}
      } // loop over z's no pt cut
      
    } // trig accept
  } // trig run

  vector< pair<Electron,TLorentzVector> > matchedEles = MatchElesWithObjs(mediumIDptCutEles, filterObjs);
  nElesMatched->Fill(matchedEles.size());

  if ( matchedEles.size() == 2 ) {
    pair<Electron,TLorentzVector> e1 = matchedEles.at(0);
    pair<Electron,TLorentzVector> e2 = matchedEles.at(1);
    TLorentzVector Z = e1.second + e2.second;
    if ( e1.first.charge() * e2.first.charge() < 0.0 
	 && fabs(Z.M() - 91.0) <= 30.0 ) {
      if ( results->wasrun(triggerIndex) ) {
	EtaEtaZPt->Fill(e1.second.Eta(), e2.second.Eta(), Z.Pt());
	EtaEtaZPz->Fill(e1.second.Eta(), e2.second.Eta(), Z.Pz());
	EtaEtaZP->Fill(e1.second.Eta(), e2.second.Eta(), Z.P());
	if ( results->accept(triggerIndex) ) {
	  EtaEtaMPass_justEles->Fill(e1.second.Eta(), e2.second.Eta(), Z.M());
	}
	else {
	  EtaEtaMFail_justEles->Fill(e1.second.Eta(), e2.second.Eta(), Z.M());
	}
      }
    }
  }

  // vector<ZBoson> matchedZs = MatchedZCands(matchedEles);

  if ( results->wasrun(triggerIndex) ) {
    vector<TriggerObjectStandAlone> refObjs = ObjectsWithPath(EMObjects, TrigNames, refTrigName+"*");
    vector< pair<Electron,TLorentzVector> > matchedRef = MatchElesWithObjs(goodElectrons, refObjs);
    vector< pair<Electron,TriggerObjectStandAlone> > ObjMatchedEles = MatchElesWithFullObjs(goodElectrons, EMObjects);
    if ( matchedRef.size() > 0 ) {
      for ( auto refEle : matchedRef ) {
	for ( auto oEle : ObjMatchedEles ) {
	  TLorentzVector tlvObjEle;
	  tlvObjEle.SetPtEtaPhiM( oEle.second.pt(), oEle.second.eta(), oEle.second.phi(), ELE_MASS );
	  if ( refEle.second.DeltaR(tlvObjEle) < 0.2 ) { continue; } // don't want to use self

	  TLorentzVector Z = refEle.second + tlvObjEle;

	  if ( oEle.second.hasFilterLabel(pathFilterLeg1) ) {
	    EtaEtaMPass_SingleLeg1->Fill( refEle.second.Eta(), tlvObjEle.Eta(), Z.M() );
	  }
	  else {
	    EtaEtaMFail_SingleLeg1->Fill( refEle.second.Eta(), tlvObjEle.Eta(), Z.M() );
	  }

	  if ( oEle.second.hasFilterLabel(pathFilterLeg2) ) {
	    EtaEtaMPass_SingleLeg2->Fill( refEle.second.Eta(), tlvObjEle.Eta(), Z.M() );
	  }
	  else{
	    EtaEtaMFail_SingleLeg2->Fill( refEle.second.Eta(), tlvObjEle.Eta(), Z.M() );
	  }
	}
      }
    }
  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
AdvCnC::beginJob()
{
  Service<TFileService> fs;
  TFileDirectory Efficiency = fs->mkdir("Efficiency");
  TFileDirectory NoPtCut    = fs->mkdir("NoPtCut");
  TFileDirectory Numbers    = fs->mkdir("Numbers");
  TFileDirectory JustEles   = fs->mkdir("JustEles");
  TFileDirectory SingleLeg  = fs->mkdir("SingleLeg");

  nEventEles = Numbers.make<TH1D>("nEventEles", "Num. of Electrons;#", 7, -0.5, 6.5);
  nGoodEles = Numbers.make<TH1D>("nGoodEles", "Num. of Electrons |#eta| < 2.5, E_{T} > 5 GeV;#", 7, -0.5, 6.5);
  nElesAbovePt = Numbers.make<TH1D>("nElesAbovePt", "Num. of Electrons with p_{T} > 25 GeV;#", 7, -0.5, 6.5);
  nElesMedAndPt = Numbers.make<TH1D>("nElesMedAndPt", "Num. of Electrons with medium ID and p_{T} > 25 GeV;#", 7, -0.5, 6.5);
  nTrigObjsFilt = Numbers.make<TH1D>("nTrigObjsFilt", "Num. of Trig Objs with filter;#", 7, -0.5, 6.5);
  nTrigObjsPath = Numbers.make<TH1D>("nTrigObjsPath", "Num. of Trig Objs with path;#", 7, -0.5, 6.5);
  nElesMatched = Numbers.make<TH1D>("nElesMatched", "Num. of Electrons matched by #Delta R;#", 7, -0.5, 6.5);
  nZs = Numbers.make<TH1D>("nZs", "Num. of Z Bosons;#", 4, -0.5, 3.5);
  nMatchedZs = Numbers.make<TH1D>("nMatchedZs", "Num. of Matched Z Bosons;#", 4, -0.5, 3.5);

  nEvents = Numbers.make<TH1D>("nEvents", "Num. of events looped over", 1, 0, 2);
  nTriggerFound = Numbers.make<TH1D>("nTriggerFound", "Num. of times trigger found", 1, 0, 2);
  nTriggerRun = Numbers.make<TH1D>("nTriggerRun", "Num. of times trigger run", 1, 0, 2);
  nTriggerAccept = Numbers.make<TH1D>("nTriggerAccept", "Num. of times trigger accepted", 1, 0, 2);

  EtaEtaMPass = Efficiency.make<TH3D>("EtaEtaMPass", "#eta_{e1} #eta_{e2} Pass;#eta_{e1};#eta_{e2};m_{ee}", nEtaBins, etaBins, nEtaBins, etaBins, nMassBins, massBins);
  EtaEtaMFail = Efficiency.make<TH3D>("EtaEtaMFail", "#eta_{e1} #eta_{e2} Fail;#eta_{e1};#eta_{e2};m_{ee}", nEtaBins, etaBins, nEtaBins, etaBins, nMassBins, massBins);
  
  EtaEtaMPass_noPtCut = NoPtCut.make<TH3D>("EtaEtaMPass_noPtCut", "#eta_{e1} #eta_{e2} Pass;#eta_{e1};#eta_{e2};m_{ee}", nEtaBins, etaBins, nEtaBins, etaBins, nMassBins, massBins);
  EtaEtaMFail_noPtCut = NoPtCut.make<TH3D>("EtaEtaMFail_noPtCut", "#eta_{e1} #eta_{e2} Fail;#eta_{e1};#eta_{e2};m_{ee}", nEtaBins, etaBins, nEtaBins, etaBins, nMassBins, massBins);

  EtaEtaMPass_justEles = JustEles.make<TH3D>("EtaEtaMPass_justEles", "#eta_{e1} #eta_{e2} Pass;#eta_{e1};#eta_{e2};m_{ee}", nEtaBins, etaBins, nEtaBins, etaBins, nMassBins, massBins);
  EtaEtaMFail_justEles = JustEles.make<TH3D>("EtaEtaMFail_justEles", "#eta_{e1} #eta_{e2} Fail;#eta_{e1};#eta_{e2};m_{ee}", nEtaBins, etaBins, nEtaBins, etaBins, nMassBins, massBins);

  ZPass = Efficiency.make<TH3D>("ZPass", "p_{T,Z} #eta_{Z} Pass;p_{T,Z};#eta{Z};m_{Z}", nPtBins, ptBins, nEtaBins, etaBins, nMassBins, massBins);
  ZFail = Efficiency.make<TH3D>("ZFail", "p_{T,Z} #eta_{Z} Fail;p_{T,Z};#eta{Z};m_{Z}", nPtBins, ptBins, nEtaBins, etaBins, nMassBins, massBins);

  ZPass_noPtCut = NoPtCut.make<TH3D>("ZPass_noPtCut", "p_{T,Z} #eta_{Z} Pass;p_{T,Z};#eta{Z};m_{Z}", nPtBins, ptBins, nEtaBins, etaBins, nMassBins, massBins);
  ZFail_noPtCut = NoPtCut.make<TH3D>("ZFail_noPtCut", "p_{T,Z} #eta_{Z} Fail;p_{T,Z};#eta{Z};m_{Z}", nPtBins, ptBins, nEtaBins, etaBins, nMassBins, massBins);

  ZPass_justEles = JustEles.make<TH3D>("ZPass_justEles", "p_{T,Z} #eta_{Z} Pass;p_{T,Z};#eta{Z};m_{Z}", nPtBins, ptBins, nEtaBins, etaBins, nMassBins, massBins);
  ZFail_justEles = JustEles.make<TH3D>("ZFail_justEles", "p_{T,Z} #eta_{Z} Fail;p_{T,Z};#eta{Z};m_{Z}", nPtBins, ptBins, nEtaBins, etaBins, nMassBins, massBins);

  MPass = Efficiency.make<TH1D>("MPass", "Z mass Pass;m_{Z}", nMassBins, massBins);
  MFail = Efficiency.make<TH1D>("MFail", "Z mass Fail;m_{Z}", nMassBins, massBins);

  MPass_noPtCut = NoPtCut.make<TH1D>("MPass_noPtCut", "Z mass Pass;m_{Z}", nMassBins, massBins);
  MFail_noPtCut = NoPtCut.make<TH1D>("MFail_noPtCut", "Z mass Fail;m_{Z}", nMassBins, massBins);

  EtaEtaZPt = JustEles.make<TH3D>("EtaEtaZPt", "#eta #eta p_{T,Z};#eta_{e1};#eta_{e2};p_{T,Z} (GeV)", nEtaBins, etaBins, nEtaBins, etaBins, nPtBins, ptBins);
  EtaEtaZPz = JustEles.make<TH3D>("EtaEtaZPz", "#eta #eta p_{z,Z};#eta_{e1};#eta_{e2};p_{z,Z} (GeV)", nEtaBins, etaBins, nEtaBins, etaBins, nPtBins, ptBins);
  EtaEtaZP  = JustEles.make<TH3D>("EtaEtaZP",  "#eta #eta p_{Z};#eta_{e1};#eta_{e2};p_{Z} (GeV)",     nEtaBins, etaBins, nEtaBins, etaBins, nPtBins, ptBins);

  EtaEtaMPass_SingleLeg1 = SingleLeg.make<TH3D>("EtaEtaMPass_SingleLeg1", "Leg 1 #eta_{tag} #eta_{probe} Pass;#eta_{tag};#eta_{probe};m_{ee} (GeV)", nEtaBins, etaBins, nEtaBins, etaBins, nMassBins, massBins);
  EtaEtaMFail_SingleLeg1 = SingleLeg.make<TH3D>("EtaEtaMFail_SingleLeg1", "Leg 1 #eta_{tag} #eta_{probe} Fail;#eta_{tag};#eta_{probe};m_{ee} (GeV)", nEtaBins, etaBins, nEtaBins, etaBins, nMassBins, massBins);
  EtaEtaMPass_SingleLeg2 = SingleLeg.make<TH3D>("EtaEtaMPass_SingleLeg2", "Leg 2 #eta_{tag} #eta_{probe} Pass;#eta_{tag};#eta_{probe};m_{ee} (GeV)", nEtaBins, etaBins, nEtaBins, etaBins, nMassBins, massBins);
  EtaEtaMFail_SingleLeg2 = SingleLeg.make<TH3D>("EtaEtaMFail_SingleLeg2", "Leg 2 #eta_{tag} #eta_{probe} Fail;#eta_{tag};#eta_{probe};m_{ee} (GeV)", nEtaBins, etaBins, nEtaBins, etaBins, nMassBins, massBins);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
AdvCnC::endJob() 
{
  cout << "nTrigFound:  " << nTrigFound  << "\n";
  cout << "nTrigRun:    " << nTrigRun    << "\n";
  cout << "nTrigAccept: " << nTrigAccept << "\n";
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
AdvCnC::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(AdvCnC);
