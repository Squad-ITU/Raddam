# Raddam
HCAL Radiation Damage Monitoring

Create a CMSSW environment

cmsrel CMSSW_12_4_4\
cd src\
cmsenv

After setting up local git client, \
git clone https://tokenhere@github.com/Squad-ITU/Raddam.git \
scram b -j32 

At CMSSW_12_4_4/src/Raddam/HBHEHFMonitor/test/01, you can try

cmsRun pythonConfig.py
