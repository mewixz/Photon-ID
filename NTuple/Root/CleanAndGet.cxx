#include "CleanAndGet/CleanAndGet.h"

CleanAndGet::CleanAndGet(TString dirname,TString dirout,Int_t stg, Int_t i, Int_t n, TString name){

	m_DirName  = dirname;
	m_DirOut   = dirout;
	m_Strategy = stg;
	m_i        = i;
	m_n        = n;
	m_name     = name;
}

CleanAndGet::~CleanAndGet() {

}

bool CleanAndGet::initilize() {

	if(m_name != "Zlly")
	{
	if(m_n != 0)
	{
		_output_file  = new TFile (m_DirOut+Form("/output_Strategy_IsoTight_%i_%s_%i.root",m_Strategy,m_name.Data(),m_n),"RECREATE");
	}else{
		_output_file  = new TFile (m_DirOut+Form("/output_Strategy_IsoTight_%i_%s.root",m_Strategy,m_name.Data()),"RECREATE");
	}
	}

	if(m_name == "Zlly")
	{
		_output_file  = new TFile (m_DirOut+Form("/output_Strategy_IsoLoose_%i_%s.root",m_Strategy,m_name.Data()),"RECREATE");
	}
	_output1_tree = new TTree("Tree_Train","Output for DNN");
	_output2_tree = new TTree("Tree_Val",  "Output for DNN");
	_output3_tree = new TTree("Tree_Test", "Output for DNN");
	_output4_tree = new TTree("Tree",      "Output for DNN");
	
	LinkOutputBranches(_output1_tree);
	LinkOutputBranches(_output2_tree);
	LinkOutputBranches(_output3_tree);
	LinkOutputBranches(_output4_tree);

	std::vector<TString> Samples = Load(m_DirName);
	TChain*	chain =	GetChain(Samples,"NTUP");
	LinkInputBranches(chain);
	Long64_t nentries = chain->GetEntriesFast();
	std::cout<<"Total Number of Entries : "<<nentries<<std::endl;
	std::cout<<"Total Number will be processed : "<<m_n<<std::endl;

	return true;
}

bool CleanAndGet::finilize() {

	_output1_tree->AutoSave();
	_output2_tree->AutoSave();
	_output3_tree->AutoSave();
	_output4_tree->AutoSave();

	_output_file->Close();
	return true;

}

std::vector<TString> CleanAndGet::Load(TString directory) {

	std::vector<TString> Samples;
	TSystemDirectory dir(directory, directory); 
	TList *files = dir.GetListOfFiles(); 
	if (files) { 
		TSystemFile *file; 
		TString fname; 
		TIter next(files); 
		while ((file=(TSystemFile*)next())) { 
			fname = file->GetName(); 
			if (!file->IsDirectory() && fname.EndsWith(".root")) { 
				Samples.push_back(directory+"/"+fname); 
			} 
		} 
	}
	return Samples;
}

TChain* CleanAndGet::GetChain(std::vector<TString> Samples, TString TreeName)
{
	chain = new TChain(TreeName);
	std::cout<<"Samples To Read "<<std::endl;
	for (unsigned int i = 0; i<Samples.size(); i++)
	{
		std::cout<<Samples[i]<<std::endl;
		chain->AddFile(Samples[i],-1,TreeName);
	}
	std::cout<<"Trees are loaded"<<std::endl;
	return chain;
}

bool CleanAndGet::runCleanAndGet()
{
	Long64_t nentries = chain->GetEntriesFast();
	if(m_n == 0) m_n = nentries;
	for(int jentry = m_i; jentry<m_n; jentry++)
	{
		if(jentry%10000 == 0) std::cout<<" Events : "<<jentry<<"/"<<m_n<<std::endl;
		Int_t nb = chain->GetEntry(jentry);
		nb++;

		//if(!ph_isIsoTightWP) continue;

		if(!ph_isIsoLooseWP) continue;
		
		bool ifcontinue = false;
		if(m_Strategy == 0) ifcontinue = Strat0();
		if(m_Strategy == 1) ifcontinue = Strat1();
		if(m_Strategy == 2) ifcontinue = Strat2();
		if(m_Strategy == 3) ifcontinue = Strat3();
		if(m_Strategy == 4) ifcontinue = Strat4();	
		if(!ifcontinue) continue;
		
		if(m_Strategy == 3) CompletCluster();
		if(m_Strategy == 1) CompletCluster();
		if(m_Strategy == 2) CompletCluster();
		if(m_Strategy == 4) CompletCluster();

		int req = -99;

		if(m_name.Contains("DiJet") )
		{
				req = 0;
			
		}else if(m_name.Contains("GammaJet"))
		{
				req = 1;
		} else if(m_name.Contains("Zlly"))
		{
			goto passTruth;
		}

		if(ph_isTruthMatch != req) continue;

		passTruth:

		//std::cout<<ph_isTruthMatch<<std::endl;

		Normalize();

		_output4_tree->Fill();
		if(evntN%4 == 0 || evntN%4 == 1){
			_output1_tree->Fill();
		}else if(evntN%4 == 2){
			_output2_tree->Fill();
		}else if(evntN%4 == 3){
			_output3_tree->Fill();
		}
	}

	return true;
}

bool CleanAndGet::Strat0()
{
	if(ph_clusterCellsLr2E7x11->size() != 77 or ph_clusterCellsLr1E7x11->size() != 112 or ph_clusterCellsLr3E7x11->size() != 44) {return false;}
	return true;
}	

bool CleanAndGet::Strat1()
{
	if(ph_clusterCellsLr2E7x11->size() == 77 and ph_clusterCellsLr3E7x11->size() == 44 and ((ph_clusterCellsLr1E7x11->size() == 112 and fabs(ph_Eta) <1.8) or (ph_clusterCellsLr1E7x11->size() == 84 and fabs(ph_Eta) > 1.8 and fabs(ph_Eta) <= 2.0)  or (ph_clusterCellsLr1E7x11->size() == 56 and fabs(ph_Eta) > 2.0 and fabs(ph_Eta) < 2.5) )) {return true;}
	return false;
}

bool CleanAndGet::Strat2()
{
	//if(fabs(ph_Eta) > 2.5 or ph_clusterCellsLr3E7x11->size() == 0 or ph_clusterCellsLr2E7x11->size() == 0 or ph_clusterCellsLr1E7x11->size() == 0) {return false;}
	if(fabs(ph_Eta) > 2.5 or Strat1() ) {return false;}
	return true;
}

bool CleanAndGet::Strat3()
{
	
	if( fabs(ph_Eta) > 1.8 or Strat0() ) {return false;}
	return true;
}

bool CleanAndGet::Strat4()
{
	if( fabs(ph_Eta) < 2.5) { return true;}
	return false;
}

void CleanAndGet::Normalize()
{
	
	m_Lr2SumE7x11 = SumCluster(ph_clusterCellsLr2E7x11);
	m_Lr1SumE7x11 = SumCluster(ph_clusterCellsLr1E7x11);
	m_Lr3SumE7x11 = SumCluster(ph_clusterCellsLr3E7x11);
	
	m_Lr2SumE5x5 = SumCluster(ph_clusterCellsLr2E5x5);
	m_Lr1SumE5x5 = SumCluster(ph_clusterCellsLr1E5x5);
	m_Lr3SumE5x5 = SumCluster(ph_clusterCellsLr3E5x5);

	m_Lr2SumE3x7 = SumCluster(ph_clusterCellsLr2E3x7);
	m_Lr1SumE3x7 = SumCluster(ph_clusterCellsLr1E3x7);
	m_Lr3SumE3x7 = SumCluster(ph_clusterCellsLr3E3x7);

	m_LrPreSSumE7x11 = SumCluster(ph_clusterCellsPreSE7x11);
	m_LrPreSSumE3x7 = SumCluster(ph_clusterCellsPreSE3x7);
	m_LrPreSSumE5x5 = SumCluster(ph_clusterCellsPreSE5x5);

/* **** */
	/*
	NormalizeCluster(ph_clusterCellsLr2E7x11, SumCluster(ph_clusterCellsLr2E7x11));
	NormalizeCluster(ph_clusterCellsLr1E7x11, SumCluster(ph_clusterCellsLr1E7x11));
	NormalizeCluster(ph_clusterCellsLr3E7x11, SumCluster(ph_clusterCellsLr3E7x11));
	
	NormalizeCluster(ph_clusterCellsLr2E5x5, SumCluster(ph_clusterCellsLr2E5x5));
	NormalizeCluster(ph_clusterCellsLr1E5x5, SumCluster(ph_clusterCellsLr1E5x5));
	NormalizeCluster(ph_clusterCellsLr3E5x5, SumCluster(ph_clusterCellsLr3E5x5));

	NormalizeCluster(ph_clusterCellsLr2E3x7, SumCluster(ph_clusterCellsLr2E3x7));
	NormalizeCluster(ph_clusterCellsLr1E3x7, SumCluster(ph_clusterCellsLr1E3x7));
	NormalizeCluster(ph_clusterCellsLr3E3x7, SumCluster(ph_clusterCellsLr3E3x7));

	NormalizeCluster(ph_clusterCellsPreSE7x11, SumCluster(ph_clusterCellsPreSE7x11));
	NormalizeCluster(ph_clusterCellsPreSE3x7, SumCluster(ph_clusterCellsPreSE3x7));
	NormalizeCluster(ph_clusterCellsPreSE5x5, SumCluster(ph_clusterCellsPreSE5x5));
	*/
	
}

void CleanAndGet::NormalizeCluster(vector<float> *&cluster, float sumE)
{
	for(unsigned int i = 0; i<cluster->size(); i++) cluster->at(i) /= sumE; 
}

float CleanAndGet::SumCluster(vector<float> *cluster)
{
	float sumE = 0;
	for(unsigned int i = 0; i<cluster->size(); i++) sumE += cluster->at(i);
	return sumE;
}

void CleanAndGet::CompletCluster()
{
	
	int Lr2_size = ph_clusterCellsLr2E7x11->size(); 
	int Lr3_size = ph_clusterCellsLr3E7x11->size();
	int Lr1_size = ph_clusterCellsLr1E7x11->size();
	
	if (Lr2_size < 77)
	{
		for(int i = Lr2_size; i < 77; i++) {ph_clusterCellsLr2E7x11->push_back(0);}
	}else if(Lr2_size > 77)
	{
		while(ph_clusterCellsLr2E7x11->size() > 77) {ph_clusterCellsLr2E7x11->pop_back();}
	}
	
	if (Lr1_size < 112)
	{
		for(int i = Lr1_size; i < 112; i++) {ph_clusterCellsLr1E7x11->push_back(0);}
	}else if(Lr1_size > 112)
	{
		while(ph_clusterCellsLr1E7x11->size() > 112) {ph_clusterCellsLr1E7x11->pop_back();}
	}

	if (Lr3_size < 44)
	{
		for(int i = Lr3_size; i < 44; i++) {ph_clusterCellsLr3E7x11->push_back(0);}
	}else if(Lr3_size > 44)
	{
		while(ph_clusterCellsLr3E7x11->size() > 44) {ph_clusterCellsLr3E7x11->pop_back();}
	}
	
}
