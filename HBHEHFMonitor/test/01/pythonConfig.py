#!/usr/bin/env python
# -*- coding: utf-8 -*-

import FWCore.ParameterSet.Config as cms  
from Configuration.StandardSequences.Eras import eras
#process = cms.Process("PHASEHFX",eras.Run2_2018) #in Yildiray
process = cms.Process("PFG",eras.Run3)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
#process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('CondCore.CondDB.CondDB_cfi')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
#process.load('Configuration.StandardSequences.ReconstructionCosmics_cff') 
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag='124X_dataRun3_Prompt_v4'
#process.GlobalTag.globaltag='123X_dataRun3_Prompt_v12'
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#//from Configuration.AlCa.GlobalTag import GlobalTag
#from Configuration.AlCa.autoCond import autoCond
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#//process.GlobalTag = GlobalTag(process.GlobalTag, '92X_dataRun2_Prompt_v4', '')
# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
#from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
#process = customisePostLS1(process)

process.maxEvents = cms.untracked.PSet(  
    input = cms.untracked.int32(-1)  
)  
process.MessageLogger = cms.Service("MessageLogger",  
 cout = cms.untracked.PSet(  
  default = cms.untracked.PSet( ## kill all messages in the log  
 
   limit = cms.untracked.int32(0)  
  ),  
  FwkJob = cms.untracked.PSet( ## but FwkJob category - those unlimitted  
 
   limit = cms.untracked.int32(-1)  
  )  
 ),  
 categories = cms.untracked.vstring('FwkJob'), 
 destinations = cms.untracked.vstring('cout')  
)  
 
process.source = cms.Source("PoolSource",  
    fileNames = cms.untracked.vstring( 
#'/store/data/Run2022C/SingleMuon/RAW-RECO/ZMu-PromptReco-v1/000/356/381/00000/fe055f95-3f7d-4fe0-a51a-b9daa243269f.root'
#'/store/data/Run2022C/SingleMuon/RAW-RECO/ZMu-PromptReco-v1/000/355/870/00000/7d656d23-d51e-4fc8-8488-90a0c363d876.root'
#'/store/data/Run2022C/SingleMuon/RAW-RECO/ZMu-PromptReco-v1/000/356/071/00000/28eb835c-b056-4646-bbe1-a85613b15e71.root'
'/store/data/Run2022C/SingleMuon/RAW-RECO/ZMu-PromptReco-v1/000/355/872/00000/3912da70-ad0f-4f70-ac74-7b68ee00bf7f.root'
#'/store/data/Run2018C/SingleMuon/RAW-RECO/ZMu-12Nov2019_UL2018-v2/100000/002BB422-02B7-704F-B2CF-4724E9C1F537.root'
#'/store/data/Run2022D/HcalNZS/RAW/v1/000/357/542/00000/3c2ab376-0320-4333-81bd-b3c8bf6fe0d5.root'
#'/store/data/Run2022B/HcalNZS/ALCARECO/HcalCalMinBias-PromptReco-v1/000/355/102/00000/cef51ae1-81fb-4ea5-9a26-35326b5725fc.root' 
#'/store/data/Run2022B/SingleMuon/ALCARECO/HcalCalIterativePhiSym-PromptReco-v1/000/355/457/00000/579a566f-58c3-4d41-9943-9a6735d832ef.root'
#'/store/data/Run2022C/HcalNZS/RAW/v1/000/355/863/00000/683eca48-87ba-4636-94a5-12105cb3c037.root'
#'/store/data/Run2022C/HcalNZS/AOD/PromptReco-v1/000/355/862/00000/d89beb24-3e4b-4667-aea1-902010b86e4c.root'
#'/store/data/Run2022C/HcalNZS/MINIAOD/PromptReco-v1/000/355/865/00000/f5f6a5ba-c8a2-4bdb-b1a5-727b57caa153.root'
   )  
)  

process.LumiCorrectionSource=cms.ESSource("LumiCorrectionSource",
        authpath=cms.untracked.string('/afs/cern.ch/cms/lumi/DB'),
        #connect=cms.string('oracle://cms_orcon_adg/cms_lumi_prod')
        connect=cms.string('frontier://LumiCalc/CMS_LUMI_PROD'),
        #normtag=cms.untracked.string('HFV2a')
        #datatag=cms.untracked.string('v3')
)

 
#process.oout = cms.OutputModule("PoolOutputModule",
#   outputCommands = cms.untracked.vstring('keep *'),
#    fileName = cms.untracked.string('event_trg_jet2012a_digi.root'),
#    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('p1')
#  )
#)


process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",  
  ignoreTotal = cms.untracked.int32(1) ## default is one  
)  
 
process.hemonitor = cms.EDAnalyzer("HBHEHFMonitor",
Viewmuon=cms.untracked.InputTag("muons"),
TriggerResults2=cms.untracked.InputTag("TriggerResults"),
TriggerResults=cms.untracked.InputTag("TriggerResults","","HLT"),
#TriggerResults=cms.untracked.InputTag("TrigResLabel"),
#LumiScalersCollection=cms.untracked.InputTag("scalersRawToDigi"),
lumiScalers=cms.InputTag("scalersRawToDigi"),  #in Yildiray
metadata=cms.InputTag("onlineMetaDataDigis"),  #in Yildiray
#PFMetCollection=cms.untracked.InputTag("metLabelx"),
PFMetCollection=cms.InputTag("pfMet","", "RECO"),
#PFJetCollection=cms.untracked.InputTag("jetLabelx"),
PFJetCollection=cms.InputTag("ak4PFJets","","RECO"),
TrackCollection=cms.untracked.InputTag("generalTracks"),
lumisummary=cms.untracked.InputTag("lumiProducer"),
hcalRecHitsHBHE =  cms.InputTag("hbhereco",""),
#hcalRecHitsHBHE =  cms.InputTag("reducedHcalRecHits","hbhereco","RECO"),
# hcalRecHitsHBHEpre =  cms.InputTag("hbheprereco",""),
#  hcalRecHitsHO =  cms.InputTag("horeco",""),
#  hcalRecHitsHF =  cms.InputTag("hfreco",""),
  VertexCollection = cms.untracked.InputTag("offlinePrimaryVertices"),
  RootFileName = cms.untracked.string('hbhehfmonitor_2022_01_355457-355463.root'),
  RECO       =  cms.untracked.bool(True),

)

process.correctedfiltr = cms.EDFilter("HLTFilter",
 )  

process.TFileService = cms.Service("TFileService", 
 fileName = cms.string('hist_hbhehfmonitor_2022_01_355457-355463.root'),  
)  
 
process.dump = cms.EDAnalyzer("EventContentAnalyzer")  # The module EventContentAnalyzer dumps all products stored in an event to the screen.

#process.load("HLTrigger.HLTfilters.correctedHighLevel_cfi")
#process.correctedHighLevel.TriggerResultsTag=cms.InputTag("TriggerResults","","HLT") # .cc filedan alıyor. .root olusturrken bu triggerları kullanmamıs belki sonra analizde kullanıcak.(Neden triggerı kullanmadık).Trigger etkilemiyormus.
#process.correctedHighLevel.HLTPaths = cms.vstring("HLT_PFJet320_v*", "HLT_L1SingleJet16_v*")
#process.correctedHighLevel.HLTPaths = cms.vstring("HLT_PFJet320_v0","HLT_PFJet320_v1","HLT_PFJet320_v2","HLT_PFJet320_v3","HLT_PFJet320_v4","HLT_PFJet320_v5","HLT_PFJet320_v6","HLT_PFJet320_v7","HLT_PFJet320_v8","HLT_PFJet320_v9","HLT_PFJet320_v10","HLT_PFJet320_v11","HLT_PFJet320_v12","HLT_PFJet320_v13","HLT_PFJet320_v14","HLT_PFJet320_v15","HLT_PFJet320_v16","HLT_PFJet320_v17","HLT_PFJet320_v18","HLT_PFJet320_v19","HLT_L1SingleJet16_v1")

## The good primary vertex filter ____________________________________________|| ilk carpısma noktası bilgilerini alıyor.Hep verilir.
#process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
#    vertexCollection = cms.untracked.InputTag('offlinePrimaryVertices'),
#    minimumNDOF = cms.uint32(4) ,
#    maxAbsZ = cms.double(24),
#    maxd0 = cms.double(2)
#)
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    minimumNDOF = cms.uint32(4) ,
    maxAbsZ = cms.double(24),
    maxd0 = cms.double(2)
)
## The beam scraping filter __________________________________________________|| primaryden sonra cıkan beam icin filter uygulama
process.noscraping = cms.EDFilter(
    "FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
    )

## The iso-based HBHE noise filter ___________________________________________|| dedektorle ilgili. HBHE icin filter.
#process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
#    inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'), # this is for the 50ns
  inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Loose'),  # this is for the 25ns
#  inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Tight'),  # this is for the 25ns
    reverseDecision = cms.bool(False)
)
#inputtaglerin hepsi cc file da verilir. ed analyzer icinde olmadıgı için yok. cfflerden herhangi birinin icindedir. tamamen googleda gorrsun bu kısmı
process.ApplyBaselineHBHENoiseFilter25nsloose =  cms.EDFilter('BooleanFlagFilter',
    inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Loose'),
    reverseDecision = cms.bool(False)
)

process.ApplyBaselineHBHENoiseFilter25nstight =  cms.EDFilter('BooleanFlagFilter',
    inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Tight'),
    reverseDecision = cms.bool(False) #buna bak
)


## The CSC beam halo tight filter ____________________________________________|| bak
process.load('RecoMET.METFilters.CSCTightHaloFilter_cfi')

## The HCAL laser filter _____________________________________________________|| bak
process.load("RecoMET.METFilters.hcalLaserEventFilter_cfi")
#process.hcalLaserEventFilter.vetoByRunEventNumber=cms.untracked.bool(False) #out Yildiray
process.hcalLaserEventFilter.vetoByRunEventNumber=cms.bool(False) #in Yildiray
#process.hcalLaserEventFilter.vetoByHBHEOccupancy=cms.untracked.bool(True) #out Yildiray
process.hcalLaserEventFilter.vetoByHBHEOccupancy=cms.bool(True) #in Yildiray


## The ECAL dead cell trigger primitive filter _______________________________|| bak
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')
## For AOD and RECO recommendation to use recovered rechits bak
process.EcalDeadCellTriggerPrimitiveFilter.tpDigiCollection = cms.InputTag("ecalTPSkimNA")

## The EE bad SuperCrystal filter ____________________________________________|| bak
process.load('RecoMET.METFilters.eeBadScFilter_cfi')

## The Good vertices collection needed by the tracking failure filter ________|| bak
process.goodVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

## The tracking failure filter _______________________________________________|| bak
process.load('RecoMET.METFilters.trackingFailureFilter_cfi')
#ismi isme atıyor.kısaltıyor.
#process.pa = cms.Path(process.correctedHighLevel)
process.p0 = cms.Path(process.primaryVertexFilter)
process.p1 = cms.Path(process.noscraping)
process.p2 = cms.Path(process.HBHENoiseFilterResultProducer*process.ApplyBaselineHBHENoiseFilter)
process.p3 = cms.Path(process.CSCTightHaloFilter)
process.p4 = cms.Path(process.hcalLaserEventFilter)
process.p5 = cms.Path(process.EcalDeadCellTriggerPrimitiveFilter)
process.p6 = cms.Path(process.goodVertices)
process.p7 = cms.Path(process.trackingFailureFilter)
process.p8 = cms.Path(process.eeBadScFilter)
process.p9 = cms.Path(process.ApplyBaselineHBHENoiseFilter25nsloose)
process.p10 = cms.Path(process.ApplyBaselineHBHENoiseFilter25nstight)

#process.end = cms.EndPath(process.hcalDigis*(process.hbheprereco*process.hbhereco+process.hfreco+process.horeco)*process.largesig)

#process.end = cms.EndPath(process.hcalDigis*(process.ecalDigis+process.ecalPreshowerDigis)*process.calolocalreco*process.largesig)  

#process.e = cms.EndPath(process.oout)
#process.end = cms.EndPath(process.hcalDigis*process.largesig)
#process.p1 = cms.Path(process.HBHENoiseFilterResultProducer*process.ApplyBaselineHBHENoiseFilter*process.ApplyBaselineHBHENoiseFilter25nstight*process.hemonitor)
process.end = cms.EndPath(process.hemonitor)#analyzermda ne varsa onu al burda bitir. p0.p1,p2.. kullanmamıs. Schedule defintion gerekli mi?
