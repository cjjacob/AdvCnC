from CRABClient.UserUtilities import config
config = config()

import sys
from time import sleep

from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

def submit(config):
    try:
        crabCommand('submit', config = config)
    except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
        print "Failed submitting task: %s" % (cle.headers)

config.General.workArea        = 'adv_jobs_crab'
config.General.transferOutputs = True
config.General.transferLogs    = False

config.JobType.pluginName         = 'Analysis'
config.JobType.psetName           = 'AdvCnC_cfg.py'
config.JobType.sendExternalFolder = True
config.JobType.maxMemoryMB        = 2500

config.Data.inputDBS         = 'global'
config.Data.splitting        = 'LumiBased'
config.Data.unitsPerJob      = 500
config.Data.lumiMask         = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
config.Data.outLFNDirBase    = '/store/group/phys_smp/cojacob/AdvCnC'
config.Data.publication      = False

config.Site.storageSite = 'T2_CH_CERN'

# DoubleEG
config.General.requestName = 'DoubleEG_Run2016B'
config.Data.inputDataset   = '/DoubleEG/Run2016B-03Feb2017_ver2-v2/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'DoubleEG_Run2016C'
config.Data.inputDataset   = '/DoubleEG/Run2016C-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'DoubleEG_Run2016D'
config.Data.inputDataset   = '/DoubleEG/Run2016D-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'DoubleEG_Run2016E'
config.Data.inputDataset   = '/DoubleEG/Run2016E-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'DoubleEG_Run2016F'
config.Data.inputDataset   = '/DoubleEG/Run2016F-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'DoubleEG_Run2016G'
config.Data.inputDataset   = '/DoubleEG/Run2016G-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'DoubleEG_Run2016Hv2'
config.Data.inputDataset   = '/DoubleEG/Run2016H-03Feb2017_ver2-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'DoubleEG_Run2016Hv3'
config.Data.inputDataset   = '/DoubleEG/Run2016H-03Feb2017_ver3-v1/MINIAOD'
submit(config)
sleep(5)

# SingleElectron
config.General.requestName = 'SingleElectron_Run2016B'
config.Data.inputDataset   = '/SingleElectron/Run2016B-03Feb2017_ver2-v2/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleElectron_Run2016C'
config.Data.inputDataset   = '/SingleElectron/Run2016C-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleElectron_Run2016D'
config.Data.inputDataset   = '/SingleElectron/Run2016D-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleElectron_Run2016E'
config.Data.inputDataset   = '/SingleElectron/Run2016E-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleElectron_Run2016F'
config.Data.inputDataset   = '/SingleElectron/Run2016F-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleElectron_Run2016G'
config.Data.inputDataset   = '/SingleElectron/Run2016G-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleElectron_Run2016Hv2'
config.Data.inputDataset   = '/SingleElectron/Run2016H-03Feb2017_ver2-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleElectron_Run2016Hv3'
config.Data.inputDataset   = '/SingleElectron/Run2016H-03Feb2017_ver3-v1/MINIAOD'
submit(config)
sleep(5)

# # MinimumBias
# config.General.requestName = 'MinimumBias_Run2016B'
# config.Data.inputDataset   = '/MinimumBias/Run2016B-03Feb2017_ver1-v1/MINIAOD'
# submit(config)
# sleep(5)
# config.General.requestName = 'MinimumBias_Run2016C'
# config.Data.inputDataset   = '/MinimumBias/Run2016C-PromptReco-v2/MINIAOD'
# submit(config)
# sleep(5)
# config.General.requestName = 'MinimumBias_Run2016D'
# config.Data.inputDataset   = '/MinimumBias/Run2016D-PromptReco-v2/MINIAOD'
# submit(config)
# sleep(5)
# config.General.requestName = 'MinimumBias_Run2016E'
# config.Data.inputDataset   = '/MinimumBias/Run2016E-PromptReco-v2/MINIAOD'
# submit(config)
# sleep(5)
# config.General.requestName = 'MinimumBias_Run2016F'
# config.Data.inputDataset   = '/MinimumBias/Run2016F-03Feb2017-v1/MINIAOD'
# submit(config)
# sleep(5)
# config.General.requestName = 'MinimumBias_Run2016G'
# config.Data.inputDataset   = '/MinimumBias/Run2016G-PromptReco-v1/MINIAOD'
# submit(config)
# sleep(5)
# config.General.requestName = 'MinimumBias_Run2016Hv2'
# config.Data.inputDataset   = '/MinimumBias/Run2016H-PromptReco-v2/MINIAOD'
# submit(config)
# sleep(5)
# config.General.requestName = 'MinimumBias_Run2016Hv3'
# config.Data.inputDataset   = '/MinimumBias/Run2016H-PromptReco-v3/MINIAOD'
# submit(config)
# sleep(5)

# SingleMuon
config.General.requestName = 'SingleMuon_Run2016B'
config.Data.inputDataset   = '/SingleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleMuon_Run2016C'
config.Data.inputDataset   = '/SingleMuon/Run2016C-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleMuon_Run2016D'
config.Data.inputDataset   = '/SingleMuon/Run2016D-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleMuon_Run2016E'
config.Data.inputDataset   = '/SingleMuon/Run2016E-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleMuon_Run2016F'
config.Data.inputDataset   = '/SingleMuon/Run2016F-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleMuon_Run2016G'
config.Data.inputDataset   = '/SingleMuon/Run2016G-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleMuon_Run2016Hv2'
config.Data.inputDataset   = '/SingleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'SingleMuon_Run2016Hv3'
config.Data.inputDataset   = '/SingleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD'
submit(config)
sleep(5)

# JetHT
config.General.requestName = 'JetHT_Run2016B'
config.Data.inputDataset   = '/JetHT/Run2016B-03Feb2017_ver2-v2/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'JetHT_Run2016C'
config.Data.inputDataset   = '/JetHT/Run2016C-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'JetHT_Run2016D'
config.Data.inputDataset   = '/JetHT/Run2016D-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'JetHT_Run2016E'
config.Data.inputDataset   = '/JetHT/Run2016E-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'JetHT_Run2016F'
config.Data.inputDataset   = '/JetHT/Run2016F-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'JetHT_Run2016G'
config.Data.inputDataset   = '/JetHT/Run2016G-03Feb2017-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'JetHT_Run2016Hv2'
config.Data.inputDataset   = '/JetHT/Run2016H-03Feb2017_ver2-v1/MINIAOD'
submit(config)
sleep(5)
config.General.requestName = 'JetHT_Run2016Hv3'
config.Data.inputDataset   = '/JetHT/Run2016H-03Feb2017_ver3-v1/MINIAOD'
submit(config)
sleep(5)


