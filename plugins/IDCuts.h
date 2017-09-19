#include <string>
#include <map>

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

using namespace std;
using namespace reco;
using namespace pat;

namespace IDCUTS
{

enum IDType {VETO_ID, LOOSE_ID, MEDIUM_ID, TIGHT_ID};
const size_t numIDs = 4;

// template <class T>
// struct IDCut
// {
//   IDCut(const string& v, const T& vv, const T& vl, const T& vm, const T& vt) : varName(v)
//   {
//     values.resize(numIDs);
//     values.at(VETO_ID)   = vv;
//     values.at(LOOSE_ID)  = vl;
//     values.at(MEDIUM_ID) = vm;
//     values.at(TIGHT_ID)  = vt;
//   }
//   string varName;
//   vector<T> values;
// };

const map<IDType,double> full5x5_sigmaIetaIeta_B = {{VETO_ID,0.0115}, {LOOSE_ID,0.011}, {MEDIUM_ID,0.00998}, {TIGHT_ID,0.00998}};
const map<IDType,double> abs_dEtaInSeed_B = {{VETO_ID,0.00749}, {LOOSE_ID,0.00477}, {MEDIUM_ID,0.00311}, {TIGHT_ID,0.00308}};
const map<IDType,double> abs_dPhiIn_B = {{VETO_ID,0.228}, {LOOSE_ID,0.222}, {MEDIUM_ID,0.103}, {TIGHT_ID,0.0816}};
const map<IDType,double> HoverE_B = {{VETO_ID,0.356}, {LOOSE_ID,0.298}, {MEDIUM_ID,0.253}, {TIGHT_ID,0.0414}};
const map<IDType,double> RelCombPFIsoWithEACorr_B = {{VETO_ID,0.175}, {LOOSE_ID,0.0994}, {MEDIUM_ID,0.0695}, {TIGHT_ID,0.0588}};
const map<IDType,double> abs_ooEmooP_B = {{VETO_ID,0.299}, {LOOSE_ID,0.241}, {MEDIUM_ID,0.134}, {TIGHT_ID,0.00129}};
const map<IDType,int> expMissInnerHits_B = {{VETO_ID,2}, {LOOSE_ID,1}, {MEDIUM_ID,1}, {TIGHT_ID,1}};

const map<IDType,double> full5x5_sigmaIetaIeta_E = {{VETO_ID,0.037}, {LOOSE_ID,0.0314}, {MEDIUM_ID,0.0298}, {TIGHT_ID,0.0292}};
const map<IDType,double> abs_dEtaInSeed_E = {{VETO_ID,0.00895}, {LOOSE_ID,0.00868}, {MEDIUM_ID,0.00609}, {TIGHT_ID,0.00605}};
const map<IDType,double> abs_dPhiIn_E = {{VETO_ID,0.213}, {LOOSE_ID,0.213}, {MEDIUM_ID,0.045}, {TIGHT_ID,0.0394}};
const map<IDType,double> HoverE_E = {{VETO_ID,0.211}, {LOOSE_ID,0.101}, {MEDIUM_ID,0.0878}, {TIGHT_ID,0.0641}};
const map<IDType,double> RelCombPFIsoWithEACorr_E = {{VETO_ID,0.159}, {LOOSE_ID,0.107}, {MEDIUM_ID,0.0821}, {TIGHT_ID,0.0571}};
const map<IDType,double> abs_ooEmooP_E = {{VETO_ID,0.15}, {LOOSE_ID,0.14}, {MEDIUM_ID,0.13}, {TIGHT_ID,0.0129}};
const map<IDType,int> expMissInnerHits_E = {{VETO_ID,3}, {LOOSE_ID,1}, {MEDIUM_ID,1}, {TIGHT_ID,1}};

const map<IDType,bool> passConvVeto = {{VETO_ID,true}, {LOOSE_ID,true}, {MEDIUM_ID,true}, {TIGHT_ID,true}};

bool ApplyElectronID(const double& rho, const Electron& ele, const IDType& id=VETO_ID);

}
