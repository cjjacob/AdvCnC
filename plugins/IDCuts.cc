#include "IDCuts.h"

namespace IDCUTS
{

bool ApplyElectronID(const double& rho, const Electron& ele, const IDType& id)
{
  bool pass = true;

  if ( passConvVeto.at(id) ) {
    pass &= ele.passConversionVeto();
  }

  const reco::GsfElectron::PflowIsolationVariables& pfIso = ele.pfIsolationVariables();
  const float chad = pfIso.sumChargedHadronPt;
  const float nhad = pfIso.sumNeutralHadronEt;
  const float pho  = pfIso.sumPhotonEt;
  
  float effArea;
  double _eta = fabs(ele.superCluster()->eta());
  if ( _eta < 1.0 ) { effArea = 0.1703f; }
  else if ( _eta < 1.479 ) { effArea = 0.1715f; }
  else if ( _eta < 2.000 ) { effArea = 0.1213f; }
  else if ( _eta < 2.200 ) { effArea = 0.1230f; }
  else if ( _eta < 2.300 ) { effArea = 0.1635f; }
  else if ( _eta < 2.400 ) { effArea = 0.1937f; }
  else { effArea = 0.2393f; }

  const float _rho = (float)rho;

  const float iso = chad + max(0.0f, nhad + pho - _rho*effArea);

  const float ooE = 1.0f/ele.ecalEnergy();
  const float eoP = ele.eSuperClusterOverP();
  const float ooemoop = fabs(1.0f - eoP)*ooE;

  if ( _eta < 1.4 ) {
    pass &= ele.sigmaIetaIeta() < full5x5_sigmaIetaIeta_B.at(id);
    pass &= ( ele.deltaEtaSuperClusterTrackAtVtx() - ele.superCluster()->eta() + ele.superCluster()->seed()->eta() ) < abs_dEtaInSeed_B.at(id);
    pass &= fabs(ele.deltaPhiSuperClusterTrackAtVtx()) < abs_dPhiIn_B.at(id);
    pass &= ele.hadronicOverEm() < HoverE_B.at(id);
    pass &= iso < RelCombPFIsoWithEACorr_B.at(id);
    pass &= ooemoop < abs_ooEmooP_B.at(id);
    pass &= ele.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS) <= expMissInnerHits_B.at(id);
  }
  else {
    pass &= ele.sigmaIetaIeta() < full5x5_sigmaIetaIeta_B.at(id);
    pass &= ( ele.deltaEtaSuperClusterTrackAtVtx() - ele.superCluster()->eta() + ele.superCluster()->seed()->eta() ) < abs_dEtaInSeed_E.at(id);
    pass &= fabs(ele.deltaPhiSuperClusterTrackAtVtx()) < abs_dPhiIn_E.at(id);
    pass &= ele.hadronicOverEm() < HoverE_E.at(id);
    pass &= iso < RelCombPFIsoWithEACorr_E.at(id);
    pass &= ooemoop < abs_ooEmooP_E.at(id);
    pass &= ele.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS) <= expMissInnerHits_E.at(id);
  }

  return pass;
}

}
