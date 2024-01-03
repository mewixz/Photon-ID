///////////////////////// -*- C++ -*- /////////////////////////////
/*
  Copyright (C) 2019-2020 CERN for the benefit of the ATLAS collaboration
*/
// CellsHandler.cxx
// Source file for class CellsHandler
// Author: BELFKIR Mohamed 
// Email : mohamed.belfkir@cern.ch
/////////////////////////////////////////////////////////////////// 

#include "DerivationFrameworkCNNPID/CellsHandler.h"

DerivationFramework::CellsHandler::CellsHandler(std::string t, int eta, int phi) : m_name(t), m_n_eta(eta), m_n_phi(phi){
	
}

DerivationFramework::CellsHandler::~CellsHandler() {
}

StatusCode DerivationFramework::CellsHandler::finalize() {

        return StatusCode::SUCCESS;
}


StatusCode DerivationFramework::CellsHandler::initialize()
{
  if( m_n_eta != 7 || m_n_phi != 11)
  {
	std::cout<<m_name<<" 7x11 Cluster is only supported, check your code"<<std::endl;
	return StatusCode::FAILURE;
  }
  m_caloFillRectangularTool.setTypeAndName(Form("CaloFillRectangularCluster/CaloFillRectangularCluster_%dx%d",m_n_eta,m_n_phi));
  if (!m_caloFillRectangularTool.empty()) {ATH_CHECK( m_caloFillRectangularTool.retrieve() );
  }else {
	std::cout<<m_name << Form(" CaloFillRectangularCluster/CaloFillRectangularCluster_%dx%d",m_n_eta,m_n_phi)<<" not found"<<std::endl;
	return StatusCode::FAILURE;
  }
  m_tool = dynamic_cast<CaloFillRectangularCluster* >( &(*m_caloFillRectangularTool ) );

  return StatusCode::SUCCESS;
}

int DerivationFramework::CellsHandler::getImages(const xAOD::Egamma* egamma, const CaloCellContainer*  CellContainer, std::vector<std::vector<float>>& Img_Lr1, std::vector<std::vector<float>>& Img_Lr2, std::vector<std::vector<float>>& Img_Lr3) 
{

  std::vector<float> cells_Lr1, cells_Lr2, cells_Lr3; // cells supporters 

  bool isBarrel = xAOD::EgammaHelpers::isBarrel(egamma);
  CaloCell_ID::CaloSample sample = isBarrel ? CaloCell_ID::EMB2 : CaloCell_ID::EME2; // define position Barrel vs End-cap

  double etaCalo = 0.;
  if(! (egamma->caloCluster()->retrieveMoment(xAOD::CaloCluster::ETACALOFRAME, etaCalo))) {
    
    return 0;
  }
  double phiCalo = 0.;
  if(! (egamma->caloCluster()->retrieveMoment(xAOD::CaloCluster::PHICALOFRAME, phiCalo))) {
    
    return 0;
  }

  double etamaxClus, phimaxClus, etamaxClusCalo, phimaxClusCalo;
  if (isBarrel) {
    etamaxClus = egamma->caloCluster()->etamax(CaloCell_ID::EMB2);
    phimaxClus = egamma->caloCluster()->phimax(CaloCell_ID::EMB2);
  }else {
    etamaxClus = egamma->caloCluster()->etamax(CaloCell_ID::EME2);
    phimaxClus = egamma->caloCluster()->phimax(CaloCell_ID::EME2);
  }

  CaloCellDetPos  ccdp;
  ccdp.getDetPosition(sample, etamaxClus, phimaxClus, etamaxClusCalo, phimaxClusCalo); // get cluster position

 
  xAOD::CaloCluster* Cells(0);

   if (not egamma->author(xAOD::EgammaParameters::AuthorCaloTopo35) )	
   {

   	Cells = CaloClusterStoreHelper::makeCluster(CellContainer,
							 etamaxClusCalo,
							 phimaxClusCalo,
							 egamma->caloCluster()->clusterSize()); // retrieve cluster
  	m_tool->makeCorrection(Cells);
  }
  int ncell = 0;
  if(Cells)
  {
  	auto first_cell = Cells->cell_begin();
	auto last_cell = Cells->cell_end();
        for (;first_cell != last_cell; ++first_cell,++ncell) {
		const CaloCell* tcell = *first_cell;
    		int sampling = tcell->caloDDE()->getSampling();
		if(sampling == CaloCell_ID::EMB1 || sampling == CaloCell_ID::EME1 ) {
			cells_Lr1.push_back(tcell->e());
		}
 		if(sampling == CaloCell_ID::EMB2 || sampling == CaloCell_ID::EME2 ) {
			cells_Lr2.push_back(tcell->e());
		}
		if(sampling == CaloCell_ID::EMB3 || sampling == CaloCell_ID::EME3 ) {
			cells_Lr3.push_back(tcell->e());
		}
 	}
	
  }

  // Fixing size and missing cells append zeros + applying normalization

  FixMissingCellsAndImageSize(cells_Lr1, 112); 
  FixMissingCellsAndImageSize(cells_Lr2, 77);
  FixMissingCellsAndImageSize(cells_Lr3, 44);

  // create image from cells in ETAxPHI plan 
  CreateImage(cells_Lr1, 56, 2, Img_Lr1);
  CreateImage(cells_Lr2, 7, 11, Img_Lr2);
  CreateImage(cells_Lr3, 4, 11, Img_Lr3);

  delete Cells;

  return 1;
}

void DerivationFramework::CellsHandler::Normalize(std::vector<float>& cells_LrX)
{
	float sumE = 0.; 
	for(unsigned int i = 0; i<cells_LrX.size(); i++) { sumE += cells_LrX[i];}
	if(sumE == 0) return;
        for(unsigned int i = 0; i<cells_LrX.size(); i++) { cells_LrX[i] /= sumE;}
}

void DerivationFramework::CellsHandler::FixMissingCellsAndImageSize(std::vector<float>& cells_LrX, unsigned int Exact_size)
{
	unsigned int Lr_size = cells_LrX.size();
    Normalize(cells_LrX); // normalise first

	if(Lr_size == Exact_size) return;

	if (Lr_size < Exact_size)
	{
		for(unsigned int i = Lr_size; i < Exact_size; i++) {cells_LrX.push_back(0);}
	}else if(Lr_size > Exact_size)
	{
		while(cells_LrX.size() > Exact_size) {cells_LrX.pop_back();}
	}

}

void DerivationFramework::CellsHandler::CreateImage(std::vector<float> cells_LrX, int ETA, int PHI, std::vector<std::vector<float>>& Image_LrX)
{
	Image_LrX.resize(1, std::vector<float>(ETA*PHI*1));
	for (int phi = 0; phi < PHI; phi++) {
	for (int eta = 0; eta < ETA; eta++) {
		Image_LrX[0][eta + ETA*phi] = cells_LrX[phi + PHI*eta]; // flipping the image according to python training 
	}	
	}
}
