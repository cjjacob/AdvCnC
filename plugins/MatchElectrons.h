#ifndef MATCH_ELECTRONS
#define MATCH_ELECTRONS

// C++ includes
#include <map>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>

// CMSSW includes
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/Candidate.h"

// ROOT includes
#include "Math/VectorUtil.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include <TMath.h>

// personal includes

using namespace std;
using namespace reco;
using namespace edm;
using namespace pat;

const double ELE_MASS = 0.000511;

vector<Electron> GoodElectrons(const vector<Electron>& slimmedElectrons);

vector<Electron> ApplyMediumID(const vector<Electron>& electrons);

vector<Electron> ElectronsAbovePtThreshold(const vector<Electron>& eles, const double& ptCut = 25.0);

vector<TriggerObjectStandAlone> ObjectsWithPath(const vector<TriggerObjectStandAlone>& trObjs, const TriggerNames& names, const string& trPath);

vector<TriggerObjectStandAlone> ObjectsWithAFilter(const vector<TriggerObjectStandAlone>& trObjs, const TriggerNames& names, const string& trFilter);

vector<TriggerObjectStandAlone> ObjectsWithAFilter(const vector<TriggerObjectStandAlone>& trObjs, const TriggerNames& names, const vector<string>& trFilters);

vector< pair<Electron,TLorentzVector> > MatchElesWithObjs(const vector<Electron>& eles, const vector<TriggerObjectStandAlone>& objs, const double& dR=0.2);

vector< pair<Electron,TriggerObjectStandAlone> > MatchElesWithFullObjs(const vector<Electron>& eles, const vector<TriggerObjectStandAlone>& objs, const double& dR=0.2);

vector<TriggerObjectStandAlone> FilterObjectsByType(const vector<TriggerObjectStandAlone>& objs, const int& type);

vector<TriggerObjectStandAlone> FilterObjectsByType(const vector<TriggerObjectStandAlone>& objs, const vector<int>& types);

template <class A, class B>
inline pair<A,B> make_pair(const A& a, const B& b)
{
  return pair<A,B>(a,b);
}

#endif // MATCH_ELECTRONS

