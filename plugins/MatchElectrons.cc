#include "MatchElectrons.h"

vector<Electron> GoodElectrons(const vector<Electron>& slimmedElectrons)
{
  vector<Electron> retvec;

  for ( const Electron& e : slimmedElectrons ) {
    if ( e.et() > 5.0 && fabs(e.eta()) < 2.5 ) {
      retvec.push_back(e);
    }
  }

  return retvec;
}

vector<Electron> ApplyMediumID(const vector<Electron>& electrons, const double& rho)
{
  vector<Electron> retvec;

  for ( const Electron& e : electrons ) {
    if ( IDCUTS::ApplyElectronID(rho, e, IDCUTS::MEDIUM_ID) ) {
      retvec.push_back(e);
    }
  }

  return retvec;
}

vector<Electron> ElectronsAbovePtThreshold(const vector<Electron>& eles, const double& ptCut)
{
  vector<Electron> retvec;

  for ( const Electron& e : eles ) {
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

vector< pair<Electron,TriggerObjectStandAlone> > MatchElesWithFullObjs(const vector<Electron>& eles, const vector<TriggerObjectStandAlone>& objs, const double& dR)
{
  vector< pair<Electron,TriggerObjectStandAlone> > retvec;

  for ( Electron e : eles ) {
    TLorentzVector tlvEle;
    tlvEle.SetPtEtaPhiM( e.pt(), e.eta(), e.phi(), ELE_MASS );
    double dr_best = dR;
    bool matched = false;
    TriggerObjectStandAlone bestMatch;

    for ( TriggerObjectStandAlone o : objs ) {
      if ( !(o.type(82) || o.type(92)) ) { continue; } // must be ele or SC object
      TLorentzVector tlvObj;
      tlvObj.SetPtEtaPhiM( o.pt(), o.eta(), o.phi(), ELE_MASS );
      double dr_current = tlvEle.DeltaR(tlvObj);
      if ( dr_current < dr_best ) {
	matched = true;
	bestMatch = o;
	dr_best = dr_current;
      }
    }

    if ( matched ) { retvec.push_back( make_pair(e, bestMatch) ); }
  }

  return retvec;
}

vector<TriggerObjectStandAlone> FilterObjectsByType(const vector<TriggerObjectStandAlone>& objs, const int& type)
{
  return FilterObjectsByType(objs, vector<int>(1,type));
}

vector<TriggerObjectStandAlone> FilterObjectsByType(const vector<TriggerObjectStandAlone>& objs, const vector<int>& types)
{
  vector<TriggerObjectStandAlone> retvec;
  
  for ( TriggerObjectStandAlone o : objs ) {
    for ( int t : types ) {
      if ( o.type(t) ) {
	retvec.push_back(o);
	break;
      }
    }
  }
  
  return retvec;
}
