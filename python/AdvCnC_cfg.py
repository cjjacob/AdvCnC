import FWCore.ParameterSet.Config as cms

process = cms.Process("AdvCnC")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)

import TriggerTools.AdvCnC.ROOTfiles_cff as DataFiles

import FWCore.Utilities.FileUtils as FileUtils

process.source = cms.Source("PoolSource",
    fileNames = DataFiles.Run2016Feb17ReReco.files,
)

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Services_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, "80X_dataRun2_2016SeptRepro_v7", "")

process.AdvCnC = cms.EDAnalyzer('AdvCnC',
    electrons = cms.InputTag("slimmedElectrons"),
    results = cms.InputTag("TriggerResults","","HLT"),
    objects = cms.InputTag("selectedPatTrigger"),

    refPath  = cms.untracked.string("HLT_Ele27_WPTight_Gsf_v"),
    pathName = cms.untracked.string("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"),

#    refFilter = cms.untracked.string("hltEle27WPTightGsfTrackIsoFilter"),
    pathFilterLeg1 = cms.untracked.string("hltEle23Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg1Filter"),
    pathFilterLeg2 = cms.untracked.string("hltEle23Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg2Filter"),

#    EFilterLeg1 = cms.untracked.string("hltEle23Ele12CaloIdLTrackIdLIsoVLEtLeg1Filter"),
#    EFilterLeg2 = cms.untracked.string("hltEle23Ele12CaloIdLTrackIdLIsoVLEtLeg2Filter"),

)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("rAdvCnC.root"),
    closeFileFast = cms.untracked.bool(False),
)

process.p = cms.Path(process.AdvCnC)
