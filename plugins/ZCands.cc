#include "ZCands.h"

ZBoson::ZBoson(const TLorentzVector& e1, const TLorentzVector& e2) : DaughtersMatch(false)
{
  Z = e1 + e2;
  Daughters.reserve(2);
  Daughters.push_back(e1);
  Daughters.push_back(e2);
}

void ZBoson::MatchDaughters(const vector<TriggerObjectStandAlone>& objs, const double& dR_thresh)
{
  int nMatched = 0;
  for ( TLorentzVector daughter : Daughters ) {
    for ( TriggerObjectStandAlone obj : objs ) {
      TLorentzVector o;
      o.SetPtEtaPhiM( obj.pt(), obj.eta(), obj.phi(), ELE_MASS );
      if ( daughter.DeltaR(o) <= dR_thresh ) {
	++nMatched;
	break;
      }
    }
  }
  DaughtersMatch = nMatched == 2;
}

vector<ZBoson> ZCands( const vector<Electron>& eles )
{
  vector<ZBoson> retvec;

  size_t nEles = eles.size();
  if ( nEles > 1 ) {
    for ( size_t iEle = 0; iEle < nEles-1; ++iEle ) {
      for ( size_t jEle = iEle+1; jEle < nEles; ++jEle ) {
	if ( eles.at(iEle).charge() * eles.at(jEle).charge() < 0.0 ) {
	  TLorentzVector e1, e2;
	  e1.SetPtEtaPhiM( eles.at(iEle).pt(), eles.at(iEle).eta(), eles.at(iEle).phi(), ELE_MASS );
	  e2.SetPtEtaPhiM( eles.at(jEle).pt(), eles.at(jEle).eta(), eles.at(jEle).phi(), ELE_MASS );
	  ZBoson Z(e1, e2);
	  if ( Z.Z.M() >= 61.0 && Z.Z.M() <= 121.0 ) {
	    retvec.push_back(Z);
	  }
	}
      }
    }
  }

  return retvec;
}

vector<ZBoson> MatchedZCands( const vector< pair<Electron,TLorentzVector> >& matchedEles)
{
  vector<ZBoson> retvec;

  size_t nEles = matchedEles.size();
  if ( nEles > 1 ) {
    for ( size_t iEle = 0; iEle < nEles-1; ++iEle ) {
      for ( size_t jEle = iEle+1; jEle < nEles; ++jEle ) {
	if ( matchedEles.at(iEle).first.charge() * matchedEles.at(jEle).first.charge() < 0.0 ) {
	  ZBoson Z(matchedEles.at(iEle).second, matchedEles.at(jEle).second);
	  if ( Z.Z.M() >= 61.0 && Z.Z.M() <= 121.0 ) {
	    retvec.push_back(Z);
	  }
	}
      }
    }
  }

  return retvec;
}
