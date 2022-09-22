# Raddam
HCAL Radiation Damage Monitoring

Create a CMSSW environment

cmsrel CMSSW_12_4_4\
cd src\
cmsenv\
mkdir HCalAnalyzer\
cd HCalAnalyzer

After setting up local git client, \
git clone https://tokenhere@github.com/username/repositoryname.git \
scram b -j32 

At CMSSW_12_4_4/src/HCalAnalyzer/HBHEHFMonitor/test/01, you can try

cmsRun pythonConfig.py
