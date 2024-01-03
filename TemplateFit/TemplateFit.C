#include "TROOT.h"
#include "TSystem.h"
#include "TString.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStopwatch.h"
#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include <iostream>
#include <cmath>
#include "TLine.h"
#include "TLatex.h"
#include "TMarker.h"
#include "TPave.h"
#include "TH1.h"
#include "TString.h"
#include <vector>
using namespace RooFit ;

vector<double> Fit(int bkgTempl, TH1F* h_Sig_1, TH1F* h_Bkg_1, TH1F* h_Data_1, TH1F* h_Sig_2, TH1F* h_Bkg_2, TH1F* h_Data_2, TString target, TString name, bool useFit);
void TemplateFit()
{
	//TString isoWP = "loose";
    	//TString isoWP = "caloonly";
    	TString isoWP = "loose";

	TString target = "llg";

	TString Case = "My";

	double PT [] = {10, 15, 20, 25, 30, 35, 40, 45, 50, 60, 80, 100};
	
	int PT_nbins = 12;

	Double_t ETA [] = {0, 0.2, 0.6, 0.8, 1.37, 1.52, 1.81, 2.01, 2.37};
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

	TFile* input = new TFile(Case+"_Eta_1_output_ComputeEff_"+target+"_"+isoWP+".root","READ");

	//TFile* output = new TFile(Case+"_Eta_1_Poly_output_TemplateFit_"+target+"_"+isoWP+".root","RECREATE"); 

	TFile* extra = new TFile("Extra.root", "RECREATE");


	for(int i = 0; i<n_flag; i++)
	{
		for(int j = 0; j<n_sel; j++)
		{
			for(int k = 0; k<n_smpl; k++)
			{
				h_E[i][j][k]   = (TH1F*)input->Get("Hist_E_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k]);
				h_ETA[i][j][k] = (TH1F*)input->Get("Hist_E_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k]); 
				h_MU[i][j][k]  = (TH1F*)input->Get("Hist_MU_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k]);
				h_Mllg[i][j][k]= (TH1F*)input->Get("Hist_Mllg_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k]);

				for(int h = 0; h<PT_nbins; h++)
				{
					h_Mllg_bin[i][j][k][h] = (TH1F*)input->Get("Hist_Mllg_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k]+"_bin_"+std::to_string(h));
				}
				for(int h = 0; h<ETA_nbins; h++)
				{
					h_Mllg_binETA[i][j][k][h] = (TH1F*)input->Get("Hist_Mllg_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k]+"_binETA_"+std::to_string(h));
				}
		
				for(int h = 0; h<MU_nbins; h++)
				{

					h_Mllg_binMU[i][j][k][h] = (TH1F*)input->Get("Hist_Mllg_"+Selections[j]+"_"+Conv_flag[i]+"_"+Samples[k]+"_binMU_"+std::to_string(h));
				}
			}

			
		}
	}
	int bin = 0;
	vector<double> reslt = Fit(0, h_Mllg_bin[0][0][0][bin], h_Mllg_bin[0][0][1][bin], h_Mllg_bin[0][0][2][bin], h_Mllg_bin[0][1][0][bin], h_Mllg_bin[0][1][1][bin], h_Mllg_bin[0][1][2][bin], "", "", true);

	return;

	TH1F* Eff_data[n_flag][n_sel];
	TH1F* Eff_MC[n_flag][n_sel];

	TH1F* Eff_data_ETA[n_flag][n_sel];
	TH1F* Eff_MC_ETA[n_flag][n_sel];

	TH1F* Eff_data_MU[n_flag][n_sel];
	TH1F* Eff_MC_MU[n_flag][n_sel];
	
	for(int i = 0; i<n_flag; i++)
	{
		for(int j = 1; j<n_sel; j++)
		{

			Eff_data[i][j] = new TH1F("Plot_Eff_Data_"+Selections[j]+"_"+Conv_flag[i], "", PT_nbins-1, PT);
			Eff_data[i][j]->SetLineWidth(2);
			Eff_data[i][j]->SetLineColor(1);
			Eff_data[i][j]->SetMarkerStyle(20);
			Eff_data[i][j]->GetYaxis()->SetRangeUser(0, 1.1);

			Eff_MC[i][j]   = new TH1F("Plot_Eff_MC_"+Selections[j]+"_"+Conv_flag[i], "", PT_nbins-1, PT);
			Eff_MC[i][j]->SetLineWidth(2);
			Eff_MC[i][j]->SetLineColor(2);
			Eff_MC[i][j]->SetMarkerStyle(25);
			Eff_MC[i][j]->GetYaxis()->SetRangeUser(0, 1.1);

			//Eff_MC[i][j]->SetDirectory(output);
			//Eff_data[i][j]->SetDirectory(output);

			Eff_data_ETA[i][j] = new TH1F("Plot_Eff_ETA_Data_"+Selections[j]+"_"+Conv_flag[i], "", ETA_nbins-1, ETA);
			Eff_data_ETA[i][j]->SetLineWidth(2);
			Eff_data_ETA[i][j]->SetLineColor(1);
			Eff_data_ETA[i][j]->SetMarkerStyle(20);
			Eff_data_ETA[i][j]->GetYaxis()->SetRangeUser(0, 1.1);

			Eff_MC_ETA[i][j]   = new TH1F("Plot_Eff_ETA_MC_"+Selections[j]+"_"+Conv_flag[i], "", ETA_nbins-1, ETA);
			Eff_MC_ETA[i][j]->SetLineWidth(2);
			Eff_MC_ETA[i][j]->SetLineColor(2);
			Eff_MC_ETA[i][j]->SetMarkerStyle(25);
			Eff_MC_ETA[i][j]->GetYaxis()->SetRangeUser(0, 1.1);

			//Eff_MC_ETA[i][j]->SetDirectory(output);
			//Eff_data_ETA[i][j]->SetDirectory(output);


			Eff_data_MU[i][j] = new TH1F("Plot_Eff_MU_Data_"+Selections[j]+"_"+Conv_flag[i], "", MU_nbins-1, MU);
			Eff_data_MU[i][j]->SetLineWidth(2);
			Eff_data_MU[i][j]->SetLineColor(1);
			Eff_data_MU[i][j]->SetMarkerStyle(20);
			Eff_data_MU[i][j]->GetYaxis()->SetRangeUser(0, 1.1);

			Eff_MC_MU[i][j]   = new TH1F("Plot_Eff_MU_MC_"+Selections[j]+"_"+Conv_flag[i], "", MU_nbins-1, MU);
			Eff_MC_MU[i][j]->SetLineWidth(2);
			Eff_MC_MU[i][j]->SetLineColor(2);
			Eff_MC_MU[i][j]->SetMarkerStyle(25);
			Eff_MC_MU[i][j]->GetYaxis()->SetRangeUser(0, 1.1);

			//Eff_MC_MU[i][j]->SetDirectory(output);
			//Eff_data_MU[i][j]->SetDirectory(output);
			

			for(int h = 0; h<PT_nbins-1; h++)
			{
				TString name = Case+"_"+target+"_iso_"+isoWP+"_"+Selections[j]+"_"+Conv_flag[i]+"_Bin_PT_"+std::to_string(h);
				vector<double> reslt = Fit(1, h_Mllg_bin[i][0][0][h], h_Mllg_bin[i][0][1][h], h_Mllg_bin[i][0][2][h], h_Mllg_bin[i][j][0][h], h_Mllg_bin[i][j][1][h], h_Mllg_bin[i][j][2][h], target, name, true);
				if(reslt.size() == 0)
				{
					reslt = Fit(1, h_Mllg_bin[i][0][0][h], h_Mllg_bin[i][0][1][h], h_Mllg_bin[i][0][2][h], h_Mllg_bin[i][j][0][h], h_Mllg_bin[i][j][1][h], h_Mllg_bin[i][j][2][h], target, name, true);
				}
				Eff_data[i][j]->SetBinContent(h+1, reslt[0]);
				Eff_data[i][j]->SetBinError(h+1, reslt[1]);
				if(h >= 6)
				{
					//Eff_data[i][j]->SetBinContent(h+1, reslt[4]);
					//Eff_data[i][j]->SetBinError(h+1, reslt[5]);
				}
				Eff_MC[i][j]->SetBinContent(h+1, reslt[2]);
				Eff_MC[i][j]->SetBinError(h+1, reslt[3]);
			}
			
	
			for(int h = 0; h<MU_nbins-1; h++)
			{
				TString name = Case+"_"+target+"_iso_"+isoWP+"_"+Selections[j]+"_"+Conv_flag[i]+"_Bin_MU_"+std::to_string(h);
				vector<double> reslt = Fit(1, h_Mllg_binMU[i][0][0][h], h_Mllg_binMU[i][0][1][h], h_Mllg_binMU[i][0][2][h], h_Mllg_binMU[i][j][0][h], h_Mllg_binMU[i][j][1][h], h_Mllg_binMU[i][j][2][h], target, name, true);
				if(reslt.size() == 0)
				{
					reslt = Fit(1, h_Mllg_binMU[i][0][0][h], h_Mllg_binMU[i][0][1][h], h_Mllg_binMU[i][0][2][h], h_Mllg_binMU[i][j][0][h], h_Mllg_binMU[i][j][1][h], h_Mllg_binMU[i][j][2][h], target, name, true);
				}
				Eff_data_MU[i][j]->SetBinContent(h+1, reslt[0]);
				Eff_data_MU[i][j]->SetBinError(h+1, reslt[1]);

				if(h >= 6)
				{
					//Eff_data_MU[i][j]->SetBinContent(h+1, reslt[4]);
					//Eff_data_MU[i][j]->SetBinError(h+1, reslt[5]);
				}

				Eff_MC_MU[i][j]->SetBinContent(h+1, reslt[2]);
				Eff_MC_MU[i][j]->SetBinError(h+1, reslt[3]);
			}

			
			
			for(int h = 0; h<ETA_nbins-1; h++)
			{
				if(h == 4) continue; 
				TString name = Case+"_"+target+"_iso_"+isoWP+"_"+Selections[j]+"_"+Conv_flag[i]+"_Bin_ETA_"+std::to_string(h);
				vector<double> reslt = Fit(1, h_Mllg_binETA[i][0][0][h], h_Mllg_binETA[i][0][1][h], h_Mllg_binETA[i][0][2][h], h_Mllg_binETA[i][j][0][h], h_Mllg_binETA[i][j][1][h], h_Mllg_binETA[i][j][2][h], target, name, true);
				if(reslt.size() == 0)
				{
					reslt = Fit(1, h_Mllg_binETA[i][0][0][h], h_Mllg_binETA[i][0][1][h], h_Mllg_binETA[i][0][2][h], h_Mllg_binETA[i][j][0][h], h_Mllg_binETA[i][j][1][h], h_Mllg_binETA[i][j][2][h], target, name, true);
				}
				Eff_data_ETA[i][j]->SetBinContent(h+1, reslt[0]);
				Eff_data_ETA[i][j]->SetBinError(h+1, reslt[1]);
				if(h >= 6)
				{
					//Eff_data_ETA[i][j]->SetBinContent(h+1, reslt[4]);
					//Eff_data_ETA[i][j]->SetBinError(h+1, reslt[5]);
				}
				Eff_MC_ETA[i][j]->SetBinContent(h+1, reslt[2]);
				Eff_MC_ETA[i][j]->SetBinError(h+1, reslt[3]);
			}
		
			
			
		}
	}

//output->Write();
//output->Close();

}


vector<double> Fit(int bkgTempl, TH1F* h_Sig_1, TH1F* h_Bkg_1, TH1F* h_Data_1, TH1F* h_Sig_2, TH1F* h_Bkg_2, TH1F* h_Data_2, TString target, TString name, bool useFit)
{
	vector<double> Values;
	cout<<h_Sig_1->GetName()<<endl;
	cout<<h_Sig_2->GetName()<<endl;

	cout<<h_Bkg_1->GetName()<<endl;
	cout<<h_Bkg_2->GetName()<<endl;

	cout<<h_Data_1->GetName()<<endl;
	cout<<h_Data_2->GetName()<<endl;


	h_Sig_1->Scale(1000./h_Sig_1->Integral());
	h_Sig_2->Scale(900./h_Sig_2->Integral());

	h_Bkg_1->Scale(100./h_Bkg_1->Integral());
	h_Bkg_2->Scale(10./h_Bkg_2->Integral());

	h_Data_1->Scale(0./h_Data_1->Integral());
	h_Data_2->Scale(0./h_Data_2->Integral());

	h_Data_1->Add(h_Sig_1, h_Bkg_1);
	h_Data_2->Add(h_Sig_2, h_Bkg_2);


	//h_Data_1->Scale(h_Sig_1->Integral()+h_Bkg_1->Integral());
	//h_Data_2->Scale(h_Sig_2->Integral()+h_Bkg_2->Integral());

	cout<<h_Sig_1->Integral()<<endl;
	cout<<h_Sig_2->Integral()<<endl;

	cout<<h_Bkg_1->Integral()<<endl;
	cout<<h_Bkg_2->Integral()<<endl;

	cout<<h_Data_1->Integral()<<endl;
	cout<<h_Data_2->Integral()<<endl;

	cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;

	double min_mllg = 65;
	double max_mllg = 105;

	double min_mllg_s = 80;
	double max_mllg_s = 100;

	double min_bin = h_Data_1->FindBin(min_mllg);
	double max_bin = h_Data_1->FindBin(max_mllg) - 1;
	
	double min_bin_s = h_Data_1->FindBin(min_mllg_s);
	double max_bin_s = h_Data_1->FindBin(max_mllg_s) - 1;

	RooRealVar mass("mass", "M_{ll#gamma} [GeV]", min_mllg, max_mllg);
	RooArgList X(mass);

	mass.setRange("s_range", min_mllg_s, max_mllg_s);
	mass.setRange("full_range", min_mllg, max_mllg);

	RooDataHist *Data_1 = new RooDataHist("Data_1", "Data Before ", X, h_Data_1);
	RooDataHist *Data_2 = new RooDataHist("Data_2", "Data After ", X, h_Data_2);

	RooDataHist *Sig_1 = new RooDataHist("Sig_1", "Signal Before ", X, h_Sig_1);
	RooDataHist *Sig_2 = new RooDataHist("Sig_2", "Signal After ", X, h_Sig_2);

	RooDataHist *Bkg_1 = new RooDataHist("Bkg_1", "Background Before ", X, h_Bkg_1);
	RooDataHist *Bkg_2 = new RooDataHist("Bkg_2", "Background After ", X, h_Bkg_2);

	// Create PDFs from Histogram directly

	RooHistPdf Pdf_Sig_1("Pdf_Sig_1", "PDF Signal Before", RooArgSet(mass), *Sig_1);
	RooHistPdf Pdf_Sig_2("Pdf_Sig_2", "PDF Signal After", RooArgSet(mass), *Sig_2);

	RooHistPdf Pdf_Bkg_1("Pdf_Bkg_1", "PDF Bkg Before", RooArgSet(mass), *Bkg_1);
	RooHistPdf Pdf_Bkg_2("Pdf_Bkg_2", "PDF Bkg After", RooArgSet(mass), *Bkg_2);

	RooRealVar a1("a1", "a1", .01, -1e6, 1e6);
	RooRealVar a2("a2", "a2", .01, -1e6, 1e6);
	RooRealVar a3("a3", "a3", -1e5, 1e5);

	RooPolynomial Pdf_Bkg_Pol_1("Pdf_Bkg_Pol_1", "Poly Bkg PDF Before", mass, RooArgList(a1)); //RooPolynomial
	RooPolynomial Pdf_Bkg_Pol_2("Pdf_Bkg_Pol_2", "Poly Bkg PDF After", mass, RooArgList(a1));


	RooRealVar f_Sig_1("f_Sig_1", "Fraction Signal Before", 0., 0., h_Sig_1->Integral());
	RooRealVar f_Sig_2("f_Sig_2", "Fraction Signal After", 0., 0., h_Sig_2->Integral());

	RooRealVar f_Bkg_1("f_Bkg_1", "Fraction Bkg Before", 0., 0., h_Bkg_1->Integral());
	RooRealVar f_Bkg_2("f_Bkg_2", "Fraction Bkg After", 0., 0., h_Bkg_2->Integral());



	RooAddPdf* Sum_1;
	RooAddPdf* Sum_2;
	
	if(bkgTempl == 1)
	{	
	 	Sum_1 = new RooAddPdf("Sum_1", "Sum PDFs Before", RooArgList(Pdf_Sig_1, Pdf_Bkg_Pol_1), RooArgList(f_Sig_1, f_Bkg_1));
		Sum_2 = new RooAddPdf("Sum_2", "Sum PDFs After", RooArgList(Pdf_Sig_2, Pdf_Bkg_Pol_2), RooArgList(f_Sig_2, f_Bkg_2));

	}else{
		Sum_1 = new RooAddPdf("Sum_1", "Sum PDFs Before", RooArgList(Pdf_Sig_1, Pdf_Bkg_1), RooArgList(f_Sig_1, f_Bkg_1));
		Sum_2 = new RooAddPdf("Sum_2", "Sum PDFs After", RooArgList(Pdf_Sig_2, Pdf_Bkg_2), RooArgList(f_Sig_2, f_Bkg_2));
	}

	RooAbsReal* nll_1 = Sum_1->createNLL(*Data_1, Extended(1));

	RooMinimizer minimizer_1(*nll_1);
  	minimizer_1.setOffsetting( 1 );
	minimizer_1.setStrategy(1);
	minimizer_1.setErrorLevel(0.001);
  	minimizer_1.minimize("Minuit2");

	RooFitResult* Fit_1 = minimizer_1.save("Fit_1", "Fit Results");

	//RooFitResult* Fit_1 = Sum_1->fitTo(*Data_1, SumW2Error(true), Save(), Range("full_range"), SumCoefRange("s_range"), PrintLevel(-1));
	//Sum_1->fixCoefRange("s_range");

	RooArgSet *Params_1 = Sum_1->getVariables();
    	Params_1->Print("v");

	RooAbsReal* nll_2 = Sum_2->createNLL(*Data_2, Extended(1));

	RooMinimizer minimizer_2(*nll_2);
  	minimizer_2.setOffsetting( 1 );
	minimizer_2.setStrategy(1);
	minimizer_2.setErrorLevel(0.001);
  	minimizer_2.minimize("Minuit2");

	RooFitResult* Fit_2 = minimizer_2.save("Fit_2", "Fit Results");

	//RooFitResult* Fit_2 = Sum_2->fitTo(*Data_2, SumW2Error(true), Save(), Range("full_range"), SumCoefRange("s_range"), PrintLevel(-1));
	//Sum_2->fixCoefRange("s_range");

	RooArgSet *Params_2 = Sum_2->getVariables();
    	Params_2->Print("v");


	cout<< " MC Signal Before : "<< h_Sig_1->Integral() <<endl;
	cout<< " MC Signal After : "<< h_Sig_2->Integral() <<endl;

	cout<< " MC Bkg Before : "<< h_Bkg_1->Integral() <<endl;
	cout<< " MC Bkg After : "<< h_Bkg_2->Integral() <<endl;

	cout<< " Total Fitted Signal Before : "<< f_Sig_1.getVal() <<endl;
	cout<< " Total Fitted Signal After : "<< f_Sig_2.getVal() <<endl;

	cout<< " Total Fitted Bkg Before : "<< f_Bkg_1.getVal() <<endl;
	cout<< " Total Fitted Bkg After : "<< f_Bkg_2.getVal() <<endl;

	RooRealVar* f_Sig_SR_1 = (RooRealVar*)Pdf_Sig_1.createIntegral(mass, NormSet(mass), Range("s_range"));
	RooRealVar* f_Sig_SR_2 = (RooRealVar*)Pdf_Sig_2.createIntegral(mass, NormSet(mass), Range("s_range"));

	RooRealVar* f_Bkg_SR_1; 
	RooRealVar* f_Bkg_SR_2;

	if(bkgTempl == 1)
	{	
		f_Bkg_SR_1 = (RooRealVar*)Pdf_Bkg_Pol_1.createIntegral(mass, NormSet(mass), Range("s_range"));
		f_Bkg_SR_2 = (RooRealVar*)Pdf_Bkg_Pol_2.createIntegral(mass, NormSet(mass), Range("s_range"));
	}else{

		f_Bkg_SR_1 = (RooRealVar*)Pdf_Bkg_1.createIntegral(mass, NormSet(mass), Range("s_range"));
		f_Bkg_SR_2 = (RooRealVar*)Pdf_Bkg_2.createIntegral(mass, NormSet(mass), Range("s_range"));
	}

	cout<< " Signal Fraction in SR Before : "<< f_Sig_SR_1->getVal() <<endl;
	cout<< " Signal Fraction in SR After : "<< f_Sig_SR_2->getVal() <<endl;

	cout<< " Bkg Fraction in SR Before : "<< f_Bkg_SR_1->getVal() <<endl;
	cout<< " Bkg Fraction in SR After : "<< f_Bkg_SR_2->getVal() <<endl;

	cout<< " Number of Signal in SR Before : "<< f_Sig_1.getVal()*f_Sig_SR_1->getVal() <<endl;
	cout<< " Number of Signal in SR After : "<< f_Sig_2.getVal()*f_Sig_SR_2->getVal() <<endl;

	cout<< " Number of Bkg in SR Before : "<< f_Bkg_1.getVal()*f_Bkg_SR_1->getVal() <<endl;
	cout<< " Number of Bkg in SR After : "<< f_Bkg_2.getVal()*f_Bkg_SR_2->getVal() <<endl;

	cout << " Number of Data in SR Before : "<< h_Data_1->Integral(min_bin_s, max_bin_s) <<endl;
	cout << " Number of Data in SR After : "<< h_Data_2->Integral(min_bin_s, max_bin_s) <<endl;

	cout <<" Number of Total Signal Before : "<< f_Sig_1.getVal() << " +/- " << f_Sig_1.getError() <<endl;
	cout <<" Number of Total Signal After  : "<< f_Sig_2.getVal() << " +/- " << f_Sig_2.getError() <<endl;

	cout <<" Number of Total Bkg Before : "<< f_Bkg_1.getVal() << " +/- " << f_Bkg_1.getError() <<endl;
	cout <<" Number of Total Bkg After  : "<< f_Bkg_2.getVal() << " +/- " << f_Bkg_2.getError() <<endl;


	double N_Data_1 = h_Data_1->Integral(min_bin_s, max_bin_s);
	double N_Data_2 = h_Data_2->Integral(min_bin_s, max_bin_s);

	Double_t N_Sig_1 = h_Sig_1->Integral(min_bin_s, max_bin_s);
	Double_t N_Sig_2 = h_Sig_2->Integral(min_bin_s, max_bin_s);

	Double_t N_Sig_Fitted_1 = f_Sig_1.getVal()*f_Sig_SR_1->getVal();
	Double_t N_Sig_Fitted_2 = f_Sig_2.getVal()*f_Sig_SR_2->getVal();

	Double_t N_Bkg_Fitted_1 = f_Bkg_1.getVal()*f_Bkg_SR_1->getVal();
	Double_t N_Bkg_Fitted_2 = f_Bkg_2.getVal()*f_Bkg_SR_2->getVal();



	TEfficiency* Eff_Data_UnCorr = new TEfficiency("Eff_Data_UnCorr", "Data Efficiency UnCorrected", 1, 0, 1);

	Eff_Data_UnCorr->SetTotalEvents(1, N_Data_1);

	if(int(N_Data_1) < int(N_Data_2))
	{
		cout<<" Something wrong N_Data_1 < N_Data_2 ... Skip" <<endl;
		return Values;
	}
	
	Eff_Data_UnCorr->SetPassedEvents(1, N_Data_2);


	TEfficiency* Eff_Sig = new TEfficiency("Eff_Sig", "Sig Efficiency", 1, 0, 1);

	Eff_Sig->SetTotalEvents(1, N_Sig_1);

	if(int(N_Sig_1) < int(N_Sig_2))
	{
		cout<<" Something wrong N_Sig_1 < N_Sig_2 ... Skip" <<endl;
		return Values;
	}
	
	Eff_Sig->SetPassedEvents(1, N_Sig_2);


	TEfficiency* Eff_Data_Corr = new TEfficiency("Eff_Data_Corr", "Data Efficiency Corrected", 1, 0, 1);

	Eff_Data_Corr->SetTotalEvents(1, N_Sig_Fitted_1);

	if(int(N_Sig_Fitted_1) < int(N_Sig_Fitted_2))
	{
		cout<<" Something wrong f_Sig_1 < f_Sig_2 ... Skip" <<endl;
		return Values;
	}
	
	Eff_Data_Corr->SetPassedEvents(1, N_Sig_Fitted_2);


	double Eff_UnCorr_Central = Eff_Data_UnCorr->GetEfficiency(1);
	double Eff_UnCorr_Sys     = sqrt(sqrt(N_Data_2)/N_Data_2*(sqrt(N_Data_2)/N_Data_2) + sqrt(N_Data_1)/N_Data_1*(sqrt(N_Data_1)/N_Data_1));

	double Eff_Sig_Central    = Eff_Sig->GetEfficiency(1);

	double Eff_Sig_Sys        = Eff_Sig->GetEfficiencyErrorLow(1);

	double Purity_1 = N_Sig_Fitted_1/(N_Bkg_Fitted_1+N_Sig_Fitted_1); 
	double Purity_2 = N_Sig_Fitted_2/(N_Bkg_Fitted_2+N_Sig_Fitted_2); 


	double Eff_Corr_Central = Eff_Data_Corr->GetEfficiency(1);
	double Eff_Corr_Sys     = sqrt(Eff_Data_Corr->GetEfficiencyErrorLow(1)*Eff_Data_Corr->GetEfficiencyErrorLow(1) + Eff_Data_Corr->GetEfficiencyErrorUp(1)*Eff_Data_Corr->GetEfficiencyErrorUp(1));

	double Fit_Err_1   = sqrt((sqrt(N_Sig_Fitted_1)/N_Sig_Fitted_1)*(sqrt(N_Sig_Fitted_1)/N_Sig_Fitted_1) + (sqrt(N_Bkg_Fitted_1)/N_Bkg_Fitted_1)*(sqrt(N_Bkg_Fitted_1)/N_Bkg_Fitted_1));

	double Fit_Err_2   = sqrt((sqrt(N_Sig_Fitted_2)/N_Sig_Fitted_2)*(sqrt(N_Sig_Fitted_2)/N_Sig_Fitted_2) + (sqrt(N_Bkg_Fitted_2)/N_Bkg_Fitted_2)*(sqrt(N_Bkg_Fitted_2)/N_Bkg_Fitted_2));

	double Eff_Fit_Sys     = 1./sqrt(1/(Fit_Err_1*Fit_Err_1) + 1/(Fit_Err_2*Fit_Err_2));

	double Eff_Alt_Central = (h_Data_2->Integral(min_bin_s, max_bin_s) - N_Bkg_Fitted_2)/(h_Data_1->Integral(min_bin_s, max_bin_s) - N_Bkg_Fitted_1);

	double Eff_Alt_Sys     = Eff_Corr_Sys;

	cout<< " Eff Data UnCorr " << Eff_UnCorr_Central << " +/- " << Eff_UnCorr_Sys << endl;
	cout<< " Eff Data Corr   " << Eff_Corr_Central << " +/- " << Eff_Corr_Sys << endl;

	cout<< " Eff Sig MC              " << Eff_Sig_Central << " +/- " << Eff_Sig_Sys << endl;
	cout<< " Eff Sig MC Error        " << Eff_Sig_Sys << endl;

	cout<< " Alt Data                " << Eff_Alt_Central << endl;
	cout<< " Alt Err                 " << Eff_Alt_Sys << endl;

	cout<< " Signal Purity in SR Before " << Purity_1 << endl;
	cout<< " Signal Purity in SR After  " << Purity_2 << endl;

	cout<< name <<endl;



	Values.push_back(Eff_Corr_Central);
	Values.push_back(Eff_Corr_Sys);
	Values.push_back(Eff_Sig_Central);
	Values.push_back(Eff_Sig_Sys);
	Values.push_back(Eff_UnCorr_Central);
	Values.push_back(Eff_UnCorr_Sys);
	Values.push_back(Eff_Alt_Central);
	Values.push_back(Eff_Alt_Sys);

	
	
	

	// Plots
	
	TCanvas *canvs_1 = new TCanvas(name+"_Before", " ", 147, 22, 800, 800);
	canvs_1->SetLogy();

	RooPlot *Plot_1 = mass.frame(Title("Data Before"));
	Data_1->plotOn(Plot_1); 

	if(bkgTempl == 1)
	{

		Sum_1->plotOn(Plot_1, Range("full_range"), Components(RooArgSet(Pdf_Sig_1, Pdf_Bkg_Pol_1)),LineColor(kBlack));

	}else{

		Sum_1->plotOn(Plot_1, Range("full_range"), Components(RooArgSet(Pdf_Sig_1, Pdf_Bkg_1)),LineColor(kBlack));
	}

	Sum_1->plotOn(Plot_1, Range("full_range"), Components(Pdf_Sig_1),LineStyle(kDashed),LineColor(kRed),MoveToBack());

	if(bkgTempl == 1)
	{
		Sum_1->plotOn(Plot_1, Range("full_range"), Components(Pdf_Bkg_Pol_1),LineStyle(kDashed),LineColor(kBlue),MoveToBack());
		
	}else{

		Sum_1->plotOn(Plot_1, Range("full_range"), Components(Pdf_Bkg_1),LineStyle(kDashed),LineColor(kBlue),MoveToBack());
	}

	//Data_1->plotOn(Plot_1);

	TLine *l1 = new TLine(80,0,80,1000) ;
    	l1->SetLineStyle(kDashed);
    	l1->SetLineColor(kGreen);
    	Plot_1->addObject(l1) ;

   	TLine *l2 = new TLine(100,0,100,1000) ;
    	l2->SetLineStyle(kDashed);
    	l2->SetLineColor(kGreen);
    	Plot_1->addObject(l2);
    	
	TH1F *h_1 = new TH1F("h_1", "h_1", 10, 0, 1);
	TH1F *h_2 = new TH1F("h_2", "h_2", 10, 0, 1);
	TH1F *h_3 = new TH1F("h_3", "h_3", 10, 0, 1);
	
	h_1->SetLineColor(kRed);
	h_1->SetLineStyle(kDashed);
	h_1->SetLineWidth(2);
	h_2->SetLineColor(kBlue);
	h_2->SetLineStyle(kDashed);
	h_2->SetLineWidth(2);
	h_3->SetLineColor(kBlack);
	h_3->SetLineWidth(2);
	h_3->SetMarkerStyle(20);

        TLegend *leg = new TLegend(0.15,0.7,0.35,0.85);
        leg->SetBorderSize(0);
    	leg->SetTextSize(0.04347826);
    	leg->SetLineColor(1);
    	leg->SetLineStyle(1);
    	leg->SetLineWidth(1);
    	leg->SetFillColor(10);
    	leg->SetFillStyle(1001);

	TString channel;
	if(target.Contains("mumug"))
	{
		channel="#mu#mu";
	}else if(target.Contains("eeg"))
	{
		channel="ee";
	}else{
		channel="ll";
	}
	leg->AddEntry(h_1,"Z("+channel+")#gamma", "l");
	if (bkgTempl == 1) leg->AddEntry(h_2,"Poly", "l");
	else leg->AddEntry(h_2,"Z("+channel+")+jets", "l");
	leg->AddEntry(h_3,"Sum", "l");
	leg->AddEntry(Data_1,"Data", "pl");
	
	Plot_1->Draw();
	leg->Draw("same");

	canvs_1->SaveAs("PDF/"+name+"_Before.pdf");

	TCanvas *canvs_2 = new TCanvas(name+"_After", " ", 147, 22, 800, 800);
	canvs_2->SetLogy();

	RooPlot *Plot_2 = mass.frame(Title("Data After"));
	Data_2->plotOn(Plot_2); 

	if(bkgTempl == 1)
	{

		Sum_2->plotOn(Plot_2, Range("full_range"), Components(RooArgSet(Pdf_Sig_2, Pdf_Bkg_Pol_2)),LineColor(kBlack));

	
	}else{

		Sum_2->plotOn(Plot_2, Range("full_range"), Components(RooArgSet(Pdf_Sig_2, Pdf_Bkg_2)),LineColor(kBlack));
	}

		Sum_2->plotOn(Plot_2, Range("full_range"), Components(Pdf_Sig_2),LineStyle(kDashed),LineColor(kRed),MoveToBack());

	if(bkgTempl == 1)
	{
		Sum_2->plotOn(Plot_2, Range("full_range"), Components(Pdf_Bkg_Pol_2),LineStyle(kDashed),LineColor(kBlue),MoveToBack());
		
	
	}else{

		Sum_2->plotOn(Plot_2, Range("full_range"), Components(Pdf_Bkg_2),LineStyle(kDashed),LineColor(kBlue),MoveToBack());
	}

	//Data_2->plotOn(Plot_2);

	Plot_2->addObject(l1);
	Plot_2->addObject(l2);

	Plot_2->Draw();
	leg->Draw("same");
	canvs_2->SaveAs("PDF/"+name+"_After.pdf");
	
	return Values;
}
