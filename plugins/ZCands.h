#ifndef ZCANDS
#define ZCANDS

#include "MatchElectrons.h"

struct ZBoson
{
  ZBoson(const TLorentzVector& e1, const TLorentzVector& e2);
  TLorentzVector Z;
  vector<TLorentzVector> Daughters;
  bool DaughtersMatch;

  void MatchDaughters(const vector<TriggerObjectStandAlone>& objs, const double& dR_thresh=0.2);
};

vector<ZBoson> ZCands( const vector<Electron>& eles );

vector<ZBoson> MatchedZCands( const vector< pair<Electron,TLorentzVector> >& matchedEles);

#endif // ZCANDS
