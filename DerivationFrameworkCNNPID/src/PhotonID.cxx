///////////////////////// -*- C++ -*- /////////////////////////////
/*
  Copyright (C) 2019-2020 CERN for the benefit of the ATLAS collaboration
*/
// PhotonID.cxx
// Source file for class PhotonID
// Author: BELFKIR Mohamed 
// Email : mohamed.belfkir@cern.ch
/////////////////////////////////////////////////////////////////// 

#include "DerivationFrameworkCNNPID/PhotonID.h"

DerivationFramework::PhotonID::PhotonID(const std::string& t, const std::string& n, const IInterface* p) : AthAlgTool(t,n,p)
{
  declareInterface<DerivationFramework::IAugmentationTool>(this);
  declareProperty("SGKey_photons", m_SGKey_photons);
  declareProperty("SGKey_caloCells", m_SGKey_caloCells);
  declareProperty("N_eta", m_n_eta);
  declareProperty("N_phi", m_n_phi);
  declareProperty("CNN_model_name", m_CNN_model_name="DerivationFrameworkCNNPID/MixtureNN_CNN_CNNn_256_DNNn_128_lr_0.0001_dr_0.08_bt_32.onnx", "CNN model");
}

DerivationFramework::PhotonID::~PhotonID() {

}

StatusCode DerivationFramework::PhotonID::initialize()
{
 ATH_MSG_VERBOSE("initialize() ...");
 m_Cells_Tool = new CellsHandler("CellsHandler_Tool",m_n_eta,m_n_phi);
 ATH_CHECK(m_Cells_Tool->initialize());
 m_CNN_Tool = new CNNHandler(m_CNN_model_name);
 ATH_CHECK(m_CNN_Tool->initialize());
 return StatusCode::SUCCESS;
}

StatusCode DerivationFramework::PhotonID::finalize()
{
  return StatusCode::SUCCESS;
}

StatusCode DerivationFramework::PhotonID::addBranches() const
{
  const xAOD::PhotonContainer* photons =  evtStore()->retrieve< const xAOD::PhotonContainer >(m_SGKey_photons);
  const CaloCellContainer* CellContainer(0);
  if (evtStore()->retrieve(CellContainer,m_SGKey_caloCells).isFailure())
  ATH_MSG_WARNING(m_SGKey_caloCells <<" not found");
  if( !photons ) {
	ATH_MSG_ERROR("Couldn't retrieve photon container with key: " << m_SGKey_photons);
	return StatusCode::FAILURE;
  }	
  for (auto ph : *photons) { 
	std::vector<std::vector<float>> Image_Lr1, Image_Lr2, Image_Lr3;
	ph->auxdecor<float>("p_CNN") = -1;	
	if(not m_Cells_Tool->getImages(ph, CellContainer, Image_Lr1, Image_Lr2, Image_Lr3)) continue;
	float predicted_proba = m_CNN_Tool->predict(Image_Lr1, Image_Lr2, Image_Lr3);
	ph->auxdecor<float>("p_CNN") = predicted_proba;
  }

  return StatusCode::SUCCESS;
}
