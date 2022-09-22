#/usr/bin/env python
# -*- coding: utf-8 -*-
#from CRABClient.UserUtilities import config, getUsernameFromSiteDB
from CRABClient.UserUtilities import config

from WMCore.Configuration import Configuration

config = Configuration() # this function provide the user to import and use the function in their CRAB cfg. file
#cdozen = getUsernameFromSiteDB()
config.section_('General') 
config.General.transferOutputs = True #(boolean) transferring output files to the destination.Defaults to True. if False, the output files are discarded and the user can not recover them
config.General.transferLogs = True #(boolean) to copy the jobs log files to the storage site. Defaults to False 
config.General.workArea = 'Area_355457-355463_Test' #(string) the area where to create the CRAB project directory. if it is not exist,CRAB will try to create it using "mkdir"
config.General.requestName = '01_Prompt_2022B_v1_355457-355463_Test10' #(string)it is used by CRAB to create a project directory where files corresponding to this particular task will be stored.

config.section_('JobType')
config.JobType.psetName = 'pythonConfig.py'#(string) The name of the CMSSW parameter-set configuration file that should be run via cmsRun . don't need this for MC generation for pp collisions. 
config.JobType.pluginName = 'Analysis'#(string) specifies if this task is running an analysis ('Analysis') on an existing dataset or is running MC event Generation ('PrivateMC')
config.JobType.allowUndistributedCMSSW = True #(boolean) Whether to allow or not using a CMSSW release possibly not avaliable at sites .Defaults to False.
config.JobType.maxMemoryMB = 500 # (integer) Maximun amount of memort (in MB) a job is allowed to use.Defaults to 2000
config.JobType.maxJobRuntimeMin = 600 # (integer) The max. runtime (in minutes) per job. Jpbs running longer than this amount of time will be removed. Defaults to 1315(21 h 55 min)
config.JobType.outputFiles = ['hbhehfmonitor_2022_01_355457-355463.root']

config.section_('Data')
#config.Data.inputDataset = '/DoubleMuon/Run2017B-PromptReco-v1/RECO'#(string) when running an anlaysis over a dataset registered in DBS, this paramete specifies the name of dataset. The dataset can be an official CMS dataset or a dataset produced by a user
config.Data.inputDataset = '/SingleMuon/Run2022B-HcalCalIterativePhiSym-PromptReco-v1/ALCARECO'
#config.Data.inputDataset = '/SingleMuon/Run2022C-ZMu-PromptReco-v1/RAW-RECO'
config.Data.splitting = 'LumiBased'
#config.Data.splitting = 'Automatic'
#config.Data.splitting = 'FileBased' #for private MC this must be'EventBased' #(string) Mode to use to split the task in jobs
config.Data.inputDBS = 'global'
config.Data.unitsPerJob = 10 #number of files per job 
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions18/13TeV/PromptReco/Cert_314472-320065_13TeV_PromptReco_Collisions18_JSON.txt'
#config.Data.lumiMask = 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions22/Cert_Collisions2022_355100_357900_Golden.json'
config.Data.lumiMask = 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions22/Cert_Collisions2022_355100_357900_Muon.json'
#config.Data.runRange = '315257-315489'
config.Data.runRange = '355457-355463'
config.Data.totalUnits = -1 #number of event
config.Data.ignoreLocality = True
#config.Data.publication = True #(boolean) Whether to publish ot not the EDM output files. The corresponding output files have to be transferred to the permanent storage element. Defaults to False 
#config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
#config.Data.publishDataName = '2015Dromptreco_v3'

#config.Data.outLFNDirBase = '/store/user/magaras/2017_315257-315489'#(string) The first part of the LFN of the output files.
config.Data.outLFNDirBase = '/store/user/ykomurcu/Raddam22/Prompt/01_Prompt_2022B_v1_355457-355463_Test'#(string) The first part of the LFN of the output files.
#config.Data.ignoreLocality = True #(boolean) if it is True: allow the jobs to run at any site, regardless of where the input dataset is hosted (this parameter has effect only when Data.Inputset is used)
config.Data.outputDatasetTag = 'collision2022B_v1_355457-355463'# (string) the LFN of the output files and the publication dataset name.
config.section_('Site')
config.Site.storageSite = 'T2_US_Purdue'
#config.Site.storageSite = 'T2_CH_CERN'#(string) Site where the output files should be permanently copied to. 
#config.Site.storageSite = 'T3_US_FNALLPC'
#config.Site.storageSite = 'T2_TR_METU'#(string) Site where the output files should be permanently copied to. 
#config.Site.blacklist = ['T1_ES_PIC'] # (list of strings) A user specified list of sites where thr jobs can tun
#config.Site.whitelist =['T1_ES_PIC']
#config.Site.whitelist =['T3_US_FNALLPC']   # inserted Yildiray
#config.Site.whitelist =['T1_US_FNAL']
config.Site.whitelist = ['T2_US_Purdue'] #(list of strings) A user specified list of sites where thr jobs can tun.Yazmazsan random gonderir.fail verirse hangi site da basarılı ise onu yaz.
 
