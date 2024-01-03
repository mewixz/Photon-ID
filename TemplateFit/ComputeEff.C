#include "TROOT.h"
#include "TSystem.h"
#include "TString.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStopwatch.h"
#include "Def.h"
void Load_Branches_MC(TChain *chain);
void Load_Branches_Data(TChain *chain);
Bool_t isCNNTight(Double_t eta, Double_t p, Double_t cov);
Bool_t isCNNLoose(Double_t eta, Double_t p, Double_t cov);
double getXSec(Int_t mc);
double  getMCScale(int name);
double getDataScale(TString name);


void ComputeEff(){

	double lumi = 43.6*10e-3;
	int    year = 2017;

	double eta_min = 0.0;
	double eta_max = 2.37;
	TString isoWP = "loose";
    	//TString isoWP = "caloonly";
    	//TString isoWP = "tight";

	
	TChain *Sig  = new TChain("output");
	TChain *Bkg  = new TChain("output");
	TChain *Data = new TChain("output");

	TString target = "llg";

	TString Case = "My";
	

	double PT [] = {10, 15, 20, 25, 30, 35, 40, 45, 50, 60, 80, 100};
	int PT_nbins = 12;

	Double_t ETA [] = {0, 0.4, 0.6, 0.8, 1.37, 1.52, 1.81, 2.01, 2.37};
	Int_t ETA_nbins = 9;

	Double_t MU [] = {0, 20, 30, 40, 50, 60, 70, 100};
	Int_t MU_nbins = 8;

	std::vector<TString> Conv_flag;
	Conv_flag.push_back("UnConverted");
	Conv_flag.push_back("Converted");

	double mllg_max = 105;
	double mllg_min = 65;
	int    mllg_bin = 16;

	const int n_flag = Conv_flag.size();

	std::vector<TString> Selections;

	Selections.push_back("All");
	Selections.push_back("Tight_E_dep");
	Selections.push_back("Tight_E_inc");
	Selections.push_back("Tight_E_CNN");
	Selections.push_back("Loose_E");
	Selections.push_back("Loose_E_CNN");

	const int n_sel = Selections.size();

	std::vector<TString> Samples;

	Samples.push_back("Sig");
	Samples.push_back("Bkg");
	Samples.push_back("Data");

	const int n_smpl = Samples.size();

	TH1F* h_E[n_flag][n_sel][n_smpl]; 
	TH1F* h_ETA[n_flag][n_sel][n_smpl]; 
	TH1F* h_MU[n_flag][n_sel][n_smpl]; 
	TH1F* h_Mllg[n_flag][n_sel][n_smpl];

	TH1F* h_Mllg_bin[n_flag][n_sel][n_smpl][PT_nbins];
	TH1F* h_Mllg_binETA[n_flag][n_sel][n_smpl][ETA_nbins];
	TH1F* h_Mllg_binMU[n_flag][n_sel][n_smpl][MU_nbins];

	TH1F* h_CNN_bin[n_flag][n_smpl][PT_nbins];
	TH1F* h_CNN_binETA[n_flag][n_smpl][ETA_nbins];
	TH1F* h_CNN_binMU[n_flag][n_smpl][MU_nbins];


	TFile* output = new TFile(Case+"_Eta_1_output_ComputeEff_"+target+"_"+isoWP+".root","RECREATE");


	for(int i = 0; i<n_flag; i++)
	{
		for(int k = 0; k<n_smpl; k++)
		{
			for(int h = 0; h<PT_nbins; h++)
			{
				h_CNN_bin[i][k][h] = new TH1F("Hist_CNN_"+Conv_flag[i]+"_"+Samples[k]+"_bin_"+std::to_string(h), " Event CNN; CNN Output; N Events ", 100, 0., 1.);
			}
			for(int h = 0; h<ETA_nbins; h++)
			{
				h_CNN_binETA[i][k][h] = new TH1F("Hist_CNN_"+Conv_flag[i]+"_"+Samples[k]+"_binETA_"+std::to_string(h), " Event CNN; CNN Output; N Events ", 100, 0., 1.);
			}
			for(int h = 0; h<MU_nbins; h++)
			{
				h_CNN_binMU[i][k][h] = new TH1F("Hist_CNN_"+Conv_flag[i]+"_"+Samples[k]+"_binMU_"+std::to_string(h), " Event CNN; CNN Output; N Events ", 100, 0., 1.);
			}
			
		}

		for(int j = 0; j<n_sel; j++)
		{
			for(int k = 0; k<n_smpl; k++)
			{
				h_E[i][j][k]   = new TH1F("Hist_E_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k], " Photon Energy; Photon E [GeV]; N Events ", PT_nbins-1, PT);
				h_ETA[i][j][k] = new TH1F("Hist_ETA_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k], " Photon #eta; Photon |#eta|; N Events ", ETA_nbins-1, ETA);
				h_MU[i][j][k]  = new TH1F("Hist_MU_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k], " Event #mu; <#mu> ; N Events", MU_nbins-1, MU);
				h_Mllg[i][j][k]= new TH1F("Hist_Mllg_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k], " Event M_{ll#gamma}; M_{ll#gamma} [GeV]; N Events ", mllg_bin, mllg_min, mllg_max);

				for(int h = 0; h<PT_nbins; h++)
				{
					h_Mllg_bin[i][j][k][h] = new TH1F("Hist_Mllg_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k]+"_bin_"+std::to_string(h), " Event M_{ll#gamma}; M_{ll#gamma} [GeV]; N Events ", mllg_bin, mllg_min, mllg_max);
					

				}

				for(int h = 0; h<ETA_nbins; h++)
				{
					h_Mllg_binETA[i][j][k][h] = new TH1F("Hist_Mllg_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k]+"_binETA_"+std::to_string(h), " Event M_{ll#gamma}; M_{ll#gamma} [GeV]; N Events ", mllg_bin, mllg_min, mllg_max);
					
				}
		
				for(int h = 0; h<MU_nbins; h++)
				{

					h_Mllg_binMU[i][j][k][h] = new TH1F("Hist_Mllg_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k]+"_binMU_"+std::to_string(h), " Event M_{ll#gamma}; M_{ll#gamma} [GeV]; N Events ", mllg_bin, mllg_min, mllg_max);
					

				}

			}

			
		}
	}

	if( target.Contains("eeg") )
	{
		if(Case.Contains("My"))
		{
			Sig->Add("/eos/user/m/mobelfki/RadiativeZlly/Zllg/Sh_Zeegamma.root");
			Bkg->Add("/eos/user/m/mobelfki/RadiativeZlly/Zllj/Sherpa_Zee.root");
			Data->Add("/eos/user/m/mobelfki/RadiativeZlly/data/data_Zeegamma.root");
		}
		if(Case.Contains("PID"))
		{
			Sig->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/mc16d_13TeV/00-03-01/mc16d.Sh_224_NN30NNLO_eegamma_LO*.DAOD_EGAM3.e7006_e5984_s3126_r10201_r10210_p3956.root");
			Bkg->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/mc16d_13TeV/00-03-01/mc16d.Sherpa_221_NNPDF30NNLO_Zee*.DAOD_EGAM3.e5299_e5984_s3126_r10201_r10210_p3954.root");
			Data->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/data17_13TeV/00-03-01/target_data17_Zeeg_p3948.root");
		}
	} 

	else if( target.Contains("mumug") )

	{
		if(Case.Contains("My"))
		{
			Sig->Add("/eos/user/m/mobelfki/RadiativeZlly/Zllg/Sh_Zmumugamma.root");
			Bkg->Add("/eos/user/m/mobelfki/RadiativeZlly/Zllj/Sherpa_Zmumu.root");
			Data->Add("/eos/user/m/mobelfki/RadiativeZlly/data/data_Zmumugamma.root");
		}
		if(Case.Contains("PID"))
		{
			Sig->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/mc16d_13TeV/00-03-01/mc16d.Sh_224_NN30NNLO_mumugamma_LO*.DAOD_EGAM4.e7006_e5984_s3126_r10201_r10210_p3956.root");
			Bkg->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/mc16d_13TeV/00-03-01/mc16d.Sherpa_221_NNPDF30NNLO_Zmumu*.DAOD_EGAM4.e5271_s3126_r10201_r10210_p3954.root");
			Data->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/data17_13TeV/00-03-01/target_data17_Zmumug_p3948.root");
		}
	}

	else 
	{
		if(Case.Contains("My"))
		{
		// all : ee channel
			Sig->Add("/eos/user/m/mobelfki/RadiativeZlly/Zllg/Sh_Zeegamma.root");
			Bkg->Add("/eos/user/m/mobelfki/RadiativeZlly/Zllj/Sherpa_Zee.root");
			Data->Add("/eos/user/m/mobelfki/RadiativeZlly/data/data_Zeegamma.root");

		// mumu channel

			Sig->Add("/eos/user/m/mobelfki/RadiativeZlly/Zllg/Sh_Zmumugamma.root");
			Bkg->Add("/eos/user/m/mobelfki/RadiativeZlly/Zllj/Sherpa_Zmumu.root");
			Data->Add("/eos/user/m/mobelfki/RadiativeZlly/data/data_Zmumugamma.root");
		}
		if(Case.Contains("PID"))
		{
			Sig->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/mc16d_13TeV/00-03-01/mc16d.Sh_224_NN30NNLO_eegamma_LO*.DAOD_EGAM3.e7006_e5984_s3126_r10201_r10210_p3956.root");
			Bkg->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/mc16d_13TeV/00-03-01/mc16d.Sherpa_221_NNPDF30NNLO_Zee*.DAOD_EGAM3.e5299_e5984_s3126_r10201_r10210_p3954.root");
			Data->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/data17_13TeV/00-03-01/target_data17_Zeeg_p3948.root");
			Sig->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/mc16d_13TeV/00-03-01/mc16d.Sh_224_NN30NNLO_mumugamma_LO*.DAOD_EGAM4.e7006_e5984_s3126_r10201_r10210_p3956.root");
			Bkg->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/mc16d_13TeV/00-03-01/mc16d.Sherpa_221_NNPDF30NNLO_Zmumu*.DAOD_EGAM4.e5271_s3126_r10201_r10210_p3954.root");
			Data->Add("/eos/atlas/atlascerngroupdisk/perf-egamma/photonID/NTUP_ZLLG/data17_13TeV/00-03-01/target_data17_Zmumug_p3948.root");
		}
	
	}
	

	Load_Branches_MC(Sig);
	Load_Branches_MC(Bkg);
	
	Load_Branches_Data(Data);

	Int_t SigEntry = (Int_t)Sig->GetEntries();

	std::cout<<"==== Signal Entries : "<< SigEntry <<std::endl; 

	//Loop Signal 
	
	for( Int_t sentry = 0; sentry<SigEntry; sentry++)
	{
	
		Sig->GetEntry(sentry);

		llg_m = llg_m/1000;
        	ll_m = ll_m/1000;
        	ph_E = ph_E/1000;
        	ph_pt = ph_pt/1000;
        	ph_pt2 = ph_pt2/1000;
        	ph_topoetcone20 = ph_topoetcone20/1000;
        	ph_topoetcone40 = ph_topoetcone40/1000;
        	ph_ptcone20 = ph_ptcone20/1000;
        	ph_ptcone40 = ph_ptcone40/1000;
        	ph_ptvarcone20 = ph_ptvarcone20/1000;
        	ph_ptvarcone40 = ph_ptvarcone40/1000;
        	
        	mc_pu_weight = 1.;

		if(isoWP.Contains("loose")) mc_weight = mc_pu_weight*mc_gen_weight*mc_l1_weight*mc_l2_weight*ph_ISO_FixedCutLoose_weight*mc_weight_lumi*getXSec(mc_channel); // need to add cross section
        	else if(isoWP.Contains("tight")) mc_weight = mc_pu_weight*mc_gen_weight*mc_l1_weight*mc_l2_weight*ph_ISO_FixedCutTight_weight*mc_weight_lumi*getXSec(mc_channel);
        	else if(isoWP.Contains("caloonly")) mc_weight =mc_pu_weight*mc_gen_weight*mc_l1_weight*mc_l2_weight*ph_ISO_FixedCutTightCaloOnly_weight*mc_weight_lumi*getXSec(mc_channel);
        	else mc_weight = mc_pu_weight*mc_gen_weight*mc_l1_weight*mc_l2_weight*mc_weight_lumi*getXSec(mc_channel);
        	
		if(Case.Contains("My")) mc_weight *= getMCScale(mc_channel);

		/*
        	if(Case.Contains("My"))
		{
        	if(target.Contains("llg"))
		{
			mc_weight *= 0.9856;
		}else if(target.Contains("eeg")){
			mc_weight *= 0.953;
		}else{
			mc_weight *= 0.9534;
		}
		}
		*/

		//if(!(Trigger_HLT_mu26_imedium || Trigger_HLT_mu26_ivarmedium_OR_HLT_mu50 || Trigger_HLT_mu26_ivarmedium || Trigger_HLT_mu50 || Trigger_HLT_mu50_0eta105_msonly || Trigger_HLT_2mu14 || Trigger_HLT_mu22_mu8noL1 || Trigger_HLT_e26_lhtight_nod0_ivarloose || Trigger_HLT_e60_lhmedium_nod0 || Trigger_HLT_e140_lhloose_nod0 || Trigger_HLT_2e24_lhvloose_nod0)) continue;

		//if( !(Trigger_HLT_g12_loose_match_gamma || Trigger_HLT_g15_loose_L1EM3_match_gamma || Trigger_HLT_g20_tight_icalovloose_L1EM15VHI_match_gamma || Trigger_HLT_g22_tight_L1EM15VHI_match_gamma || Trigger_HLT_g25_loose_match_gamma || Trigger_HLT_g25_medium_L1EM20VH_match_gamma || Trigger_HLT_g35_medium_L1EM20VH_match_gamma || Trigger_HLT_g50_loose_L1EM20VH_match_gamma)) continue;

        	if (ph_l_dRmin < 0.4) continue;
		if(isoWP.Contains("loose"))
        	{
            		if ((ph_topoetcone20 > 0.065*ph_pt) || (ph_ptcone20_TightTTVA_pt1000/ph_pt > 0.05)) continue;
        	}
        	else if(isoWP.Contains("tight"))
        	{
            		if ((ph_topoetcone40 > 0.022*ph_pt + 2.45) || (ph_ptcone20_TightTTVA_pt1000/ph_pt > 0.05)) continue;
        	}
        	else if(isoWP.Contains("caloonly"))
        	{
            		if (!ph_isotightcaloonly) continue;
        	}
        	
        	if (ll_m > 83) continue;
		//if (ll_m < 40) continue;

		//if (llg_m < 60) continue;
		//if (llg_m > 120) continue;
        	
        	if (fabs(ph_eta2) < eta_min || fabs(ph_eta2) > eta_max) continue;
		
        	if (ph_E < 10 || ph_E > 100) continue;

		if(llg_m > 80 && llg_m < 100)
		{
			if(ph_convFlag == 0)
			{
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
					
					h_CNN_bin[0][0][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
				
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
				
					h_CNN_binETA[0][0][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
				
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 
					
					h_CNN_binMU[0][0][bin]->Fill(pCNN, mc_weight);

				}
			}else{
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
					
					h_CNN_bin[1][0][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
				
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
				
					h_CNN_binETA[1][0][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
				
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 
					
					h_CNN_binMU[1][0][bin]->Fill(pCNN, mc_weight);

				}
	
			}
		}
        	
		if (ph_truth_pdgId != 22) continue;
        	
		if(ph_convFlag == 0)
		{
			//UnConverted
			
			//All 
			h_E[0][0][0]->Fill(ph_E, mc_weight);
			h_ETA[0][0][0]->Fill(fabs(ph_eta2), mc_weight);
			h_MU[0][0][0]->Fill(actual_mu, mc_weight);
			h_Mllg[0][0][0]->Fill(llg_m, mc_weight);	
			
			for(int bin = 0; bin<PT_nbins-1; bin++)
			{
				double PT_max = PT[bin+1];
				double PT_min = PT[bin];
				
				if(ph_E > PT_max || ph_E < PT_min) continue;
				
				h_Mllg_bin[0][0][0][bin]->Fill(llg_m, mc_weight);
			}

			for(int bin = 0; bin<ETA_nbins-1; bin++)
			{
				double ETA_max = ETA[bin+1];
				double ETA_min = ETA[bin];
				
				if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 

				h_Mllg_binETA[0][0][0][bin]->Fill(llg_m, mc_weight);
			}

			for(int bin = 0; bin<MU_nbins-1; bin++)
			{
				double MU_max = MU[bin+1];
				double MU_min = MU[bin];
				
				if( actual_mu < MU_min || actual_mu > MU_max) continue; 

				h_Mllg_binMU[0][0][0][bin]->Fill(llg_m, mc_weight);
			}
			
			if(ph_tight_id_dep)
			{
				// Tight E-dependent
				h_E[0][1][0]->Fill(ph_E, mc_weight);
				h_ETA[0][1][0]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][1][0]->Fill(actual_mu, mc_weight);
				h_Mllg[0][1][0]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][1][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][1][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][1][0][bin]->Fill(llg_m, mc_weight);
				}
				
			}
			
			if(ph_tight_id_incl)
			{
				// Tight Inclusive
				h_E[0][2][0]->Fill(ph_E, mc_weight);
				h_ETA[0][2][0]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][2][0]->Fill(actual_mu, mc_weight);
				h_Mllg[0][2][0]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][2][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][2][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][2][0][bin]->Fill(llg_m, mc_weight);
				}
			}
			
			if(isCNNTight(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Tight CNN
				h_E[0][3][0]->Fill(ph_E, mc_weight);
				h_ETA[0][3][0]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][3][0]->Fill(actual_mu, mc_weight);
				h_Mllg[0][3][0]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][3][0][bin]->Fill(llg_m, mc_weight);
				}
				
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][3][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][3][0][bin]->Fill(llg_m, mc_weight);
				}
			}
			
			if(ph_loose_AOD) // To be change
			{
				// Loose Inclusive
				h_E[0][4][0]->Fill(ph_E, mc_weight);
				h_ETA[0][4][0]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][4][0]->Fill(actual_mu, mc_weight);
				h_Mllg[0][4][0]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][4][0][bin]->Fill(llg_m, mc_weight);
				}
		
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][4][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][4][0][bin]->Fill(llg_m, mc_weight);
				}
			}
			
			if(isCNNLoose(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Loose CNN
				h_E[0][5][0]->Fill(ph_E, mc_weight);
				h_ETA[0][5][0]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][5][0]->Fill(actual_mu, mc_weight);
				h_Mllg[0][5][0]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][5][0][bin]->Fill(llg_m, mc_weight);
				}
		
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][5][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][5][0][bin]->Fill(llg_m, mc_weight);
				}
			}
			
		}else{
			//Converted
			
			//All 
			h_E[1][0][0]->Fill(ph_E, mc_weight);
			h_ETA[1][0][0]->Fill(fabs(ph_eta2), mc_weight);
			h_MU[1][0][0]->Fill(actual_mu, mc_weight);
			h_Mllg[1][0][0]->Fill(llg_m, mc_weight);
			
			for(int bin = 0; bin<PT_nbins-1; bin++)
			{
				double PT_max = PT[bin+1];
				double PT_min = PT[bin];
				
				if(ph_E > PT_max || ph_E < PT_min) continue;
				
				h_Mllg_bin[1][0][0][bin]->Fill(llg_m, mc_weight);
			}

			for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][0][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][0][0][bin]->Fill(llg_m, mc_weight);
				}	
			
			if(ph_tight_id_dep)
			{
				// Tight E-dependent
				h_E[1][1][0]->Fill(ph_E, mc_weight);
				h_ETA[1][1][0]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][1][0]->Fill(actual_mu, mc_weight);
				h_Mllg[1][1][0]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][1][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][1][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][1][0][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(ph_tight_id_incl)
			{
				// Tight Inclusive
				h_E[1][2][0]->Fill(ph_E, mc_weight);
				h_ETA[1][2][0]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][2][0]->Fill(actual_mu, mc_weight);
				h_Mllg[1][2][0]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][2][0][bin]->Fill(llg_m, mc_weight);
				}
			
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][2][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][2][0][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(isCNNTight(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Tight CNN
				h_E[1][3][0]->Fill(ph_E, mc_weight);
				h_ETA[1][3][0]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][3][0]->Fill(actual_mu, mc_weight);
				h_Mllg[1][3][0]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][3][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][3][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][3][0][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(ph_loose_AOD) // To be change
			{
				// Loose Inclusive
				h_E[1][4][0]->Fill(ph_E, mc_weight);
				h_ETA[1][4][0]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][4][0]->Fill(actual_mu, mc_weight);
				h_Mllg[1][4][0]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][4][0][bin]->Fill(llg_m, mc_weight);
				}
			
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][4][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][4][0][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(isCNNLoose(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Loose CNN
				h_E[1][5][0]->Fill(ph_E, mc_weight);
				h_ETA[1][5][0]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][5][0]->Fill(actual_mu, mc_weight);
				h_Mllg[1][5][0]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][5][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][5][0][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][5][0][bin]->Fill(llg_m, mc_weight);
				}	
			}
				
		}
		
	}
	
	std::cout<<" N Signal : UnConverted  = "<< h_Mllg[0][0][0]->Integral()  << " * Converted = "<< h_Mllg[1][0][0]->Integral() <<std::endl;
	
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	Int_t BkgEntry = (Int_t)Bkg->GetEntries();

	std::cout<<"==== Background Entries : "<< BkgEntry <<std::endl; 

	//Loop Background 
	
	for( Int_t bentry = 0; bentry<BkgEntry; bentry++)
	{
	
		Bkg->GetEntry(bentry);
		
		llg_m = llg_m/1000;
        	ll_m = ll_m/1000;
        	ph_E = ph_E/1000;
        	ph_pt = ph_pt/1000;
        	ph_pt2 = ph_pt2/1000;
        	ph_topoetcone20 = ph_topoetcone20/1000;
        	ph_topoetcone40 = ph_topoetcone40/1000;
        	ph_ptcone20 = ph_ptcone20/1000;
        	ph_ptcone40 = ph_ptcone40/1000;
        	ph_ptvarcone20 = ph_ptvarcone20/1000;
        	ph_ptvarcone40 = ph_ptvarcone40/1000;
        	
        	mc_pu_weight = 1.;

		if(isoWP.Contains("loose")) mc_weight = mc_pu_weight*mc_gen_weight*mc_l1_weight*mc_l2_weight*ph_ISO_FixedCutLoose_weight*mc_weight_lumi*getXSec(mc_channel); // need to add cross section
        	else if(isoWP.Contains("tight")) mc_weight = mc_pu_weight*mc_gen_weight*mc_l1_weight*mc_l2_weight*ph_ISO_FixedCutTight_weight*mc_weight_lumi*getXSec(mc_channel);
        	else if(isoWP.Contains("caloonly")) mc_weight =mc_pu_weight*mc_gen_weight*mc_l1_weight*mc_l2_weight*ph_ISO_FixedCutTightCaloOnly_weight*mc_weight_lumi*getXSec(mc_channel);
        	else mc_weight = mc_pu_weight*mc_gen_weight*mc_l1_weight*mc_l2_weight*mc_weight_lumi*getXSec(mc_channel);
        	

		if(Case.Contains("My")) mc_weight *= getMCScale(mc_channel);

/*
		
		if(Case.Contains("My"))
		{
		if(target.Contains("llg"))
		{
			mc_weight *= 0.8641;
		}else if(target.Contains("eeg")){
			mc_weight *= 0.6007;
		}else{
			mc_weight *= 0.9827;
		}
        	}
		
*/
		//if(!(Trigger_HLT_mu26_imedium || Trigger_HLT_mu26_ivarmedium_OR_HLT_mu50 || Trigger_HLT_mu26_ivarmedium || Trigger_HLT_mu50 || Trigger_HLT_mu50_0eta105_msonly || Trigger_HLT_2mu14 || Trigger_HLT_mu22_mu8noL1 || Trigger_HLT_e26_lhtight_nod0_ivarloose || Trigger_HLT_e60_lhmedium_nod0 || Trigger_HLT_e140_lhloose_nod0 || Trigger_HLT_2e24_lhvloose_nod0)) continue;

		//if( !(Trigger_HLT_g12_loose_match_gamma || Trigger_HLT_g15_loose_L1EM3_match_gamma || Trigger_HLT_g20_tight_icalovloose_L1EM15VHI_match_gamma || Trigger_HLT_g22_tight_L1EM15VHI_match_gamma || Trigger_HLT_g25_loose_match_gamma || Trigger_HLT_g25_medium_L1EM20VH_match_gamma || Trigger_HLT_g35_medium_L1EM20VH_match_gamma || Trigger_HLT_g50_loose_L1EM20VH_match_gamma)) continue;


        	if (ph_l_dRmin < 0.4) continue;
		if(isoWP.Contains("loose"))
        	{
            		if ((ph_topoetcone20 > 0.065*ph_pt) || (ph_ptcone20_TightTTVA_pt1000/ph_pt > 0.05)) continue;
        	}
        	else if(isoWP.Contains("tight"))
        	{
            		if ((ph_topoetcone40 > 0.022*ph_pt + 2.45) || (ph_ptcone20_TightTTVA_pt1000/ph_pt > 0.05)) continue;
        	}
        	else if(isoWP.Contains("caloonly"))
        	{
            		if (!ph_isotightcaloonly) continue;
        	}
        	
        	if (ll_m > 83) continue;
		//if (ll_m < 40) continue;

		//if (llg_m < 60) continue;
		//if (llg_m > 120) continue;
        	
        	if (fabs(ph_eta2) < eta_min || fabs(ph_eta2) > eta_max) continue;
        	if (ph_E < 10 || ph_E > 100) continue;
        	
        	
		//if (ph_truth_pdgId == 22) continue;

		if(llg_m > 80 && llg_m < 100)
		{
			if(ph_convFlag == 0)
			{
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
					
					h_CNN_bin[0][1][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
				
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
				
					h_CNN_binETA[0][1][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
				
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 
					
					h_CNN_binMU[0][1][bin]->Fill(pCNN, mc_weight);

				}
			}else{
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
					
					h_CNN_bin[1][1][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
				
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
				
					h_CNN_binETA[1][1][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
				
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 
					
					h_CNN_binMU[1][1][bin]->Fill(pCNN, mc_weight);

				}
	
			}
		}

		if (ph_truth_origin == 40) continue;

		if(ph_convFlag == 0)
		{
			//UnConverted
			
			//All 
			h_E[0][0][1]->Fill(ph_E, mc_weight);
			h_ETA[0][0][1]->Fill(fabs(ph_eta2), mc_weight);
			h_MU[0][0][1]->Fill(actual_mu, mc_weight);
			h_Mllg[0][0][1]->Fill(llg_m, mc_weight);	
			
			for(int bin = 0; bin<PT_nbins-1; bin++)
			{
				double PT_max = PT[bin+1];
				double PT_min = PT[bin];
				
				if(ph_E > PT_max || ph_E < PT_min) continue;
				
				h_Mllg_bin[0][0][1][bin]->Fill(llg_m, mc_weight);
			}

			for(int bin = 0; bin<ETA_nbins-1; bin++)
			{
				double ETA_max = ETA[bin+1];
				double ETA_min = ETA[bin];
				
				if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 

				h_Mllg_binETA[0][0][1][bin]->Fill(llg_m, mc_weight);
			}

			for(int bin = 0; bin<MU_nbins-1; bin++)
			{
				double MU_max = MU[bin+1];
				double MU_min = MU[bin];
				
				if( actual_mu < MU_min || actual_mu > MU_max) continue; 

				h_Mllg_binMU[0][0][1][bin]->Fill(llg_m, mc_weight);
			}
			
			if(ph_tight_id_dep)
			{
				// Tight E-dependent
				h_E[0][1][1]->Fill(ph_E, mc_weight);
				h_ETA[0][1][1]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][1][1]->Fill(actual_mu, mc_weight);
				h_Mllg[0][1][1]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][1][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][1][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][1][1][bin]->Fill(llg_m, mc_weight);
				}
				
			}
			
			if(ph_tight_id_incl)
			{
				// Tight Inclusive
				h_E[0][2][1]->Fill(ph_E, mc_weight);
				h_ETA[0][2][1]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][2][1]->Fill(actual_mu, mc_weight);
				h_Mllg[0][2][1]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][2][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][2][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][2][1][bin]->Fill(llg_m, mc_weight);
				}
			}
			
			if(isCNNTight(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Tight CNN
				h_E[0][3][1]->Fill(ph_E, mc_weight);
				h_ETA[0][3][1]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][3][1]->Fill(actual_mu, mc_weight);
				h_Mllg[0][3][1]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][3][1][bin]->Fill(llg_m, mc_weight);
				}
				
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][3][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][3][1][bin]->Fill(llg_m, mc_weight);
				}
			}
			
			if(ph_loose_AOD) // To be change
			{
				// Loose Inclusive
				h_E[0][4][1]->Fill(ph_E, mc_weight);
				h_ETA[0][4][1]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][4][1]->Fill(actual_mu, mc_weight);
				h_Mllg[0][4][1]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][4][1][bin]->Fill(llg_m, mc_weight);
				}
		
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][4][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][4][1][bin]->Fill(llg_m, mc_weight);
				}
			}
			
			if(isCNNLoose(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Loose CNN
				h_E[0][5][1]->Fill(ph_E, mc_weight);
				h_ETA[0][5][1]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][5][1]->Fill(actual_mu, mc_weight);
				h_Mllg[0][5][1]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][5][1][bin]->Fill(llg_m, mc_weight);
				}
		
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][5][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][5][1][bin]->Fill(llg_m, mc_weight);
				}
			}
			
		}else{
			//Converted
			
			//All 
			h_E[1][0][1]->Fill(ph_E, mc_weight);
			h_ETA[1][0][1]->Fill(fabs(ph_eta2), mc_weight);
			h_MU[1][0][1]->Fill(actual_mu, mc_weight);
			h_Mllg[1][0][1]->Fill(llg_m, mc_weight);
			
			for(int bin = 0; bin<PT_nbins-1; bin++)
			{
				double PT_max = PT[bin+1];
				double PT_min = PT[bin];
				
				if(ph_E > PT_max || ph_E < PT_min) continue;
				
				h_Mllg_bin[1][0][1][bin]->Fill(llg_m, mc_weight);
			}

			for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][0][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][0][1][bin]->Fill(llg_m, mc_weight);
				}	
			
			if(ph_tight_id_dep)
			{
				// Tight E-dependent
				h_E[1][1][1]->Fill(ph_E, mc_weight);
				h_ETA[1][1][1]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][1][1]->Fill(actual_mu, mc_weight);
				h_Mllg[1][1][1]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][1][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][1][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][1][1][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(ph_tight_id_incl)
			{
				// Tight Inclusive
				h_E[1][2][1]->Fill(ph_E, mc_weight);
				h_ETA[1][2][1]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][2][1]->Fill(actual_mu, mc_weight);
				h_Mllg[1][2][1]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][2][1][bin]->Fill(llg_m, mc_weight);
				}
			
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][2][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][2][1][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(isCNNTight(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Tight CNN
				h_E[1][3][1]->Fill(ph_E, mc_weight);
				h_ETA[1][3][1]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][3][1]->Fill(actual_mu, mc_weight);
				h_Mllg[1][3][1]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][3][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][3][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][3][1][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(ph_loose_AOD) // To be change
			{
				// Loose Inclusive
				h_E[1][4][1]->Fill(ph_E, mc_weight);
				h_ETA[1][4][1]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][4][1]->Fill(actual_mu, mc_weight);
				h_Mllg[1][4][1]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][4][1][bin]->Fill(llg_m, mc_weight);
				}
			
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][4][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][4][1][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(isCNNLoose(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Loose CNN
				h_E[1][5][1]->Fill(ph_E, mc_weight);
				h_ETA[1][5][1]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][5][1]->Fill(actual_mu, mc_weight);
				h_Mllg[1][5][1]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][5][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][5][1][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][5][1][bin]->Fill(llg_m, mc_weight);
				}	
			}
				
		}

	}

	std::cout<<" N Background : UnConverted  = "<< h_Mllg[0][0][1]->Integral()  << " * Converted = "<< h_Mllg[1][0][1]->Integral() <<std::endl;	
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	Int_t DataEntry = (Int_t)Data->GetEntries();

	std::cout<<"==== Data Entries : "<< DataEntry <<std::endl; 

	//Loop Data
	
	for( Int_t dentry = 0; dentry<DataEntry; dentry++)
	{
	
		Data->GetEntry(dentry);
		
		llg_m = llg_m/1000;
        	ll_m = ll_m/1000;
        	ph_E = ph_E/1000;
        	ph_pt = ph_pt/1000;
        	ph_pt2 = ph_pt2/1000;
        	ph_topoetcone20 = ph_topoetcone20/1000;
        	ph_topoetcone40 = ph_topoetcone40/1000;
        	ph_ptcone20 = ph_ptcone20/1000;
        	ph_ptcone40 = ph_ptcone40/1000;
        	ph_ptvarcone20 = ph_ptvarcone20/1000;
        	ph_ptvarcone40 = ph_ptvarcone40/1000;
        	
        	
        	mc_weight = 1.;

		if(Case.Contains("My"))
		{
			mc_weight *= getDataScale(Data->GetFile()->GetName());
        	}

		


	//	if(!(Trigger_HLT_mu26_imedium || Trigger_HLT_mu26_ivarmedium_OR_HLT_mu50 || Trigger_HLT_mu26_ivarmedium || Trigger_HLT_mu50 || Trigger_HLT_mu50_0eta105_msonly || Trigger_HLT_2mu14 || Trigger_HLT_mu22_mu8noL1 || Trigger_HLT_e26_lhtight_nod0_ivarloose || Trigger_HLT_e60_lhmedium_nod0 || Trigger_HLT_e140_lhloose_nod0 || Trigger_HLT_2e24_lhvloose_nod0)) continue;

		//if( !(Trigger_HLT_g12_loose_match_gamma || Trigger_HLT_g15_loose_L1EM3_match_gamma || Trigger_HLT_g20_tight_icalovloose_L1EM15VHI_match_gamma || Trigger_HLT_g22_tight_L1EM15VHI_match_gamma || Trigger_HLT_g25_loose_match_gamma || Trigger_HLT_g25_medium_L1EM20VH_match_gamma || Trigger_HLT_g35_medium_L1EM20VH_match_gamma || Trigger_HLT_g50_loose_L1EM20VH_match_gamma)) continue;

        	if (ph_l_dRmin < 0.4) continue;
		if(isoWP.Contains("loose"))
        	{
            		if ((ph_topoetcone20 > 0.065*ph_pt) || (ph_ptcone20_TightTTVA_pt1000/ph_pt > 0.05)) continue;
        	}
        	else if(isoWP.Contains("tight"))
        	{
            		if ((ph_topoetcone40 > 0.022*ph_pt + 2.45) || (ph_ptcone20_TightTTVA_pt1000/ph_pt > 0.05)) continue;
        	}
        	else if(isoWP.Contains("caloonly"))
        	{
            		if (!ph_isotightcaloonly) continue;
        	}
        	
        	if (ll_m > 83) continue;
		//if (ll_m < 40) continue;

		//if (llg_m < 60) continue;
		//if (llg_m > 120) continue;
        	
        	if (fabs(ph_eta2) < eta_min || fabs(ph_eta2) > eta_max) continue;
        	if (ph_E < 10 || ph_E > 100) continue;
        	
        	if(llg_m > 80 && llg_m < 100)
		{
			if(ph_convFlag == 0)
			{
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
					
					h_CNN_bin[0][2][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
				
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
				
					h_CNN_binETA[0][2][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
				
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 
					
					h_CNN_binMU[0][2][bin]->Fill(pCNN, mc_weight);

				}
			}else{
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
					
					h_CNN_bin[1][2][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
				
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
				
					h_CNN_binETA[1][2][bin]->Fill(pCNN, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
				
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 
					
					h_CNN_binMU[1][2][bin]->Fill(pCNN, mc_weight);

				}
	
			}
		}
		if(ph_convFlag == 0)
		{
			//UnConverted
			
			//All 
			h_E[0][0][2]->Fill(ph_E, mc_weight);
			h_ETA[0][0][2]->Fill(fabs(ph_eta2), mc_weight);
			h_MU[0][0][2]->Fill(actual_mu, mc_weight);
			h_Mllg[0][0][2]->Fill(llg_m, mc_weight);	
			
			for(int bin = 0; bin<PT_nbins-1; bin++)
			{
				double PT_max = PT[bin+1];
				double PT_min = PT[bin];
				
				if(ph_E > PT_max || ph_E < PT_min) continue;
				
				h_Mllg_bin[0][0][2][bin]->Fill(llg_m, mc_weight);
			}

			for(int bin = 0; bin<ETA_nbins-1; bin++)
			{
				double ETA_max = ETA[bin+1];
				double ETA_min = ETA[bin];
				
				if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 

				h_Mllg_binETA[0][0][2][bin]->Fill(llg_m, mc_weight);
			}

			for(int bin = 0; bin<MU_nbins-1; bin++)
			{
				double MU_max = MU[bin+1];
				double MU_min = MU[bin];
				
				if( actual_mu < MU_min || actual_mu > MU_max) continue; 

				h_Mllg_binMU[0][0][2][bin]->Fill(llg_m, mc_weight);
			}
			
			if(ph_tight_id_dep)
			{
				// Tight E-dependent
				h_E[0][1][2]->Fill(ph_E, mc_weight);
				h_ETA[0][1][2]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][1][2]->Fill(actual_mu, mc_weight);
				h_Mllg[0][1][2]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][1][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][1][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][1][2][bin]->Fill(llg_m, mc_weight);
				}
				
			}
			
			if(ph_tight_id_incl)
			{
				// Tight Inclusive
				h_E[0][2][2]->Fill(ph_E, mc_weight);
				h_ETA[0][2][2]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][2][2]->Fill(actual_mu, mc_weight);
				h_Mllg[0][2][2]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][2][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][2][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][2][2][bin]->Fill(llg_m, mc_weight);
				}
			}
			
			if(isCNNTight(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Tight CNN
				h_E[0][3][2]->Fill(ph_E, mc_weight);
				h_ETA[0][3][2]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][3][2]->Fill(actual_mu, mc_weight);
				h_Mllg[0][3][2]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][3][2][bin]->Fill(llg_m, mc_weight);
				}
				
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][3][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][3][2][bin]->Fill(llg_m, mc_weight);
				}
			}
			
			if(ph_loose_AOD) // To be change
			{
				// Loose Inclusive
				h_E[0][4][2]->Fill(ph_E, mc_weight);
				h_ETA[0][4][2]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][4][2]->Fill(actual_mu, mc_weight);
				h_Mllg[0][4][2]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][4][2][bin]->Fill(llg_m, mc_weight);
				}
		
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][4][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][4][2][bin]->Fill(llg_m, mc_weight);
				}
			}
			
			if(isCNNLoose(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Loose CNN
				h_E[0][5][2]->Fill(ph_E, mc_weight);
				h_ETA[0][5][2]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[0][5][2]->Fill(actual_mu, mc_weight);
				h_Mllg[0][5][2]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[0][5][2][bin]->Fill(llg_m, mc_weight);
				}
		
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[0][5][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[0][5][2][bin]->Fill(llg_m, mc_weight);
				}
			}
			
		}else{
			//Converted
			
			//All 
			h_E[1][0][2]->Fill(ph_E, mc_weight);
			h_ETA[1][0][2]->Fill(fabs(ph_eta2), mc_weight);
			h_MU[1][0][2]->Fill(actual_mu, mc_weight);
			h_Mllg[1][0][2]->Fill(llg_m, mc_weight);
			
			for(int bin = 0; bin<PT_nbins-1; bin++)
			{
				double PT_max = PT[bin+1];
				double PT_min = PT[bin];
				
				if(ph_E > PT_max || ph_E < PT_min) continue;
				
				h_Mllg_bin[1][0][2][bin]->Fill(llg_m, mc_weight);
			}

			for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][0][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][0][2][bin]->Fill(llg_m, mc_weight);
				}	
			
			if(ph_tight_id_dep)
			{
				// Tight E-dependent
				h_E[1][1][2]->Fill(ph_E, mc_weight);
				h_ETA[1][1][2]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][1][2]->Fill(actual_mu, mc_weight);
				h_Mllg[1][1][2]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][1][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][1][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][1][2][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(ph_tight_id_incl)
			{
				// Tight Inclusive
				h_E[1][2][2]->Fill(ph_E, mc_weight);
				h_ETA[1][2][2]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][2][2]->Fill(actual_mu, mc_weight);
				h_Mllg[1][2][2]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][2][2][bin]->Fill(llg_m, mc_weight);
				}
			
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][2][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][2][2][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(isCNNTight(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Tight CNN
				h_E[1][3][2]->Fill(ph_E, mc_weight);
				h_ETA[1][3][2]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][3][2]->Fill(actual_mu, mc_weight);
				h_Mllg[1][3][2]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][3][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][3][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][3][2][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(ph_loose_AOD) // To be change
			{
				// Loose Inclusive
				h_E[1][4][2]->Fill(ph_E, mc_weight);
				h_ETA[1][4][2]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][4][2]->Fill(actual_mu, mc_weight);
				h_Mllg[1][4][2]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][4][2][bin]->Fill(llg_m, mc_weight);
				}
			
				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][4][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][4][2][bin]->Fill(llg_m, mc_weight);
				}	
			}
			
			if(isCNNLoose(fabs(ph_eta2), pCNN, ph_convFlag))
			{
				// Loose CNN
				h_E[1][5][2]->Fill(ph_E, mc_weight);
				h_ETA[1][5][2]->Fill(fabs(ph_eta2), mc_weight);
				h_MU[1][5][2]->Fill(actual_mu, mc_weight);
				h_Mllg[1][5][2]->Fill(llg_m, mc_weight);
				
				for(int bin = 0; bin<PT_nbins-1; bin++)
				{
					double PT_max = PT[bin+1];
					double PT_min = PT[bin];
				
					if(ph_E > PT_max || ph_E < PT_min) continue;
				
					h_Mllg_bin[1][5][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<ETA_nbins-1; bin++)
				{
					double ETA_max = ETA[bin+1];
					double ETA_min = ETA[bin];
					
					if(abs(ph_eta2)< ETA_min || abs(ph_eta2) > ETA_max) continue; 
	
					h_Mllg_binETA[1][5][2][bin]->Fill(llg_m, mc_weight);
				}

				for(int bin = 0; bin<MU_nbins-1; bin++)
				{
					double MU_max = MU[bin+1];
					double MU_min = MU[bin];
					
					if( actual_mu < MU_min || actual_mu > MU_max) continue; 

					h_Mllg_binMU[1][5][2][bin]->Fill(llg_m, mc_weight);
				}	
			}
				
		}

	}

	std::cout<<" N Data : UnConverted  = "<< h_Mllg[0][0][2]->Integral()  << " * Converted = "<< h_Mllg[1][0][2]->Integral() <<std::endl;	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	for(int i = 0; i<n_flag; i++)
	{
		for(int j = 0; j<n_sel; j++)
		{
			for(int k = 0; k<n_smpl; k++)
			{
				/*
				h_E[i][j][k]->Scale(1./h_E[i][j][k]->Integral());
				h_ETA[i][j][k]->Scale(1./h_ETA[i][j][k]->Integral());
				h_MU[i][j][k]->Scale(1./h_MU[i][j][k]->Integral());
				h_Mllg[i][j][k]->Scale(1./h_Mllg[i][j][k]->Integral());
				*/

				h_E[i][j][k]->SetLineWidth(2);
				h_ETA[i][j][k]->SetLineWidth(2);
				h_MU[i][j][k]->SetLineWidth(2);
				h_Mllg[i][j][k]->SetLineWidth(2);

				h_E[i][j][k]->SetDirectory(output);
				h_ETA[i][j][k]->SetDirectory(output);
				h_MU[i][j][k]->SetDirectory(output);
				h_Mllg[i][j][k]->SetDirectory(output);

				for(int h = 0; h<PT_nbins; h++)
				{
					//h_Mllg_bin[i][j][k][h]->Scale(1./h_Mllg_bin[i][j][k][h]->Integral());

					h_Mllg_bin[i][j][k][h]->SetLineWidth(2);

					h_Mllg_bin[i][j][k][h]->SetDirectory(output);
				}
			}

			
		}
	}

output->Write();
output->Close();
}




void Load_Branches_MC(TChain *chain)
{

    if (!chain) return;

    chain->SetBranchAddress("ph.E",&ph_E);
    chain->SetBranchAddress("ph.pt",&ph_pt);
    chain->SetBranchAddress("ph.pt2",&ph_pt2);
    chain->SetBranchAddress("ph.eta2",&ph_eta2);
    chain->SetBranchAddress("ph.eta",&ph_eta);
    chain->SetBranchAddress("ph.convFlag",&ph_convFlag);
    chain->SetBranchAddress("ph.topoetcone20",&ph_topoetcone20);
    chain->SetBranchAddress("ph.topoetcone40",&ph_topoetcone40);
    chain->SetBranchAddress("ph.ptcone20_TightTTVA_pt1000",&ph_ptcone20_TightTTVA_pt1000);
    chain->SetBranchAddress("ph.ptcone20_TightTTVA_pt500",&ph_ptcone20_TightTTVA_pt500);
    chain->SetBranchAddress("ph.ptcone20_TightTTVALooseCone_pt1000",&ph_ptcone20_TightTTVALooseCone_pt1000);
    chain->SetBranchAddress("ph.ptcone20_TightTTVALooseCone_pt500",&ph_ptcone20_TightTTVALooseCone_pt500);
    chain->SetBranchAddress("ph.ptcone20",&ph_ptcone20);
    chain->SetBranchAddress("ph.ptcone40",&ph_ptcone40);
    chain->SetBranchAddress("ph.ptvarcone20",&ph_ptvarcone20);
    chain->SetBranchAddress("ph.ptvarcone40",&ph_ptvarcone40);
    chain->SetBranchAddress("ph.loose_AOD",&ph_loose_AOD);
    chain->SetBranchAddress("ph.tight_AOD",&ph_tight_AOD);
    chain->SetBranchAddress("ph.loose_id",&ph_loose_mc15);
    chain->SetBranchAddress("ph.tight_id_incl",&ph_tight_id_incl);
    chain->SetBranchAddress("ph.tight_id_dep",&ph_tight_id_dep);
    chain->SetBranchAddress("ph.tight_test",&ph_tight_test);
    chain->SetBranchAddress("ph.isEM_tight_test",&ph_isEM_tight_test);
    chain->SetBranchAddress("ll.m",&ll_m);
    chain->SetBranchAddress("llg.m",&llg_m);
    chain->SetBranchAddress("ph.truth_mother_pdgId",&ph_truth_mother_pdgId);
    chain->SetBranchAddress("ph.truth_pdgId",&ph_truth_pdgId);
    chain->SetBranchAddress("ph.truth_origin",&ph_truth_origin);
    chain->SetBranchAddress("ph.truth_type",&ph_truth_type);
    chain->SetBranchAddress("ph.truth_isConv",&ph_truth_isConv);
    chain->SetBranchAddress("ph.isotightcaloonly",&ph_isotightcaloonly); 
    chain->SetBranchAddress("ph.isotight",&ph_isotight); 
    chain->SetBranchAddress("ph.isoloose",&ph_isoloose); 
    chain->SetBranchAddress("ph.looseprime2",&ph_looseprime2);
    chain->SetBranchAddress("ph.looseprime3",&ph_looseprime3);
    chain->SetBranchAddress("ph.looseprime4",&ph_looseprime4);
    chain->SetBranchAddress("ph.looseprime5",&ph_looseprime5);
    chain->SetBranchAddress("EventInfo.mu", &mu);
    chain->SetBranchAddress("EventInfo.actual_mu", &actual_mu);
    chain->SetBranchAddress("ph.ambiguous_isel",&ph_ambiguous_isel);
    chain->SetBranchAddress("mc_weight.phID",&ph_ID_mc_weight);
    chain->SetBranchAddress("mc_weight.gen",&mc_gen_weight);
    chain->SetBranchAddress("mc_weight.pu",&mc_pu_weight);
    chain->SetBranchAddress("mc_weight.l1",&mc_l1_weight);
    chain->SetBranchAddress("mc_weight.l2",&mc_l2_weight);
    chain->SetBranchAddress("mc_weight.xs",&mc_weight_xs);
    chain->SetBranchAddress("mc_weight.phID_1up",&ph_ID_mc_weight_1up);
    chain->SetBranchAddress("mc_weight.phID_1dn",&ph_ID_mc_weight_1dn);
    chain->SetBranchAddress("mc_weight.trigger",&mc_trigger_weight);
    chain->SetBranchAddress("mc_weight.phISO_FixedCutLoose",&ph_ISO_FixedCutLoose_weight);
    chain->SetBranchAddress("mc_weight.phISO_FixedCutTight",&ph_ISO_FixedCutTight_weight);
    chain->SetBranchAddress("mc_weight.phISO_FixedCutTightCaloOnly",&ph_ISO_FixedCutTightCaloOnly_weight);
    chain->SetBranchAddress("mc_weight.lumi",&mc_weight_lumi);
    chain->SetBranchAddress("ph_l1.dR",&ph_l1_dR);
    chain->SetBranchAddress("ph_l2.dR",&ph_l2_dR);
    chain->SetBranchAddress("ph_l.dRmin",&ph_l_dRmin);
    chain->SetBranchAddress("ph.pCNN", &pCNN);
    chain->SetBranchAddress("ph.loose_id", &ph_loose_id);
    chain->SetBranchAddress("mc.channelNumber", &mc_channel);

    chain->SetBranchAddress("Trigger.HLT_mu26_imedium",&Trigger_HLT_mu26_imedium);
    chain->SetBranchAddress("Trigger.HLT_mu26_ivarmedium_OR_HLT_mu50",&Trigger_HLT_mu26_ivarmedium_OR_HLT_mu50);
    chain->SetBranchAddress("Trigger.HLT_mu26_ivarmedium",&Trigger_HLT_mu26_ivarmedium);
    chain->SetBranchAddress("Trigger.HLT_mu50",&Trigger_HLT_mu50);
    chain->SetBranchAddress("Trigger.HLT_mu50_0eta105_msonly",&Trigger_HLT_mu50_0eta105_msonly);
    chain->SetBranchAddress("Trigger.HLT_2mu14",&Trigger_HLT_2mu14);
    chain->SetBranchAddress("Trigger.HLT_mu22_mu8noL1",&Trigger_HLT_mu22_mu8noL1);
    chain->SetBranchAddress("Trigger.HLT_e26_lhtight_nod0_ivarloose",&Trigger_HLT_e26_lhtight_nod0_ivarloose);
    chain->SetBranchAddress("Trigger.HLT_e60_lhmedium_nod0",&Trigger_HLT_e60_lhmedium_nod0);
    chain->SetBranchAddress("Trigger.HLT_e140_lhloose_nod0",&Trigger_HLT_e140_lhloose_nod0);
    chain->SetBranchAddress("Trigger.HLT_2e24_lhvloose_nod0",&Trigger_HLT_2e24_lhvloose_nod0);

    chain->SetBranchAddress("Trigger.HLT_g12_loose_match_gamma", &Trigger_HLT_g12_loose_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g15_loose_L1EM3_match_gamma", &Trigger_HLT_g15_loose_L1EM3_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g20_tight_icalovloose_L1EM15VHI_match_gamma", &Trigger_HLT_g20_tight_icalovloose_L1EM15VHI_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g22_tight_L1EM15VHI_match_gamma", &Trigger_HLT_g22_tight_L1EM15VHI_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g25_loose_match_gamma", &Trigger_HLT_g25_loose_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g25_medium_L1EM20VH_match_gamma", &Trigger_HLT_g25_medium_L1EM20VH_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g35_medium_L1EM20VH_match_gamma", &Trigger_HLT_g35_medium_L1EM20VH_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g50_loose_L1EM20VH_match_gamma",&Trigger_HLT_g50_loose_L1EM20VH_match_gamma);

}

void Load_Branches_Data(TChain *chain)
{

	if (!chain) return;

	chain->SetBranchAddress("ph.E",&ph_E);
    chain->SetBranchAddress("ph.pt",&ph_pt);
    chain->SetBranchAddress("ph.pt2",&ph_pt2);
    chain->SetBranchAddress("ph.eta2",&ph_eta2);
    chain->SetBranchAddress("ph.eta",&ph_eta);
    chain->SetBranchAddress("ph.convFlag",&ph_convFlag);
    chain->SetBranchAddress("ph.topoetcone20",&ph_topoetcone20);
    chain->SetBranchAddress("ph.topoetcone40",&ph_topoetcone40);
    chain->SetBranchAddress("ph.ptcone20",&ph_ptcone20);
    chain->SetBranchAddress("ph.ptcone40",&ph_ptcone40);
    chain->SetBranchAddress("ph.ptvarcone20",&ph_ptvarcone20);
    chain->SetBranchAddress("ph.ptvarcone40",&ph_ptvarcone40);
    chain->SetBranchAddress("ph.ptcone20_TightTTVA_pt1000",&ph_ptcone20_TightTTVA_pt1000);
    chain->SetBranchAddress("ph.ptcone20_TightTTVA_pt500",&ph_ptcone20_TightTTVA_pt500);
    chain->SetBranchAddress("ph.ptcone20_TightTTVALooseCone_pt1000",&ph_ptcone20_TightTTVALooseCone_pt1000);
    chain->SetBranchAddress("ph.ptcone20_TightTTVALooseCone_pt500",&ph_ptcone20_TightTTVALooseCone_pt500);
    chain->SetBranchAddress("ph.loose_AOD",&ph_loose_AOD);
    chain->SetBranchAddress("ph.tight_AOD",&ph_tight_AOD);
    chain->SetBranchAddress("ph.loose_id",&ph_loose_mc15);
    chain->SetBranchAddress("ph.tight_id_incl",&ph_tight_id_incl);
    chain->SetBranchAddress("ph.tight_id_dep",&ph_tight_id_dep);
    chain->SetBranchAddress("ph.tight_test",&ph_tight_test);
    chain->SetBranchAddress("ph.isEM_tight_test",&ph_isEM_tight_test);
    chain->SetBranchAddress("ll.m",&ll_m);
    chain->SetBranchAddress("llg.m",&llg_m);
    chain->SetBranchAddress("ph.isotightcaloonly",&ph_isotightcaloonly); 
    chain->SetBranchAddress("ph.isotight",&ph_isotight); 
    chain->SetBranchAddress("ph.isoloose",&ph_isoloose); 
    chain->SetBranchAddress("ph.looseprime2",&ph_looseprime2);
    chain->SetBranchAddress("ph.looseprime3",&ph_looseprime3);
    chain->SetBranchAddress("ph.looseprime4",&ph_looseprime4);
    chain->SetBranchAddress("ph.looseprime5",&ph_looseprime5);
    chain->SetBranchAddress("EventInfo.mu", &mu);
    chain->SetBranchAddress("EventInfo.actual_mu", &actual_mu);
    chain->SetBranchAddress("ph.ambiguous_isel",&ph_ambiguous_isel);
    chain->SetBranchAddress("ph_l1.dR",&ph_l1_dR);
    chain->SetBranchAddress("ph_l2.dR",&ph_l2_dR);
    chain->SetBranchAddress("ph_l.dRmin",&ph_l_dRmin);
    chain->SetBranchAddress("ph.pCNN", &pCNN);
    chain->SetBranchAddress("ph.loose_id", &ph_loose_id);
    chain->SetBranchAddress("Trigger.HLT_mu26_imedium",&Trigger_HLT_mu26_imedium);
    chain->SetBranchAddress("Trigger.HLT_mu26_ivarmedium_OR_HLT_mu50",&Trigger_HLT_mu26_ivarmedium_OR_HLT_mu50);
    chain->SetBranchAddress("Trigger.HLT_mu26_ivarmedium",&Trigger_HLT_mu26_ivarmedium);
    chain->SetBranchAddress("Trigger.HLT_mu50",&Trigger_HLT_mu50);
    chain->SetBranchAddress("Trigger.HLT_mu50_0eta105_msonly",&Trigger_HLT_mu50_0eta105_msonly);
    chain->SetBranchAddress("Trigger.HLT_2mu14",&Trigger_HLT_2mu14);
    chain->SetBranchAddress("Trigger.HLT_mu22_mu8noL1",&Trigger_HLT_mu22_mu8noL1);
    chain->SetBranchAddress("Trigger.HLT_e26_lhtight_nod0_ivarloose",&Trigger_HLT_e26_lhtight_nod0_ivarloose);
    chain->SetBranchAddress("Trigger.HLT_e60_lhmedium_nod0",&Trigger_HLT_e60_lhmedium_nod0);
    chain->SetBranchAddress("Trigger.HLT_e140_lhloose_nod0",&Trigger_HLT_e140_lhloose_nod0);
    chain->SetBranchAddress("Trigger.HLT_2e24_lhvloose_nod0",&Trigger_HLT_2e24_lhvloose_nod0);

    chain->SetBranchAddress("Trigger.HLT_g12_loose_match_gamma", &Trigger_HLT_g12_loose_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g15_loose_L1EM3_match_gamma", &Trigger_HLT_g15_loose_L1EM3_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g20_tight_icalovloose_L1EM15VHI_match_gamma", &Trigger_HLT_g20_tight_icalovloose_L1EM15VHI_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g22_tight_L1EM15VHI_match_gamma", &Trigger_HLT_g22_tight_L1EM15VHI_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g25_loose_match_gamma", &Trigger_HLT_g25_loose_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g25_medium_L1EM20VH_match_gamma", &Trigger_HLT_g25_medium_L1EM20VH_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g35_medium_L1EM20VH_match_gamma", &Trigger_HLT_g35_medium_L1EM20VH_match_gamma);
    chain->SetBranchAddress("Trigger.HLT_g50_loose_L1EM20VH_match_gamma",&Trigger_HLT_g50_loose_L1EM20VH_match_gamma);


}

Bool_t isCNNTight(Double_t eta, Double_t p, Double_t cov)
{

        if(eta < .6 && eta > 0.0){
                if(p > .715 && cov==0)
                        return true;
                if(p > .662 && cov!=0)
                        return true;
        }
	if(eta < .8 && eta > .6){
                if(p > 0.756 && cov==0)
                        return true;
                if(p > 0.477 && cov!=0)
                        return true;
        }
	if(eta < 1.15 && eta > .8){
                if(p > 0.795 && cov==0)
                        return true;
                if(p > 0.578 && cov!=0)
                        return true;
        }
	if(eta < 1.37 && eta > 1.15){
                if(p > 0.81 && cov==0)
                        return true;
                if(p > 0.49 && cov!=0)
                        return true;
        }
	if(eta < 1.81 && eta > 1.52){
                if(p > 0.875 && cov==0)
                        return true;
                if(p > 0.742 && cov!=0)
                        return true;
        }
	if(eta < 2.01 && eta > 1.81){
                if(p > 0.93 && cov==0)
                        return true;
                if(p > 0.626 && cov!=0)
                        return true;
        }
	if(eta > 2.01){
                if(p > 0.892 && cov==0)
                        return true;
                if(p > 0.722 && cov!=0)
                        return true;
        }
	//return false;

return false;

}


Bool_t isCNNLoose(Double_t eta, Double_t p, Double_t cov)
{

        if(eta < .6 && eta > 0.0){
                if(p > .033 && cov==0)
                        return true;
                if(p > .025 && cov!=0)
                        return true;
        }
	if(eta < .8 && eta > .6){
                if(p > 0.027 && cov==0)
                        return true;
                if(p > 0.022 && cov!=0)
                        return true;
        }
	if(eta < 1.15 && eta > .8){
                if(p > 0.044 && cov==0)
                        return true;
                if(p > 0.023 && cov!=0)
                        return true;
        }
	if(eta < 1.37 && eta > 1.15){
                if(p > 0.04 && cov==0)
                        return true;
                if(p > 0.027 && cov!=0)
                        return true;
        }
	if(eta < 1.81 && eta > 1.52){
                if(p > 0.187 && cov==0)
                        return true;
                if(p > 0.081 && cov!=0)
                        return true;
        }
	if(eta < 2.01 && eta > 1.81){
                if(p > 0.62 && cov==0)
                        return true;
                if(p > 0.125 && cov!=0)
                        return true;
        }
	if(eta > 2.01){
                if(p > 0.132 && cov==0)
                        return true;
                if(p > 0.578 && cov!=0)
                        return true;
        }
	//return false;

return false;
}

double getXSec(int mc)
{
	if(mc == 366140) return 37.101; 
	if(mc == 366141) return 5.90794; 
	if(mc == 366142) return 8.27481; 
	if(mc == 366143) return 1.26426; 
	if(mc == 366144) return 0.16643; 

	if(mc == 366145) return 55.0558; 
	if(mc == 366146) return 5.87266; 
	if(mc == 366147) return 8.24597; 
	if(mc == 366148) return 1.26287; 
	if(mc == 366149) return 0.164809; 

	if(mc == 364100) return 1982.3*0.82179;
	if(mc == 364101) return 1982.1*0.11356;
	if(mc == 364102) return 1981.6*0.06574;
	if(mc == 364103) return 109.07*0.68978;
	if(mc == 364104) return 108.94*0.19588;
	if(mc == 364105) return 108.98*0.12052;
	if(mc == 364106) return 39.884*0.60179;
	if(mc == 364107) return 39.857*0.23545;
	if(mc == 364108) return 39.892*0.15628;
	if(mc == 364109) return 8.526*0.56012;
	if(mc == 364110) return 8.5261*0.26632;
	if(mc == 364111) return 8.5276*0.17657;
	if(mc == 364112) return 1.7871*1.;
	if(mc == 364113) return 0.14758*1.;

	if(mc == 364114) return 1981.5*0.82129;
	if(mc == 364115) return 1981.7*0.11424;
	if(mc == 364116) return 1982.1*0.06575;
	if(mc == 364117) return 110.63*0.69757;
	if(mc == 364118) return 110.45*0.19092;
	if(mc == 364119) return 110.49*0.11943;
	if(mc == 364120) return 40.711*0.61612;
	if(mc == 364121) return 40.674*0.23283;
	if(mc == 364122) return 40.675*0.15343;
	if(mc == 364123) return 8.6718*0.56355;
	if(mc == 364124) return 8.6654*0.26667;
	if(mc == 364125) return 8.6821*0.17626;
	if(mc == 364126) return 1.8098*1.;
	if(mc == 364127) return 0.1487*1.;
	return 0;
}
double getMCScale(int mc)
{
	double N_Zeeg = 0.953;
	double N_Zmmg = 0.797;
	double N_Zeej = 0.145;
	double N_Zmmj = 0.853;
 
	if(mc == 366140) return N_Zeeg; 
	if(mc == 366141) return N_Zeeg; 
	if(mc == 366142) return N_Zeeg; 
	if(mc == 366143) return N_Zeeg; 
	if(mc == 366144) return N_Zeeg; 

	if(mc == 366145) return N_Zmmg; 
	if(mc == 366146) return N_Zmmg; 
	if(mc == 366147) return N_Zmmg; 
	if(mc == 366148) return N_Zmmg; 
	if(mc == 366149) return N_Zmmg; 

	if(mc == 364100) return N_Zmmj;
	if(mc == 364101) return N_Zmmj;
	if(mc == 364102) return N_Zmmj;
	if(mc == 364103) return N_Zmmj;
	if(mc == 364104) return N_Zmmj;
	if(mc == 364105) return N_Zmmj;
	if(mc == 364106) return N_Zmmj;
	if(mc == 364107) return N_Zmmj;
	if(mc == 364108) return N_Zmmj;
	if(mc == 364109) return N_Zmmj;
	if(mc == 364110) return N_Zmmj;
	if(mc == 364111) return N_Zmmj;
	if(mc == 364112) return N_Zmmj;
	if(mc == 364113) return N_Zmmj;

	if(mc == 364114) return N_Zeej;
	if(mc == 364115) return N_Zeej;
	if(mc == 364116) return N_Zeej;
	if(mc == 364117) return N_Zeej;
	if(mc == 364118) return N_Zeej;
	if(mc == 364119) return N_Zeej;
	if(mc == 364120) return N_Zeej;
	if(mc == 364121) return N_Zeej;
	if(mc == 364122) return N_Zeej;
	if(mc == 364123) return N_Zeej;
	if(mc == 364124) return N_Zeej;
	if(mc == 364125) return N_Zeej;
	if(mc == 364126) return N_Zeej;
	if(mc == 364127) return N_Zeej;
	return 0;
}
double getDataScale(TString name)
{
	if(name.Contains("Zee"))
	{

		return 1.236;
	}

	if(name.Contains("Zmumu"))
	{
		return 1.286;
	}
	return 0.;
	

}
