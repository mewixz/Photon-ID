/*
  ZllyAthDerivation to access cells and select Zlly events
  @autor : Mohamed BELFKIR
  @email : mohamed.belfkir@cern.ch
  @date  : 08-03-2019 
  Copyright (C) 2019 CERN for the benefit of the ATLAS collaboration
*/
#include <AsgTools/MessageCheck.h>
#include <MyAnalysis/MyxAODAnalysis.h>
#include <MyAnalysis/MCLumi.h>
#include <xAODEventInfo/EventInfo.h>


MyxAODAnalysis :: MyxAODAnalysis (const std::string& name,
                                  ISvcLocator *pSvcLocator)
    : EL::AnaAlgorithm (name, pSvcLocator)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  This is also where you
  // declare all properties for your algorithm.  Note that things like
  // resetting statistics variables or booking histograms should
  // rather go into the initialize() function.
}



StatusCode MyxAODAnalysis :: initialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
	
	int Year = 2017;
	isAOD = true;
        retrieveCells = true;	
	DSID = 423300;
	doSinglePhoton = true;

	if(Year == 2015)
	{
		fullGRLFilePath = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data15_13TeV/20170619/data15_13TeV.periodAllYear_DetStatus-v89-pro21-02_Unknown_PHYS_StandardGRL_All_Good_25ns.xml";	
		triggerList = {"HLT_mu20_iloose_L1MU15_OR_HLT_mu50",
                            "HLT_mu20_iloose_L1MU15",
                            "HLT_mu50",
                            "HLT_mu60_0eta105_msonly",
                            "HLT_2mu10",
                            "HLT_e24_lhmedium_L1EM18VH",
                            "HLT_e24_lhmedium_iloose_L1EM18VH",
                            "HLT_e24_lhmedium_iloose_L1EM20VH",
                            "HLT_e24_lhmedium_L1EM20VH",
                            "HLT_e60_lhmedium",
                            "HLT_e120_lhloose",
                            "HLT_2e12_lhloose_L12EM10VH"  };
	}
	if(Year == 2016)
	{
		fullGRLFilePath = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data16_13TeV/20180129/data16_13TeV.periodAllYear_DetStatus-v89-pro21-01_DQDefects-00-02-04_PHYS_StandardGRL_All_Good_25ns.xml";
		triggerList = {"HLT_mu26_ivarmedium_OR_HLT_mu50",
                            "HLT_mu26_ivarmedium",
                            "HLT_mu50",
                            "HLT_mu60_0eta105_msonly",
                            "HLT_2mu14",
                            "HLT_mu20_mu8noL1",
                            "HLT_2mu14_nomucomb",
                            "HLT_e24_lhtight_nod0_ivarloose",
                            "HLT_e26_lhtight_nod0_ivarloose",
                            "HLT_e60_lhmedium_nod0",
                            "HLT_e140_lhloose_nod0",
                            "HLT_2e15_lhvloose_nod0_L12EM13VH",
                            "HLT_2e17_lhvloose_nod0"};
	}
	if(Year == 2017)
	{
		fullGRLFilePath = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data17_13TeV/20180619/data17_13TeV.periodAllYear_DetStatus-v99-pro22-01_Unknown_PHYS_StandardGRL_All_Good_25ns_Triggerno17e33prim.xml";
		triggerList = {"HLT_mu26_imedium",
                            "HLT_mu26_ivarmedium_OR_HLT_mu50",
                            "HLT_mu26_ivarmedium",
                            "HLT_mu50",
                            "HLT_mu50_0eta105_msonly",
                            "HLT_2mu14",
                            "HLT_mu22_mu8noL1",
                            "HLT_e26_lhtight_nod0_ivarloose",
                            "HLT_e60_lhmedium_nod0",
                            "HLT_e140_lhloose_nod0",
                            "HLT_2e24_lhvloose_nod0"};
	}

	if(Year == 2018)
	{
		fullGRLFilePath = "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data18_13TeV/20190318/data18_13TeV.periodAllYear_DetStatus-v102-pro22-04_Unknown_PHYS_StandardGRL_All_Good_25ns_Triggerno17e33prim.xml";
		triggerList = {"HLT_mu26_imedium",
                            "HLT_mu26_ivarmedium_OR_HLT_mu50",
                            "HLT_mu26_ivarmedium",
                            "HLT_mu50",
                            "HLT_mu50_0eta105_msonly",
                            "HLT_2mu14",
                            "HLT_mu22_mu8noL1",
                            "HLT_e26_lhtight_nod0_ivarloose",
                            "HLT_e26_lhtight_nod0",
                            "HLT_e60_lhmedium_nod0",
                            "HLT_e140_lhloose_nod0",
                            "HLT_2e17_lhvloose_nod0_L12EM15VHI",
                            "HLT_2e24_lhvloose_nod0"};
	}
	
// Good Run Lists

    GRL_tool.setTypeAndName("GoodRunsListSelectionTool/GRLSelection");
  	
  	std::vector<std::string> vecStringGRL;
  	vecStringGRL.push_back(fullGRLFilePath);
  	ANA_CHECK(GRL_tool.setProperty( "GoodRunsListVec", vecStringGRL));
  	ANA_CHECK(GRL_tool.setProperty("PassThrough", false));
  	ANA_CHECK(GRL_tool.retrieve());

// PileUp Tool 

    PileUp_tool.setTypeAndName("CP::PileupReweightingTool/PileUp_tool");
	std::vector<std::string> LumicalcFiles;
      if(Year == 2018)    LumicalcFiles.push_back("/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data18_13TeV/20190318/ilumicalc_histograms_None_348885-364292_OflLumi-13TeV-010.root");
      if(Year == 2017)    LumicalcFiles.push_back("/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data17_13TeV/20180309/physics_25ns_Triggerno17e33prim.lumicalc.OflLumi-13TeV-010.root");
      if(Year == 2016)    LumicalcFiles.push_back("/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data16_13TeV/20180129/PHYS_StandardGRL_All_Good_25ns_297730-311481_OflLumi-13TeV-009.root");
      if(Year == 2015)    LumicalcFiles.push_back("/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/GoodRunsLists/data15_13TeV/20170619/PHYS_StandardGRL_All_Good_25ns_276262-284484_OflLumi-13TeV-008.root");

	std::vector<std::string> pileup_file;
	if(DSID == 423099) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e4453_e5984_a875_r10201_r10210.root",DSID));
	if(DSID == 423100) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3791_e5984_a875_r10201_r10210.root",DSID));
	if(DSID == 423101) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3904_e5984_a875_r10201_r10210.root",DSID));
	if(DSID == 423102) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3791_e5984_a875_r10201_r10210.root",DSID));
	if(DSID == 423103) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3791_e5984_a875_r10201_r10210.root",DSID));
	if(DSID == 423104) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3791_e5984_a875_r10201_r10210.root",DSID));

	if(DSID == 423300) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3848_s3126_r10201_r10210_p3384_p3385.root",DSID));
	if(DSID == 423301) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3848_s3126_r10201_r10210_p3384_p3385.root",DSID));
	if(DSID == 423302) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3848_s3126_r10201_r10210_p3384_p3385.root",DSID));
	if(DSID == 423303) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3848_s3126_r10201_r10210_p3384_p3385.root",DSID));

	if(Year == 2017 && (DSID != 423099 && DSID != 423100 && DSID != 423101 && DSID != 423102 && DSID != 423103 && DSID != 423104 && DSID != 423300 && DSID != 423301 && DSID != 423302 && DSID != 423303)) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3952_s3126_r10201_r10210.root",DSID)); //mc16d -> 2017
	if(Year == 2018 && (DSID != 423099 && DSID != 423100 && DSID != 423101 && DSID != 423102 && DSID != 423103 && DSID != 423104 && DSID != 423300 && DSID != 423301 && DSID != 423302 && DSID != 423303)) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3952_s3126_r9781_r9778.root",DSID)); //mc16e -> 2018 is not availble need to be updated
	if((Year == 2015 or Year == 2016) && (DSID != 423099 && DSID != 423100 && DSID != 423101 && DSID != 423102 && DSID != 423103 && DSID != 423104 && DSID != 423300 && DSID != 423301 && DSID != 423302 && DSID != 423303)) pileup_file.push_back(Form("MyAnalysis/PRW/mc16_13TeV.%d.merge.PRW.e3952_s3126_r9364_r9315.root",DSID)); //mc16a -> 2015+2016
	
	
	ANA_CHECK(PileUp_tool.setProperty("ConfigFiles",pileup_file));
	ANA_CHECK(PileUp_tool.setProperty("LumiCalcFiles",LumicalcFiles));
	ANA_CHECK(PileUp_tool.initialize());

// Trigger Tool 
    Trigger_dic_tool.setTypeAndName("Trig::TrigDecisionTool/TrigDecisionTool");
	ANA_CHECK(Trigger_dic_tool.retrieve());

   Trigger_match_tool.setTypeAndName("Trig::MatchingTool/Trigger_match_tool");
  	ANA_CHECK(Trigger_match_tool.retrieve()); 


// Electron Isolation 

    Electron_iso_tool = new CP::IsolationSelectionTool("Electron_iso_tool");
	ANA_CHECK( Electron_iso_tool->setProperty("ElectronWP","GradientLoose"));
	ANA_CHECK( Electron_iso_tool->initialize());

// Electron Identification 

   Electron_id_tool = new AsgElectronLikelihoodTool ("Electron_id_tool");
	ANA_CHECK( Electron_id_tool->setProperty("WorkingPoint", "MediumLHElectron"));
	ANA_CHECK( Electron_id_tool->setProperty("ConfigFile", "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/ElectronPhotonSelectorTools/offline/mc16_20170828/ElectronLikelihoodMediumOfflineConfig2017_Smooth.conf"));
	ANA_CHECK( Electron_id_tool->initialize());

// Electron Reco Efficiency 

   Electron_RecoSF_tool = new AsgElectronEfficiencyCorrectionTool("Electron_RecoSF_tool");
   	ANA_CHECK( Electron_RecoSF_tool->setProperty("MapFilePath", "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/ElectronEfficiencyCorrection/2015_2017/rel21.2/Consolidation_September2018_v1/map0.txt"));
   	ANA_CHECK( Electron_RecoSF_tool->setProperty("RecoKey", "Reconstruction"));;
  	ANA_CHECK( Electron_RecoSF_tool->setProperty("ForceDataType",1));
   	ANA_CHECK( Electron_RecoSF_tool->initialize());

// Electron Iso Efficiency 

   Electron_IsoSF_tool	= new AsgElectronEfficiencyCorrectionTool("Electron_IsoSF_tool");
   	ANA_CHECK( Electron_IsoSF_tool->setProperty("MapFilePath", "/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/ElectronEfficiencyCorrection/2015_2017/rel21.2/Consolidation_September2018_v1/map0.txt"));
   	ANA_CHECK( Electron_IsoSF_tool->setProperty("IdKey", "Medium"));
   	ANA_CHECK( Electron_IsoSF_tool->setProperty("ForceDataType",1));
   	ANA_CHECK( Electron_IsoSF_tool->initialize());


// Egamma Calibration

    Egamma_calib_tool = new CP::EgammaCalibrationAndSmearingTool("Egamma_Calib_tool");
    	ANA_CHECK( Egamma_calib_tool->setProperty("ESModel", "es2017_R21_v1"));
  	ANA_CHECK( Egamma_calib_tool->setProperty("decorrelationModel", "FULL_v1"));
 // if (m_isFastMC) CHECK("SetupEgammaTools()", m_egammaCalibrationAndSmearingTool->setProperty("useAFII", true));
  	ANA_CHECK( Egamma_calib_tool->initialize());

// Egamma Embguity Tool 

    m_fakeTool = new EGammaAmbiguityTool("EGamma_Ambiguity_Tool");
	ANA_CHECK( m_fakeTool->initialize());

//

    m_deadHVTool.setTypeAndName("AsgDeadHVCellRemovalTool/deadHVTool");
	ANA_CHECK( m_deadHVTool.retrieve());


//Photon identification 

	//Offline
	// Loose
   Photon_Off_isEMLoose_tool = new AsgPhotonIsEMSelector( "Photon_Off_isEMLoose_tool" );
	ANA_CHECK(Photon_Off_isEMLoose_tool->setProperty("isEMMask",egammaPID::PhotonLoose));
	ANA_CHECK(Photon_Off_isEMLoose_tool->setProperty("ConfigFile","/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/ElectronPhotonSelectorTools/offline/mc15_20150712/PhotonIsEMLooseSelectorCutDefs.conf"));
	ANA_CHECK(Photon_Off_isEMLoose_tool->initialize());

	//Tight Inc
   Photon_Off_isEMTightInc_tool = new AsgPhotonIsEMSelector( "Photon_Off_isEMTightInc_tool" );
	ANA_CHECK(Photon_Off_isEMTightInc_tool->setProperty("isEMMask",egammaPID::PhotonTight));
	ANA_CHECK(Photon_Off_isEMTightInc_tool->setProperty("ConfigFile","/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/ElectronPhotonSelectorTools/offline/20180116/PhotonIsEMTightSelectorCutDefs.conf"));
	ANA_CHECK(Photon_Off_isEMTightInc_tool->initialize());

	//Tight pT dep
   Photon_Off_isEMTightDep_tool = new AsgPhotonIsEMSelector( "Photon_Off_isEMTightDep_tool" );
	ANA_CHECK(Photon_Off_isEMTightDep_tool->setProperty("isEMMask",egammaPID::PhotonTight));
	ANA_CHECK(Photon_Off_isEMTightDep_tool->setProperty("ConfigFile","/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/ElectronPhotonSelectorTools/offline/20180825/PhotonIsEMTightSelectorCutDefs.conf"));
	ANA_CHECK(Photon_Off_isEMTightDep_tool->initialize());

	//Online 
	//Loose

   Photon_On_isEMLoose_tool = new AsgPhotonIsEMSelector( "Photon_On_isEMLoose_tool" );
	ANA_CHECK(Photon_On_isEMLoose_tool->setProperty("isEMMask",egammaPID::PhotonLoose));
	ANA_CHECK(Photon_On_isEMLoose_tool->setProperty("ConfigFile","/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/ElectronPhotonSelectorTools/trigger/rel21_20180312/PhotonIsEMLooseSelectorCutDefs.conf"));
	ANA_CHECK(Photon_On_isEMLoose_tool->initialize());

	//Tight

   Photon_On_isEMTight_tool = new AsgPhotonIsEMSelector( "Photon_On_isEMTight_tool" );
	ANA_CHECK(Photon_On_isEMTight_tool->setProperty("isEMMask",egammaPID::PhotonTight));
	ANA_CHECK(Photon_On_isEMTight_tool->setProperty("ConfigFile","/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/ElectronPhotonSelectorTools/trigger/rel21_20180312/PhotonIsEMTightSelectorCutDefs.conf"));
	ANA_CHECK(Photon_On_isEMTight_tool->initialize());

	//Medium

   Photon_On_isEMMedium_tool = new AsgPhotonIsEMSelector( "Photon_On_isEMMedium_tool" );
	ANA_CHECK(Photon_On_isEMMedium_tool->setProperty("isEMMask",egammaPID::PhotonMedium));
	ANA_CHECK(Photon_On_isEMMedium_tool->setProperty("ConfigFile","/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/ElectronPhotonSelectorTools/trigger/rel21_20180312/PhotonIsEMMediumSelectorCutDefs.conf"));
	ANA_CHECK(Photon_On_isEMMedium_tool->initialize());
	
// Photon isolation 
	// Tight 
   Photon_tight_tool = new CP::IsolationSelectionTool("Photon_tight_tool");
  	ANA_CHECK( Photon_tight_tool->setProperty("PhotonWP","FixedCutTight"));
	ANA_CHECK( Photon_tight_tool->initialize());
	// Loose 
   Photon_loose_tool = new CP::IsolationSelectionTool("Photon_loose_tool");
  	ANA_CHECK( Photon_loose_tool->setProperty("PhotonWP","FixedCutLoose"));
	ANA_CHECK( Photon_loose_tool->initialize());
	//TightCaloOnly WP
   Photon_tightCaloOnly_tool = new CP::IsolationSelectionTool("Photon_tightCaloOnly_tool");
  	ANA_CHECK( Photon_tightCaloOnly_tool->setProperty("PhotonWP","FixedCutTightCaloOnly"));
	ANA_CHECK( Photon_tightCaloOnly_tool->initialize());

// Photon Iso Efficincy

	// Tight
   Photon_IsoTighSF_tool = new AsgPhotonEfficiencyCorrectionTool("Photon_IsoTighSF_tool");
  	ANA_CHECK( Photon_IsoTighSF_tool->setProperty("MapFilePath","/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/PhotonEfficiencyCorrection/2015_2017/rel21.2/Winter2018_Prerec_v1/map1.txt"));
	ANA_CHECK( Photon_IsoTighSF_tool->setProperty("IsoKey","Tight"));
	ANA_CHECK( Photon_IsoTighSF_tool->setProperty("ForceDataType",1));
	ANA_CHECK( Photon_IsoTighSF_tool->initialize());

   Photon_IsoLooseSF_tool = new AsgPhotonEfficiencyCorrectionTool("Photon_IsoLooseSF_tool");
  	ANA_CHECK( Photon_IsoLooseSF_tool->setProperty("MapFilePath","/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/PhotonEfficiencyCorrection/2015_2017/rel21.2/Winter2018_Prerec_v1/map1.txt"));
	ANA_CHECK( Photon_IsoLooseSF_tool->setProperty("IsoKey","Loose"));
	ANA_CHECK( Photon_IsoLooseSF_tool->setProperty("ForceDataType",1));
	ANA_CHECK( Photon_IsoLooseSF_tool->initialize());

  Photon_IsoTighCaloSF_tool = new AsgPhotonEfficiencyCorrectionTool("Photon_IsoTighCaloSF_tool");
  	ANA_CHECK( Photon_IsoTighCaloSF_tool->setProperty("MapFilePath","/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/PhotonEfficiencyCorrection/2015_2017/rel21.2/Winter2018_Prerec_v1/map1.txt"));
	ANA_CHECK( Photon_IsoTighCaloSF_tool->setProperty("IsoKey","TightCaloOnly"));
	ANA_CHECK( Photon_IsoTighCaloSF_tool->setProperty("ForceDataType",1));
	ANA_CHECK( Photon_IsoTighCaloSF_tool->initialize());

// Muon Isolation

  Muon_iso_tool = new CP::IsolationSelectionTool("Muon_iso_tool");	
	ANA_CHECK( Muon_iso_tool->setProperty("MuonWP","FixedCutLoose"));
	ANA_CHECK( Muon_iso_tool->initialize()); 

// Muon Identification 

  Muon_id_tool = new CP::MuonSelectionTool("Muon_id_tool");
	ANA_CHECK( Muon_id_tool->setProperty( "MaxEta", 2.5 ));
        ANA_CHECK( Muon_id_tool->setProperty( "MuQuality", (int) xAOD::Muon::Quality::Medium)); 
        ANA_CHECK( Muon_id_tool->initialize());	

// Muon Calibration 

   Muon_calib_tool = new CP::MuonCalibrationAndSmearingTool("Muon_calib_tool");	 
	ANA_CHECK( Muon_calib_tool->setProperty("Year","Data16"));
        ANA_CHECK( Muon_calib_tool->setProperty("Algo","muons"));
        ANA_CHECK( Muon_calib_tool->setProperty("SagittaCorr",true));
        ANA_CHECK( Muon_calib_tool->setProperty("doSagittaMCDistortion",false));
	ANA_CHECK( Muon_calib_tool->initialize());

   Muon_calib17_tool = new CP::MuonCalibrationAndSmearingTool("Muon_calib17_tool");
	ANA_CHECK( Muon_calib17_tool->setProperty("Year","Data17"));
        ANA_CHECK( Muon_calib17_tool->setProperty("SagittaCorr",false));
        ANA_CHECK( Muon_calib17_tool->setProperty("doSagittaMCDistortion",false));
	ANA_CHECK( Muon_calib17_tool->initialize());
// Muon Scale Factor 

   Muon_RecoSF_tool  = new CP::MuonEfficiencyScaleFactors("Muon_RecoSF_tool");
	ANA_CHECK( Muon_RecoSF_tool->setProperty("WorkingPoint", "Medium"));
        ANA_CHECK( Muon_RecoSF_tool->initialize());

// Setup Tree 

	SetupTree();

  return StatusCode::SUCCESS;
}



StatusCode MyxAODAnalysis :: execute ()
{
  
  
  ANA_CHECK (evtStore()->retrieve (eventInfo, "EventInfo"));

  const xAOD::PhotonContainer *photons = nullptr;
  ANA_CHECK(evtStore()->retrieve( photons, "Photons" ));
//  if(!isAOD){ANA_CHECK(evtStore()->retrieve( photons, "NewSwPhotons" ));}

  const xAOD::MuonContainer *muons = nullptr;
  ANA_CHECK(evtStore()->retrieve( muons, "Muons"));

  const xAOD::ElectronContainer *electrons = nullptr;
  ANA_CHECK(evtStore()->retrieve( electrons, "Electrons")); 

  const xAOD::VertexContainer *vxContainer = nullptr;
  ANA_CHECK(evtStore()->retrieve(vxContainer, "PrimaryVertices"));

  const xAOD::CaloClusterContainer *CaloClusters = nullptr;	
  ANA_CHECK(evtStore()->retrieve( CaloClusters, "egammaClusters"));

  std::pair< xAOD::ElectronContainer*, xAOD::ShallowAuxContainer*> ElectronContainers = xAOD::shallowCopyContainer(*electrons);
  xAOD::ElectronContainer::iterator el = ElectronContainers.first->begin();
  xAOD::ElectronContainer::iterator elLast = ElectronContainers.first->end();

  std::pair< xAOD::MuonContainer*, xAOD::ShallowAuxContainer*> MuonContainers = xAOD::shallowCopyContainer(*muons);
  xAOD::MuonContainer::iterator mu = MuonContainers.first->begin();
  xAOD::MuonContainer::iterator muLast = MuonContainers.first->end();

  std::pair< xAOD::PhotonContainer*, xAOD::ShallowAuxContainer*> PhotonContainers = xAOD::shallowCopyContainer(*photons);
  xAOD::PhotonContainer::iterator ph = PhotonContainers.first->begin();
  xAOD::PhotonContainer::iterator phLast = PhotonContainers.first->end();

	m_singlePhotons.clear();
	m_selPhotons.clear();
	m_selElectrons.clear();
	m_selMuons.clear();
	mc_wgt  = 0.;
  	mc_xsec = 0.;
        pu_wgt  = 0.;
        pu_mu   = 0.;
	m_ee.clear();
	m_eey.clear();
	m_mumu.clear();
	m_mumuy.clear();

  ANA_CHECK(PileUp_tool->apply(*eventInfo));
	
  if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
       isMC = true; 
  }

   unsigned int runNumber;

   if(!isMC)
   {

	runNumber = eventInfo->runNumber();
   }else {

   	 if(eventInfo->auxdata<unsigned int>("RandomRunNumber")) 
	{
     		runNumber = eventInfo->auxdata<unsigned int>("RandomRunNumber");  
  	 } else 
	{ 
    		 ANA_MSG_WARNING("Failed to find the RandomRunNumber decoration. You'll receive SFs from the most recent period.");
     		runNumber = 9999999;
	}
  }
   
	
  if(!isMC)
  {

   if ( ( (eventInfo->errorState(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error) ||
	  (eventInfo->errorState(xAOD::EventInfo::SCT)==xAOD::EventInfo::Error) ||
	  (eventInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error) ||
	  (eventInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18) ) ) ) {
   		return StatusCode::SUCCESS; 
    }

     // Check GRL 
     if(!GRL_tool->passRunLB(*eventInfo)){
        return StatusCode::SUCCESS; 
     } 
     // Check Trigger 
     passTrigger = false;
     int cnt = 0;
     for (auto thisTrig : triggerList) {

   	auto cg = Trigger_dic_tool->getChainGroup(thisTrig.Data());
    	passTrigger = cg->isPassed();
	if(passTrigger)
	{
		if(thisTrig.Contains("HLT_e"))
		{
			passSingleElTrigger = true;
		}else if(thisTrig.Contains("HLT_2e"))
		{
			passDoubleElTrigger = true;
		}else if(thisTrig.Contains("HLT_mu") && !thisTrig.Contains("_mu8noL1"))
		{
			passSingleMuTrigger = true;
		}else if(thisTrig.Contains("HLT_2mu") || thisTrig.Contains("_mu8noL1"))
		{
			passDoubleMuTrigger = true;
		}
		cnt++;
	}
     	
     }
     if(cnt == 0)
     {
	return StatusCode::SUCCESS;
     }	

	mc_wgt  = 1.;
	pu_wgt  = 1.;
	pu_mu   = PileUp_tool->getCorrectedAverageInteractionsPerCrossing( *eventInfo, true );
        mc_xsec = 1.; 

  }
  
  if(isMC)
  {
	mc_event_number = eventInfo->eventNumber();
	std::vector<float> vector_weight = eventInfo->mcEventWeights();
	if(vector_weight.size() >= 0){
	mc_wgt = vector_weight[0];
	}
	pu_wgt = PileUp_tool->getCombinedWeight( *eventInfo );
	pu_mu     = PileUp_tool->getCorrectedAverageInteractionsPerCrossing( *eventInfo, true );
        //mc_xsec= (mclumi->getXsec(DSID));
	mc_xsec= 0.;
  }

  // Primary Vertex 
  const xAOD::Vertex* pVertex = nullptr;
  if( vxContainer->size() > 0 ){
  	pVertex = (*vxContainer)[0];
  } 

  double m_PV_z = pVertex->z();   

  //SinglePhoton 


  if(doSinglePhoton)
  {
  	int nph = ApplySinglePhotonSelection();

	if(nph == 0) 
	{
		ANA_MSG_INFO("SinglePhoton : Zero Photon");
		return StatusCode::SUCCESS;
	}
	
	if(nph >= 1)
	{
		StoreSinglePhoton();
		return StatusCode::SUCCESS;
	}
	
  	return StatusCode::SUCCESS;
  }
  
  // RadiativeZ 
  // Electrons Selection and Calibration 

  int i = 0;
  for(; el != elLast; ++el)
  {
	Egamma_calib_tool->setRandomSeed(eventInfo->eventNumber()+100*i++);
	Egamma_calib_tool->applyCorrection(**el);
	double pt = (*el)->pt() * 1e-3;
	float eta = fabs((*el)->eta());

	if( pt <= 10 ) continue;
	if( eta >= 2.47 ) continue; // track region
	if( eta >= 1.37 && eta <= 1.52 ) continue; // crack region

	const ElementLink< xAOD::TrackParticleContainer >& link = (*el)->trackParticleLink();
	double d0 = 100.;
	double d0err = 100.;
	double z0 = 100.;
   	if ( link.isValid() ) {
     		const xAOD::TrackParticle* track = *link;	
     		d0 = fabs(track->d0());
     		d0err = sqrt(track->definingParametersCovMatrix () (0,0));
     		z0 = fabs(track->z0() + track->vz() - m_PV_z);
   	}
	if( z0 > 10. ) continue;
	if( d0/d0err > 10. ) continue;
	if( !(Electron_id_tool->accept((*el))) ) continue;
	if( !(Electron_iso_tool->accept(*(*el))) ) continue; 

		m_selElectrons.push_back((*el));

  }
 
  // Muon Selection and Calibration 

  for(; mu !=muLast; ++mu)
  {
	if( runNumber < 320000)
	{
		Muon_calib_tool->applyCorrection(**mu);
	}else{
		Muon_calib17_tool->applyCorrection(**mu);
	}

	double pt = (*mu)->pt() * 1e-3;
	float eta = fabs((*mu)->eta());
	
	if( pt <= 10 ) continue;
	if( eta >= 2.5 ) continue;
	if ((*mu)->muonType() != xAOD::Muon::Combined) continue;

	const ElementLink< xAOD::TrackParticleContainer >& link = (*mu)->primaryTrackParticleLink();
	double d0 = 100.;
	double d0err = 100.;
	double z0 = 100.;
   	if ( link.isValid() ) {
     		const xAOD::TrackParticle* track = *link;	
     		d0 = fabs(track->d0());
     		d0err = sqrt(track->definingParametersCovMatrix () (0,0));
     		z0 = fabs(track->z0() + track->vz() - m_PV_z);
   	}
	if( z0 > 10. ) continue;
	if( d0/d0err > 10. ) continue;
	if( !(Muon_iso_tool->accept(*(*mu))) ) continue;
	
	if( !(Muon_id_tool->accept(*(*mu))) ) continue;
	
		m_selMuons.push_back((*mu));

  }

  // Photons Selection 
 int  ii = 0;
  for(; ph !=phLast; ++ph)
  {
	Egamma_calib_tool->setRandomSeed(eventInfo->eventNumber()+100*ii++);
	Egamma_calib_tool->applyCorrection(**ph);

	const float energy =  ( (*ph)->caloCluster() ) ? (*ph)->caloCluster()->e()*1e-3 : -999.0; 
 	double eta = ( (*ph)->caloCluster() ) ? (*ph)->caloCluster()->etaBE(2) : -999.0;
	const float cosheta = cosh(eta);
	const float e = (cosheta != 0.) ? energy/cosheta : 0.;

	if( e <= 10 ) continue;
	if( (*ph)->pt() *1e-3 > 140 )continue;
	if( (*ph)->pt() *1e-3 < 10 )continue;
	if( fabs(eta) >= 2.37 ) continue;
	if( fabs(eta) >= 1.37 && fabs(eta) <= 1.52 ) continue;
	
	if( !(!( ((*ph)->OQ()&1073741824)!=0 || ( ((*ph)->OQ()&134217728)!=0 && ((*ph)->showerShapeValue(xAOD::EgammaParameters::Reta) > 0.98
        || (*ph)->showerShapeValue(xAOD::EgammaParameters::f1) > 0.4 || ((*ph)->OQ()&67108864)!=0)) )) ) continue;

	bool isloose = Photon_loose_tool->accept(*(*ph));
	bool istight = Photon_tight_tool->accept(*(*ph));
	bool istightCaloOnly = Photon_tightCaloOnly_tool->accept(*(*ph));


	bool isEMLoose_OFF = Photon_Off_isEMLoose_tool->accept((*ph));
	bool isEMTightInc_OFF = Photon_Off_isEMTightInc_tool->accept((*ph));
	bool isEMTightDep_OFF = Photon_Off_isEMTightDep_tool->accept((*ph));

	bool isEMLoose_ON = Photon_On_isEMLoose_tool->accept((*ph));
	bool isEMTight_ON = Photon_On_isEMTight_tool->accept((*ph));
	bool isEMMedium_ON = Photon_On_isEMMedium_tool->accept((*ph));

	static SG::AuxElement::Decorator< bool > isLoose( "ph_isLoose" );
	static SG::AuxElement::Decorator< bool > isTight( "ph_isTight" );
	static SG::AuxElement::Decorator< bool > isCaloTight( "ph_isCaloTight" );

	static SG::AuxElement::Decorator< bool > isEMLooseOFF( "ph_isEMLooseOFF" );
	static SG::AuxElement::Decorator< bool > isEMTightIncOFF( "ph_isEMTightIncOFF" );
	static SG::AuxElement::Decorator< bool > isEMTightDepOFF( "ph_isEMTightDepOFF" );

	static SG::AuxElement::Decorator< bool > isEMLooseON( "ph_isEMLooseON" );
	static SG::AuxElement::Decorator< bool > isEMTightON( "ph_isEMTightON" );
	static SG::AuxElement::Decorator< bool > isEMMediumON( "ph_isEMMediumON" );

	static SG::AuxElement::Decorator< float > Conv( "ConvertionType" );

	isLoose(*(*ph)) = isloose;
	isTight(*(*ph)) = istight;
 	isCaloTight(*(*ph)) = istightCaloOnly;
	Conv(*(*ph)) = (*(*ph)).conversionType();

	isEMLooseOFF( *(*ph) )    = isEMLoose_OFF;
	isEMTightIncOFF( *(*ph) ) = isEMTightInc_OFF;
	isEMTightDepOFF( *(*ph) ) = isEMTightDep_OFF;

	isEMLooseON( *(*ph) ) = isEMLoose_ON;
	isEMTightON( *(*ph) ) = isEMTight_ON;
	isEMMediumON( *(*ph)) = isEMMedium_ON;

		m_selPhotons.push_back((*ph));
  }

 // Select Event;

	ANA_MSG_INFO("Number of Photons : " << m_selPhotons.size() << " Number of Electrons : " << m_selElectrons.size() << " Number of Muons : " << m_selMuons.size());

	SelectZeeyEvents(m_selPhotons, m_selElectrons);
	SelectZmumuyEvents(m_selPhotons, m_selMuons);
  return StatusCode::SUCCESS;
}

StatusCode MyxAODAnalysis :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.
  return StatusCode::SUCCESS;
}

void MyxAODAnalysis :: ClearVariables(){

	isZeey = false;
	isZmumuy = false;
	
	//mc_event_number = 0.;
        l1_pt  = 0.;
	l2_pt  = 0.;
	l1_eta = 0.;
	l2_eta = 0.;
	l1_phi = 0.;
	l2_phi = 0.;
	l1_e   = 0.;
	l2_e   = 0.;
	l1_Charge = 0.;
	l2_Charge = 0.;
	l1_SF = 0.;
	l2_SF = 0.;

	ph_IsoLooseSF = 0.;
	ph_IsoTightSF = 0.;
	ph_IsoTightCaloSF = 0.;

	ph_pt             = 0.;
        ph_eta            = 0.;
	ph_phi            = 0.;
	ph_E              = 0.;
	ph_topoetcone20   = 0.;
	ph_topoetcone30   = 0.;
	ph_topoetcone40   = 0.;
	ph_ptcone20       = 0.;
	ph_ptcone30       = 0.;
	ph_ptcone40       = 0.;
	ph_etcone20       = 0.;
	ph_etcone30       = 0.;
	ph_etcone40       = 0.;
	ph_ptvarcone20    = 0.;
	ph_ptvarcone30    = 0.;
	ph_ptvarcone40    = 0.;
	ph_isIsoLoose     = 0.;
	ph_isIsoTightCalo = 0.;
	ph_isIsoTight     = 0.;
	ph_conv           = 0.;

	ph_Rhad1   = 0.;
	ph_Rhad    = 0.;
	ph_E277    = 0.;
	ph_Reta    = 0.;
	ph_Rphi    = 0.;
	ph_Weta1   = 0.;
	ph_Weta2   = 0.;
	ph_f1      = 0.;
	ph_f3      = 0.;
	ph_f3core  = 0.;
	ph_fracs1  = 0.;
	ph_Wtots1  = 0.;
	ph_deltaE  = 0.;
	ph_Eratio  = 0.;
	ph_E011    = 0.;
	ph_E132    = 0.;
	ph_E237    = 0.;
	ph_E2tsts1 = 0.;
	ph_Emins1  = 0.;
	ph_Emaxs1  = 0.;
	ph_Ethad   = 0.; 
	ph_Ethad1  = 0.;

        ph_isTruthMatch = false;
        ph_TruthConv    = false;
	ph_PDG          = -99;

	ph_isEMLooseOff = false;
	ph_isEMTightIncOff = false;
	ph_isEMTightDepOff = false;
	
	ph_isEMLooseOn = false;
	ph_isEMTightOn = false;
	ph_isEMMediumOn = false;

	if(retrieveCells)
	{

	ph_ClusterSize7x11Lr2 = 0.;
	ph_ClusterSize3x7Lr2 = 0.;
	ph_ClusterSize5x5Lr2 = 0.;
	ph_ClusterSize7x11Lr1 = 0.;
	ph_ClusterSize3x7Lr1 = 0.;
	ph_ClusterSize5x5Lr1 = 0.;
	ph_ClusterSize7x11Lr3 = 0.;
	ph_ClusterSize3x7Lr3 = 0.;
	ph_ClusterSize5x5Lr3 = 0.;
	ph_ClusterSize7x11PreS = 0.;
	ph_ClusterSize3x7PreS = 0.;
	ph_ClusterSize5x5PreS = 0.;

	ph_clusterCellsLr2E711.clear();
  	ph_clusterCellsLr2Eta711.clear();
	ph_clusterCellsLr2Phi711.clear();

  	ph_clusterCellsLr2X711.clear();
  	ph_clusterCellsLr2Y711.clear();
  	ph_clusterCellsLr2Z711.clear();
  	ph_clusterCellsLr2T711.clear();
  	ph_clusterCellsLr2isBAD711.clear();

  	ph_clusterCellsLr2E37.clear();
  	ph_clusterCellsLr2Eta37.clear();
  	ph_clusterCellsLr2Phi37.clear();

  	ph_clusterCellsLr2X37.clear();
  	ph_clusterCellsLr2Y37.clear();
  	ph_clusterCellsLr2Z37.clear();
  	ph_clusterCellsLr2T37.clear();
  	ph_clusterCellsLr2isBAD37.clear();

  	ph_clusterCellsLr2E55.clear();
  	ph_clusterCellsLr2Eta55.clear();
  	ph_clusterCellsLr2Phi55.clear();

  	ph_clusterCellsLr2X55.clear();
  	ph_clusterCellsLr2Y55.clear();
  	ph_clusterCellsLr2Z55.clear();
  	ph_clusterCellsLr2T55.clear();
  	ph_clusterCellsLr2isBAD55.clear();

//Lr3

  	ph_clusterCellsLr3E711.clear();
  	ph_clusterCellsLr3Eta711.clear();
  	ph_clusterCellsLr3Phi711.clear();

  	ph_clusterCellsLr3X711.clear();
  	ph_clusterCellsLr3Y711.clear();
  	ph_clusterCellsLr3Z711.clear();
  	ph_clusterCellsLr3T711.clear();
  	ph_clusterCellsLr3isBAD711.clear();

  	ph_clusterCellsLr3E37.clear();
  	ph_clusterCellsLr3Eta37.clear();
  	ph_clusterCellsLr3Phi37.clear();

  	ph_clusterCellsLr3X37.clear();
  	ph_clusterCellsLr3Y37.clear();
  	ph_clusterCellsLr3Z37.clear();
	ph_clusterCellsLr3T37.clear();
  	ph_clusterCellsLr3isBAD37.clear();

  	ph_clusterCellsLr3E55.clear();
  	ph_clusterCellsLr3Eta55.clear();
  	ph_clusterCellsLr3Phi55.clear();

  	ph_clusterCellsLr3X55.clear();
  	ph_clusterCellsLr3Y55.clear();
  	ph_clusterCellsLr3Z55.clear();
  	ph_clusterCellsLr3T55.clear();
  	ph_clusterCellsLr3isBAD55.clear();

//Lr1

  	ph_clusterCellsLr1E711.clear();
  	ph_clusterCellsLr1Eta711.clear();
  	ph_clusterCellsLr1Phi711.clear();

  	ph_clusterCellsLr1X711.clear();
  	ph_clusterCellsLr1Y711.clear();
  	ph_clusterCellsLr1Z711.clear();
  	ph_clusterCellsLr1T711.clear();
  	ph_clusterCellsLr1isBAD711.clear();

  	ph_clusterCellsLr1E37.clear();
  	ph_clusterCellsLr1Eta37.clear();
  	ph_clusterCellsLr1Phi37.clear();

  	ph_clusterCellsLr1X37.clear();
  	ph_clusterCellsLr1Y37.clear();
  	ph_clusterCellsLr1Z37.clear();
  	ph_clusterCellsLr1T37.clear();
  	ph_clusterCellsLr1isBAD37.clear();

  	ph_clusterCellsLr1E55.clear();
  	ph_clusterCellsLr1Eta55.clear();
  	ph_clusterCellsLr1Phi55.clear();

  	ph_clusterCellsLr1X55.clear();
  	ph_clusterCellsLr1Y55.clear();
  	ph_clusterCellsLr1Z55.clear();
  	ph_clusterCellsLr1T55.clear();
  	ph_clusterCellsLr1isBAD55.clear();

//PreS

  	ph_clusterCellsPreSE711.clear();
  	ph_clusterCellsPreSEta711.clear();
  	ph_clusterCellsPreSPhi711.clear();

  	ph_clusterCellsPreSX711.clear();
  	ph_clusterCellsPreSY711.clear();
  	ph_clusterCellsPreSZ711.clear();
  	ph_clusterCellsPreST711.clear();
  	ph_clusterCellsPreSisBAD711.clear();

  	ph_clusterCellsPreSE37.clear();
  	ph_clusterCellsPreSEta37.clear();
  	ph_clusterCellsPreSPhi37.clear();

  	ph_clusterCellsPreSX37.clear();
  	ph_clusterCellsPreSY37.clear();
  	ph_clusterCellsPreSZ37.clear();
  	ph_clusterCellsPreST37.clear();
  	ph_clusterCellsPreSisBAD37.clear();

  	ph_clusterCellsPreSE55.clear();
 	ph_clusterCellsPreSEta55.clear();
 	ph_clusterCellsPreSPhi55.clear();

  	ph_clusterCellsPreSX55.clear();
  	ph_clusterCellsPreSY55.clear();
  	ph_clusterCellsPreSZ55.clear();
  	ph_clusterCellsPreST55.clear();
  	ph_clusterCellsPreSisBAD55.clear();


	l1_ClusterSize7x11Lr2 = 0.;
	l1_ClusterSize3x7Lr2 = 0.;
	l1_ClusterSize5x5Lr2 = 0.;
	l1_ClusterSize7x11Lr1 = 0.;
	l1_ClusterSize3x7Lr1 = 0.;
	l1_ClusterSize5x5Lr1 = 0.;
	l1_ClusterSize7x11Lr3 = 0.;
	l1_ClusterSize3x7Lr3 = 0.;
	l1_ClusterSize5x5Lr3 = 0.;

	l1_clusterCellsLr2E711.clear();
 	l1_clusterCellsLr2Eta711.clear();
	l1_clusterCellsLr2Phi711.clear();

	l1_clusterCellsLr3E711.clear();
 	l1_clusterCellsLr3Eta711.clear();
	l1_clusterCellsLr3Phi711.clear();

	l1_clusterCellsLr1E711.clear();
 	l1_clusterCellsLr1Eta711.clear();
	l1_clusterCellsLr1Phi711.clear();

  	l1_clusterCellsLr2E37.clear();
  	l1_clusterCellsLr2Eta37.clear();
  	l1_clusterCellsLr2Phi37.clear();
	
	l1_clusterCellsLr1E37.clear();
  	l1_clusterCellsLr1Eta37.clear();
  	l1_clusterCellsLr1Phi37.clear();

	l1_clusterCellsLr3E37.clear();
  	l1_clusterCellsLr3Eta37.clear();
  	l1_clusterCellsLr3Phi37.clear();

	l1_clusterCellsLr1E55.clear();
  	l1_clusterCellsLr1Eta55.clear();
  	l1_clusterCellsLr1Phi55.clear();

	l1_clusterCellsLr2E55.clear();
  	l1_clusterCellsLr2Eta55.clear();
  	l1_clusterCellsLr2Phi55.clear();

	l1_clusterCellsLr3E55.clear();
  	l1_clusterCellsLr3Eta55.clear();
  	l1_clusterCellsLr3Phi55.clear();

	l2_ClusterSize7x11Lr2 = 0.;
	l2_ClusterSize3x7Lr2 = 0.;
	l2_ClusterSize5x5Lr2 = 0.;
	l2_ClusterSize7x11Lr1 = 0.;
	l2_ClusterSize3x7Lr1 = 0.;
	l2_ClusterSize5x5Lr1 = 0.;
	l2_ClusterSize7x11Lr3 = 0.;
	l2_ClusterSize3x7Lr3 = 0.;
	l2_ClusterSize5x5Lr3 = 0.;

	l2_clusterCellsLr2E711.clear();
 	l2_clusterCellsLr2Eta711.clear();
	l2_clusterCellsLr2Phi711.clear();

	l2_clusterCellsLr3E711.clear();
 	l2_clusterCellsLr3Eta711.clear();
	l2_clusterCellsLr3Phi711.clear();

	l2_clusterCellsLr1E711.clear();
 	l2_clusterCellsLr1Eta711.clear();
	l2_clusterCellsLr1Phi711.clear();

  	l2_clusterCellsLr2E37.clear();
  	l2_clusterCellsLr2Eta37.clear();
  	l2_clusterCellsLr2Phi37.clear();
	
	l2_clusterCellsLr1E37.clear();
  	l2_clusterCellsLr1Eta37.clear();
  	l2_clusterCellsLr1Phi37.clear();

	l2_clusterCellsLr3E37.clear();
  	l2_clusterCellsLr3Eta37.clear();
  	l2_clusterCellsLr3Phi37.clear();

	l2_clusterCellsLr1E55.clear();
  	l2_clusterCellsLr1Eta55.clear();
  	l2_clusterCellsLr1Phi55.clear();

	l2_clusterCellsLr2E55.clear();
  	l2_clusterCellsLr2Eta55.clear();
  	l2_clusterCellsLr2Phi55.clear();

	l2_clusterCellsLr3E55.clear();
  	l2_clusterCellsLr3Eta55.clear();
  	l2_clusterCellsLr3Phi55.clear();
	
	}

}
void MyxAODAnalysis :: SetupTree()
{
StatusCode isSetup;
if(doSinglePhoton)
{
	isSetup = book (TTree ("NTUP", "SinglePhoton"));
}else{
	isSetup = book (TTree ("NTUP", "Zlly Ntuple"));
}
if( isSetup != StatusCode::SUCCESS )
{ 
	ANA_MSG_ERROR("Failed to setup output Tree");
	return;
}

tree ("NTUP")->Branch("ph_Pt",&ph_pt);
tree ("NTUP")->Branch("ph_Eta",&ph_eta);
tree ("NTUP")->Branch("ph_Phi",&ph_phi);
tree ("NTUP")->Branch("ph_E",&ph_E);
tree ("NTUP")->Branch("ph_topoetcone20",&ph_topoetcone20);
tree ("NTUP")->Branch("ph_topoetcone30",&ph_topoetcone30);
tree ("NTUP")->Branch("ph_topoetcone40",&ph_topoetcone40);
tree ("NTUP")->Branch("ph_ptcone20",&ph_ptcone20);
tree ("NTUP")->Branch("ph_ptcone30",&ph_ptcone30);
tree ("NTUP")->Branch("ph_ptcone40",&ph_ptcone40);
tree ("NTUP")->Branch("ph_etcone20",&ph_etcone20);
tree ("NTUP")->Branch("ph_etcone30",&ph_etcone30);
tree ("NTUP")->Branch("ph_etcone40",&ph_etcone40);
tree ("NTUP")->Branch("ph_ptvarcone20",&ph_ptvarcone20);
tree ("NTUP")->Branch("ph_ptvarcone30",&ph_ptvarcone30);
tree ("NTUP")->Branch("ph_ptvarcone40",&ph_ptvarcone40);
tree ("NTUP")->Branch("ph_isIsoLooseWP",&ph_isIsoLoose);
tree ("NTUP")->Branch("ph_isIsoTightWP",&ph_isIsoTight);
tree ("NTUP")->Branch("ph_isIsoTightCaloOnlyWP",&ph_isIsoTightCalo);

tree ("NTUP")->Branch("ph_isEMLooseOff",&ph_isEMLooseOff);
tree ("NTUP")->Branch("ph_isEMTightIncOff",&ph_isEMTightIncOff);
tree ("NTUP")->Branch("ph_isEMTightDepOff",&ph_isEMTightDepOff);

tree ("NTUP")->Branch("ph_isEMLooseOn",&ph_isEMLooseOn);
tree ("NTUP")->Branch("ph_isEMTightOn",&ph_isEMTightOn);
tree ("NTUP")->Branch("ph_isEMMediumOn",&ph_isEMMediumOn);


tree ("NTUP")->Branch("ph_Conversion",&ph_conv);
tree ("NTUP")->Branch("ph_Ethad",&ph_Ethad);
tree ("NTUP")->Branch("ph_Ethad1",&ph_Ethad1);
tree ("NTUP")->Branch("ph_Rhad1",&ph_Rhad1);
tree ("NTUP")->Branch("ph_Rhad",&ph_Rhad);
tree ("NTUP")->Branch("ph_E011",&ph_E011);
tree ("NTUP")->Branch("ph_E132",&ph_E132);
tree ("NTUP")->Branch("ph_E237",&ph_E237);
tree ("NTUP")->Branch("ph_E277",&ph_E277);
tree ("NTUP")->Branch("ph_Reta",&ph_Reta);
tree ("NTUP")->Branch("ph_Rphi",&ph_Rphi);
tree ("NTUP")->Branch("ph_Weta1",&ph_Weta1);
tree ("NTUP")->Branch("ph_Weta2",&ph_Weta2);
tree ("NTUP")->Branch("ph_f1",&ph_f1);
tree ("NTUP")->Branch("ph_f3",&ph_f3);
tree ("NTUP")->Branch("ph_f3core",&ph_f3core);
tree ("NTUP")->Branch("ph_fracs1",&ph_fracs1);
tree ("NTUP")->Branch("ph_Wstot1",&ph_Wtots1);
tree ("NTUP")->Branch("ph_deltaE",&ph_deltaE);
tree ("NTUP")->Branch("ph_Eratio",&ph_Eratio);
tree ("NTUP")->Branch("ph_E2tsts1",&ph_E2tsts1);
tree ("NTUP")->Branch("ph_Emins1",&ph_Emins1);
tree ("NTUP")->Branch("ph_Emaxs1",&ph_Emaxs1);

tree ("NTUP")->Branch("ph_isTruthMatch",&ph_isTruthMatch);
tree ("NTUP")->Branch("ph_TruthConv",&ph_TruthConv);
tree ("NTUP")->Branch("ph_PDG",&ph_PDG);

if(retrieveCells){

tree ("NTUP")->Branch("ph_ClusterSize7x11Lr2",&ph_ClusterSize7x11Lr2);
tree ("NTUP")->Branch("ph_ClusterSize3x7Lr2",&ph_ClusterSize3x7Lr2);
tree ("NTUP")->Branch("ph_ClusterSize5x5Lr2",&ph_ClusterSize5x5Lr2);

tree ("NTUP")->Branch("ph_ClusterSize7x11Lr3",&ph_ClusterSize7x11Lr3);
tree ("NTUP")->Branch("ph_ClusterSize3x7Lr3",&ph_ClusterSize3x7Lr3);
tree ("NTUP")->Branch("ph_ClusterSize5x5Lr3",&ph_ClusterSize5x5Lr3);

tree ("NTUP")->Branch("ph_ClusterSize7x11Lr1",&ph_ClusterSize7x11Lr1);
tree ("NTUP")->Branch("ph_ClusterSize3x7Lr1",&ph_ClusterSize3x7Lr1);
tree ("NTUP")->Branch("ph_ClusterSize5x5Lr1",&ph_ClusterSize5x5Lr1);

tree ("NTUP")->Branch("ph_ClusterSize7x11PreS",&ph_ClusterSize7x11PreS);
tree ("NTUP")->Branch("ph_ClusterSize3x7PreS",&ph_ClusterSize3x7PreS);
tree ("NTUP")->Branch("ph_ClusterSize5x5PreS",&ph_ClusterSize5x5PreS);

tree ("NTUP")->Branch("ph_clusterCellsLr2E7x11",&ph_clusterCellsLr2E711);
tree ("NTUP")->Branch("ph_clusterCellsLr2Eta7x11",&ph_clusterCellsLr2Eta711);
tree ("NTUP")->Branch("ph_clusterCellsLr2Phi7x11",&ph_clusterCellsLr2Phi711);
tree ("NTUP")->Branch("ph_clusterCellsLr2X7x11",&ph_clusterCellsLr2X711);
tree ("NTUP")->Branch("ph_clusterCellsLr2Y7x11",&ph_clusterCellsLr2Y711);
tree ("NTUP")->Branch("ph_clusterCellsLr2Z7x11",&ph_clusterCellsLr2Z711);
tree ("NTUP")->Branch("ph_clusterCellsLr2T7x11",&ph_clusterCellsLr2T711);
tree ("NTUP")->Branch("ph_clusterCellsLr2isBAD7x11",&ph_clusterCellsLr2isBAD711);
tree ("NTUP")->Branch("ph_clusterCellsLr2E3x7",&ph_clusterCellsLr2E37);
tree ("NTUP")->Branch("ph_clusterCellsLr2Eta3x7",&ph_clusterCellsLr2Eta37);
tree ("NTUP")->Branch("ph_clusterCellsLr2Phi3x7",&ph_clusterCellsLr2Phi37);
tree ("NTUP")->Branch("ph_clusterCellsLr2X3x7",&ph_clusterCellsLr2X37);
tree ("NTUP")->Branch("ph_clusterCellsLr2Y3x7",&ph_clusterCellsLr2Y37);
tree ("NTUP")->Branch("ph_clusterCellsLr2Z3x7",&ph_clusterCellsLr2Z37);
tree ("NTUP")->Branch("ph_clusterCellsLr2T3x7",&ph_clusterCellsLr2T37);
tree ("NTUP")->Branch("ph_clusterCellsLr2isBAD3x7",&ph_clusterCellsLr2isBAD37);
tree ("NTUP")->Branch("ph_clusterCellsLr2E5x5",&ph_clusterCellsLr2E55);
tree ("NTUP")->Branch("ph_clusterCellsLr2Eta5x5",&ph_clusterCellsLr2Eta55);
tree ("NTUP")->Branch("ph_clusterCellsLr2Phi5x5",&ph_clusterCellsLr2Phi55);
tree ("NTUP")->Branch("ph_clusterCellsLr2X5x5",&ph_clusterCellsLr2X55);
tree ("NTUP")->Branch("ph_clusterCellsLr2Y5x5",&ph_clusterCellsLr2Y55);
tree ("NTUP")->Branch("ph_clusterCellsLr2Z5x5",&ph_clusterCellsLr2Z55);
tree ("NTUP")->Branch("ph_clusterCellsLr2T5x5",&ph_clusterCellsLr2T55);
tree ("NTUP")->Branch("ph_clusterCellsLr2isBAD5x5",&ph_clusterCellsLr2isBAD55);

tree ("NTUP")->Branch("ph_clusterCellsLr1E7x11",&ph_clusterCellsLr1E711);
tree ("NTUP")->Branch("ph_clusterCellsLr1Eta7x11",&ph_clusterCellsLr1Eta711);
tree ("NTUP")->Branch("ph_clusterCellsLr1Phi7x11",&ph_clusterCellsLr1Phi711);
tree ("NTUP")->Branch("ph_clusterCellsLr1X7x11",&ph_clusterCellsLr1X711);
tree ("NTUP")->Branch("ph_clusterCellsLr1Y7x11",&ph_clusterCellsLr1Y711);
tree ("NTUP")->Branch("ph_clusterCellsLr1Z7x11",&ph_clusterCellsLr1Z711);
tree ("NTUP")->Branch("ph_clusterCellsLr1T7x11",&ph_clusterCellsLr1T711);
tree ("NTUP")->Branch("ph_clusterCellsLr1isBAD7x11",&ph_clusterCellsLr1isBAD711);
tree ("NTUP")->Branch("ph_clusterCellsLr1E3x7",&ph_clusterCellsLr1E37);
tree ("NTUP")->Branch("ph_clusterCellsLr1Eta3x7",&ph_clusterCellsLr1Eta37);
tree ("NTUP")->Branch("ph_clusterCellsLr1Phi3x7",&ph_clusterCellsLr1Phi37);
tree ("NTUP")->Branch("ph_clusterCellsLr1X3x7",&ph_clusterCellsLr1X37);
tree ("NTUP")->Branch("ph_clusterCellsLr1Y3x7",&ph_clusterCellsLr1Y37);
tree ("NTUP")->Branch("ph_clusterCellsLr1Z3x7",&ph_clusterCellsLr1Z37);
tree ("NTUP")->Branch("ph_clusterCellsLr1T3x7",&ph_clusterCellsLr1T37);
tree ("NTUP")->Branch("ph_clusterCellsLr1isBAD3x7",&ph_clusterCellsLr1isBAD37);
tree ("NTUP")->Branch("ph_clusterCellsLr1E5x5",&ph_clusterCellsLr1E55);
tree ("NTUP")->Branch("ph_clusterCellsLr1Eta5x5",&ph_clusterCellsLr1Eta55);
tree ("NTUP")->Branch("ph_clusterCellsLr1Phi5x5",&ph_clusterCellsLr1Phi55);
tree ("NTUP")->Branch("ph_clusterCellsLr1X5x5",&ph_clusterCellsLr1X55);
tree ("NTUP")->Branch("ph_clusterCellsLr1Y5x5",&ph_clusterCellsLr1Y55);
tree ("NTUP")->Branch("ph_clusterCellsLr1Z5x5",&ph_clusterCellsLr1Z55);
tree ("NTUP")->Branch("ph_clusterCellsLr1T5x5",&ph_clusterCellsLr1T55);
tree ("NTUP")->Branch("ph_clusterCellsLr1isBAD5x5",&ph_clusterCellsLr1isBAD55);

tree ("NTUP")->Branch("ph_clusterCellsLr3E7x11",&ph_clusterCellsLr3E711);
tree ("NTUP")->Branch("ph_clusterCellsLr3Eta7x11",&ph_clusterCellsLr3Eta711);
tree ("NTUP")->Branch("ph_clusterCellsLr3Phi7x11",&ph_clusterCellsLr3Phi711);
tree ("NTUP")->Branch("ph_clusterCellsLr3X7x11",&ph_clusterCellsLr3X711);
tree ("NTUP")->Branch("ph_clusterCellsLr3Y7x11",&ph_clusterCellsLr3Y711);
tree ("NTUP")->Branch("ph_clusterCellsLr3Z7x11",&ph_clusterCellsLr3Z711);
tree ("NTUP")->Branch("ph_clusterCellsLr3T7x11",&ph_clusterCellsLr3T711);
tree ("NTUP")->Branch("ph_clusterCellsLr3isBAD7x11",&ph_clusterCellsLr3isBAD711);
tree ("NTUP")->Branch("ph_clusterCellsLr3E3x7",&ph_clusterCellsLr3E37);
tree ("NTUP")->Branch("ph_clusterCellsLr3Eta3x7",&ph_clusterCellsLr3Eta37);
tree ("NTUP")->Branch("ph_clusterCellsLr3Phi3x7",&ph_clusterCellsLr3Phi37);
tree ("NTUP")->Branch("ph_clusterCellsLr3X3x7",&ph_clusterCellsLr3X37);
tree ("NTUP")->Branch("ph_clusterCellsLr3Y3x7",&ph_clusterCellsLr3Y37);
tree ("NTUP")->Branch("ph_clusterCellsLr3Z3x7",&ph_clusterCellsLr3Z37);
tree ("NTUP")->Branch("ph_clusterCellsLr3T3x7",&ph_clusterCellsLr3T37);
tree ("NTUP")->Branch("ph_clusterCellsLr3isBAD3x7",&ph_clusterCellsLr3isBAD37);
tree ("NTUP")->Branch("ph_clusterCellsLr3E5x5",&ph_clusterCellsLr3E55);
tree ("NTUP")->Branch("ph_clusterCellsLr3Eta5x5",&ph_clusterCellsLr3Eta55);
tree ("NTUP")->Branch("ph_clusterCellsLr3Phi5x5",&ph_clusterCellsLr3Phi55);
tree ("NTUP")->Branch("ph_clusterCellsLr3X5x5",&ph_clusterCellsLr3X55);
tree ("NTUP")->Branch("ph_clusterCellsLr3Y5x5",&ph_clusterCellsLr3Y55);
tree ("NTUP")->Branch("ph_clusterCellsLr3Z5x5",&ph_clusterCellsLr3Z55);
tree ("NTUP")->Branch("ph_clusterCellsLr3T5x5",&ph_clusterCellsLr3T55);
tree ("NTUP")->Branch("ph_clusterCellsLr3isBAD5x5",&ph_clusterCellsLr3isBAD55);

tree ("NTUP")->Branch("ph_clusterCellsPreSE7x11",&ph_clusterCellsPreSE711);
tree ("NTUP")->Branch("ph_clusterCellsPreSEta7x11",&ph_clusterCellsPreSEta711);
tree ("NTUP")->Branch("ph_clusterCellsPreSPhi7x11",&ph_clusterCellsPreSPhi711);
tree ("NTUP")->Branch("ph_clusterCellsPreSX7x11",&ph_clusterCellsPreSX711);
tree ("NTUP")->Branch("ph_clusterCellsPreSY7x11",&ph_clusterCellsPreSY711);
tree ("NTUP")->Branch("ph_clusterCellsPreSZ7x11",&ph_clusterCellsPreSZ711);
tree ("NTUP")->Branch("ph_clusterCellsPreST7x11",&ph_clusterCellsPreST711);
tree ("NTUP")->Branch("ph_clusterCellsPreSisBAD7x11",&ph_clusterCellsPreSisBAD711);
tree ("NTUP")->Branch("ph_clusterCellsPreSE3x7",&ph_clusterCellsPreSE37);
tree ("NTUP")->Branch("ph_clusterCellsPreSEta3x7",&ph_clusterCellsPreSEta37);
tree ("NTUP")->Branch("ph_clusterCellsPreSPhi3x7",&ph_clusterCellsPreSPhi37);
tree ("NTUP")->Branch("ph_clusterCellsPreSX3x7",&ph_clusterCellsPreSX37);
tree ("NTUP")->Branch("ph_clusterCellsPreSY3x7",&ph_clusterCellsPreSY37);
tree ("NTUP")->Branch("ph_clusterCellsPreSZ3x7",&ph_clusterCellsPreSZ37);
tree ("NTUP")->Branch("ph_clusterCellsPreST3x7",&ph_clusterCellsPreST37);
tree ("NTUP")->Branch("ph_clusterCellsPreSisBAD3x7",&ph_clusterCellsPreSisBAD37);
tree ("NTUP")->Branch("ph_clusterCellsPreSE5x5",&ph_clusterCellsPreSE55);
tree ("NTUP")->Branch("ph_clusterCellsPreSEta5x5",&ph_clusterCellsPreSEta55);
tree ("NTUP")->Branch("ph_clusterCellsPreSPhi5x5",&ph_clusterCellsPreSPhi55);
tree ("NTUP")->Branch("ph_clusterCellsPreSX5x5",&ph_clusterCellsPreSX55);
tree ("NTUP")->Branch("ph_clusterCellsPreSY5x5",&ph_clusterCellsPreSY55);
tree ("NTUP")->Branch("ph_clusterCellsPreSZ5x5",&ph_clusterCellsPreSZ55);
tree ("NTUP")->Branch("ph_clusterCellsPreST5x5",&ph_clusterCellsPreST55);
tree ("NTUP")->Branch("ph_clusterCellsPreSisBAD5x5",&ph_clusterCellsPreSisBAD55);

if(!doSinglePhoton)
{

tree ("NTUP")->Branch("l1_ClusterSize7x11Lr2",&l1_ClusterSize7x11Lr2);
tree ("NTUP")->Branch("l1_ClusterSize3x7Lr2",&l1_ClusterSize3x7Lr2);
tree ("NTUP")->Branch("l1_ClusterSize5x5Lr2",&l1_ClusterSize5x5Lr2);

tree ("NTUP")->Branch("l1_ClusterSize7x11Lr3",&l1_ClusterSize7x11Lr3);
tree ("NTUP")->Branch("l1_ClusterSize3x7Lr3",&l1_ClusterSize3x7Lr3);
tree ("NTUP")->Branch("l1_ClusterSize5x5Lr3",&l1_ClusterSize5x5Lr3);

tree ("NTUP")->Branch("l1_ClusterSize7x11Lr1",&l1_ClusterSize7x11Lr1);
tree ("NTUP")->Branch("l1_ClusterSize3x7Lr1",&l1_ClusterSize3x7Lr1);
tree ("NTUP")->Branch("l1_ClusterSize5x5Lr1",&l1_ClusterSize5x5Lr1);

tree ("NTUP")->Branch("l1_clusterCellsLr2E7x11",&l1_clusterCellsLr2E711);
tree ("NTUP")->Branch("l1_clusterCellsLr2Eta7x11",&l1_clusterCellsLr2Eta711);
tree ("NTUP")->Branch("l1_clusterCellsLr2Phi7x11",&l1_clusterCellsLr2Phi711);
tree ("NTUP")->Branch("l1_clusterCellsLr2E3x7",&l1_clusterCellsLr2E37);
tree ("NTUP")->Branch("l1_clusterCellsLr2Eta3x7",&l1_clusterCellsLr2Eta37);
tree ("NTUP")->Branch("l1_clusterCellsLr2Phi3x7",&l1_clusterCellsLr2Phi37);
tree ("NTUP")->Branch("l1_clusterCellsLr2E5x5",&l1_clusterCellsLr2E55);
tree ("NTUP")->Branch("l1_clusterCellsLr2Eta5x5",&l1_clusterCellsLr2Eta55);
tree ("NTUP")->Branch("l1_clusterCellsLr2Phi5x5",&l1_clusterCellsLr2Phi55);

tree ("NTUP")->Branch("l1_clusterCellsLr1E7x11",&l1_clusterCellsLr1E711);
tree ("NTUP")->Branch("l1_clusterCellsLr1Eta7x11",&l1_clusterCellsLr1Eta711);
tree ("NTUP")->Branch("l1_clusterCellsLr1Phi7x11",&l1_clusterCellsLr1Phi711);
tree ("NTUP")->Branch("l1_clusterCellsLr1E3x7",&l1_clusterCellsLr1E37);
tree ("NTUP")->Branch("l1_clusterCellsLr1Eta3x7",&l1_clusterCellsLr1Eta37);
tree ("NTUP")->Branch("l1_clusterCellsLr1Phi3x7",&l1_clusterCellsLr1Phi37);
tree ("NTUP")->Branch("l1_clusterCellsLr1E5x5",&l1_clusterCellsLr1E55);
tree ("NTUP")->Branch("l1_clusterCellsLr1Eta5x5",&l1_clusterCellsLr1Eta55);
tree ("NTUP")->Branch("l1_clusterCellsLr1Phi5x5",&l1_clusterCellsLr1Phi55);

tree ("NTUP")->Branch("l1_clusterCellsLr3E7x11",&l1_clusterCellsLr3E711);
tree ("NTUP")->Branch("l1_clusterCellsLr3Eta7x11",&l1_clusterCellsLr3Eta711);
tree ("NTUP")->Branch("l1_clusterCellsLr3Phi7x11",&l1_clusterCellsLr3Phi711);
tree ("NTUP")->Branch("l1_clusterCellsLr3E3x7",&l1_clusterCellsLr3E37);
tree ("NTUP")->Branch("l1_clusterCellsLr3Eta3x7",&l1_clusterCellsLr3Eta37);
tree ("NTUP")->Branch("l1_clusterCellsLr3Phi3x7",&l1_clusterCellsLr3Phi37);
tree ("NTUP")->Branch("l1_clusterCellsLr3E5x5",&l1_clusterCellsLr3E55);
tree ("NTUP")->Branch("l1_clusterCellsLr3Eta5x5",&l1_clusterCellsLr3Eta55);
tree ("NTUP")->Branch("l1_clusterCellsLr3Phi5x5",&l1_clusterCellsLr3Phi55);


tree ("NTUP")->Branch("l2_ClusterSize7x11Lr2",&l2_ClusterSize7x11Lr2);
tree ("NTUP")->Branch("l2_ClusterSize3x7Lr2",&l2_ClusterSize3x7Lr2);
tree ("NTUP")->Branch("l2_ClusterSize5x5Lr2",&l2_ClusterSize5x5Lr2);

tree ("NTUP")->Branch("l2_ClusterSize7x11Lr3",&l2_ClusterSize7x11Lr3);
tree ("NTUP")->Branch("l2_ClusterSize3x7Lr3",&l2_ClusterSize3x7Lr3);
tree ("NTUP")->Branch("l2_ClusterSize5x5Lr3",&l2_ClusterSize5x5Lr3);

tree ("NTUP")->Branch("l2_ClusterSize7x11Lr1",&l2_ClusterSize7x11Lr1);
tree ("NTUP")->Branch("l2_ClusterSize3x7Lr1",&l2_ClusterSize3x7Lr1);
tree ("NTUP")->Branch("l2_ClusterSize5x5Lr1",&l2_ClusterSize5x5Lr1);

tree ("NTUP")->Branch("l2_clusterCellsLr2E7x11",&l2_clusterCellsLr2E711);
tree ("NTUP")->Branch("l2_clusterCellsLr2Eta7x11",&l2_clusterCellsLr2Eta711);
tree ("NTUP")->Branch("l2_clusterCellsLr2Phi7x11",&l2_clusterCellsLr2Phi711);
tree ("NTUP")->Branch("l2_clusterCellsLr2E3x7",&l2_clusterCellsLr2E37);
tree ("NTUP")->Branch("l2_clusterCellsLr2Eta3x7",&l2_clusterCellsLr2Eta37);
tree ("NTUP")->Branch("l2_clusterCellsLr2Phi3x7",&l2_clusterCellsLr2Phi37);
tree ("NTUP")->Branch("l2_clusterCellsLr2E5x5",&l2_clusterCellsLr2E55);
tree ("NTUP")->Branch("l2_clusterCellsLr2Eta5x5",&l2_clusterCellsLr2Eta55);
tree ("NTUP")->Branch("l2_clusterCellsLr2Phi5x5",&l2_clusterCellsLr2Phi55);

tree ("NTUP")->Branch("l2_clusterCellsLr1E7x11",&l2_clusterCellsLr1E711);
tree ("NTUP")->Branch("l2_clusterCellsLr1Eta7x11",&l2_clusterCellsLr1Eta711);
tree ("NTUP")->Branch("l2_clusterCellsLr1Phi7x11",&l2_clusterCellsLr1Phi711);
tree ("NTUP")->Branch("l2_clusterCellsLr1E3x7",&l2_clusterCellsLr1E37);
tree ("NTUP")->Branch("l2_clusterCellsLr1Eta3x7",&l2_clusterCellsLr1Eta37);
tree ("NTUP")->Branch("l2_clusterCellsLr1Phi3x7",&l2_clusterCellsLr1Phi37);
tree ("NTUP")->Branch("l2_clusterCellsLr1E5x5",&l2_clusterCellsLr1E55);
tree ("NTUP")->Branch("l2_clusterCellsLr1Eta5x5",&l2_clusterCellsLr1Eta55);
tree ("NTUP")->Branch("l2_clusterCellsLr1Phi5x5",&l2_clusterCellsLr1Phi55);

tree ("NTUP")->Branch("l2_clusterCellsLr3E7x11",&l2_clusterCellsLr3E711);
tree ("NTUP")->Branch("l2_clusterCellsLr3Eta7x11",&l2_clusterCellsLr3Eta711);
tree ("NTUP")->Branch("l2_clusterCellsLr3Phi7x11",&l2_clusterCellsLr3Phi711);
tree ("NTUP")->Branch("l2_clusterCellsLr3E3x7",&l2_clusterCellsLr3E37);
tree ("NTUP")->Branch("l2_clusterCellsLr3Eta3x7",&l2_clusterCellsLr3Eta37);
tree ("NTUP")->Branch("l2_clusterCellsLr3Phi3x7",&l2_clusterCellsLr3Phi37);
tree ("NTUP")->Branch("l2_clusterCellsLr3E5x5",&l2_clusterCellsLr3E55);
tree ("NTUP")->Branch("l2_clusterCellsLr3Eta5x5",&l2_clusterCellsLr3Eta55);
tree ("NTUP")->Branch("l2_clusterCellsLr3Phi5x5",&l2_clusterCellsLr3Phi55);
}
}

if(!doSinglePhoton)
{
tree ("NTUP")->Branch("l1_Pt",&l1_pt);
tree ("NTUP")->Branch("l1_Eta",&l1_eta);
tree ("NTUP")->Branch("l1_Phi",&l1_phi);
tree ("NTUP")->Branch("l1_E",&l1_e);
tree ("NTUP")->Branch("l1_Charge",&l1_Charge);
tree ("NTUP")->Branch("l2_Pt",&l2_pt);
tree ("NTUP")->Branch("l2_Eta",&l2_eta);
tree ("NTUP")->Branch("l2_Phi",&l2_phi);
tree ("NTUP")->Branch("l2_E",&l2_e);
tree ("NTUP")->Branch("l2_Charge",&l2_Charge);
tree ("NTUP")->Branch("l1_SF",&l1_SF);
tree ("NTUP")->Branch("l2_SF",&l2_SF);

tree ("NTUP")->Branch("isZeey",&isZeey);
tree ("NTUP")->Branch("isZmumuy",&isZmumuy);
tree ("NTUP")->Branch("M_ee",&m_ee);
tree ("NTUP")->Branch("M_eey",&m_eey);
tree ("NTUP")->Branch("M_mumu",&m_mumu);
tree ("NTUP")->Branch("M_mumuy",&m_mumuy);

}

tree ("NTUP")->Branch("ph_IsoLooseSF",&ph_IsoLooseSF);
tree ("NTUP")->Branch("ph_IsoTightSF",&ph_IsoTightSF);
tree ("NTUP")->Branch("ph_IsoTightCaloSF",&ph_IsoTightCaloSF);

tree ("NTUP")->Branch("pu_wgt",&pu_wgt);
tree ("NTUP")->Branch("evntN",&mc_event_number);
tree ("NTUP")->Branch("mu",&pu_mu);
tree ("NTUP")->Branch("mc_wgt",&mc_wgt);
tree ("NTUP")->Branch("mc_xsec",&mc_xsec);


}

void MyxAODAnalysis :: StoreEvent (int iph, int il1, int il2, TString Type){

	ClearVariables();
	ANA_MSG_INFO("Start Filling the Tree");
	const xAOD::Photon *photon = m_selPhotons[iph];
	TLorentzVector lepton1;
	TLorentzVector lepton2;

	if(Type == "Zeey")
	{
		lepton1.SetPtEtaPhiE(m_selElectrons[il1]->pt(),m_selElectrons[il1]->eta(),m_selElectrons[il1]->phi(),m_selElectrons[il1]->e());
		lepton2.SetPtEtaPhiE(m_selElectrons[il2]->pt(),m_selElectrons[il2]->eta(),m_selElectrons[il2]->phi(),m_selElectrons[il2]->e());
		l1_Charge = m_selElectrons[il1]->charge();
		l2_Charge = m_selElectrons[il2]->charge();
		l1_SF = getSF(il1,"Electron");
		l2_SF = getSF(il2,"Electron");
		isZeey = true;
	}

	if(Type == "Zmumuy")
	{
		lepton1.SetPtEtaPhiE(m_selMuons[il1]->pt(),m_selMuons[il1]->eta(),m_selMuons[il1]->phi(),m_selMuons[il1]->e());
		lepton2.SetPtEtaPhiE(m_selMuons[il2]->pt(),m_selMuons[il2]->eta(),m_selMuons[il2]->phi(),m_selMuons[il2]->e());
		l1_Charge = m_selMuons[il1]->charge();
		l2_Charge = m_selMuons[il2]->charge();
		l1_SF = getSF(il1,"Muon");
		l2_SF = getSF(il2,"Muon");
		isZmumuy = true;
	}
	
	//Storing Leptons 
	l1_pt  = lepton1.Pt();
	l2_pt  = lepton2.Pt();
	l1_eta = lepton1.Eta();
	l2_eta = lepton2.Eta();
	l1_phi = lepton1.Phi();
	l2_phi = lepton2.Phi();
	l1_e   = lepton1.E();
	l2_e   = lepton2.E();

	//Storing Photon

	ph_pt             = photon->pt();
        ph_eta            = photon->eta();
	ph_phi            = photon->phi();
	ph_E              = photon->e();
	ph_topoetcone20   = photon->auxdata<float>("topoetcone20");
	ph_topoetcone30   = photon->auxdata<float>("topoetcone30");
	ph_topoetcone40   = photon->auxdata<float>("topoetcone40");
	ph_ptcone20       = photon->auxdata<float>("ptcone20");
	ph_ptcone30       = photon->auxdata<float>("ptcone30");
	ph_ptcone40       = photon->auxdata<float>("ptcone40");
	ph_etcone20       = photon->auxdata<float>("etcone20");
	ph_etcone30       = photon->auxdata<float>("etcone30");
	ph_etcone40       = photon->auxdata<float>("etcone40");
	ph_ptvarcone20    = photon->auxdata<float>("ptvarcone20");
	ph_ptvarcone30    = photon->auxdata<float>("ptvarcone30");
	ph_ptvarcone40    = photon->auxdata<float>("ptvarcone40");
	ph_isIsoLoose     = photon->auxdata<bool>("ph_isLoose");
	ph_isIsoTightCalo = photon->auxdata<bool>("ph_isCaloTight");
	ph_isIsoTight     = photon->auxdata<bool>("ph_isTight");
	ph_conv           = photon->auxdata<float>("ConvertionType");

	decorateWithTruthInfo(photon);

	ph_PDG		          = photon->auxdata<int>("ph_PDGID");
	ph_isTruthMatch		  = photon->auxdata<bool>("ph_isTruthMatch");
	ph_TruthConv		  = photon->auxdata<int>("ph_TruthConversion");	

        ph_IsoLooseSF             = getSF(iph,"PhotonLoose");
	ph_IsoTightSF             = getSF(iph,"PhotonTight");
	ph_IsoTightCaloSF         = getSF(iph,"PhotonTightCalo");

	float rhad1,rhad,e277,reta,rphi,weta1,weta2,f1,f3,f3core,wtots1,deltae,eratio,fracs1,e011,e132,e237,e2tsts1,emins1,emaxs1,ethad,ethad1;
	//get shower shape
	getPhotonsShowerShapeValues(photon,rhad1,rhad,e277,reta,rphi,weta1,weta2,f1,f3,f3core,wtots1,deltae,eratio,fracs1,e011,e132,e237,e2tsts1,emins1,emaxs1,ethad,ethad1);
	//push shower shape
	ph_Rhad1   = rhad1;
	ph_Rhad    = rhad;
	ph_E277    = e277;
	ph_Reta    = reta;
	ph_Rphi    = rphi;
	ph_Weta1   = weta1;
	ph_Weta2   = weta2;
	ph_f1      = f1;
	ph_f3      = f3;
	ph_f3core  = f3core;
	ph_fracs1  = fracs1;
	ph_Wtots1  = wtots1;
	ph_deltaE  = deltae;
	ph_Eratio  = eratio;
	ph_E011    = e011;
	ph_E132    = e132;
	ph_E237    = e237;
	ph_E2tsts1 = e2tsts1;
	ph_Emins1  = emins1;
	ph_Emaxs1  = emaxs1;
	ph_Ethad   = ethad; 
	ph_Ethad1  = ethad1;

	if(isAOD == true)
	{
		decoratePhotonObject(photon,3,7);
		decoratePhotonObject(photon,5,5);
		decoratePhotonObject(photon,7,11);

		if(isZeey){

			decorateElectronObject(m_selElectrons[il1],3,7);
			decorateElectronObject(m_selElectrons[il1],5,5);
			decorateElectronObject(m_selElectrons[il1],7,11);

			decorateElectronObject(m_selElectrons[il2],3,7);
			decorateElectronObject(m_selElectrons[il2],5,5);
			decorateElectronObject(m_selElectrons[il2],7,11);
		}
	}

	if(retrieveCells)
	{

	ph_ClusterSize7x11Lr2 = photon->auxdata<int>("7x11ClusterLr2Size");
	ph_ClusterSize3x7Lr2 = photon->auxdata<int>("3x7ClusterLr2Size");
	ph_ClusterSize5x5Lr2 = photon->auxdata<int>("5x5ClusterLr2Size");
	ph_ClusterSize7x11Lr1 = photon->auxdata<int>("7x11ClusterLr1Size");
	ph_ClusterSize3x7Lr1 = photon->auxdata<int>("3x7ClusterLr1Size");
	ph_ClusterSize5x5Lr1 = photon->auxdata<int>("5x5ClusterLr1Size");
	ph_ClusterSize7x11Lr3 = photon->auxdata<int>("7x11ClusterLr3Size");
	ph_ClusterSize3x7Lr3 = photon->auxdata<int>("3x7ClusterLr3Size");
	ph_ClusterSize5x5Lr3 = photon->auxdata<int>("5x5ClusterLr3Size");
	ph_ClusterSize7x11Lr3 = photon->auxdata<int>("7x11ClusterPreSSize");
	ph_ClusterSize3x7Lr3 = photon->auxdata<int>("3x7ClusterPreSSize");
	ph_ClusterSize5x5Lr3 = photon->auxdata<int>("5x5ClusterPreSSize");
	
	//77 Lr2

	ph_clusterCellsLr2E711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2E");
 	ph_clusterCellsLr2Eta711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2Eta");
	ph_clusterCellsLr2Phi711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2Phi");
 	ph_clusterCellsLr2X711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2X");
	ph_clusterCellsLr2Y711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2Y");
 	ph_clusterCellsLr2Z711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2Z");
	ph_clusterCellsLr2T711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2T");
 	ph_clusterCellsLr2isBAD711 = photon->auxdata<std::vector<bool>>("7x11ClusterLr2isBAD");

	ph_clusterCellsLr3E711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3E");
 	ph_clusterCellsLr3Eta711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3Eta");
	ph_clusterCellsLr3Phi711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3Phi");
 	ph_clusterCellsLr3X711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3X");
	ph_clusterCellsLr3Y711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3Y");
 	ph_clusterCellsLr3Z711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3Z");
	ph_clusterCellsLr3T711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3T");
 	ph_clusterCellsLr3isBAD711 = photon->auxdata<std::vector<bool>>("7x11ClusterLr3isBAD");

	ph_clusterCellsLr1E711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1E");
 	ph_clusterCellsLr1Eta711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1Eta");
	ph_clusterCellsLr1Phi711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1Phi");
 	ph_clusterCellsLr1X711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1X");
	ph_clusterCellsLr1Y711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1Y");
 	ph_clusterCellsLr1Z711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1Z");
	ph_clusterCellsLr1T711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1T");
 	ph_clusterCellsLr1isBAD711 = photon->auxdata<std::vector<bool>>("7x11ClusterLr1isBAD");

	ph_clusterCellsPreSE711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSE");
 	ph_clusterCellsPreSEta711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSEta");
	ph_clusterCellsPreSPhi711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSPhi");
 	ph_clusterCellsPreSX711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSX");
	ph_clusterCellsPreSY711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSY");
 	ph_clusterCellsPreSZ711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSZ");
	ph_clusterCellsPreST711 = photon->auxdata<std::vector<float>>("7x11ClusterPreST");
 	ph_clusterCellsPreSisBAD711 = photon->auxdata<std::vector<bool>>("7x11ClusterPreSisBAD");

  	ph_clusterCellsLr2E37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2E");
  	ph_clusterCellsLr2Eta37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2Eta");
  	ph_clusterCellsLr2Phi37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2Phi");
  	ph_clusterCellsLr2X37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2X");
  	ph_clusterCellsLr2Y37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2Y");
  	ph_clusterCellsLr2Z37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2Z");
  	ph_clusterCellsLr2T37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2T");
  	ph_clusterCellsLr2isBAD37 = photon->auxdata<std::vector<bool>>("3x7ClusterLr2isBAD");

	ph_clusterCellsLr1E37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1E");
  	ph_clusterCellsLr1Eta37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1Eta");
  	ph_clusterCellsLr1Phi37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1Phi");
	ph_clusterCellsLr1X37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1X");
  	ph_clusterCellsLr1Y37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1Y");
  	ph_clusterCellsLr1Z37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1Z");
  	ph_clusterCellsLr1T37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1T");
  	ph_clusterCellsLr1isBAD37 = photon->auxdata<std::vector<bool>>("3x7ClusterLr1isBAD");

	ph_clusterCellsLr3E37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3E");
  	ph_clusterCellsLr3Eta37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3Eta");
  	ph_clusterCellsLr3Phi37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3Phi");
	ph_clusterCellsLr3X37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3X");
  	ph_clusterCellsLr3Y37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3Y");
  	ph_clusterCellsLr3Z37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3Z");
  	ph_clusterCellsLr3T37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3T");
  	ph_clusterCellsLr3isBAD37 = photon->auxdata<std::vector<bool>>("3x7ClusterLr3isBAD");

	ph_clusterCellsPreSE37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSE");
  	ph_clusterCellsPreSEta37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSEta");
  	ph_clusterCellsPreSPhi37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSPhi");
	ph_clusterCellsPreSX37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSX");
  	ph_clusterCellsPreSY37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSY");
  	ph_clusterCellsPreSZ37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSZ");
  	ph_clusterCellsPreST37 = photon->auxdata<std::vector<float>>("3x7ClusterPreST");
  	ph_clusterCellsPreSisBAD37 = photon->auxdata<std::vector<bool>>("3x7ClusterPreSisBAD");

	ph_clusterCellsLr1E55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1E");
  	ph_clusterCellsLr1Eta55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1Eta");
  	ph_clusterCellsLr1Phi55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1Phi");
	ph_clusterCellsLr1X55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1X");
  	ph_clusterCellsLr1Y55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1Y");
  	ph_clusterCellsLr1Z55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1Z");
	ph_clusterCellsLr1T55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1T");
  	ph_clusterCellsLr1isBAD55 = photon->auxdata<std::vector<bool>>("5x5ClusterLr1isBAD");

	ph_clusterCellsLr2E55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2E");
  	ph_clusterCellsLr2Eta55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2Eta");
  	ph_clusterCellsLr2Phi55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2Phi");
	ph_clusterCellsLr2X55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2X");
  	ph_clusterCellsLr2Y55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2Y");
  	ph_clusterCellsLr2Z55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2Z");
	ph_clusterCellsLr2T55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2T");
  	ph_clusterCellsLr2isBAD55 = photon->auxdata<std::vector<bool>>("5x5ClusterLr2isBAD");

	ph_clusterCellsLr3E55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3E");
  	ph_clusterCellsLr3Eta55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3Eta");
  	ph_clusterCellsLr3Phi55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3Phi");
	ph_clusterCellsLr3X55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3X");
  	ph_clusterCellsLr3Y55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3Y");
  	ph_clusterCellsLr3Z55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3Z");
	ph_clusterCellsLr3T55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3T");
  	ph_clusterCellsLr3isBAD55 = photon->auxdata<std::vector<bool>>("5x5ClusterLr3isBAD");

	ph_clusterCellsPreSE55   = photon->auxdata<std::vector<float>>("5x5ClusterPreSE");
  	ph_clusterCellsPreSEta55 = photon->auxdata<std::vector<float>>("5x5ClusterPreSEta");
  	ph_clusterCellsPreSPhi55 = photon->auxdata<std::vector<float>>("5x5ClusterPreSPhi");
	ph_clusterCellsPreSX55   = photon->auxdata<std::vector<float>>("5x5ClusterPreSX");
  	ph_clusterCellsPreSY55   = photon->auxdata<std::vector<float>>("5x5ClusterPreSY");
  	ph_clusterCellsPreSZ55   = photon->auxdata<std::vector<float>>("5x5ClusterPreSZ");
	ph_clusterCellsPreST55   = photon->auxdata<std::vector<float>>("5x5ClusterPreST");
  	ph_clusterCellsPreSisBAD55 = photon->auxdata<std::vector<bool>>("5x5ClusterPreSisBAD");


	if(isZeey)
	{
	
	l1_ClusterSize7x11Lr2 = m_selElectrons[il1]->auxdata<int>("7x11ClusterLr2Size");
	l1_ClusterSize3x7Lr2 = m_selElectrons[il1]->auxdata<int>("3x7ClusterLr2Size");
	l1_ClusterSize5x5Lr2 = m_selElectrons[il1]->auxdata<int>("5x5ClusterLr2Size");
	l1_ClusterSize7x11Lr1 = m_selElectrons[il1]->auxdata<int>("7x11ClusterLr1Size");
	l1_ClusterSize3x7Lr1 = m_selElectrons[il1]->auxdata<int>("3x7ClusterLr1Size");
	l1_ClusterSize5x5Lr1 = m_selElectrons[il1]->auxdata<int>("5x5ClusterLr1Size");
	l1_ClusterSize7x11Lr3 = m_selElectrons[il1]->auxdata<int>("7x11ClusterLr3Size");
	l1_ClusterSize3x7Lr3 = m_selElectrons[il1]->auxdata<int>("3x7ClusterLr3Size");
	l1_ClusterSize5x5Lr3 = m_selElectrons[il1]->auxdata<int>("5x5ClusterLr3Size");

	l1_clusterCellsLr2E711 = m_selElectrons[il1]->auxdata<std::vector<float>>("7x11ClusterLr2E");
 	l1_clusterCellsLr2Eta711 = m_selElectrons[il1]->auxdata<std::vector<float>>("7x11ClusterLr2Eta");
	l1_clusterCellsLr2Phi711 = m_selElectrons[il1]->auxdata<std::vector<float>>("7x11ClusterLr2Phi");

	l1_clusterCellsLr3E711 = m_selElectrons[il1]->auxdata<std::vector<float>>("7x11ClusterLr3E");
 	l1_clusterCellsLr3Eta711 = m_selElectrons[il1]->auxdata<std::vector<float>>("7x11ClusterLr3Eta");
	l1_clusterCellsLr3Phi711 = m_selElectrons[il1]->auxdata<std::vector<float>>("7x11ClusterLr3Phi");

	l1_clusterCellsLr1E711 = m_selElectrons[il1]->auxdata<std::vector<float>>("7x11ClusterLr1E");
 	l1_clusterCellsLr1Eta711 = m_selElectrons[il1]->auxdata<std::vector<float>>("7x11ClusterLr1Eta");
	l1_clusterCellsLr1Phi711 = m_selElectrons[il1]->auxdata<std::vector<float>>("7x11ClusterLr1Phi");

  	l1_clusterCellsLr2E37 = m_selElectrons[il1]->auxdata<std::vector<float>>("3x7ClusterLr2E");
  	l1_clusterCellsLr2Eta37 = m_selElectrons[il1]->auxdata<std::vector<float>>("3x7ClusterLr2Eta");
  	l1_clusterCellsLr2Phi37 = m_selElectrons[il1]->auxdata<std::vector<float>>("3x7ClusterLr2Phi");
	
	l1_clusterCellsLr1E37 = m_selElectrons[il1]->auxdata<std::vector<float>>("3x7ClusterLr1E");
  	l1_clusterCellsLr1Eta37 = m_selElectrons[il1]->auxdata<std::vector<float>>("3x7ClusterLr1Eta");
  	l1_clusterCellsLr1Phi37 = m_selElectrons[il1]->auxdata<std::vector<float>>("3x7ClusterLr1Phi");

	l1_clusterCellsLr3E37 = m_selElectrons[il1]->auxdata<std::vector<float>>("3x7ClusterLr3E");
  	l1_clusterCellsLr3Eta37 = m_selElectrons[il1]->auxdata<std::vector<float>>("3x7ClusterLr3Eta");
  	l1_clusterCellsLr3Phi37 = m_selElectrons[il1]->auxdata<std::vector<float>>("3x7ClusterLr3Phi");

	l1_clusterCellsLr1E55 = m_selElectrons[il1]->auxdata<std::vector<float>>("5x5ClusterLr1E");
  	l1_clusterCellsLr1Eta55 = m_selElectrons[il1]->auxdata<std::vector<float>>("5x5ClusterLr1Eta");
  	l1_clusterCellsLr1Phi55 = m_selElectrons[il1]->auxdata<std::vector<float>>("5x5ClusterLr1Phi");

	l1_clusterCellsLr2E55 = m_selElectrons[il1]->auxdata<std::vector<float>>("5x5ClusterLr2E");
  	l1_clusterCellsLr2Eta55 = m_selElectrons[il1]->auxdata<std::vector<float>>("5x5ClusterLr2Eta");
  	l1_clusterCellsLr2Phi55 = m_selElectrons[il1]->auxdata<std::vector<float>>("5x5ClusterLr2Phi");

	l1_clusterCellsLr3E55 = m_selElectrons[il1]->auxdata<std::vector<float>>("5x5ClusterLr3E");
  	l1_clusterCellsLr3Eta55 = m_selElectrons[il1]->auxdata<std::vector<float>>("5x5ClusterLr3Eta");
  	l1_clusterCellsLr3Phi55 = m_selElectrons[il1]->auxdata<std::vector<float>>("5x5ClusterLr3Phi");
	

	l2_ClusterSize7x11Lr2 = m_selElectrons[il2]->auxdata<int>("7x11ClusterLr2Size");
	l2_ClusterSize3x7Lr2 = m_selElectrons[il2]->auxdata<int>("3x7ClusterLr2Size");
	l2_ClusterSize5x5Lr2 = m_selElectrons[il2]->auxdata<int>("5x5ClusterLr2Size");
	l2_ClusterSize7x11Lr1 = m_selElectrons[il2]->auxdata<int>("7x11ClusterLr1Size");
	l2_ClusterSize3x7Lr1 = m_selElectrons[il2]->auxdata<int>("3x7ClusterLr1Size");
	l2_ClusterSize5x5Lr1 = m_selElectrons[il2]->auxdata<int>("5x5ClusterLr1Size");
	l2_ClusterSize7x11Lr3 = m_selElectrons[il2]->auxdata<int>("7x11ClusterLr3Size");
	l2_ClusterSize3x7Lr3 = m_selElectrons[il2]->auxdata<int>("3x7ClusterLr3Size");
	l2_ClusterSize5x5Lr3 = m_selElectrons[il2]->auxdata<int>("5x5ClusterLr3Size");

	l2_clusterCellsLr2E711 = m_selElectrons[il2]->auxdata<std::vector<float>>("7x11ClusterLr2E");
 	l2_clusterCellsLr2Eta711 = m_selElectrons[il2]->auxdata<std::vector<float>>("7x11ClusterLr2Eta");
	l2_clusterCellsLr2Phi711 = m_selElectrons[il2]->auxdata<std::vector<float>>("7x11ClusterLr2Phi");

	l2_clusterCellsLr3E711 = m_selElectrons[il2]->auxdata<std::vector<float>>("7x11ClusterLr3E");
 	l2_clusterCellsLr3Eta711 = m_selElectrons[il2]->auxdata<std::vector<float>>("7x11ClusterLr3Eta");
	l2_clusterCellsLr3Phi711 = m_selElectrons[il2]->auxdata<std::vector<float>>("7x11ClusterLr3Phi");

	l2_clusterCellsLr1E711 = m_selElectrons[il2]->auxdata<std::vector<float>>("7x11ClusterLr1E");
 	l2_clusterCellsLr1Eta711 = m_selElectrons[il2]->auxdata<std::vector<float>>("7x11ClusterLr1Eta");
	l2_clusterCellsLr1Phi711 = m_selElectrons[il2]->auxdata<std::vector<float>>("7x11ClusterLr1Phi");

  	l2_clusterCellsLr2E37 = m_selElectrons[il2]->auxdata<std::vector<float>>("3x7ClusterLr2E");
  	l2_clusterCellsLr2Eta37 = m_selElectrons[il2]->auxdata<std::vector<float>>("3x7ClusterLr2Eta");
  	l2_clusterCellsLr2Phi37 = m_selElectrons[il2]->auxdata<std::vector<float>>("3x7ClusterLr2Phi");
	
	l2_clusterCellsLr1E37 = m_selElectrons[il2]->auxdata<std::vector<float>>("3x7ClusterLr1E");
  	l2_clusterCellsLr1Eta37 = m_selElectrons[il2]->auxdata<std::vector<float>>("3x7ClusterLr1Eta");
  	l2_clusterCellsLr1Phi37 = m_selElectrons[il2]->auxdata<std::vector<float>>("3x7ClusterLr1Phi");

	l2_clusterCellsLr3E37 = m_selElectrons[il2]->auxdata<std::vector<float>>("3x7ClusterLr3E");
  	l2_clusterCellsLr3Eta37 = m_selElectrons[il2]->auxdata<std::vector<float>>("3x7ClusterLr3Eta");
  	l2_clusterCellsLr3Phi37 = m_selElectrons[il2]->auxdata<std::vector<float>>("3x7ClusterLr3Phi");

	l2_clusterCellsLr1E55 = m_selElectrons[il2]->auxdata<std::vector<float>>("5x5ClusterLr1E");
  	l2_clusterCellsLr1Eta55 = m_selElectrons[il2]->auxdata<std::vector<float>>("5x5ClusterLr1Eta");
  	l2_clusterCellsLr1Phi55 = m_selElectrons[il2]->auxdata<std::vector<float>>("5x5ClusterLr1Phi");

	l2_clusterCellsLr2E55 = m_selElectrons[il2]->auxdata<std::vector<float>>("5x5ClusterLr2E");
  	l2_clusterCellsLr2Eta55 = m_selElectrons[il2]->auxdata<std::vector<float>>("5x5ClusterLr2Eta");
  	l2_clusterCellsLr2Phi55 = m_selElectrons[il2]->auxdata<std::vector<float>>("5x5ClusterLr2Phi");

	l2_clusterCellsLr3E55 = m_selElectrons[il2]->auxdata<std::vector<float>>("5x5ClusterLr3E");
  	l2_clusterCellsLr3Eta55 = m_selElectrons[il2]->auxdata<std::vector<float>>("5x5ClusterLr3Eta");
  	l2_clusterCellsLr3Phi55 = m_selElectrons[il2]->auxdata<std::vector<float>>("5x5ClusterLr3Phi");

	}
	
	}
	
	tree("NTUP")->Fill();

	ClearVariables();
}

void MyxAODAnalysis :: getPhotonsShowerShapeValues(const xAOD::Photon* object, float& rhad1, float& rhad, float& e277, float& reta, float& rphi, float& weta1, float& weta2, float& f1, float& f3, float& f3core, float& wtots1, float& deltae, float& eratio, float& fracs1, float& e011, float& e132, float& e237, float& e2tsts1, float& emins1, float& emaxs1, float& ethad, float& ethad1)
{
object->showerShapeValue(rhad1, xAOD::EgammaParameters::Rhad);
object->showerShapeValue(rhad, xAOD::EgammaParameters::Rhad1);
object->showerShapeValue(e277, xAOD::EgammaParameters::e277);
object->showerShapeValue(reta, xAOD::EgammaParameters::Reta);
object->showerShapeValue(rphi, xAOD::EgammaParameters::Rphi);
object->showerShapeValue(weta1, xAOD::EgammaParameters::weta1);
object->showerShapeValue(weta2, xAOD::EgammaParameters::weta2);
object->showerShapeValue(f1, xAOD::EgammaParameters::f1);
object->showerShapeValue(f3, xAOD::EgammaParameters::f3);
object->showerShapeValue(f3core, xAOD::EgammaParameters::f3core);
object->showerShapeValue(wtots1, xAOD::EgammaParameters::wtots1);
object->showerShapeValue(deltae, xAOD::EgammaParameters::DeltaE);
object->showerShapeValue(eratio, xAOD::EgammaParameters::Eratio);
object->showerShapeValue(fracs1, xAOD::EgammaParameters::fracs1);
object->showerShapeValue(e011, xAOD::EgammaParameters::e011);
object->showerShapeValue(e132, xAOD::EgammaParameters::e132);
object->showerShapeValue(e237, xAOD::EgammaParameters::e237);
object->showerShapeValue(e2tsts1, xAOD::EgammaParameters::e2tsts1);
object->showerShapeValue(emins1, xAOD::EgammaParameters::emins1);
object->showerShapeValue(emaxs1, xAOD::EgammaParameters::emaxs1);
object->showerShapeValue(ethad, xAOD::EgammaParameters::ethad);
object->showerShapeValue(ethad1, xAOD::EgammaParameters::ethad1);
}

double MyxAODAnalysis :: getSF(int il,TString Type)
{
	if(!isMC)
	{
		return 1.;
	}
	double SF = 1.;
	if(Type == "Electron")
	{
		double RecoSF;
		double IsoSF;
		Electron_RecoSF_tool->getEfficiencyScaleFactor(*(m_selElectrons[il]), RecoSF);
		Electron_IsoSF_tool->getEfficiencyScaleFactor(*(m_selElectrons[il]),  IsoSF);
		SF = RecoSF*IsoSF;
	}
	if(Type == "Muon")
	{
		float RecoSF;
		float IsoSF = 1.;
		Muon_RecoSF_tool->getEfficiencyScaleFactor(*(m_selMuons[il]), RecoSF);	
		SF = RecoSF*IsoSF;
	}

        if(Type == "PhotonTight")
	{
		Photon_IsoTighSF_tool->getEfficiencyScaleFactor(*(m_selPhotons[il]), SF);
	}
	if(Type == "PhotonLoose")
	{
		Photon_IsoLooseSF_tool->getEfficiencyScaleFactor(*(m_selPhotons[il]), SF);
	}
	if(Type == "PhotonTightCalo")
	{
		Photon_IsoTighCaloSF_tool->getEfficiencyScaleFactor(*(m_selPhotons[il]), SF);
	}

	return SF;	
}

void MyxAODAnalysis :: SelectZeeyEvents( std::vector< const xAOD::Photon* > photons, std::vector< const xAOD::Electron* > electrons )
{
	unsigned int nph = photons.size();
	unsigned int nel = electrons.size();
	if(nel < 2 || nph == 0) return;

	bool CheckTrigger = false ;
	if(passSingleElTrigger || passDoubleElTrigger){
		CheckTrigger = true;
	}else if(isMC){
		CheckTrigger = true;
	}
	if(CheckTrigger){
	for(unsigned int ph1 = 0; ph1<nph; ph1++)
	{
		for(unsigned int el1 = 0; el1<nel; el1++)
		{
			float q1 = electrons[el1]->charge();

			for(unsigned int el2 = el1+1; el2<nel; el2++)
			{

				float q2 = electrons[el2]->charge();
				float dr1 = ( photons[ph1]->p4() ).DeltaR( electrons[el1]->p4() );
				float dr2 = ( photons[ph1]->p4() ).DeltaR( electrons[el2]->p4() );
				float mee = ( electrons[el1]->p4() + electrons[el2]->p4()).M() * 1e-3;
				float meey =( electrons[el1]->p4() + electrons[el2]->p4() + photons[ph1]->p4()).M() * 1e-3;
				m_ee.push_back(mee);
				m_eey.push_back(meey);
				
				/*
				if(isMC)
				{
				if(!matchTrigger(el1,el2,"Zeey")) 
				{
					ANA_MSG_INFO("Electrons doesn't match Trigger");
				continue;
				}
				}
				*/

				if(q1*q2 > 0) continue;
				if(dr1 < mindREl || dr2 < mindREl) continue;
				if(mee < mllMin || mee > mllMax ) continue;
				if(meey < mllyMin || meey > mllyMax ) continue;

					ANA_MSG_INFO("Start storing Zeey event");
					StoreEvent (ph1, el1, el2, "Zeey");
			}
		}
	}
	}
	return;
}
void MyxAODAnalysis :: SelectZmumuyEvents( std::vector< const xAOD::Photon* > photons, std::vector< const xAOD::Muon* > muons )
{
	unsigned int nph = photons.size();
	unsigned int nmu = muons.size();
	if(nmu < 2 || nph == 0) return;

	bool CheckTrigger = false ;
	if(passSingleMuTrigger || passDoubleMuTrigger){
		CheckTrigger = true;
	}else if(isMC){
		CheckTrigger = true;
	}
	if(CheckTrigger){
	for(unsigned int ph1 = 0; ph1<nph; ph1++)
	{
		for(unsigned int mu1 = 0; mu1<nmu; mu1++)
		{
			float q1 = muons[mu1]->charge();
			for(unsigned int mu2 = mu1+1; mu2<nmu; mu2++)
			{
				float q2 = muons[mu2]->charge();
				float dr1 = ( photons[ph1]->p4() ).DeltaR( muons[mu1]->p4() );
				float dr2 = ( photons[ph1]->p4() ).DeltaR( muons[mu2]->p4() );
				float mmumu = ( muons[mu1]->p4() + muons[mu2]->p4()).M() * 1e-3;
				float mmumuy = ( muons[mu1]->p4() + muons[mu2]->p4() + photons[ph1]->p4()).M() * 1e-3;
				m_mumu.push_back(mmumu);
				m_mumuy.push_back(mmumuy);
				
				/*
				if(isMC)
				{
				if(!matchTrigger(mu1,mu2,"Zmumuy")) 
				{
					ANA_MSG_INFO("Muons doesn't match Trigger");
				continue;
				}
				}
				*/

				if(q1*q2 > 0) continue;
				if(dr1 < mindRMu || dr2 < mindRMu) continue;
				if(mmumu < mllMin || mmumu > mllMax ) continue;
				if(mmumuy < mllyMin || mmumuy > mllyMax ) continue;

				ANA_MSG_INFO("Start storing Zmumuy event");
				StoreEvent (ph1, mu1, mu2, "Zmumuy");
			}
		}
	}
	}
	return;
}


bool MyxAODAnalysis :: matchTrigger(int l1, int l2, TString Type)
{
	int cnt = 0.;
	for (auto thisTrig : triggerList) {

	if(Type == "Zmumuy")
	{
		const xAOD::Muon *mu1 = m_selMuons[l1];
      		const xAOD::Muon *mu2 = m_selMuons[l2];
		if (thisTrig.Contains("HLT_mu") && !thisTrig.Contains("_mu8noL1")) {
			std::vector<const xAOD::IParticle*> myParticles;
			myParticles.clear();
       			myParticles.push_back(mu1);
			if (Trigger_match_tool->match(myParticles, thisTrig.Data(), 0.1)) cnt++;
			myParticles.clear();
       			myParticles.push_back(mu2);
			if (Trigger_match_tool->match(myParticles, thisTrig.Data(), 0.1)) cnt++;
		}
	
		if (thisTrig.Contains("HLT_2mu") || thisTrig.Contains("_mu8noL1")) {
       		 	std::vector<const xAOD::IParticle*> myParticles;
        	 	myParticles.clear();
        	 	myParticles.push_back(mu1);
        	 	myParticles.push_back(mu2);
       		 	if(Trigger_match_tool->match(myParticles, thisTrig.Data(), 0.1)) cnt++; 
		}

	} 	
	if(Type == "Zeey")
	{
		const xAOD::Electron *el1 = m_selElectrons[l1];
      		const xAOD::Electron *el2 = m_selElectrons[l2];
		if (thisTrig.Contains("HLT_e")) {
			std::vector<const xAOD::IParticle*> myParticles;
			myParticles.clear();
       			myParticles.push_back(el1);
			if (Trigger_match_tool->match(myParticles, thisTrig.Data(), 0.07)) cnt++;
			myParticles.clear();
       			myParticles.push_back(el2);
			if (Trigger_match_tool->match(myParticles, thisTrig.Data(), 0.07)) cnt++;
		}
	
		if (thisTrig.Contains("HLT_2e")) {
       		 	std::vector<const xAOD::IParticle*> myParticles;
        	 	myParticles.clear();
        	 	myParticles.push_back(el1);
        	 	myParticles.push_back(el2);
       		 	if(Trigger_match_tool->match(myParticles, thisTrig.Data(), 0.07)) cnt++; 
		}
	} 
     	}
	return (cnt>0);
}


void MyxAODAnalysis :: decoratePhotonObject(const xAOD::Photon*& egamma, int m_eta_size, int m_phi_size){

if (not egamma or not egamma->caloCluster()) return;

  ANA_MSG_INFO( "Start decoration ... ");

  const CaloCellContainer* cellCont(0);
  xAOD::CaloCluster* egcClone(0);

  bool isBarrel = xAOD::EgammaHelpers::isBarrel(egamma);
  CaloCell_ID::CaloSample sample = isBarrel ? CaloCell_ID::EMB2 : CaloCell_ID::EME2;

  double etaCalo = 0.;
  if(! (egamma->caloCluster()->retrieveMoment(xAOD::CaloCluster::ETACALOFRAME, etaCalo))) {
    ANA_MSG_ERROR("Failed to retrieve eta from calo");
  }
  double phiCalo = 0.;
  if(! (egamma->caloCluster()->retrieveMoment(xAOD::CaloCluster::PHICALOFRAME, phiCalo))) {
    ANA_MSG_ERROR("Failed to retrieve phi from calo");
  }

  //double eta0 = egamma->caloCluster()->eta0() + etaCalo - egamma->caloCluster()->eta();
  //double phi0 = egamma->caloCluster()->phi0() + phiCalo - egamma->caloCluster()->phi();

  CaloCellDetPos  *ccdp; 
  double etamaxClus, phimaxClus, etamaxClusCalo, phimaxClusCalo;
  if (isBarrel) {
    etamaxClus = egamma->caloCluster()->etamax(CaloCell_ID::EMB2);
    phimaxClus = egamma->caloCluster()->phimax(CaloCell_ID::EMB2);
     }
  else {
    etamaxClus = egamma->caloCluster()->etamax(CaloCell_ID::EME2);
    phimaxClus = egamma->caloCluster()->phimax(CaloCell_ID::EME2);
      }

   ccdp->getDetPosition(sample, etamaxClus, phimaxClus, etamaxClusCalo, phimaxClusCalo); 
   caloFillRectangularTools.setTypeAndName(Form("CaloFillRectangularCluster/CaloFillRectangularCluster_%dx%d",m_eta_size,m_phi_size));
   StatusCode isDone = caloFillRectangularTools.retrieve();
   if( isDone != StatusCode::SUCCESS) return;
   Calotool = dynamic_cast<CaloFillRectangularCluster* >( &(*caloFillRectangularTools ));

   if (evtStore()->retrieve(cellCont,"AODCellContainer").isFailure())
   	ANA_MSG_WARNING("AODCellContainer" << " not found");
  else if (not egamma->author(xAOD::EgammaParameters::AuthorCaloTopo35) )
  {  
    	egcClone = CaloClusterStoreHelper::makeCluster( cellCont,
                                                    etamaxClusCalo,
                                                    phimaxClusCalo,
                                                    egamma->caloCluster()->clusterSize());
    	Calotool->makeCorrection(egcClone);
  }

  auto first_cell = egcClone->cell_begin();
  auto last_cell = egcClone->cell_end();
  int ncelll1 = 0 ;
  int ncelll2 = 0 ;
  int ncelll3 = 0;
  int npres = 0;
  int ncell = 0;   
  std::vector<float> EClusterLr1Eta, EClusterLr1Phi, EClusterLr1E, EClusterLr1X, EClusterLr1Y, EClusterLr1Z, EClusterLr1T; std::vector<bool> EClusterLr1isBAD;
  std::vector<float> EClusterLr2Eta, EClusterLr2Phi, EClusterLr2E, EClusterLr2X, EClusterLr2Y, EClusterLr2Z, EClusterLr2T; std::vector<bool> EClusterLr2isBAD;
  std::vector<float> EClusterLr3Eta, EClusterLr3Phi, EClusterLr3E, EClusterLr3X, EClusterLr3Y, EClusterLr3Z, EClusterLr3T; std::vector<bool> EClusterLr3isBAD;
  std::vector<float> EClusterPreSEta, EClusterPreSPhi, EClusterPreSE, EClusterPreSX, EClusterPreSY, EClusterPreSZ, EClusterPreST; std::vector<bool> EClusterPreSisBAD;

  for (;first_cell != last_cell; ++first_cell,++ncell) {
    const CaloCell* tcell = *first_cell;
    int sampling = tcell->caloDDE()->getSampling();
    
    if( sampling== CaloCell_ID::EMB1 || sampling== CaloCell_ID::EME1 ){
      EClusterLr1Eta.push_back(tcell->eta());
      EClusterLr1Phi.push_back(tcell->phi());
      EClusterLr1E.push_back(tcell->e());

      EClusterLr1X.push_back(tcell->x());
      EClusterLr1Y.push_back(tcell->y());
      EClusterLr1Z.push_back(tcell->z());
      EClusterLr1T.push_back(tcell->time());
      EClusterLr1isBAD.push_back(tcell->badcell());     	

      ncelll1++;
    }
    
    if(sampling== CaloCell_ID::EMB2 || sampling== CaloCell_ID::EME2 ){
      EClusterLr2Eta.push_back(tcell->eta());
      EClusterLr2Phi.push_back(tcell->phi());
      EClusterLr2E.push_back(tcell->e());

      EClusterLr2X.push_back(tcell->x());
      EClusterLr2Y.push_back(tcell->y());
      EClusterLr2Z.push_back(tcell->z());
      EClusterLr2T.push_back(tcell->time());
      EClusterLr2isBAD.push_back(tcell->badcell());

      ncelll2++;
    }
    
    if(sampling== CaloCell_ID::EMB3 || sampling== CaloCell_ID::EME3 ){
      EClusterLr3Eta.push_back(tcell->eta());
      EClusterLr3Phi.push_back(tcell->phi());
      EClusterLr3E.push_back(tcell->e());

      EClusterLr3X.push_back(tcell->x());
      EClusterLr3Y.push_back(tcell->y());
      EClusterLr3Z.push_back(tcell->z());
      EClusterLr3T.push_back(tcell->time());
      EClusterLr3isBAD.push_back(tcell->badcell());

      ncelll3++;
    }
    
    if( sampling== CaloCell_ID::PreSamplerB || sampling== CaloCell_ID::PreSamplerE ){
      EClusterPreSEta.push_back(tcell->eta());
      EClusterPreSPhi.push_back(tcell->phi());
      EClusterPreSE.push_back(tcell->e());

      EClusterPreSX.push_back(tcell->x());
      EClusterPreSY.push_back(tcell->y());
      EClusterPreSZ.push_back(tcell->z());
      EClusterPreST.push_back(tcell->time());
      EClusterPreSisBAD.push_back(tcell->badcell());

      npres++;
    }
  }
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr1Eta",m_eta_size,m_phi_size)) = EClusterLr1Eta;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr1Phi",m_eta_size,m_phi_size)) = EClusterLr1Phi;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr1E",m_eta_size,m_phi_size)) = EClusterLr1E;

  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr1X",m_eta_size,m_phi_size)) = EClusterLr1X;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr1Y",m_eta_size,m_phi_size)) = EClusterLr1Y;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr1Z",m_eta_size,m_phi_size)) = EClusterLr1Z;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr1T",m_eta_size,m_phi_size)) = EClusterLr1T;
  egamma->auxdecor<std::vector<bool>>(Form("%dx%dClusterLr1isBAD",m_eta_size,m_phi_size)) = EClusterLr1isBAD;

  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr2Eta",m_eta_size,m_phi_size)) = EClusterLr2Eta;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr2Phi",m_eta_size,m_phi_size)) = EClusterLr2Phi;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr2E",m_eta_size,m_phi_size)) = EClusterLr2E;

  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr2X",m_eta_size,m_phi_size)) = EClusterLr2X;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr2Y",m_eta_size,m_phi_size)) = EClusterLr2Y;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr2Z",m_eta_size,m_phi_size)) = EClusterLr2Z;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr2T",m_eta_size,m_phi_size)) = EClusterLr2T;
  egamma->auxdecor<std::vector<bool>>(Form("%dx%dClusterLr2isBAD",m_eta_size,m_phi_size)) = EClusterLr2isBAD;

  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr3Eta",m_eta_size,m_phi_size)) = EClusterLr3Eta;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr3Phi",m_eta_size,m_phi_size)) = EClusterLr3Phi;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr3E",m_eta_size,m_phi_size)) = EClusterLr3E;

  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr3X",m_eta_size,m_phi_size)) = EClusterLr3X;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr3Y",m_eta_size,m_phi_size)) = EClusterLr3Y;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr3Z",m_eta_size,m_phi_size)) = EClusterLr3Z;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr3T",m_eta_size,m_phi_size)) = EClusterLr3T;
  egamma->auxdecor<std::vector<bool>>(Form("%dx%dClusterLr3isBAD",m_eta_size,m_phi_size)) = EClusterLr3isBAD;
  
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterPreSEta",m_eta_size,m_phi_size)) = EClusterPreSEta;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterPreSPhi",m_eta_size,m_phi_size)) = EClusterPreSPhi;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterPreSE",m_eta_size,m_phi_size)) = EClusterPreSE;

  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterPreSX",m_eta_size,m_phi_size)) = EClusterPreSX;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterPreSY",m_eta_size,m_phi_size)) = EClusterPreSY;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterPreSZ",m_eta_size,m_phi_size)) = EClusterPreSZ;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterPreST",m_eta_size,m_phi_size)) = EClusterPreST;
  egamma->auxdecor<std::vector<bool>>(Form("%dx%dClusterPreSisBAD",m_eta_size,m_phi_size)) = EClusterPreSisBAD;
  
  egamma->auxdecor<int>(Form("%dx%dClusterLr1Size",m_eta_size, m_phi_size)) = ncelll1;
  egamma->auxdecor<int>(Form("%dx%dClusterLr2Size",m_eta_size, m_phi_size)) = ncelll2;
  egamma->auxdecor<int>(Form("%dx%dClusterLr3Size",m_eta_size, m_phi_size)) = ncelll3;
  egamma->auxdecor<int>(Form("%dx%dClusterPreSSize",m_eta_size, m_phi_size)) = npres;

 std::vector<unsigned int> m_layers = {0,1,2,3};
  for (unsigned int layer : m_layers)
    egamma->auxdecor<float>(Form("E%dx%d_Lr%d", m_eta_size, m_phi_size, layer)) =\
      egcClone ? egcClone->energyBE(layer) : 0.;

  delete egcClone;

}

void MyxAODAnalysis :: decorateElectronObject(const xAOD::Electron*& egamma, int m_eta_size, int m_phi_size){

if (not egamma or not egamma->caloCluster()) return;

  ANA_MSG_INFO( "Start decoration ... ");

  const CaloCellContainer* cellCont(0);
  xAOD::CaloCluster* egcClone(0);

  bool isBarrel = xAOD::EgammaHelpers::isBarrel(egamma);
  CaloCell_ID::CaloSample sample = isBarrel ? CaloCell_ID::EMB2 : CaloCell_ID::EME2;

  double etaCalo = 0.;
  if(! (egamma->caloCluster()->retrieveMoment(xAOD::CaloCluster::ETACALOFRAME, etaCalo))) {
    ANA_MSG_ERROR("Failed to retrieve eta from calo");
  }
  double phiCalo = 0.;
  if(! (egamma->caloCluster()->retrieveMoment(xAOD::CaloCluster::PHICALOFRAME, phiCalo))) {
    ANA_MSG_ERROR("Failed to retrieve phi from calo");
  }

  //double eta0 = egamma->caloCluster()->eta0() + etaCalo - egamma->caloCluster()->eta();
  //double phi0 = egamma->caloCluster()->phi0() + phiCalo - egamma->caloCluster()->phi();

  CaloCellDetPos  *ccdp; 
  double etamaxClus, phimaxClus, etamaxClusCalo, phimaxClusCalo;
  if (isBarrel) {
    etamaxClus = egamma->caloCluster()->etamax(CaloCell_ID::EMB2);
    phimaxClus = egamma->caloCluster()->phimax(CaloCell_ID::EMB2);
     }
  else {
    etamaxClus = egamma->caloCluster()->etamax(CaloCell_ID::EME2);
    phimaxClus = egamma->caloCluster()->phimax(CaloCell_ID::EME2);
      }

   ccdp->getDetPosition(sample, etamaxClus, phimaxClus, etamaxClusCalo, phimaxClusCalo); 
   caloFillRectangularTools.setTypeAndName(Form("CaloFillRectangularCluster/CaloFillRectangularCluster_%dx%d",m_eta_size,m_phi_size));
   StatusCode isDone = caloFillRectangularTools.retrieve();
   if( isDone != StatusCode::SUCCESS) return;
   Calotool = dynamic_cast<CaloFillRectangularCluster* >( &(*caloFillRectangularTools ));

   if (evtStore()->retrieve(cellCont,"AODCellContainer").isFailure())
   	ANA_MSG_WARNING("AODCellContainer" << " not found");
  else if (not egamma->author(xAOD::EgammaParameters::AuthorCaloTopo35) )
  {  
    	egcClone = CaloClusterStoreHelper::makeCluster( cellCont,
                                                    etamaxClusCalo,
                                                    phimaxClusCalo,
                                                    egamma->caloCluster()->clusterSize());
    	Calotool->makeCorrection(egcClone);
  }

  auto first_cell = egcClone->cell_begin();
  auto last_cell = egcClone->cell_end();
  int ncelll1 = 0 ;
  int ncelll2 = 0 ;
  int ncelll3 = 0;
  int npres = 0;
  int ncell = 0;   
  std::vector<float>EClusterLr1Eta, EClusterLr1Phi, EClusterLr1E;
  std::vector<float>EClusterLr2Eta, EClusterLr2Phi, EClusterLr2E;
  std::vector<float>EClusterLr3Eta, EClusterLr3Phi, EClusterLr3E;
  std::vector<float>EClusterPreSEta, EClusterPreSPhi, EClusterPreSE;

  for (;first_cell != last_cell; ++first_cell,++ncell) {
    const CaloCell* tcell = *first_cell;
    int sampling = tcell->caloDDE()->getSampling();
    
    if( sampling== CaloCell_ID::EMB1 || sampling== CaloCell_ID::EME1 ){
      EClusterLr1Eta.push_back(tcell->eta());
      EClusterLr1Phi.push_back(tcell->phi());
      EClusterLr1E.push_back(tcell->e());   
	//std::cout<<tcell->weight()<<std::endl;   
      ncelll1++;
    }
    
    if(sampling== CaloCell_ID::EMB2 || sampling== CaloCell_ID::EME2 ){
      EClusterLr2Eta.push_back(tcell->eta());
      EClusterLr2Phi.push_back(tcell->phi());
      EClusterLr2E.push_back(tcell->e());
      ncelll2++;
    }
    
    if(sampling== CaloCell_ID::EMB3 || sampling== CaloCell_ID::EME3 ){
      EClusterLr3Eta.push_back(tcell->eta());
      EClusterLr3Phi.push_back(tcell->phi());
      EClusterLr3E.push_back(tcell->e());
      ncelll3++;
    }
    
    if( sampling== CaloCell_ID::PreSamplerB || sampling== CaloCell_ID::PreSamplerE ){
      EClusterPreSEta.push_back(tcell->eta());
      EClusterPreSPhi.push_back(tcell->phi());
      EClusterPreSE.push_back(tcell->e());
      npres++;
    }
  }
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr1Eta",m_eta_size,m_phi_size)) = EClusterLr1Eta;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr1Phi",m_eta_size,m_phi_size)) = EClusterLr1Phi;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr1E",m_eta_size,m_phi_size)) = EClusterLr1E;

  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr2Eta",m_eta_size,m_phi_size)) = EClusterLr2Eta;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr2Phi",m_eta_size,m_phi_size)) = EClusterLr2Phi;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr2E",m_eta_size,m_phi_size)) = EClusterLr2E;

  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr3Eta",m_eta_size,m_phi_size)) = EClusterLr3Eta;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr3Phi",m_eta_size,m_phi_size)) = EClusterLr3Phi;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterLr3E",m_eta_size,m_phi_size)) = EClusterLr3E;
  
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterPreSEta",m_eta_size,m_phi_size)) = EClusterPreSEta;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterPreSPhi",m_eta_size,m_phi_size)) = EClusterPreSPhi;
  egamma->auxdecor<std::vector<float>>(Form("%dx%dClusterPreSE",m_eta_size,m_phi_size)) = EClusterPreSE;
  
  egamma->auxdecor<int>(Form("%dx%dClusterLr1Size",m_eta_size, m_phi_size)) = ncelll1;
  egamma->auxdecor<int>(Form("%dx%dClusterLr2Size",m_eta_size, m_phi_size)) = ncelll2;
  egamma->auxdecor<int>(Form("%dx%dClusterLr3Size",m_eta_size, m_phi_size)) = ncelll3;
  egamma->auxdecor<int>(Form("%dx%dClusterPreSSize",m_eta_size, m_phi_size)) = npres;

 std::vector<unsigned int> m_layers = {0,1,2,3};
  for (unsigned int layer : m_layers)
    egamma->auxdecor<float>(Form("E%dx%d_Lr%d", m_eta_size, m_phi_size, layer)) =\
      egcClone ? egcClone->energyBE(layer) : 0.;

  delete egcClone;

}

int MyxAODAnalysis :: ApplySinglePhotonSelection()
{

	const xAOD::PhotonContainer *photons = nullptr;
  	StatusCode cc = evtStore()->retrieve( photons, "Photons" );

	std::pair< xAOD::PhotonContainer*, xAOD::ShallowAuxContainer*> PhotonContainers = xAOD::shallowCopyContainer(*photons);
	xAOD::PhotonContainer::iterator ph = PhotonContainers.first->begin();
  	xAOD::PhotonContainer::iterator phLast = PhotonContainers.first->end();
	
     //   static SG::AuxElement::ConstAccessor<uint16_t> author("author");

	int i = 0;
	//loop over photons
	for(; ph !=phLast; ++ph)
  	{
			
		if (!passAuthorCut((*ph))) continue;

		Egamma_calib_tool->setRandomSeed(eventInfo->eventNumber()+100*i++);
		Egamma_calib_tool->applyCorrection(**ph);
		
		if( !(*ph)->isGoodOQ(xAOD::EgammaParameters::BADCLUSPHOTON)) continue;
		if( !(!( ((*ph)->OQ()&1073741824)!=0 || ( ((*ph)->OQ()&134217728)!=0 && ((*ph)->showerShapeValue(xAOD::EgammaParameters::Reta) > 0.98
        || (*ph)->showerShapeValue(xAOD::EgammaParameters::f1) > 0.4 || ((*ph)->OQ()&67108864)!=0)) )) ) continue;
			
		if ( !m_fakeTool->accept(*(*ph))) continue;
					
		if ( !m_deadHVTool->accept(*(*ph))) continue;
						
		const float energy =  ( (*ph)->caloCluster() ) ? (*ph)->caloCluster()->e()*1e-3 : -999.0; 
 		double eta = ( (*ph)->caloCluster() ) ? (*ph)->caloCluster()->etaBE(2) : -999.0;
		const float cosheta = cosh(eta);
		const float e = (cosheta != 0.) ? energy/cosheta : 0.;

		if( (*ph)->pt() *1e-3 < 7. ) continue;
		if( fabs(eta) > 2.5 ) continue;
		//if( fabs(eta) > 1.37 && fabs(eta) < 1.52 ) continue;

		bool isloose = Photon_loose_tool->accept(*(*ph));
		bool istight = Photon_tight_tool->accept(*(*ph));
		bool istightCaloOnly = Photon_tightCaloOnly_tool->accept(*(*ph));

		bool isEMLoose_OFF = Photon_Off_isEMLoose_tool->accept((*ph));
		bool isEMTightInc_OFF = Photon_Off_isEMTightInc_tool->accept((*ph));
		bool isEMTightDep_OFF = Photon_Off_isEMTightDep_tool->accept((*ph));

		bool isEMLoose_ON = Photon_On_isEMLoose_tool->accept((*ph));
		bool isEMTight_ON = Photon_On_isEMTight_tool->accept((*ph));
		bool isEMMedium_ON = Photon_On_isEMMedium_tool->accept((*ph));

		static SG::AuxElement::Decorator< bool > isLoose( "ph_isLoose" );
		static SG::AuxElement::Decorator< bool > isTight( "ph_isTight" );
		static SG::AuxElement::Decorator< bool > isCaloTight( "ph_isCaloTight" );

		static SG::AuxElement::Decorator< bool > isEMLooseOFF( "ph_isEMLooseOFF" );
		static SG::AuxElement::Decorator< bool > isEMTightIncOFF( "ph_isEMTightIncOFF" );
		static SG::AuxElement::Decorator< bool > isEMTightDepOFF( "ph_isEMTightDepOFF" );

		static SG::AuxElement::Decorator< bool > isEMLooseON( "ph_isEMLooseON" );
		static SG::AuxElement::Decorator< bool > isEMTightON( "ph_isEMTightON" );
		static SG::AuxElement::Decorator< bool > isEMMediumON( "ph_isEMMediumON" );

		static SG::AuxElement::Decorator< float > Conv( "ConvertionType" );

		isLoose(*(*ph)) = isloose;
		isTight(*(*ph)) = istight;
		
 		isCaloTight(*(*ph)) = istightCaloOnly;
		Conv(*(*ph)) = (*(*ph)).conversionType();

		isEMLooseOFF( *(*ph) )    = isEMLoose_OFF;
		isEMTightIncOFF( *(*ph) ) = isEMTightInc_OFF;
		isEMTightDepOFF( *(*ph) ) = isEMTightDep_OFF;

		isEMLooseON( *(*ph) ) = isEMLoose_ON;
		isEMTightON( *(*ph) ) = isEMTight_ON;
		isEMMediumON( *(*ph)) = isEMMedium_ON;

		//if(!istight) continue;

		m_singlePhotons.push_back((*ph));
	}
	return m_singlePhotons.size();
}

void MyxAODAnalysis :: StoreSinglePhoton()
{
	//ClearVariables();
	//ANA_MSG_INFO("Start Filling the Tree");
	
	for(int i = 0; i < m_singlePhotons.size(); i++)
	{
	const xAOD::Photon *photon = m_singlePhotons[i];

	decorateWithTruthInfo(photon);

	bool isTruth = photon->auxdata<bool>("ph_isTruthMatch");
	
	if(isTruth){

		ClearVariables();
		delete photon;
		continue;
	}else{
	
	ph_pt             = photon->pt();
        ph_eta            = photon->eta();
	ph_phi            = photon->phi();
	ph_E              = photon->e();
	ph_topoetcone20   = photon->auxdata<float>("topoetcone20");
	ph_topoetcone30   = photon->auxdata<float>("topoetcone30");
	ph_topoetcone40   = photon->auxdata<float>("topoetcone40");
	ph_ptcone20       = photon->auxdata<float>("ptcone20");
	ph_ptcone30       = photon->auxdata<float>("ptcone30");
	ph_ptcone40       = photon->auxdata<float>("ptcone40");
	ph_etcone20       = photon->auxdata<float>("etcone20");
	ph_etcone30       = photon->auxdata<float>("etcone30");
	ph_etcone40       = photon->auxdata<float>("etcone40");
	ph_ptvarcone20    = photon->auxdata<float>("ptvarcone20");
	ph_ptvarcone30    = photon->auxdata<float>("ptvarcone30");
	ph_ptvarcone40    = photon->auxdata<float>("ptvarcone40");
	ph_isIsoLoose     = photon->auxdata<bool>("ph_isLoose");
	ph_isIsoTightCalo = photon->auxdata<bool>("ph_isCaloTight");
	ph_isIsoTight     = photon->auxdata<bool>("ph_isTight");
	ph_conv           = photon->auxdata<float>("ConvertionType");

	ph_isEMLooseOff    = photon->auxdata<bool>("ph_isEMLooseOFF");
	ph_isEMTightIncOff = photon->auxdata<bool>("ph_isEMTightIncOFF");
	ph_isEMTightDepOff = photon->auxdata<bool>("ph_isEMTightDepOFF");
	ph_isEMLooseOn     = photon->auxdata<bool>("ph_isEMLooseON");
	ph_isEMTightOn     = photon->auxdata<bool>("ph_isEMTightON");
	ph_isEMMediumOn    = photon->auxdata<bool>("ph_isEMMediumON");

	Photon_IsoTighSF_tool->getEfficiencyScaleFactor(*(photon), ph_IsoTightSF );
	
	Photon_IsoLooseSF_tool->getEfficiencyScaleFactor(*(photon), ph_IsoLooseSF);

	Photon_IsoTighCaloSF_tool->getEfficiencyScaleFactor(*(photon), ph_IsoTightCaloSF);

	//decorateWithTruthInfo(photon);
	
	ph_PDG		          = photon->auxdata<int>("ph_PDGID");
	ph_isTruthMatch		  = photon->auxdata<bool>("ph_isTruthMatch");
	ph_TruthConv		  = photon->auxdata<bool>("ph_TruthConversion");	

	float rhad1,rhad,e277,reta,rphi,weta1,weta2,f1,f3,f3core,wtots1,deltae,eratio,fracs1,e011,e132,e237,e2tsts1,emins1,emaxs1,ethad,ethad1;
	//get shower shape
	getPhotonsShowerShapeValues(photon,rhad1,rhad,e277,reta,rphi,weta1,weta2,f1,f3,f3core,wtots1,deltae,eratio,fracs1,e011,e132,e237,e2tsts1,emins1,emaxs1,ethad,ethad1);
	//push shower shape
	ph_Rhad1   = rhad1;
	ph_Rhad    = rhad;
	ph_E277    = e277;
	ph_Reta    = reta;
	ph_Rphi    = rphi;
	ph_Weta1   = weta1;
	ph_Weta2   = weta2;
	ph_f1      = f1;
	ph_f3      = f3;
	ph_f3core  = f3core;
	ph_fracs1  = fracs1;
	ph_Wtots1  = wtots1;
	ph_deltaE  = deltae;
	ph_Eratio  = eratio;
	ph_E011    = e011;
	ph_E132    = e132;
	ph_E237    = e237;
	ph_E2tsts1 = e2tsts1;
	ph_Emins1  = emins1;
	ph_Emaxs1  = emaxs1;
	ph_Ethad   = ethad; 
	ph_Ethad1  = ethad1;

	if(isAOD == true)
	{
		decoratePhotonObject(photon,3,7);
		decoratePhotonObject(photon,5,5);
		decoratePhotonObject(photon,7,11);
	}

	if(retrieveCells)
	{

	ph_ClusterSize7x11Lr2 = photon->auxdata<int>("7x11ClusterLr2Size");
	ph_ClusterSize3x7Lr2 = photon->auxdata<int>("3x7ClusterLr2Size");
	ph_ClusterSize5x5Lr2 = photon->auxdata<int>("5x5ClusterLr2Size");
	ph_ClusterSize7x11Lr1 = photon->auxdata<int>("7x11ClusterLr1Size");
	ph_ClusterSize3x7Lr1 = photon->auxdata<int>("3x7ClusterLr1Size");
	ph_ClusterSize5x5Lr1 = photon->auxdata<int>("5x5ClusterLr1Size");
	ph_ClusterSize7x11Lr3 = photon->auxdata<int>("7x11ClusterLr3Size");
	ph_ClusterSize3x7Lr3 = photon->auxdata<int>("3x7ClusterLr3Size");
	ph_ClusterSize5x5Lr3 = photon->auxdata<int>("5x5ClusterLr3Size");
	ph_ClusterSize7x11PreS = photon->auxdata<int>("7x11ClusterPreSSize");
	ph_ClusterSize3x7PreS = photon->auxdata<int>("3x7ClusterPreSSize");
	ph_ClusterSize5x5PreS = photon->auxdata<int>("5x5ClusterPreSSize");
	
	//77 Lr2

	ph_clusterCellsLr2E711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2E");
 	ph_clusterCellsLr2Eta711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2Eta");
	ph_clusterCellsLr2Phi711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2Phi");
 	ph_clusterCellsLr2X711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2X");
	ph_clusterCellsLr2Y711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2Y");
 	ph_clusterCellsLr2Z711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2Z");
	ph_clusterCellsLr2T711 = photon->auxdata<std::vector<float>>("7x11ClusterLr2T");
 	ph_clusterCellsLr2isBAD711 = photon->auxdata<std::vector<bool>>("7x11ClusterLr2isBAD");

	ph_clusterCellsLr3E711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3E");
 	ph_clusterCellsLr3Eta711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3Eta");
	ph_clusterCellsLr3Phi711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3Phi");
 	ph_clusterCellsLr3X711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3X");
	ph_clusterCellsLr3Y711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3Y");
 	ph_clusterCellsLr3Z711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3Z");
	ph_clusterCellsLr3T711 = photon->auxdata<std::vector<float>>("7x11ClusterLr3T");
 	ph_clusterCellsLr3isBAD711 = photon->auxdata<std::vector<bool>>("7x11ClusterLr3isBAD");

	ph_clusterCellsLr1E711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1E");
 	ph_clusterCellsLr1Eta711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1Eta");
	ph_clusterCellsLr1Phi711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1Phi");
 	ph_clusterCellsLr1X711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1X");
	ph_clusterCellsLr1Y711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1Y");
 	ph_clusterCellsLr1Z711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1Z");
	ph_clusterCellsLr1T711 = photon->auxdata<std::vector<float>>("7x11ClusterLr1T");
 	ph_clusterCellsLr1isBAD711 = photon->auxdata<std::vector<bool>>("7x11ClusterLr1isBAD");

	ph_clusterCellsPreSE711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSE");
 	ph_clusterCellsPreSEta711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSEta");
	ph_clusterCellsPreSPhi711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSPhi");
 	ph_clusterCellsPreSX711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSX");
	ph_clusterCellsPreSY711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSY");
 	ph_clusterCellsPreSZ711 = photon->auxdata<std::vector<float>>("7x11ClusterPreSZ");
	ph_clusterCellsPreST711 = photon->auxdata<std::vector<float>>("7x11ClusterPreST");
 	ph_clusterCellsPreSisBAD711 = photon->auxdata<std::vector<bool>>("7x11ClusterPreSisBAD");

  	ph_clusterCellsLr2E37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2E");
  	ph_clusterCellsLr2Eta37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2Eta");
  	ph_clusterCellsLr2Phi37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2Phi");
  	ph_clusterCellsLr2X37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2X");
  	ph_clusterCellsLr2Y37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2Y");
  	ph_clusterCellsLr2Z37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2Z");
  	ph_clusterCellsLr2T37 = photon->auxdata<std::vector<float>>("3x7ClusterLr2T");
  	ph_clusterCellsLr2isBAD37 = photon->auxdata<std::vector<bool>>("3x7ClusterLr2isBAD");

	ph_clusterCellsLr1E37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1E");
  	ph_clusterCellsLr1Eta37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1Eta");
  	ph_clusterCellsLr1Phi37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1Phi");
	ph_clusterCellsLr1X37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1X");
  	ph_clusterCellsLr1Y37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1Y");
  	ph_clusterCellsLr1Z37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1Z");
  	ph_clusterCellsLr1T37 = photon->auxdata<std::vector<float>>("3x7ClusterLr1T");
  	ph_clusterCellsLr1isBAD37 = photon->auxdata<std::vector<bool>>("3x7ClusterLr1isBAD");

	ph_clusterCellsLr3E37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3E");
  	ph_clusterCellsLr3Eta37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3Eta");
  	ph_clusterCellsLr3Phi37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3Phi");
	ph_clusterCellsLr3X37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3X");
  	ph_clusterCellsLr3Y37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3Y");
  	ph_clusterCellsLr3Z37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3Z");
  	ph_clusterCellsLr3T37 = photon->auxdata<std::vector<float>>("3x7ClusterLr3T");
  	ph_clusterCellsLr3isBAD37 = photon->auxdata<std::vector<bool>>("3x7ClusterLr3isBAD");

	ph_clusterCellsPreSE37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSE");
  	ph_clusterCellsPreSEta37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSEta");
  	ph_clusterCellsPreSPhi37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSPhi");
	ph_clusterCellsPreSX37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSX");
  	ph_clusterCellsPreSY37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSY");
  	ph_clusterCellsPreSZ37 = photon->auxdata<std::vector<float>>("3x7ClusterPreSZ");
  	ph_clusterCellsPreST37 = photon->auxdata<std::vector<float>>("3x7ClusterPreST");
  	ph_clusterCellsPreSisBAD37 = photon->auxdata<std::vector<bool>>("3x7ClusterPreSisBAD");

	ph_clusterCellsLr1E55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1E");
  	ph_clusterCellsLr1Eta55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1Eta");
  	ph_clusterCellsLr1Phi55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1Phi");
	ph_clusterCellsLr1X55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1X");
  	ph_clusterCellsLr1Y55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1Y");
  	ph_clusterCellsLr1Z55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1Z");
	ph_clusterCellsLr1T55 = photon->auxdata<std::vector<float>>("5x5ClusterLr1T");
  	ph_clusterCellsLr1isBAD55 = photon->auxdata<std::vector<bool>>("5x5ClusterLr1isBAD");

	ph_clusterCellsLr2E55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2E");
  	ph_clusterCellsLr2Eta55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2Eta");
  	ph_clusterCellsLr2Phi55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2Phi");
	ph_clusterCellsLr2X55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2X");
  	ph_clusterCellsLr2Y55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2Y");
  	ph_clusterCellsLr2Z55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2Z");
	ph_clusterCellsLr2T55 = photon->auxdata<std::vector<float>>("5x5ClusterLr2T");
  	ph_clusterCellsLr2isBAD55 = photon->auxdata<std::vector<bool>>("5x5ClusterLr2isBAD");

	ph_clusterCellsLr3E55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3E");
  	ph_clusterCellsLr3Eta55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3Eta");
  	ph_clusterCellsLr3Phi55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3Phi");
	ph_clusterCellsLr3X55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3X");
  	ph_clusterCellsLr3Y55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3Y");
  	ph_clusterCellsLr3Z55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3Z");
	ph_clusterCellsLr3T55 = photon->auxdata<std::vector<float>>("5x5ClusterLr3T");
  	ph_clusterCellsLr3isBAD55 = photon->auxdata<std::vector<bool>>("5x5ClusterLr3isBAD");

	ph_clusterCellsPreSE55   = photon->auxdata<std::vector<float>>("5x5ClusterPreSE");
  	ph_clusterCellsPreSEta55 = photon->auxdata<std::vector<float>>("5x5ClusterPreSEta");
  	ph_clusterCellsPreSPhi55 = photon->auxdata<std::vector<float>>("5x5ClusterPreSPhi");
	ph_clusterCellsPreSX55   = photon->auxdata<std::vector<float>>("5x5ClusterPreSX");
  	ph_clusterCellsPreSY55   = photon->auxdata<std::vector<float>>("5x5ClusterPreSY");
  	ph_clusterCellsPreSZ55   = photon->auxdata<std::vector<float>>("5x5ClusterPreSZ");
	ph_clusterCellsPreST55   = photon->auxdata<std::vector<float>>("5x5ClusterPreST");
  	ph_clusterCellsPreSisBAD55 = photon->auxdata<std::vector<bool>>("5x5ClusterPreSisBAD");

	}
	ANA_MSG_INFO("Start Filling the Tree");
	tree("NTUP")->Fill();

	//ClearVariables();
	//delete photon;
	}
	ClearVariables();
	delete photon;
	
	}
	ClearVariables();
}

void MyxAODAnalysis :: decorateWithTruthInfo(const xAOD::Photon*& egamma)
{
	const xAOD::TruthParticle *truthPh = xAOD::TruthHelpers::getTruthParticle(*egamma);

	bool isTruthMatch = false;
	bool TruthConversion = false;
	int PDG = -99;

	if(truthPh != nullptr && isMC)
	{

	bool isStable = truthPh->status() == 1 && truthPh->barcode() < 200000;
	bool isPhoton = MCUtils::PID::isPhoton(truthPh->pdgId());
	bool isNotFromHadron = notFromHadron(truthPh);

	isTruthMatch = isStable && isPhoton && isNotFromHadron;

	TruthConversion = xAOD::EgammaHelpers::isTrueConvertedPhoton(truthPh);

	PDG = truthPh->pdgId();

	}

	static SG::AuxElement::Decorator< bool > TruthMatch( "ph_isTruthMatch" );
	static SG::AuxElement::Decorator< bool > isTruthConv( "ph_TruthConversion" );
	static SG::AuxElement::Decorator< int > TruthPDG( "ph_PDGID" );
	TruthMatch((*egamma))   = isTruthMatch;
	isTruthConv((*egamma))  = TruthConversion;
	TruthPDG((*egamma))     = PDG;

	
}

bool MyxAODAnalysis :: notFromHadron(const xAOD::TruthParticle *ptcl)
  {
    int ID = ptcl->pdgId();

    // if the particle is a hadron, return false
    if (MCUtils::PID::isHadron(ID)) { return false; }

    // if there are no parents, not from hadron
    if (ptcl->nParents() == 0) { return true; }

    const xAOD::TruthParticle *parent = ptcl->parent(0);
    int parentID = parent->pdgId();

    if (MCUtils::PID::isHadron(parentID)) { return false; } // from hadron!

    if (MCUtils::PID::isTau(parentID) || parentID == ID) { return notFromHadron(parent); }

    // if we get here, all is good
    return true;
}
bool MyxAODAnalysis :: passAuthorCut(const xAOD::Photon *gam)
  {
    uint16_t author = gam->author();

    if (author & xAOD::EgammaParameters::AuthorPhoton) { return true; }

    if (author & xAOD::EgammaParameters::AuthorAmbiguous) { return true; }

    return false;
}



