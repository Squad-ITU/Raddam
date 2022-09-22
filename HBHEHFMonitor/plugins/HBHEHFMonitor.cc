

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
//#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/ESInputTag.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"//9june
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"//9june
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"

// Lumi instLumi
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"
#include "DataFormats/Luminosity/interface/LumiDetails.h"
// Lumi corrections
#include "FWCore/Framework/interface/EventSetupRecord.h"
#include "RecoLuminosity/LumiProducer/interface/LumiCorrectionParam.h"
#include "RecoLuminosity/LumiProducer/interface/LumiCorrectionParamRcd.h"
#include "DataFormats/METReco/interface/HcalCaloFlagLabels.h"

// Necessary includes for identify severity of flagged problems in rechits
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"
#include "DataFormats/MuonReco/interface/Muon.h"

#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/Scalers/interface/LumiScalers.h"
#include "DataFormats/OnlineMetaData/interface/OnlineLuminosityRecord.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"


#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TTree.h"
#include "TMath.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "TRandom.h"

#include <string>

#include <iostream>
#include <fstream>


using namespace std;
using namespace edm;
using namespace reco;  
using namespace CLHEP;


const float mu_mass = 0.105658;
const float pival = acos(-1.);

const int nlumimx=27;  //Yildiray
double lumirange[nlumimx+1]={10.,12.,14.,16.,18.,20.,22.,24.,26.,28.,30.,32.,34.,36.,38.,40.,42.,44.,46.,48.,50.,52.,54.,56.,58.,60.,62.,64.};

const int ncorScale=10;
double cor_scales[ncorScale]={0.80, 0.84, 0.88, 0.91, 0.94, 0.97, 1.0, 1.03, 1.06, 1.10};

const int nhbheetamx=58;
const int nhbhephimx=72;
const int nhbhedepthmx=7;  //Yildiray
const int nhbdepthmx=4;  //Yildiray
const int nhbetabin=16; //Yildiray

const int nheeetamn=16;
const int nheeetamx=29;
const int nheephimx=72;
const int nheedepthmx=7; 
const int nheeetabin= nheeetamx - nheeetamn + 1;
const int nheethrmx=1;
const int nhbthrmx=1; //Yildiray
double heeenthrmx[nheethrmx+1] = {/*0.4,*/ 0.5, /*6.0, 8.0, 10.0,*/ 200.0};
double hbenthrmx[nhbthrmx+1] = {/*0.4,*/ 0.5, /*6.0, 8.0, 10.0,*/ 200.0};


const int nfltrmx=11;
void getlumiweight(double val, int nbmx, double* array, int& ilumi, double& lumiwt) {
	ilumi=-1;
	lumiwt=0;
	for (int ix=0; ix<nbmx; ix++) {
		//    cout <<"val "<<val<<" "<<ix<<" "<<array[ix]<<" "<< array[ix+1]<<endl;
		if (val > array[ix] && val < array[ix+1]) { 
			ilumi = ix;
			lumiwt = (0.5*(array[ix]+array[ix+1])/val);
			break;
		}
	}
}

const int nlogsig=120;
// start with -5 to 5, and then log scale

double logptjets[nlogsig+1]={10, 10.4713, 10.9648, 11.4815, 12.0226, 12.5893, 13.1826, 13.8038, 14.4544, 15.1356, 15.8489, 16.5959, 17.378, 18.197, 19.0546, 19.9526, 20.893, 21.8776, 22.9087, 23.9883, 25.1189, 26.3027, 27.5423, 28.8403, 30.1995, 31.6228, 33.1131, 34.6737, 36.3078, 38.0189, 39.8107, 41.6869, 43.6516, 45.7088, 47.863, 50.1187, 52.4807, 54.9541, 57.544, 60.256, 63.0957, 66.0693, 69.1831, 72.4436, 75.8578, 79.4328, 83.1764, 87.0964, 91.2011, 95.4993, 100, 104.713, 109.648, 114.815, 120.226, 125.893, 131.826, 138.038, 144.544, 151.356, 158.489, 165.959, 173.78, 181.97, 190.546, 199.526, 208.93, 218.776, 229.087, 239.883, 251.189, 263.027, 275.423, 288.403, 301.995, 316.228, 331.131, 346.737, 363.078, 380.189, 398.107, 416.869, 436.516, 457.088, 478.63, 501.187, 524.807, 549.541, 575.44, 602.56, 630.957, 660.693, 691.831, 724.436, 758.578, 794.328, 831.764, 870.964, 912.011, 954.993, 1000, 1047.13, 1096.48, 1148.15, 1202.26, 1258.93, 1318.26, 1380.38, 1445.44, 1513.56, 1584.89, 1659.59, 1737.8, 1819.7, 1905.46, 1995.26, 2089.3, 2187.76, 2290.87, 2398.83, 2511.89};



static const int nHLTmx=16;
const char* hlt_name[nHLTmx] ={"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_",
        "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
	"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
	"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_", // 3
	"HLT_IsoMu20_v",//4
	"HLT_IsoTkMu20_v",//5
	"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_",//6
	"HLT_Mu20_Mu10_DZ_", // 7
	"HLT_Mu17_TkMu8_DZ_", // 8
	"HLT_Mu27_TkMu8_", // 9
	"HLT_Mu30_TkMu11_", // 10
	"HLT_Mu40_TkMu11_", // 11
	"HLT_Mu8_TrkIsoVVL_", // 12
	"HLT_Mu17_TrkIsoVVL_", // 13
	"HLT_Mu24_TrkIsoVVL_", // 14
	"HLT_Mu34_TrkIsoVVL_"}; // 15

static const int nHLTFillmx=1; //HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ and all others
//
// class declaration
//

class HBHEHFMonitor : public edm::one::EDAnalyzer<> {
	public:
		explicit HBHEHFMonitor(const edm::ParameterSet&);
//		~HBHEHFMonitor();
                ~HBHEHFMonitor() override;

		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


	private:
		virtual void beginJob() ;
//		virtual void analyze(const edm::Event&, const edm::EventSetup&);
                void analyze(const edm::Event&, const edm::EventSetup&) override;
		virtual void endJob() ;

		virtual void beginRun(edm::Run const&, edm::EventSetup const&);
		virtual void endRun(edm::Run const&, edm::EventSetup const&);
		virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

		int  getHBHEieta(int ij) { return (ij<nhbheetamx/2) ? -nhbheetamx/2 + ij : -nhbheetamx/2 + ij + 1;}
		int  invert_HBHEieta(int ieta) { return (ieta<0) ? nhbheetamx/2+ ieta : nhbheetamx/2 +ieta - 1;}

		// ----------member data ---------------------------
		std::string theRootFileName;
		bool isRECO;
		int  isIndEnergy;           // Look for signal in individual towers (2), ind rmrbx energy(1)
		TFile* theFile;
		TTree* T1;
		int irun, ilumi, ils, ifltr, hlttr, ibunch, Nevt, nprim, ntrks, njets, nrmtag;
		float invmass, dimupt, dimupz, mupt1, mueta1, muphi1, mupt2, mueta2, muphi2;
		float tkpt1, tkpt2, trkdz1, trkdz2, trkdr1, trkdr2;
		float inslumi, avelumi, avepu;
		float misetpf, misphipf, tmpmiset;
		unsigned int mypow_2[32], ievt;
		HLTConfigProvider hltConfig_;

		// HLTPrescaleProvider() hltConfig2_;
		edm::EDGetTokenT<reco::VertexCollection>  vtxTag_;
		edm::EDGetTokenT<reco::TrackCollection>  trck_;
		edm::EDGetTokenT<reco::PFJetCollection>  jets_;
		edm::EDGetTokenT<reco::PFMETCollection>  mets_;
//		edm::EDGetTokenT<LumiScalersCollection>  lumis_;  //out Yildiray
		edm::EDGetTokenT<edm::TriggerResults>  trigs_;
		edm::EDGetTokenT<TriggerResults>  trigs2_;
		edm::EDGetTokenT<edm::View<reco::Muon> > muons_;
		edm::EDGetTokenT<LumiSummary> lumisum_;
		edm::EDGetTokenT<HBHERecHitCollection> tok_hbheht_;
                edm::EDGetTokenT<LumiScalersCollection> scalerToken_;
                edm::EDGetTokenT<OnlineLuminosityRecord> metaDataToken_;

               // edm::ESGetToken<HcalChannelQuality, HcalChannelQualityRcd> hcalChannelQualityToken_; //in Yildiray

		int nhbhe, nhftow;

		int irunold;
		int nHLTevt[nHLTFillmx];

//		TH1F* hbhe_count[nlumimx][2][nheethrmx+1]; //Count in barrel and endcap region. Combined depths
		//Energy energy, mean rms in individual tower
//		TH2F* hbhe_entry[nlumimx][ncorScale][nhbhedepthmx][nheethrmx+1];
//		TH2F* hbhe_energy[nlumimx][ncorScale][nhbhedepthmx][nheethrmx+1];

		TH1F* lumi_entry[nHLTFillmx];
		TH1F* lumi_value[nHLTFillmx];

		//HE
		TH1F* hee_en_pos_eta[nlumimx][nHLTFillmx][nheedepthmx][nheeetabin];
		TH1F* hee_en_neg_eta[nlumimx][nHLTFillmx][nheedepthmx][nheeetabin];

		TH1F* hee_phi_pos_eta[nlumimx][nHLTFillmx][nheedepthmx][nheeetabin][nheethrmx];
		TH1F* hee_phi_neg_eta[nlumimx][nHLTFillmx][nheedepthmx][nheeetabin][nheethrmx];

                //HB                
                TH1F* hb_en_pos_eta[nlumimx][nHLTFillmx][nhbdepthmx][nhbetabin];
                TH1F* hb_en_neg_eta[nlumimx][nHLTFillmx][nhbdepthmx][nhbetabin];

                TH1F* hb_phi_pos_eta[nlumimx][nHLTFillmx][nhbdepthmx][nhbetabin][nhbthrmx];
                TH1F* hb_phi_neg_eta[nlumimx][nHLTFillmx][nhbdepthmx][nhbetabin][nhbthrmx];

		TH1F* h_muonpt1;
		TH1F* h_muonpt2;
		TH1F* h_muoneta1p;
		TH1F* h_muonphi1p;
		TH1F* h_muoneta2p;
		TH1F* h_muonphi2p;
		TH1F* h_muoneta1m;
		TH1F* h_muonphi1m;
		TH1F* h_muoneta2m;
		TH1F* h_muonphi2m;


		TH1F* h_tkpt03;
		TH1F* h_trkdz;
		TH2F* h_trkdxy;
		TH1F* h_dimumass;
		TH1F* h_dimupt;
		TH1F* h_dimupz;

		TH1F* h_dimumass1;
		TH1F* h_dimupt1;
		TH1F* h_dimupz1;

		TH1F* h_dimumass2;
		TH1F* h_dimupt2;
		TH1F* h_dimupz2;     

		TH1F* all_hlttr;
		TH1F* all_inslumi;
		TH1F* all_avepu;
		TH1F* all_ibunch;
		TH1F* all_ntrks;
		TH1F* all_nprim;
		TH1F* all_ifltr;
		TH1F* all_njets;
		TH1F* all_jetpt;
		TH1F* all_jeteta;
		TH1F* all_jetphi;
		TH1F* all_misetpf;
		TH1F* all_misphipf;

		//hcal severity ES
		const HcalChannelQuality* theHcalChStatus;
		//  edm::ESHandle<HcalChannelQuality> theHcalChStatus;
		edm::ESHandle<HcalSeverityLevelComputer> hcalSevLvlComputerHndl;


};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

HBHEHFMonitor::HBHEHFMonitor(const edm::ParameterSet& iConfig) {
    
        //cout <<" - - checkpoint 0 - - "<<endl;
        

	muons_=consumes<edm::View<reco::Muon> >(iConfig.getUntrackedParameter<edm::InputTag>("Viewmuon"));
	trigs2_=consumes<TriggerResults>(iConfig.getUntrackedParameter<edm::InputTag>("TriggerResults2"));
	trigs_=consumes<edm::TriggerResults>(iConfig.getUntrackedParameter<edm::InputTag>("TriggerResults"));
	//lumis_=consumes<LumiScalersCollection>(iConfig.getUntrackedParameter<edm::InputTag>("LumiScalersCollection"));  //out Yildiray
	mets_=consumes<reco::PFMETCollection> ( iConfig.getParameter<edm::InputTag>("PFMetCollection"));
	jets_=consumes<reco::PFJetCollection > ( iConfig.getParameter<edm::InputTag>("PFJetCollection"));
	trck_=consumes<reco::TrackCollection >(iConfig.getUntrackedParameter<edm::InputTag>("TrackCollection"));
	vtxTag_ =  consumes<reco::VertexCollection >(iConfig.getUntrackedParameter<edm::InputTag>("VertexCollection"));
	lumisum_=consumes<LumiSummary,edm::InLumi>(iConfig.getUntrackedParameter<edm::InputTag>("lumisummary"));
        tok_hbheht_ =  consumes<HBHERecHitCollection>( iConfig.getParameter<edm::InputTag>("hcalRecHitsHBHE"));
        scalerToken_ = consumes<LumiScalersCollection>(iConfig.getParameter<edm::InputTag>("lumiScalers"));
        metaDataToken_ = consumes<OnlineLuminosityRecord>(iConfig.getParameter<edm::InputTag>("metadata"));

	edm::Service<TFileService> fs; // FOR THE HISTOGRAM FILE
	theRootFileName = iConfig.getUntrackedParameter<string>("RootFileName");
	isRECO    = iConfig.getUntrackedParameter<bool>("RECO", true);  
	isIndEnergy = iConfig.getUntrackedParameter<int>("IndEnergy", 1);

	theFile = new TFile(theRootFileName.c_str(), "RECREATE"); // FOR THE TREE FILE
	theFile->cd();

	T1 = new TTree("T1", "HBHEMonitor"); 
	T1->Branch("irun", &irun, "irun/I");  
	T1->Branch("ilumi", &ilumi, "ilumi/I"); 
	T1->Branch("ifltr", &ifltr, "ifltr/I");    
	T1->Branch("hlttr", &hlttr, "hlttr/I"); 
	T1->Branch("ibunch", &ibunch, "ibunch/I");   

	T1->Branch("ievt", &ievt, "ievt/i");
	T1->Branch("nprim", &nprim, "nprim/I");
	T1->Branch("ntrks", &ntrks, "ntrks/I");

	T1->Branch("inslumi",&inslumi,"inslumi/F");
	T1->Branch("avelumi",&avelumi,"avelumi/F");
	T1->Branch("avepu",&avepu,"avepu/F");

	T1->Branch("nhbhe",&nhbhe,"nhbhe/I"); 
	T1->Branch("nhftow",&nhftow,"nhftow/I"); 
	T1->Branch("invmass", &invmass, "invmass/F");
	T1->Branch("dimupt", &dimupt, "dimupt/F");
	T1->Branch("dimupz", &dimupz, "dimupz/F");
	T1->Branch("mupt1", &mupt1, "mupt1/F");
	T1->Branch("mueta1", &mueta1, "mueta1/F");
	T1->Branch("muphi1", &muphi1, "muphi1/F");
	T1->Branch("tkpt1", &tkpt1, "tkpt1/F");
	T1->Branch("trkdz1", &trkdz1, "trkdz1/F");
	T1->Branch("trkdr1", &trkdr1, "trkdr1/F");  
	T1->Branch("mupt2", &mupt2, "mupt2/F");
	T1->Branch("mueta2", &mueta2, "mueta2/F");
	T1->Branch("muphi2", &muphi2, "muphi2/F");
	T1->Branch("tkpt2", &tkpt2, "tkpt2/F");
	T1->Branch("trkdz2", &trkdz2, "trkdz2/F");
	T1->Branch("trkdr2", &trkdr2, "trkdr2/F");  

	char name[200];
	char title[200];
/*	for (int lm=0; lm<nlumimx; lm++) {
		for (int ij=0; ij<2; ij++) {
			for (int jk=0; jk<=nheethrmx; jk++) {
				sprintf(name, "hbhe_count_%i_%i_%i", lm, ij, jk);
				sprintf(title, "hbhe Count Lumi:[%g - %g] %s E_{th}=%i", lumirange[lm], lumirange[lm+1], (ij==0) ? "HB":"HE", int(heeenthrmx[jk]));
				hbhe_count[lm][ij][jk] = fs->make<TH1F>(name, title, 300-20*jk, -0.5, 300-20*jk-0.5);
			}
		}
	}

	//2d energy/energy/mean/rms in HBHE
	for (int lm=0; lm<nlumimx; lm++) {
		for (int mn=0; mn<ncorScale; mn++) { 

			for (int ij=0; ij<nhbhedepthmx; ij++) {
				for (int jk=0; jk<=nheethrmx; jk++) {
					sprintf(name, "hbhe_entry_%i_%i_%i_%i", lm, mn, ij, jk);
					sprintf(title, "hbhe entry Lumi:[%g - %g] cor=%g dep%i E_{th}=%i", lumirange[lm], lumirange[lm+1], cor_scales[mn], ij+1, int(heeenthrmx[jk]));
					hbhe_entry[lm][mn][ij][jk] = fs->make<TH2F>(name, title, nhbheetamx+1, -nhbheetamx/2-0.5, nhbheetamx/2+0.5,  nhbhephimx, 0.5, nhbhephimx+0.5);

					sprintf(name, "hbhe_energy_%i_%i_%i_%i", lm, mn, ij, jk);
					sprintf(title, "hbhe energy (GeV) Lumi:[%g - %g] cor=%g dep%i E_{th}=%i", lumirange[lm], lumirange[lm+1], cor_scales[mn], ij+1, int(heeenthrmx[jk]));
					hbhe_energy[lm][mn][ij][jk] = fs->make<TH2F>(name, title, nhbheetamx+1, -nhbheetamx/2-0.5, nhbheetamx/2+0.5,  nhbhephimx, 0.5, nhbhephimx+0.5);
				}
			}


		}
	}*/

	//Luminosity

	for (int jk=0; jk<nHLTFillmx; jk++) {
		sprintf(name, "lumi_entry_%i", jk);
		sprintf(title, "Lumi Entry in %s", hlt_name[jk]);
		lumi_entry[jk] = fs->make<TH1F>(name, title, nlumimx, -0.5, nlumimx-0.5);
		lumi_entry[jk]->Sumw2();

		sprintf(name, "lumi_value_%i", jk);
		sprintf(title, "Lumi Value in %s", hlt_name[jk]);
		lumi_value[jk] = fs->make<TH1F>(name, title, nlumimx, -0.5, nlumimx-0.5);
		lumi_value[jk]->Sumw2();

	}


	//HE
	for (int ij=0; ij<nlumimx; ij++) {
		for (int jk=0; jk<nHLTFillmx; jk++) {

			for (int kl=0; kl<nheedepthmx; kl++) {
				for (int lm=0; lm<nheeetabin; lm++) {
   
                                                      if (lm==0 && kl>3) continue;            //ieta 16 exists in depths 1-2-3-4     //Yildiray 
						      if (lm==1 && (kl==0 || kl>2)) continue; //ieta 17 exists in depths 2-3         //Yildiray
						      if (lm==2 && kl>4) continue;            //ieta 18 exists in depths 1-2-3-4-5   //Yildiray  
						      if (lm>=3 && lm<=9 && kl==6) continue;  //ieta 19-25 exist in all depths but 7 //Yildiray
						      if (lm==13 && kl>2) continue;           //ieta 29 exists in depths 1-2-3       //Yildiray
			                                                                      //ieta 26-28 exist in all depths       //Yildiray

					sprintf(name, "hee_en_pos_eta_%i_%i_%i_%i", ij, jk, kl, lm);
					sprintf(title, "HEE Energy in +ve tower lumi[%g - %g] %s dep%i #eta=%i", lumirange[ij], lumirange[ij+1], hlt_name[jk], kl+1, lm+nheeetamn);
					hee_en_pos_eta[ij][jk][kl][lm] = fs->make<TH1F>(name, title, 525, -5, 100);


					sprintf(name, "hee_en_neg_eta_%i_%i_%i_%i", ij, jk, kl, lm);
					sprintf(title, "HEE Energy in -ve tower lumi[%g - %g] %s dep%i #eta=%i", lumirange[ij], lumirange[ij+1], hlt_name[jk], kl+1, -(lm+nheeetamn));
					hee_en_neg_eta[ij][jk][kl][lm] = fs->make<TH1F>(name, title, 525, -5, 100);

					for (int mn=0; mn<nheethrmx; mn++) {

						sprintf(name, "hee_phi_pos_eta_%i_%i_%i_%i_%i", ij, jk, kl, lm, mn);
						sprintf(title, "HEE all Phi tower lumi[%g - %g] %s dep%i #eta=%i E_{th}=%g ", lumirange[ij], lumirange[ij+1], hlt_name[jk], kl+1, lm+nheeetamn, heeenthrmx[mn]);
						hee_phi_pos_eta[ij][jk][kl][lm][mn] = fs->make<TH1F>(name, title, nheephimx, 0.5, 72.5);
						hee_phi_pos_eta[ij][jk][kl][lm][mn]->Sumw2();       


						sprintf(name, "hee_phi_neg_eta_%i_%i_%i_%i_%i", ij, jk, kl, lm, mn);
						sprintf(title, "HEE all Phi tower lumi[%g - %g] %s dep%i #eta=%i E_{th}=%g ", lumirange[ij], lumirange[ij+1], hlt_name[jk], kl+1, -(lm+nheeetamn), heeenthrmx[mn]);
						hee_phi_neg_eta[ij][jk][kl][lm][mn] = fs->make<TH1F>(name, title, nheephimx, 0.5, 72.5);
						hee_phi_neg_eta[ij][jk][kl][lm][mn]->Sumw2();       

					}
				}	
			}


		}

	}

        //HB
        for (int ij=0; ij<nlumimx; ij++) {
                for (int jk=0; jk<nHLTFillmx; jk++) {

                        for (int kl=0; kl<nhbdepthmx; kl++) {
                                for (int lm=0; lm<nhbetabin; lm++) {


                                        sprintf(name, "hb_en_pos_eta_%i_%i_%i_%i", ij, jk, kl, lm);
                                        sprintf(title, "HB Energy in +ve tower lumi[%g - %g] %s dep%i #eta=%i", lumirange[ij], lumirange[ij+1], hlt_name[jk], kl+1, lm+1);
                                        hb_en_pos_eta[ij][jk][kl][lm] = fs->make<TH1F>(name, title, 525, -5, 100);


                                        sprintf(name, "hb_en_neg_eta_%i_%i_%i_%i", ij, jk, kl, lm);
                                        sprintf(title, "HB Energy in -ve tower lumi[%g - %g] %s dep%i #eta=%i", lumirange[ij], lumirange[ij+1], hlt_name[jk], kl+1, -(lm+1));
                                        hb_en_neg_eta[ij][jk][kl][lm] = fs->make<TH1F>(name, title, 525, -5, 100);

                                        for (int mn=0; mn<nhbthrmx; mn++) {

                                                sprintf(name, "hb_phi_pos_eta_%i_%i_%i_%i_%i", ij, jk, kl, lm, mn);
                                                sprintf(title, "HB all Phi tower lumi[%g - %g] %s dep%i #eta=%i E_{th}=%g ", lumirange[ij], lumirange[ij+1], hlt_name[jk], kl+1, lm+1, hbenthrmx[mn]);
                                                hb_phi_pos_eta[ij][jk][kl][lm][mn] = fs->make<TH1F>(name, title, nheephimx, 0.5, 72.5);
                                                hb_phi_pos_eta[ij][jk][kl][lm][mn]->Sumw2();


                                                sprintf(name, "hb_phi_neg_eta_%i_%i_%i_%i_%i", ij, jk, kl, lm, mn);
                                                sprintf(title, "HB all Phi tower lumi[%g - %g] %s dep%i #eta=%i E_{th}=%g ", lumirange[ij], lumirange[ij+1], hlt_name[jk], kl+1, -(lm+1), hbenthrmx[mn]);
                                                hb_phi_neg_eta[ij][jk][kl][lm][mn] = fs->make<TH1F>(name, title, nheephimx, 0.5, 72.5);
                                                hb_phi_neg_eta[ij][jk][kl][lm][mn]->Sumw2();

                                        }
                                }
                        }


                }

        }


	h_muonpt1 = fs->make<TH1F>("muonpt1", "muonpt1", 240, -240, 240.0);
	h_muonpt2 = fs->make<TH1F>("muonpt2", "muonpt2", 240, -240, 240.0);

	h_muoneta1p = fs->make<TH1F>("muoneta1p", "muoneta1p", 120, -3.0, 3.0);
	h_muonphi1p = fs->make<TH1F>("muonphi1p", "muonphi1p", 120, -pival, pival);
	h_muoneta2p = fs->make<TH1F>("muoneta2p", "muoneta2p", 120, -3.0, 3.0);
	h_muonphi2p = fs->make<TH1F>("muonphi2p", "muonphi2p", 120, -pival, pival);

	h_muoneta1m = fs->make<TH1F>("muoneta1m", "muoneta1m", 120, -3.0, 3.0);
	h_muonphi1m = fs->make<TH1F>("muonphi1m", "muonphi1m", 120, -pival, pival);
	h_muoneta2m = fs->make<TH1F>("muoneta2m", "muoneta2m", 120, -3.0, 3.0);
	h_muonphi2m = fs->make<TH1F>("muonphi2m", "muonphi2m", 120, -pival, pival);


	h_tkpt03 = fs->make<TH1F>("tkpt03", "tkpt03", 120, 0.0, 4.5);
	h_trkdz = fs->make<TH1F>("trkdz", "trkdz", 120, -18.0, 18.0);
	h_trkdxy = fs->make<TH2F>("trkdxy", "trkdxy", 120, -pival, pival, 120, -2.0, 2.0);

	h_dimumass = fs->make<TH1F>("dimumass", "dimumass", 120, 0.0, 240.0);
	h_dimupt = fs->make<TH1F>("dimupt", "dimupt", 180, 0.0, 360.0);
	h_dimupz = fs->make<TH1F>("dimupz", "dimupz", 300, -600.0, 600.0);

	h_dimumass1 = fs->make<TH1F>("dimumass1", "dimumass1", 120, 0.0, 240.0);
	h_dimupt1 = fs->make<TH1F>("dimupt1", "dimup1t", 180, 0.0, 360.0);
	h_dimupz1 = fs->make<TH1F>("dimupz1", "dimupz1", 300, -600.0, 600.0);

	h_dimumass2 = fs->make<TH1F>("dimumass2", "dimumass2", 120, 0.0, 240.0);
	h_dimupt2 = fs->make<TH1F>("dimupt2", "dimupt2", 180, 0.0, 360.0);
	h_dimupz2 = fs->make<TH1F>("dimupz2", "dimupz2", 300, -600.0, 600.0);

	all_hlttr = fs->make<TH1F>("all_hlttr","all HLT trigger", 32, -0.5, 31.5);
//	all_inslumi = fs->make<TH1F>("all_inslumi","Instantaneous luminiosity", 120, 0.0, 1.0);  //out Yildiray
        all_inslumi = fs->make<TH1F>("all_inslumi","Instantaneous luminiosity", 100, 1000.0, 3000.0);
	all_avepu = fs->make<TH1F>("all_avepu","Average Pileup", 140, -0.5, 69.5);     //Yildiray
	all_ibunch = fs->make<TH1F>("all_ibunch","LHC Bunch", 3564, -0.5, 3563.5);
	all_ntrks = fs->make<TH1F>("all_ntrks","# of tracks", 240, -0.5, 2399.5);
	all_nprim = fs->make<TH1F>("all_nprim","# of vextices", 90, -0.5, 89.5);
	all_ifltr = fs->make<TH1F>("all_ifltr","Noise Filter", 15, -0.5, 14.5);
	all_njets = fs->make<TH1F>("all_njets","# of Jets (Pt#gt 20 GeV)", 31, -0.5, 30.5);
	all_jetpt = fs->make<TH1F>("all_jetpt","p#_{T} of Jets", nlogsig, logptjets);
	all_jeteta = fs->make<TH1F>("all_jeteta","#eta of Jets (Pt#gt 50 GeV)", 120, -5.0, 5.0);
	all_jetphi = fs->make<TH1F>("all_jetphi","#phi of Jets (Pt#gt 50 GeV)", 120, -pival, pival);

	all_misetpf = fs->make<TH1F>("all_misetpf","Missing ET", 120, 0.0, 1200.);
	all_misphipf = fs->make<TH1F>("all_misphipf","#phi of MET", 120, -pival, pival);   

/*	double hbhecount[30][3]={{0}};
	if (isIndEnergy>=1) {
		for (int ij=0; ij<nhbheetamx; ij++) {
			int ieta=getHBHEieta(ij);
			for (int jk=0; jk<nhbhephimx; jk++) {
				if (abs(ieta)>=21 && jk%2==1) continue; // here width in phi is 10 degree 
				for (int kl=0; kl<nhbhedepthmx; kl++) { //Need not to book all of these

					if ((abs(ieta)<=14 || abs(ieta)==17) && kl>0) continue; // Only one layer
					if ((abs(ieta)==15 || (abs(ieta)>=18 && abs(ieta)<=26) || abs(ieta)==29) && kl>1) continue; // only two layers
					hbhecount[abs(ieta)][kl]++;

				}
			}
		}

	}
*/

	Nevt=0;
	for (unsigned ij=0; ij<nHLTFillmx; ij++) {
		nHLTevt[ij]= 0.0;
	}
	for (int ix=0; ix<32; ix++) { mypow_2[ix] = pow(2,ix);}
	irunold = -1;
}


HBHEHFMonitor::~HBHEHFMonitor()
{

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
	void
HBHEHFMonitor::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
    
        //cout <<" - - checkpoint 11 - - "<<endl;
    
	bool trgpass[nHLTFillmx]={0};
	hlttr=0;
	invmass = 0.0;
	irun = iEvent.id().run();
	ilumi = iEvent.luminosityBlock();
        ils = iEvent.id().luminosityBlock();  //in Yildiray
	ibunch = iEvent.bunchCrossing();
	all_ibunch->Fill(ibunch);
	ievt = iEvent.id().event();

	Nevt++;
	int jsonpass = 1;

	edm::Handle<reco::TrackCollection> tracks;
	iEvent.getByToken(trck_,tracks);
	ntrks = (tracks.isValid()) ? tracks->size() : -1;
	all_ntrks->Fill(ntrks);

	Handle<VertexCollection> primaryVertices;
	iEvent.getByToken(vtxTag_,primaryVertices);
	nprim = (primaryVertices.isValid()) ? primaryVertices->size() : -1;
	all_nprim->Fill(nprim);

        //cout <<" - - checkpoint 12 - - "<<endl;

	njets = 0;
	edm::Handle<reco::PFJetCollection> PFJets;
	iEvent.getByToken(jets_,PFJets);  

	edm::Handle<PFMETCollection> pfmets;
	iEvent.getByToken(mets_,pfmets);   

	misetpf = misphipf = tmpmiset = -100;
	if (pfmets.isValid()) {
		misetpf = pfmets->begin()->et();
		misphipf = pfmets->begin()->phi();
		tmpmiset =min(misetpf,float(1199.0));

		all_misetpf->Fill(tmpmiset);
		all_misphipf->Fill(misphipf);
	}

        //cout <<" - - checkpoint 13 - - "<<endl;

	if (PFJets.isValid()) {
		for (unsigned jet = 0; jet<PFJets->size(); jet++) {
			if (abs((*PFJets)[jet].eta())>5.0) continue;
			all_jetpt->Fill((*PFJets)[jet].pt());
			if ((*PFJets)[jet].pt()<30) continue;
			all_jeteta->Fill((*PFJets)[jet].eta());
			all_jetphi->Fill((*PFJets)[jet].phi());
			njets++;
		}
	}

        //cout <<" - - checkpoint 14 - - "<<endl;

	all_njets->Fill(njets);

	if (irun !=irunold) {
		irunold = irun;
	}

	inslumi=0.0;
	avelumi=0.0;
	avepu=0;

//	edm::Handle<LumiScalersCollection> lumiScale;  //out Yildiray
        edm::Handle<LumiScalersCollection> lumiScale = iEvent.getHandle(scalerToken_);  //in Yildiray
//	iEvent.getByToken(lumis_,lumiScale);  
        edm::Handle<OnlineLuminosityRecord> metaData = iEvent.getHandle(metaDataToken_);

        //cout <<" - - checkpoint 15 - - "<<endl;

//	if (lumiScale.isValid()){  //out Yildiray
        if (lumiScale.isValid() && !lumiScale->empty()){

           if (lumiScale->begin() != lumiScale->end()) {
 
        //cout <<" - - checkpoint 151 - - "<<endl;

		inslumi=lumiScale->begin()->instantLumi();
		avepu = lumiScale->begin()->pileup();

		all_inslumi->Fill(inslumi);
		all_avepu->Fill(avepu);

//        cout <<" - - inside lumiScale - - "<<" inslumi is = "<< inslumi <<endl;
//        cout <<" - - inside lumiScale - - "<<" avepu is = "<< avepu <<endl;

        //cout <<" - - checkpoint 152 - - "<<endl;

        }

        }
          else if (metaData.isValid()) {
              inslumi = metaData->instLumi();
              avepu = metaData->avgPileUp();

                all_inslumi->Fill(inslumi);
                all_avepu->Fill(avepu);

        //cout <<" - - checkpoint 153 - - "<<endl;
//        cout <<" - - inside metaData - - "<<" inslumi is = "<< inslumi <<endl;
//        cout <<" - - inside metaData - - "<<" avepu is = "<< avepu <<endl;
//        cout <<" - - inside metaData - - "<<" lumisection = "<< ils <<endl;


        }

          else {
               edm::LogInfo("ShallowEventDataProducer")
               << "LumiScalers collection not found in the event; will write dummy values";
        }	

	bool anytrg=false;

        //cout <<" - - checkpoint 16 - - "<<endl;

	Handle<edm::TriggerResults> trigRes;
	iEvent.getByToken(trigs_,trigRes); 
	trgpass[0] = false; //true only for HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_ //Yildiray
	trgpass[1] = false; //will be set true for all triggers //Yildiray
	trgpass[2] = false; 
	if (trigRes.isValid()) {
		edm::TriggerNames triggerNames = iEvent.triggerNames(*trigRes);

                //cout <<" - - checkpoint 17 - - "<<endl;

		unsigned int size = trigRes->size();
		for(unsigned ij = 0; ij<size; ++ij) {
			std::string name = triggerNames.triggerName(ij);
			const char* variab1 = name.c_str(); 
			int ihlt =  trigRes->accept(ij);
			if (ihlt >0) { 
				for (unsigned jk=0; jk<nHLTmx; jk++) {
					if ((strstr(variab1,hlt_name[jk])) && strlen(variab1)-strlen(hlt_name[jk])<5) {
						hlttr +=mypow_2[jk];
						all_hlttr->Fill(jk);

						anytrg = true; //Use combined distribution
						if ((strstr(variab1,hlt_name[jk])) && strlen(variab1)-strlen(hlt_name[jk])<4) {    

							if (jk==0) {
								trgpass[0] = true;
								cout <<"varname "<< Nevt<<" "<<jk<<" "<<variab1<< "" <<hlt_name[jk]<< "trgpass[0]"<<endl;
							} // HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_
							nHLTevt[0]++;

		//					if(jk==1){
		//						trgpass[1] = true;
		//						cout <<"varname "<< Nevt<<" "<<jk<<" "<<variab1<< "" <<hlt_name[jk]<< "trgpass[1]"<<endl;
		//					}

		//					if(jk==2){
		//						trgpass[2] = true;
		//						cout <<"varname "<< Nevt<<" "<<jk<<" "<<variab1<< "" <<hlt_name[jk]<< "trgpass[2]"<<endl;
		//					}
						}	    

					}

				}
			}
		}
	}

        cout <<" - - checkpoint 18 - - "<<endl;

	//RC
	// we want all the triggers in the SingleMu skim under trgpass[0]
	trgpass[0]=true; 
//        trgpass[1]=true; //all triggers option is passed onto trgpass[1] //Yildiray

	ifltr = 0;
	if (jsonpass==1) { 
		ifltr = mypow_2[nfltrmx];
		all_ifltr->Fill(nfltrmx);
	}

	int inoise_shape=0;
	Handle<TriggerResults> myTrig;
	iEvent.getByToken(trigs2_,myTrig); 
	edm::TriggerNames triggerNames = iEvent.triggerNames(*myTrig);
	if(myTrig.isValid()){
		int hltCount2 = myTrig->size();
		if (hltCount2>nfltrmx) hltCount2=nfltrmx;

		for(int ij=0; ij<hltCount2; ij++){
			if (myTrig->accept(ij)) { 
				ifltr += mypow_2[ij];
				all_ifltr->Fill(ij);
			} else {
				if (ij==9 || ij==10) {inoise_shape=1;}
				//	if (ij!=0 && ij!=7) {isnoise=1;}
				//	npass[ij] = false;
			}
		}
	}

	edm::Handle<edm::View<reco::Muon> > muons;
	iEvent.getByToken(muons_,muons);  
	if (!muons.isValid() || muons->size()<2) return;
	edm::View<reco::Muon>::const_iterator muon1;

	bool zbosonLoose = false;

	for( muon1 = muons->begin(); muon1 < muons->end(); muon1++ ) {
		reco::Muon recoMu = *muon1;
		if ( !muon::isMediumMuon(recoMu) ) continue; 
		//if ( !muon::isLooseMuon(recoMu) ) continue;
		//  if (!muon1->isGlobalMuon()) continue;
		// if (!muon1->isTrackerMuon()) continue;
		//cout<<"sonuc= "<<!muon::isMediumMuon(recoMu)<<endl;   
		TrackRef trktrk =muon1->innerTrack();
		HepLorentzVector tmpmuon1(trktrk->px(),trktrk->py(),trktrk->pz(), sqrt(trktrk->p()*trktrk->p()+mu_mass*mu_mass));
		int chg1 = trktrk->charge();
		if(trgpass[0]){
			h_muonpt1->Fill(chg1*tmpmuon1.perp());}

		if (tmpmuon1.perp()<8) continue;
		if (chg1>0) { 
			if(trgpass[0]){
				h_muoneta1p->Fill(tmpmuon1.eta());
				h_muonphi1p->Fill(tmpmuon1.phi());}

		} else {
			if(trgpass[0]){
				h_muoneta1m->Fill(tmpmuon1.eta());
				h_muonphi1m->Fill(tmpmuon1.phi());}

		}
		mupt1 = chg1*tmpmuon1.perp();
		mueta1 = tmpmuon1.eta();
		muphi1 = tmpmuon1.phi();
		tkpt1 = (muon1->isolationR03()).emEt;
		if (tkpt1>10) continue; 
		trkdz1 = trktrk->dz();
		trkdr1 = trktrk->dxy(); 
		h_tkpt03->Fill(tkpt1/mupt1);
		h_trkdz->Fill(trkdz1);
		h_trkdxy->Fill(muphi1, trkdr1);

		edm::View<reco::Muon>::const_iterator muon2;

		for( muon2 = muons->begin(); muon2 < muons->end(); muon2++ ) {
			if (muon1==muon2) continue;
			reco::Muon recoMu1 = *muon2;
			if ( !muon::isMediumMuon(recoMu1) ) continue;
			TrackRef trktrk2 =muon2->innerTrack();
			int chg2 = trktrk2->charge();
			if (chg1*chg2>0) continue;
			HepLorentzVector tmpmuon2(trktrk2->px(),trktrk2->py(),trktrk2->pz(), sqrt(trktrk2->p()*trktrk2->p()+mu_mass*mu_mass));
			h_muonpt2->Fill(chg1*tmpmuon2.perp());
			if (tmpmuon2.perp()<8) continue;
			if (chg2>0) { 
				if(trgpass[0]){
					h_muoneta2p->Fill(tmpmuon2.eta());
					h_muonphi2p->Fill(tmpmuon2.phi());}

			} else {
				if(trgpass[0]){
					h_muoneta2m->Fill(tmpmuon2.eta());
					h_muonphi2m->Fill(tmpmuon2.phi());}

			}

			mupt2 = chg2*tmpmuon2.perp();
			mueta2 = tmpmuon2.eta();
			muphi2 = tmpmuon2.phi();

			trkdz2 = trktrk2->dz();
			trkdr2 = trktrk2->dxy(); 
			h_trkdz->Fill(trkdz2);
			h_trkdxy->Fill(muphi2, trkdr2);

			tkpt2 = (muon2->isolationR03()).emEt;
			if (tkpt2>10) continue; 
			h_tkpt03->Fill(tkpt2/mupt2);
			invmass = (tmpmuon1 + tmpmuon2).mag();

			if(trgpass[0]){
				h_dimumass->Fill(invmass);
				h_dimupt->Fill(min(359.0,(tmpmuon1 + tmpmuon2).perp()));
				h_dimupz->Fill(max(-599.0,min(599.0,(tmpmuon1 + tmpmuon2).pz())));
			}

/*
			if(trgpass[1]){
				h_dimumass1->Fill(invmass);
				h_dimupt1->Fill(min(359.0,(tmpmuon1 + tmpmuon2).perp()));
				h_dimupz1->Fill(max(-599.0,min(599.0,(tmpmuon1 + tmpmuon2).pz())));
			}

			if(trgpass[2]){
				h_dimumass2->Fill(invmass);
				h_dimupt2->Fill(min(359.0,(tmpmuon1 + tmpmuon2).perp()));
				h_dimupz2->Fill(max(-599.0,min(599.0,(tmpmuon1 + tmpmuon2).pz())));
			}
*/			dimupt = (tmpmuon1 + tmpmuon2).perp();
			dimupz = (tmpmuon1 + tmpmuon2).pz();	
			if (dimupt<500 && abs(dimupz)<1000.0 && tkpt1<5 && tkpt2<5 && abs(mupt1)>12 && abs(mupt2)>10 && invmass>40) { 
				zbosonLoose = true;
				//if (invmass>80 && invmass<110) { zboson = true; break;}
			}

		}

	}

    //    //cout <<" - - checkpoint 3 - - "<<endl;

	//RC: the first condition is for trgpass[1] & trgpass[2], the second condition is for trgpass[0]
//	if( (jsonpass==1 && anytrg && inoise_shape==0 && zbosonLoose) || (jsonpass==1 && inoise_shape==0 && trgpass[0])) { 
//        if( (jsonpass==1 && anytrg && inoise_shape==0 && zbosonLoose) || (jsonpass==1 && inoise_shape==0 && trgpass[1])) {   //Yildiray
          if(jsonpass==1 && anytrg && inoise_shape==0 && zbosonLoose) {   //Yildiray
 
		edm::Handle<LumiSummary> lumisum;
		iEvent.getLuminosityBlock().getByToken(lumisum_,lumisum);      
		if (lumisum.isValid()) { 
			avelumi = lumisum->avgInsDelLumi();
		}

		int ilumbn = -1; // inslumi*nlumimx/lumiscl;
		double lumiwt =0;
		getlumiweight(avepu, nlumimx, lumirange, ilumbn, lumiwt);
		//RC: not sure what this lumiwt is needed for, so setting the weight to 1.
		lumiwt=1.;

		if (ilumbn<0) ilumbn = 0;
		if (ilumbn >=nlumimx) ilumbn=nlumimx-1;
		for (unsigned yy=0; yy<nHLTFillmx; yy++) {
			if (trgpass[yy]) {
				lumi_entry[yy]->Fill(ilumbn);
				lumi_value[yy]->Fill(ilumbn, lumiwt);
			}
		}

		edm::Handle<HBHERecHitCollection> hbheht;

                //cout <<" - - checkpoint 19 - - "<<endl;

		if (isRECO) {
			iEvent.getByToken(tok_hbheht_,hbheht);
		}

		cout <<"numberofev= " <<Nevt<<"\t"<<"digi "<<int(hbheht.isValid())<<endl;

		//HBHE RECO signal
//		double hbhecnt[nlumimx][2][nheethrmx+1]={{{0}}};

		if (hbheht.isValid() && (*hbheht).size()>0) {
			nhbhe = (*hbheht).size();
	//		if (trgpass[0]) {hbhe_entry[ilumbn][0][0][0]->Fill(-1.,-1.);}   // Total HBHE Entries
			for (HBHERecHitCollection::const_iterator ij=(*hbheht).begin(); ij!=(*hbheht).end(); ij++) {
				HcalDetId id =(*ij).id();
				int tmpeta= id.ieta();
				int tmpphi= id.iphi(); 
				int tmpdep = id.depth()-1;

//				int ihbhe = (abs(tmpeta)<=15 || (abs(tmpeta)==16 && tmpdep<=2)) ? 0 : 1;

				double energy = (*ij).energy();
	/*			if (trgpass[0] && energy > cor_scales[0]*heeenthrmx[0]) { 
					for (int jk=0; jk<=nheethrmx; jk++) { 
						if (energy > heeenthrmx[jk] && (jk==nheethrmx || (jk<nheethrmx && energy<heeenthrmx[nheethrmx])))  { 
							hbhecnt[ilumbn][ihbhe][jk]++;
						}
						for (int mn=0; mn<ncorScale; mn++) {
							if (energy*cor_scales[mn] > heeenthrmx[jk] && (jk==nheethrmx || (jk<nheethrmx && energy<heeenthrmx[nheethrmx])))  { 

								hbhe_entry[ilumbn][mn][tmpdep][jk]->Fill(tmpeta, tmpphi);
								hbhe_energy[ilumbn][mn][tmpdep][jk]->Fill(tmpeta, tmpphi, energy);
							}
						}
					}
				}*/

				if (abs(tmpeta)>=nheeetamn) { //Look only on HE towers, |ieta|>=16
					for (unsigned yy=0; yy<nHLTFillmx; yy++) {
						if (trgpass[yy]) { 
							int tmpeta1 = (tmpeta>0) ? tmpeta-nheeetamn+1 : tmpeta+nheeetamn-1;
							int tmpeta2 = abs(tmpeta1)-1;

							if (tmpeta>0) { hee_en_pos_eta[ilumbn][yy][tmpdep][tmpeta2]->Fill(energy);} 
							else { hee_en_neg_eta[ilumbn][yy][tmpdep][tmpeta2]->Fill(energy);	}

							if (energy <heeenthrmx[nheethrmx]) { 
								for (int ien =0; ien<nheethrmx; ien++) { 
									if (energy >heeenthrmx[ien]) { 

										if (tmpeta>0) {
											hee_phi_pos_eta[ilumbn][yy][tmpdep][tmpeta2][ien]->Fill(tmpphi, lumiwt*energy);
                                                                                    //    if(tmpeta2==12 && tmpdep==0){cout<<" - - Energy of ieta 28 depth 1 = "<<energy<<" - PileUp = "<<2*ilumbn+11<<endl;}
										} else {
											hee_phi_neg_eta[ilumbn][yy][tmpdep][tmpeta2][ien]->Fill(tmpphi, lumiwt*energy);
										}

									}
								}
							} // if (energy <heeenthrmx[nheethrmx])
						} //if (trgpass[yy])
					} //for (unsigned yy=0; yy<nHLTFillmx; yy++)
				} //if (abs(tmpeta) >=nheeetamn)


                                if (abs(tmpeta)<=nheeetamn) {  ////Look only on HB towers, |ieta|<=16 
                                        for (unsigned yy=0; yy<nHLTFillmx; yy++) {
                                                if (trgpass[yy]) {  
                                                        int tmpeta3 = abs(tmpeta)-1;

                                                        if (tmpeta>0) { hb_en_pos_eta[ilumbn][yy][tmpdep][tmpeta3]->Fill(energy);}
                                                        else { hb_en_neg_eta[ilumbn][yy][tmpdep][tmpeta3]->Fill(energy);       }

                                                        if (energy <hbenthrmx[nhbthrmx]) { 
                                                                for (int ien =0; ien<nhbthrmx; ien++) {
                                                                        if (energy >hbenthrmx[ien]) {

                                                                                if (tmpeta>0) {
                                                                                        hb_phi_pos_eta[ilumbn][yy][tmpdep][tmpeta3][ien]->Fill(tmpphi, lumiwt*energy);
                                                                                } else {
                                                                                        hb_phi_neg_eta[ilumbn][yy][tmpdep][tmpeta3][ien]->Fill(tmpphi, lumiwt*energy);
                                                                                }

                                                                        }
                                                                }
                                                        } //if (energy <hbenthrmx[nhbthrmx])
                                                } //if (trgpass[yy]) 
                                        } //for (unsigned yy=0; yy<nHLTFillmx; yy++)
                                } //if (abs(tmpeta) >=nheeetamn)

			} //for (HBHERecHitCollection::const_iterator ij=(*hbheht).begin(); ij!=(*hbheht).end(); ij++)
		} //if (hbheht.isValid() && (*hbheht).size()>0)

/*
		if (trgpass[0]) { 
			for (int lm=0; lm<nlumimx; lm++) {
				for (int ij=0; ij<2; ij++) {
					for (int jk=0; jk<=nheethrmx; jk++) {
						hbhe_count[lm][ij][jk]->Fill(hbhecnt[lm][ij][jk]);
					}
				}
			}
		}*/

	} // if (json/zboson/inoise)

        //cout <<" - - checkpoint 20 - - "<<endl;

	if (invmass>25) { T1->Fill(); }
}


// ------------ method called once each job just before starting event loop  ------------
	void 
HBHEHFMonitor::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
	void 
HBHEHFMonitor::endJob() 
{

	theFile->cd();

	theFile->Write();
	theFile->Close();
	cout<<"End of HBHEHFMonitor with event "<<Nevt<<endl;
}

// ------------ method called when starting to processes a run  ------------
	void 
HBHEHFMonitor::beginRun(edm::Run const& run, edm::EventSetup const& es)
{
	// HCAL channel status map ****************************************
//	edm::ESHandle<HcalChannelQuality> hcalChStatus;    
//	es.get<HcalChannelQualityRcd>().get("withTopo", hcalChStatus );
//	theHcalChStatus = hcalChStatus.product();

//        edm::ESGetToken<HcalChannelQuality, HcalChannelQualityRcd> hcalChannelQualityToken_;
//        hcalChannelQualityToken_{esConsumes<HcalChannelQuality, HcalChannelQualityRcd>(edm::ESInputTag("", "withTopo"))};
//        theHcalChStatus = &es.getData(hcalChannelQualityToken_);

	bool changed(true);
	if (hltConfig_.init(run, es, "HLT",changed)) {
		hltConfig_.dump("Triggers");
		hltConfig_.dump("PrescaleTable"); 

	}

}

// ------------ method called when ending the processing of a run  ------------
	void 
HBHEHFMonitor::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
	void 
HBHEHFMonitor::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
	void 
HBHEHFMonitor::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HBHEHFMonitor::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}


//define this as a plug-in
DEFINE_FWK_MODULE(HBHEHFMonitor);
