///////////////////////// -*- C++ -*- /////////////////////////////

/*
  Copyright (C) 2019-2020 CERN for the benefit of the ATLAS collaboration
*/

// CleanAndGet.h
// header file for class CleanAndGet
// Author: BELFKIR Mohamed 
// Email : mohamed.belfkir@cern.ch
///////////////////////////////////////////////////////////////////

#ifndef CleanAndGet_H
#define CleanAndGet_H 1

#include <memory>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

// ROOT include(s):
#include <TFile.h>
#include <TH1F.h>
#include <TChain.h>
#include <TError.h>
#include <TString.h>
#include <TStopwatch.h>
#include <TSystem.h>
#include "TLorentzVector.h"
#include "TMath.h"
#include "TROOT.h"
#include "TVector3.h"
#include "Variables.h"
// Infrastructure include(s):
#ifdef ROOTCORE
#   include "xAODRootAccess/Init.h"
#   include "xAODRootAccess/TEvent.h"
// Local include(s):
#include "CPAnalysisExamples/errorcheck.h"
#endif // ROOTCORE
#include "TSystemDirectory.h"


class CleanAndGet {

public:

	CleanAndGet(TString dirname,TString dirout,Int_t stg, Int_t i, Int_t n, TString name);
	virtual ~CleanAndGet();
	virtual bool initilize();
	virtual bool finilize();
	virtual bool runCleanAndGet();

private:

	std::vector<TString> Load(TString directory);
	TChain* GetChain(std::vector<TString> Samples, TString TreeName);
	bool Strat0();
	bool Strat1();
	bool Strat2();
	bool Strat3();
	bool Strat4();
	void Normalize();
	void NormalizeCluster(vector<float> *&cluster, float sumE);
	float SumCluster(vector<float> *cluster);
	void CompletCluster();
	
private:

TString m_DirName;
TString m_DirOut;
TChain *chain;
Int_t m_Strategy;
Int_t m_i;
Int_t m_n;
TString m_name;


TFile *_output_file; 
TTree *_output1_tree; 
TTree *_output2_tree;
TTree *_output3_tree;
TTree *_output4_tree;

inline void LinkInputBranches(TChain* fChain)
{
   // Set object pointer
   ph_clusterCellsLr2E7x11 = 0;
   ph_clusterCellsLr2Eta7x11 = 0;
   ph_clusterCellsLr2Phi7x11 = 0;
   ph_clusterCellsLr2X7x11 = 0;
   ph_clusterCellsLr2Y7x11 = 0;
   ph_clusterCellsLr2Z7x11 = 0;
   ph_clusterCellsLr2T7x11 = 0;
   ph_clusterCellsLr2isBAD7x11 = 0;
   ph_clusterCellsLr2E3x7 = 0;
   ph_clusterCellsLr2Eta3x7 = 0;
   ph_clusterCellsLr2Phi3x7 = 0;
   ph_clusterCellsLr2X3x7 = 0;
   ph_clusterCellsLr2Y3x7 = 0;
   ph_clusterCellsLr2Z3x7 = 0;
   ph_clusterCellsLr2T3x7 = 0;
   ph_clusterCellsLr2isBAD3x7 = 0;
   ph_clusterCellsLr2E5x5 = 0;
   ph_clusterCellsLr2Eta5x5 = 0;
   ph_clusterCellsLr2Phi5x5 = 0;
   ph_clusterCellsLr2X5x5 = 0;
   ph_clusterCellsLr2Y5x5 = 0;
   ph_clusterCellsLr2Z5x5 = 0;
   ph_clusterCellsLr2T5x5 = 0;
   ph_clusterCellsLr2isBAD5x5 = 0;
   ph_clusterCellsLr1E7x11 = 0;
   ph_clusterCellsLr1Eta7x11 = 0;
   ph_clusterCellsLr1Phi7x11 = 0;
   ph_clusterCellsLr1X7x11 = 0;
   ph_clusterCellsLr1Y7x11 = 0;
   ph_clusterCellsLr1Z7x11 = 0;
   ph_clusterCellsLr1T7x11 = 0;
   ph_clusterCellsLr1isBAD7x11 = 0;
   ph_clusterCellsLr1E3x7 = 0;
   ph_clusterCellsLr1Eta3x7 = 0;
   ph_clusterCellsLr1Phi3x7 = 0;
   ph_clusterCellsLr1X3x7 = 0;
   ph_clusterCellsLr1Y3x7 = 0;
   ph_clusterCellsLr1Z3x7 = 0;
   ph_clusterCellsLr1T3x7 = 0;
   ph_clusterCellsLr1isBAD3x7 = 0;
   ph_clusterCellsLr1E5x5 = 0;
   ph_clusterCellsLr1Eta5x5 = 0;
   ph_clusterCellsLr1Phi5x5 = 0;
   ph_clusterCellsLr1X5x5 = 0;
   ph_clusterCellsLr1Y5x5 = 0;
   ph_clusterCellsLr1Z5x5 = 0;
   ph_clusterCellsLr1T5x5 = 0;
   ph_clusterCellsLr1isBAD5x5 = 0;
   ph_clusterCellsLr3E7x11 = 0;
   ph_clusterCellsLr3Eta7x11 = 0;
   ph_clusterCellsLr3Phi7x11 = 0;
   ph_clusterCellsLr3X7x11 = 0;
   ph_clusterCellsLr3Y7x11 = 0;
   ph_clusterCellsLr3Z7x11 = 0;
   ph_clusterCellsLr3T7x11 = 0;
   ph_clusterCellsLr3isBAD7x11 = 0;
   ph_clusterCellsLr3E3x7 = 0;
   ph_clusterCellsLr3Eta3x7 = 0;
   ph_clusterCellsLr3Phi3x7 = 0;
   ph_clusterCellsLr3X3x7 = 0;
   ph_clusterCellsLr3Y3x7 = 0;
   ph_clusterCellsLr3Z3x7 = 0;
   ph_clusterCellsLr3T3x7 = 0;
   ph_clusterCellsLr3isBAD3x7 = 0;
   ph_clusterCellsLr3E5x5 = 0;
   ph_clusterCellsLr3Eta5x5 = 0;
   ph_clusterCellsLr3Phi5x5 = 0;
   ph_clusterCellsLr3X5x5 = 0;
   ph_clusterCellsLr3Y5x5 = 0;
   ph_clusterCellsLr3Z5x5 = 0;
   ph_clusterCellsLr3T5x5 = 0;
   ph_clusterCellsLr3isBAD5x5 = 0;
   ph_clusterCellsPreSE7x11 = 0;
   ph_clusterCellsPreSEta7x11 = 0;
   ph_clusterCellsPreSPhi7x11 = 0;
   ph_clusterCellsPreSX7x11 = 0;
   ph_clusterCellsPreSY7x11 = 0;
   ph_clusterCellsPreSZ7x11 = 0;
   ph_clusterCellsPreST7x11 = 0;
   ph_clusterCellsPreSisBAD7x11 = 0;
   ph_clusterCellsPreSE3x7 = 0;
   ph_clusterCellsPreSEta3x7 = 0;
   ph_clusterCellsPreSPhi3x7 = 0;
   ph_clusterCellsPreSX3x7 = 0;
   ph_clusterCellsPreSY3x7 = 0;
   ph_clusterCellsPreSZ3x7 = 0;
   ph_clusterCellsPreST3x7 = 0;
   ph_clusterCellsPreSisBAD3x7 = 0;
   ph_clusterCellsPreSE5x5 = 0;
   ph_clusterCellsPreSEta5x5 = 0;
   ph_clusterCellsPreSPhi5x5 = 0;
   ph_clusterCellsPreSX5x5 = 0;
   ph_clusterCellsPreSY5x5 = 0;
   ph_clusterCellsPreSZ5x5 = 0;
   ph_clusterCellsPreST5x5 = 0;
   ph_clusterCellsPreSisBAD5x5 = 0;

   fChain->SetBranchAddress("ph_Pt", &ph_Pt);
   fChain->SetBranchAddress("ph_Eta", &ph_Eta);
   fChain->SetBranchAddress("ph_Phi", &ph_Phi);
   fChain->SetBranchAddress("ph_E", &ph_E);
   fChain->SetBranchAddress("ph_topoetcone20", &ph_topoetcone20);
   fChain->SetBranchAddress("ph_topoetcone30", &ph_topoetcone30);
   fChain->SetBranchAddress("ph_topoetcone40", &ph_topoetcone40);
   fChain->SetBranchAddress("ph_ptcone20", &ph_ptcone20);
   fChain->SetBranchAddress("ph_ptcone30", &ph_ptcone30);
   fChain->SetBranchAddress("ph_ptcone40", &ph_ptcone40);
   fChain->SetBranchAddress("ph_etcone20", &ph_etcone20);
   fChain->SetBranchAddress("ph_etcone30", &ph_etcone30);
   fChain->SetBranchAddress("ph_etcone40", &ph_etcone40);
   fChain->SetBranchAddress("ph_ptvarcone20", &ph_ptvarcone20);
   fChain->SetBranchAddress("ph_ptvarcone30", &ph_ptvarcone30);
   fChain->SetBranchAddress("ph_ptvarcone40", &ph_ptvarcone40);
   fChain->SetBranchAddress("ph_isIsoLooseWP", &ph_isIsoLooseWP);
   fChain->SetBranchAddress("ph_isIsoTightWP", &ph_isIsoTightWP);
   fChain->SetBranchAddress("ph_isIsoTightCaloOnlyWP", &ph_isIsoTightCaloOnlyWP);
   fChain->SetBranchAddress("ph_isEMLooseOff", &ph_isEMLooseOff);
   fChain->SetBranchAddress("ph_isEMTightIncOff", &ph_isEMTightIncOff);
   fChain->SetBranchAddress("ph_isEMTightDepOff", &ph_isEMTightDepOff);
   fChain->SetBranchAddress("ph_isEMLooseOn", &ph_isEMLooseOn);
   fChain->SetBranchAddress("ph_isEMTightOn", &ph_isEMTightOn);
   fChain->SetBranchAddress("ph_isEMMediumOn", &ph_isEMMediumOn);
   fChain->SetBranchAddress("ph_Conversion", &ph_Conversion);
   fChain->SetBranchAddress("ph_Ethad", &ph_Ethad);
   fChain->SetBranchAddress("ph_Ethad1", &ph_Ethad1);
   fChain->SetBranchAddress("ph_Rhad1", &ph_Rhad1);
   fChain->SetBranchAddress("ph_Rhad", &ph_Rhad);
   fChain->SetBranchAddress("ph_E011", &ph_E011);
   fChain->SetBranchAddress("ph_E132", &ph_E132);
   fChain->SetBranchAddress("ph_E237", &ph_E237);
   fChain->SetBranchAddress("ph_E277", &ph_E277);
   fChain->SetBranchAddress("ph_Reta", &ph_Reta);
   fChain->SetBranchAddress("ph_Rphi", &ph_Rphi);
   fChain->SetBranchAddress("ph_Weta1", &ph_Weta1);
   fChain->SetBranchAddress("ph_Weta2", &ph_Weta2);
   fChain->SetBranchAddress("ph_f1", &ph_f1);
   fChain->SetBranchAddress("ph_f3", &ph_f3);
   fChain->SetBranchAddress("ph_f3core", &ph_f3core);
   fChain->SetBranchAddress("ph_fracs1", &ph_fracs1);
   fChain->SetBranchAddress("ph_Wstot1", &ph_Wstot1);
   fChain->SetBranchAddress("ph_deltaE", &ph_deltaE);
   fChain->SetBranchAddress("ph_Eratio", &ph_Eratio);
   fChain->SetBranchAddress("ph_E2tsts1", &ph_E2tsts1);
   fChain->SetBranchAddress("ph_Emins1", &ph_Emins1);
   fChain->SetBranchAddress("ph_Emaxs1", &ph_Emaxs1);
   fChain->SetBranchAddress("ph_isTruthMatch", &ph_isTruthMatch);
   fChain->SetBranchAddress("ph_TruthConv", &ph_TruthConv);
   fChain->SetBranchAddress("ph_PDG", &ph_PDG);
   fChain->SetBranchAddress("ph_ClusterSize7x11Lr2", &ph_ClusterSize7x11Lr2);
   fChain->SetBranchAddress("ph_ClusterSize3x7Lr2", &ph_ClusterSize3x7Lr2);
   fChain->SetBranchAddress("ph_ClusterSize5x5Lr2", &ph_ClusterSize5x5Lr2);
   fChain->SetBranchAddress("ph_ClusterSize7x11Lr3", &ph_ClusterSize7x11Lr3);
   fChain->SetBranchAddress("ph_ClusterSize3x7Lr3", &ph_ClusterSize3x7Lr3);
   fChain->SetBranchAddress("ph_ClusterSize5x5Lr3", &ph_ClusterSize5x5Lr3);
   fChain->SetBranchAddress("ph_ClusterSize7x11Lr1", &ph_ClusterSize7x11Lr1);
   fChain->SetBranchAddress("ph_ClusterSize3x7Lr1", &ph_ClusterSize3x7Lr1);
   fChain->SetBranchAddress("ph_ClusterSize5x5Lr1", &ph_ClusterSize5x5Lr1);
   fChain->SetBranchAddress("ph_ClusterSize7x11PreS", &ph_ClusterSize7x11PreS);
   fChain->SetBranchAddress("ph_ClusterSize3x7PreS", &ph_ClusterSize3x7PreS);
   fChain->SetBranchAddress("ph_ClusterSize5x5PreS", &ph_ClusterSize5x5PreS);
   fChain->SetBranchAddress("ph_clusterCellsLr2E7x11", &ph_clusterCellsLr2E7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr2Eta7x11", &ph_clusterCellsLr2Eta7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr2Phi7x11", &ph_clusterCellsLr2Phi7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr2X7x11", &ph_clusterCellsLr2X7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr2Y7x11", &ph_clusterCellsLr2Y7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr2Z7x11", &ph_clusterCellsLr2Z7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr2T7x11", &ph_clusterCellsLr2T7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr2isBAD7x11", &ph_clusterCellsLr2isBAD7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr2E3x7", &ph_clusterCellsLr2E3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr2Eta3x7", &ph_clusterCellsLr2Eta3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr2Phi3x7", &ph_clusterCellsLr2Phi3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr2X3x7", &ph_clusterCellsLr2X3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr2Y3x7", &ph_clusterCellsLr2Y3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr2Z3x7", &ph_clusterCellsLr2Z3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr2T3x7", &ph_clusterCellsLr2T3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr2isBAD3x7", &ph_clusterCellsLr2isBAD3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr2E5x5", &ph_clusterCellsLr2E5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr2Eta5x5", &ph_clusterCellsLr2Eta5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr2Phi5x5", &ph_clusterCellsLr2Phi5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr2X5x5", &ph_clusterCellsLr2X5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr2Y5x5", &ph_clusterCellsLr2Y5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr2Z5x5", &ph_clusterCellsLr2Z5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr2T5x5", &ph_clusterCellsLr2T5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr2isBAD5x5", &ph_clusterCellsLr2isBAD5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr1E7x11", &ph_clusterCellsLr1E7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr1Eta7x11", &ph_clusterCellsLr1Eta7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr1Phi7x11", &ph_clusterCellsLr1Phi7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr1X7x11", &ph_clusterCellsLr1X7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr1Y7x11", &ph_clusterCellsLr1Y7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr1Z7x11", &ph_clusterCellsLr1Z7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr1T7x11", &ph_clusterCellsLr1T7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr1isBAD7x11", &ph_clusterCellsLr1isBAD7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr1E3x7", &ph_clusterCellsLr1E3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr1Eta3x7", &ph_clusterCellsLr1Eta3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr1Phi3x7", &ph_clusterCellsLr1Phi3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr1X3x7", &ph_clusterCellsLr1X3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr1Y3x7", &ph_clusterCellsLr1Y3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr1Z3x7", &ph_clusterCellsLr1Z3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr1T3x7", &ph_clusterCellsLr1T3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr1isBAD3x7", &ph_clusterCellsLr1isBAD3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr1E5x5", &ph_clusterCellsLr1E5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr1Eta5x5", &ph_clusterCellsLr1Eta5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr1Phi5x5", &ph_clusterCellsLr1Phi5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr1X5x5", &ph_clusterCellsLr1X5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr1Y5x5", &ph_clusterCellsLr1Y5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr1Z5x5", &ph_clusterCellsLr1Z5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr1T5x5", &ph_clusterCellsLr1T5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr1isBAD5x5", &ph_clusterCellsLr1isBAD5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr3E7x11", &ph_clusterCellsLr3E7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr3Eta7x11", &ph_clusterCellsLr3Eta7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr3Phi7x11", &ph_clusterCellsLr3Phi7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr3X7x11", &ph_clusterCellsLr3X7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr3Y7x11", &ph_clusterCellsLr3Y7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr3Z7x11", &ph_clusterCellsLr3Z7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr3T7x11", &ph_clusterCellsLr3T7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr3isBAD7x11", &ph_clusterCellsLr3isBAD7x11);
   fChain->SetBranchAddress("ph_clusterCellsLr3E3x7", &ph_clusterCellsLr3E3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr3Eta3x7", &ph_clusterCellsLr3Eta3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr3Phi3x7", &ph_clusterCellsLr3Phi3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr3X3x7", &ph_clusterCellsLr3X3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr3Y3x7", &ph_clusterCellsLr3Y3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr3Z3x7", &ph_clusterCellsLr3Z3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr3T3x7", &ph_clusterCellsLr3T3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr3isBAD3x7", &ph_clusterCellsLr3isBAD3x7);
   fChain->SetBranchAddress("ph_clusterCellsLr3E5x5", &ph_clusterCellsLr3E5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr3Eta5x5", &ph_clusterCellsLr3Eta5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr3Phi5x5", &ph_clusterCellsLr3Phi5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr3X5x5", &ph_clusterCellsLr3X5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr3Y5x5", &ph_clusterCellsLr3Y5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr3Z5x5", &ph_clusterCellsLr3Z5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr3T5x5", &ph_clusterCellsLr3T5x5);
   fChain->SetBranchAddress("ph_clusterCellsLr3isBAD5x5", &ph_clusterCellsLr3isBAD5x5);
   fChain->SetBranchAddress("ph_clusterCellsPreSE7x11", &ph_clusterCellsPreSE7x11);
   fChain->SetBranchAddress("ph_clusterCellsPreSEta7x11", &ph_clusterCellsPreSEta7x11);
   fChain->SetBranchAddress("ph_clusterCellsPreSPhi7x11", &ph_clusterCellsPreSPhi7x11);
   fChain->SetBranchAddress("ph_clusterCellsPreSX7x11", &ph_clusterCellsPreSX7x11);
   fChain->SetBranchAddress("ph_clusterCellsPreSY7x11", &ph_clusterCellsPreSY7x11);
   fChain->SetBranchAddress("ph_clusterCellsPreSZ7x11", &ph_clusterCellsPreSZ7x11);
   fChain->SetBranchAddress("ph_clusterCellsPreST7x11", &ph_clusterCellsPreST7x11);
   fChain->SetBranchAddress("ph_clusterCellsPreSisBAD7x11", &ph_clusterCellsPreSisBAD7x11);
   fChain->SetBranchAddress("ph_clusterCellsPreSE3x7", &ph_clusterCellsPreSE3x7);
   fChain->SetBranchAddress("ph_clusterCellsPreSEta3x7", &ph_clusterCellsPreSEta3x7);
   fChain->SetBranchAddress("ph_clusterCellsPreSPhi3x7", &ph_clusterCellsPreSPhi3x7);
   fChain->SetBranchAddress("ph_clusterCellsPreSX3x7", &ph_clusterCellsPreSX3x7);
   fChain->SetBranchAddress("ph_clusterCellsPreSY3x7", &ph_clusterCellsPreSY3x7);
   fChain->SetBranchAddress("ph_clusterCellsPreSZ3x7", &ph_clusterCellsPreSZ3x7);
   fChain->SetBranchAddress("ph_clusterCellsPreST3x7", &ph_clusterCellsPreST3x7);
   fChain->SetBranchAddress("ph_clusterCellsPreSisBAD3x7", &ph_clusterCellsPreSisBAD3x7);
   fChain->SetBranchAddress("ph_clusterCellsPreSE5x5", &ph_clusterCellsPreSE5x5);
   fChain->SetBranchAddress("ph_clusterCellsPreSEta5x5", &ph_clusterCellsPreSEta5x5);
   fChain->SetBranchAddress("ph_clusterCellsPreSPhi5x5", &ph_clusterCellsPreSPhi5x5);
   fChain->SetBranchAddress("ph_clusterCellsPreSX5x5", &ph_clusterCellsPreSX5x5);
   fChain->SetBranchAddress("ph_clusterCellsPreSY5x5", &ph_clusterCellsPreSY5x5);
   fChain->SetBranchAddress("ph_clusterCellsPreSZ5x5", &ph_clusterCellsPreSZ5x5);
   fChain->SetBranchAddress("ph_clusterCellsPreST5x5", &ph_clusterCellsPreST5x5);
   fChain->SetBranchAddress("ph_clusterCellsPreSisBAD5x5", &ph_clusterCellsPreSisBAD5x5);
   fChain->SetBranchAddress("ph_IsoLooseSF", &ph_IsoLooseSF);
   fChain->SetBranchAddress("ph_IsoTightSF", &ph_IsoTightSF);
   fChain->SetBranchAddress("ph_IsoTightCaloSF", &ph_IsoTightCaloSF);
   fChain->SetBranchAddress("pu_wgt", &pu_wgt);
   fChain->SetBranchAddress("evntN", &evntN);
   fChain->SetBranchAddress("mu", &mu);
   fChain->SetBranchAddress("mc_wgt", &mc_wgt);
   fChain->SetBranchAddress("mc_xsec", &mc_xsec);

   fChain->SetBranchAddress("ll.m", &ll_m);
   fChain->SetBranchAddress("llg.m", &llg_m);
}	

inline void LinkOutputBranches(TTree* fChain)
{
   
   fChain->Branch("pt", &ph_Pt);
   fChain->Branch("eta", &ph_Eta);
   fChain->Branch("phi", &ph_Phi);
   fChain->Branch("e", &ph_E);
   fChain->Branch("topoetcone20", &ph_topoetcone20);
   fChain->Branch("topoetcone30", &ph_topoetcone30);
   fChain->Branch("topoetcone40", &ph_topoetcone40);
   fChain->Branch("ptcone20", &ph_ptcone20);
   fChain->Branch("ptcone30", &ph_ptcone30);
   fChain->Branch("ptcone40", &ph_ptcone40);
   fChain->Branch("etcone20", &ph_etcone20);
   fChain->Branch("etcone30", &ph_etcone30);
   fChain->Branch("etcone40", &ph_etcone40);
   fChain->Branch("ptvarcone20", &ph_ptvarcone20);
   fChain->Branch("ptvarcone30", &ph_ptvarcone30);
   fChain->Branch("ptvarcone40", &ph_ptvarcone40);
   fChain->Branch("isIsoLooseWP", &ph_isIsoLooseWP);
   fChain->Branch("isIsoTightWP", &ph_isIsoTightWP);
   fChain->Branch("isIsoTightCaloOnlyWP", &ph_isIsoTightCaloOnlyWP);
   fChain->Branch("isEMLooseOff", &ph_isEMLooseOff);
   fChain->Branch("isEMTightIncOff", &ph_isEMTightIncOff);
   fChain->Branch("isEMTightDepOff", &ph_isEMTightDepOff);
   fChain->Branch("isEMLooseOn", &ph_isEMLooseOn);
   fChain->Branch("isEMTightOn", &ph_isEMTightOn);
   fChain->Branch("isEMMediumOn", &ph_isEMMediumOn);
   fChain->Branch("Conversion", &ph_Conversion);
   fChain->Branch("Ethad", &ph_Ethad);
   fChain->Branch("Ethad1", &ph_Ethad1);
   fChain->Branch("Rhad1", &ph_Rhad1);
   fChain->Branch("Rhad", &ph_Rhad);
   fChain->Branch("E011", &ph_E011);
   fChain->Branch("E132", &ph_E132);
   fChain->Branch("E237", &ph_E237);
   fChain->Branch("E277", &ph_E277);
   fChain->Branch("Reta", &ph_Reta);
   fChain->Branch("Rphi", &ph_Rphi);
   fChain->Branch("Weta1", &ph_Weta1);
   fChain->Branch("Weta2", &ph_Weta2);
   fChain->Branch("f1", &ph_f1);
   fChain->Branch("f3", &ph_f3);
   fChain->Branch("f3core", &ph_f3core);
   fChain->Branch("fracs1", &ph_fracs1);
   fChain->Branch("Wstot1", &ph_Wstot1);
   fChain->Branch("deltaE", &ph_deltaE);
   fChain->Branch("Eratio", &ph_Eratio);
   fChain->Branch("E2tsts1", &ph_E2tsts1);
   fChain->Branch("Emins1", &ph_Emins1);
   fChain->Branch("Emaxs1", &ph_Emaxs1);
   fChain->Branch("isTruthMatch", &ph_isTruthMatch);
   fChain->Branch("TruthConv", &ph_TruthConv);
   fChain->Branch("PDG", &ph_PDG);

   fChain->Branch("clusterCellsLr1E7x11", &ph_clusterCellsLr1E7x11);
   fChain->Branch("clusterCellsLr1Eta7x11", &ph_clusterCellsLr1Eta7x11);
   fChain->Branch("clusterCellsLr1Phi7x11", &ph_clusterCellsLr1Phi7x11);
   fChain->Branch("clusterCellsLr1X7x11", &ph_clusterCellsLr1X7x11);
   fChain->Branch("clusterCellsLr1Y7x11", &ph_clusterCellsLr1Y7x11);
   fChain->Branch("clusterCellsLr1Z7x11", &ph_clusterCellsLr1Z7x11);
   fChain->Branch("clusterCellsLr1T7x11", &ph_clusterCellsLr1T7x11);

   fChain->Branch("clusterCellsLr2E7x11", &ph_clusterCellsLr2E7x11);
   fChain->Branch("clusterCellsLr2Eta7x11", &ph_clusterCellsLr2Eta7x11);
   fChain->Branch("clusterCellsLr2Phi7x11", &ph_clusterCellsLr2Phi7x11);
   fChain->Branch("clusterCellsLr2X7x11", &ph_clusterCellsLr2X7x11);
   fChain->Branch("clusterCellsLr2Y7x11", &ph_clusterCellsLr2Y7x11);
   fChain->Branch("clusterCellsLr2Z7x11", &ph_clusterCellsLr2Z7x11);
   fChain->Branch("clusterCellsLr2T7x11", &ph_clusterCellsLr2T7x11);

   fChain->Branch("clusterCellsLr3E7x11", &ph_clusterCellsLr3E7x11);
   fChain->Branch("clusterCellsLr3Eta7x11", &ph_clusterCellsLr3Eta7x11);
   fChain->Branch("clusterCellsLr3Phi7x11", &ph_clusterCellsLr3Phi7x11);
   fChain->Branch("clusterCellsLr3X7x11", &ph_clusterCellsLr3X7x11);
   fChain->Branch("clusterCellsLr3Y7x11", &ph_clusterCellsLr3Y7x11);
   fChain->Branch("clusterCellsLr3Z7x11", &ph_clusterCellsLr3Z7x11);
   fChain->Branch("clusterCellsLr3T7x11", &ph_clusterCellsLr3T7x11);

   fChain->Branch("clusterCellsLr2E3x7", &ph_clusterCellsLr2E3x7);
   fChain->Branch("clusterCellsLr2Eta3x7", &ph_clusterCellsLr2Eta3x7);
   fChain->Branch("clusterCellsLr2Phi3x7", &ph_clusterCellsLr2Phi3x7);
   fChain->Branch("clusterCellsLr2X3x7", &ph_clusterCellsLr2X3x7);
   fChain->Branch("clusterCellsLr2Y3x7", &ph_clusterCellsLr2Y3x7);
   fChain->Branch("clusterCellsLr2Z3x7", &ph_clusterCellsLr2Z3x7);
   fChain->Branch("clusterCellsLr2T3x7", &ph_clusterCellsLr2T3x7);

   fChain->Branch("clusterCellsLr2E5x5", &ph_clusterCellsLr2E5x5);
   fChain->Branch("clusterCellsLr2Eta5x5", &ph_clusterCellsLr2Eta5x5);
   fChain->Branch("clusterCellsLr2Phi5x5", &ph_clusterCellsLr2Phi5x5);
   fChain->Branch("clusterCellsLr2X5x5", &ph_clusterCellsLr2X5x5);
   fChain->Branch("clusterCellsLr2Y5x5", &ph_clusterCellsLr2Y5x5);
   fChain->Branch("clusterCellsLr2Z5x5", &ph_clusterCellsLr2Z5x5);
   fChain->Branch("clusterCellsLr2T5x5", &ph_clusterCellsLr2T5x5);

   fChain->Branch("clusterCellsLr1E3x7", &ph_clusterCellsLr1E3x7);
   fChain->Branch("clusterCellsLr1Eta3x7", &ph_clusterCellsLr1Eta3x7);
   fChain->Branch("clusterCellsLr1Phi3x7", &ph_clusterCellsLr1Phi3x7);
   fChain->Branch("clusterCellsLr1X3x7", &ph_clusterCellsLr1X3x7);
   fChain->Branch("clusterCellsLr1Y3x7", &ph_clusterCellsLr1Y3x7);
   fChain->Branch("clusterCellsLr1Z3x7", &ph_clusterCellsLr1Z3x7);
   fChain->Branch("clusterCellsLr1T3x7", &ph_clusterCellsLr1T3x7);

   fChain->Branch("clusterCellsLr1E5x5", &ph_clusterCellsLr1E5x5);
   fChain->Branch("clusterCellsLr1Eta5x5", &ph_clusterCellsLr1Eta5x5);
   fChain->Branch("clusterCellsLr1Phi5x5", &ph_clusterCellsLr1Phi5x5);
   fChain->Branch("clusterCellsLr1X5x5", &ph_clusterCellsLr1X5x5);
   fChain->Branch("clusterCellsLr1Y5x5", &ph_clusterCellsLr1Y5x5);
   fChain->Branch("clusterCellsLr1Z5x5", &ph_clusterCellsLr1Z5x5);
   fChain->Branch("clusterCellsLr1T5x5", &ph_clusterCellsLr1T5x5);

   fChain->Branch("clusterCellsLr3E3x7", &ph_clusterCellsLr3E3x7);
   fChain->Branch("clusterCellsLr3Eta3x7", &ph_clusterCellsLr3Eta3x7);
   fChain->Branch("clusterCellsLr3Phi3x7", &ph_clusterCellsLr3Phi3x7);
   fChain->Branch("clusterCellsLr3X3x7", &ph_clusterCellsLr3X3x7);
   fChain->Branch("clusterCellsLr3Y3x7", &ph_clusterCellsLr3Y3x7);
   fChain->Branch("clusterCellsLr3Z3x7", &ph_clusterCellsLr3Z3x7);
   fChain->Branch("clusterCellsLr3T3x7", &ph_clusterCellsLr3T3x7);

   fChain->Branch("clusterCellsLr3E5x5", &ph_clusterCellsLr3E5x5);
   fChain->Branch("clusterCellsLr3Eta5x5", &ph_clusterCellsLr3Eta5x5);
   fChain->Branch("clusterCellsLr3Phi5x5", &ph_clusterCellsLr3Phi5x5);
   fChain->Branch("clusterCellsLr3X5x5", &ph_clusterCellsLr3X5x5);
   fChain->Branch("clusterCellsLr3Y5x5", &ph_clusterCellsLr3Y5x5);
   fChain->Branch("clusterCellsLr3Z5x5", &ph_clusterCellsLr3Z5x5);
   fChain->Branch("clusterCellsLr3T5x5", &ph_clusterCellsLr3T5x5);

   fChain->Branch("Lr1SumE7x11", &m_Lr1SumE7x11);
   fChain->Branch("Lr2SumE7x11", &m_Lr2SumE7x11);
   fChain->Branch("Lr3SumE7x11", &m_Lr3SumE7x11);
   fChain->Branch("LrPreSSumE7x11", &m_LrPreSSumE7x11);

   fChain->Branch("Lr1SumE5x5", &m_Lr1SumE5x5);
   fChain->Branch("Lr2SumE5x5", &m_Lr2SumE5x5);
   fChain->Branch("Lr3SumE5x5", &m_Lr3SumE5x5);
   fChain->Branch("LrPreSSumE5x5", &m_LrPreSSumE5x5);

   fChain->Branch("Lr1SumE3x7", &m_Lr1SumE3x7);
   fChain->Branch("Lr2SumE3x7", &m_Lr2SumE3x7);
   fChain->Branch("Lr3SumE3x7", &m_Lr3SumE3x7);
   fChain->Branch("LrPreSSumE3x7", &m_LrPreSSumE3x7);


   /*
   fChain->Branch("ph_clusterCellsPreSE7x11", &ph_clusterCellsPreSE7x11);
   fChain->Branch("ph_clusterCellsPreSEta7x11", &ph_clusterCellsPreSEta7x11);
   fChain->Branch("ph_clusterCellsPreSPhi7x11", &ph_clusterCellsPreSPhi7x11);
   fChain->Branch("ph_clusterCellsPreSX7x11", &ph_clusterCellsPreSX7x11);
   fChain->Branch("ph_clusterCellsPreSY7x11", &ph_clusterCellsPreSY7x11);
   fChain->Branch("ph_clusterCellsPreSZ7x11", &ph_clusterCellsPreSZ7x11);
   fChain->Branch("ph_clusterCellsPreST7x11", &ph_clusterCellsPreST7x11);
   fChain->Branch("ph_clusterCellsPreSisBAD7x11", &ph_clusterCellsPreSisBAD7x11);
   fChain->Branch("ph_clusterCellsPreSE3x7", &ph_clusterCellsPreSE3x7);
   fChain->Branch("ph_clusterCellsPreSEta3x7", &ph_clusterCellsPreSEta3x7);
   fChain->Branch("ph_clusterCellsPreSPhi3x7", &ph_clusterCellsPreSPhi3x7);
   fChain->Branch("ph_clusterCellsPreSX3x7", &ph_clusterCellsPreSX3x7);
   fChain->Branch("ph_clusterCellsPreSY3x7", &ph_clusterCellsPreSY3x7);
   fChain->Branch("ph_clusterCellsPreSZ3x7", &ph_clusterCellsPreSZ3x7);
   fChain->Branch("ph_clusterCellsPreST3x7", &ph_clusterCellsPreST3x7);
   fChain->Branch("ph_clusterCellsPreSisBAD3x7", &ph_clusterCellsPreSisBAD3x7);
   fChain->Branch("ph_clusterCellsPreSE5x5", &ph_clusterCellsPreSE5x5);
   fChain->Branch("ph_clusterCellsPreSEta5x5", &ph_clusterCellsPreSEta5x5);
   fChain->Branch("ph_clusterCellsPreSPhi5x5", &ph_clusterCellsPreSPhi5x5);
   fChain->Branch("ph_clusterCellsPreSX5x5", &ph_clusterCellsPreSX5x5);
   fChain->Branch("ph_clusterCellsPreSY5x5", &ph_clusterCellsPreSY5x5);
   fChain->Branch("ph_clusterCellsPreSZ5x5", &ph_clusterCellsPreSZ5x5);
   fChain->Branch("ph_clusterCellsPreST5x5", &ph_clusterCellsPreST5x5);
   fChain->Branch("ph_clusterCellsPreSisBAD5x5", &ph_clusterCellsPreSisBAD5x5);
   */

   fChain->Branch("IsoLooseSF", &ph_IsoLooseSF);
   fChain->Branch("IsoTightSF", &ph_IsoTightSF);
   fChain->Branch("IsoTightCaloSF", &ph_IsoTightCaloSF);
   fChain->Branch("puwgt", &pu_wgt);
   fChain->Branch("evntN", &evntN);
   fChain->Branch("mu", &mu);
   fChain->Branch("mcwgt", &mc_wgt);
   fChain->Branch("mcxsec", &mc_xsec);
}	

};

#endif //> !CleanAndGet_H
