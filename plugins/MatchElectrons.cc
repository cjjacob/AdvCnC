#include "MatchElectrons.h"

vector<Electron> GoodElectrons(const vector<Electron>& slimmedElectrons)
{
  vector<Electron> retvec;

  for ( Electron e : slimmedElectrons ) {
    if ( e.et() > 5.0 && fabs(e.eta()) < 2.5 ) {
      retvec.push_back(e);
    }
  }

  return retvec;
}

vector<Electron> ApplyMediumID(const vector<Electron>& electrons)
{
  vector<Electron> retvec;

  for ( Electron e : electrons ) {
    if ( fabs( (1.0/e.energy()) - (1.0/e.p()) ) < 0.05
	 && e.ecalPFClusterIso() < 0.15 
	 && ( ( fabs(e.eta()) <= 1.479
	        && e.sigmaIetaIeta() < 0.01
		&& e.hcalOverEcal() < 0.12
	      )
	      ||
	      ( fabs(e.eta()) > 1.479 && fabs(e.eta()) < 2.5
	        && e.sigmaIetaIeta() < 0.03
		&& e.hcalOverEcal() < 0.10
	      ) 
            ) 
       ) {
      retvec.push_back(e);
    }
  }

  return retvec;
}

vector<Electron> ElectronsAbovePtThreshold(const vector<Electron>& eles, const double& ptCut)
{
  vector<Electron> retvec;

  for ( Electron e : eles ) {
    if ( e.pt() >= ptCut ) {
      retvec.push_back(e);
    }
  }

  return retvec;
}

vector<TriggerObjectStandAlone> ObjectsWithPath(const vector<TriggerObjectStandAlone>& trObjs, const TriggerNames& names, const string& trPath)
{
  vector<TriggerObjectStandAlone> retvec;

  for ( TriggerObjectStandAlone o : trObjs ) {
    o.unpackPathNames(names);
    if ( o.hasPathName(trPath) ) {
      retvec.push_back(o);
    }
  }

  return retvec;
}

vector<TriggerObjectStandAlone> ObjectsWithAFilter(const vector<TriggerObjectStandAlone>& trObjs, const TriggerNames& names, const string& trFilter)
{
  
  return ObjectsWithAFilter(trObjs, names, vector<string>(1,trFilter));
}

vector<TriggerObjectStandAlone> ObjectsWithAFilter(const vector<TriggerObjectStandAlone>& trObjs, const TriggerNames& names, const vector<string>& trFilters)
{
  vector<TriggerObjectStandAlone> retvec;

  for ( TriggerObjectStandAlone o : trObjs ) {
    o.unpackPathNames(names);
    bool hasAFilter = false;
    for ( string filter : trFilters ) {
      hasAFilter |= o.hasFilterLabel(filter);
    }
    if ( hasAFilter ) {
      retvec.push_back(o);
    }
  }

  return retvec;
}

vector< pair<Electron,TLorentzVector> > MatchElesWithObjs(const vector<Electron>& eles, const vector<TriggerObjectStandAlone>& objs, const double& dR)
{
  vector< pair<Electron,TLorentzVector> > retvec;

  vector<TLorentzVector> tlvObjs;
  for ( TriggerObjectStandAlone o : objs ) {
    TLorentzVector v;
    v.SetPtEtaPhiM( o.pt(), o.eta(), o.phi(), ELE_MASS );
    tlvObjs.push_back(v);
  }

  for ( Electron e : eles ) {
    TLorentzVector tlvEle;
    tlvEle.SetPtEtaPhiM( e.pt(), e.eta(), e.phi(), ELE_MASS );
    double dr_best = dR;
    TLorentzVector bestMatch;
    bool matched = false;
    for ( TLorentzVector tlvObj : tlvObjs ) {
      double dr_current = tlvEle.DeltaR(tlvObj);
      if ( dr_current < dr_best ) {
	matched = true;
	bestMatch = tlvObj;
	dr_best = dr_current;
      }
    }
    if ( matched ) {
      retvec.push_back( make_pair(e, bestMatch) );
    }
  }

  return retvec;
}

inline pair<Electron,TLorentzVector> make_pair(const Electron& e, const TLorentzVector& o)
{
  return pair<Electron,TLorentzVector>(e,o);
}
