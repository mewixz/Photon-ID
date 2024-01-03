///////////////////////// -*- C++ -*- /////////////////////////////
/*
  Copyright (C) 2019-2020 CERN for the benefit of the ATLAS collaboration
*/
// PhotonID.h
// Header file for class PhotonID
// Author: BELFKIR Mohamed 
// Email : mohamed.belfkir@cern.ch
/////////////////////////////////////////////////////////////////// 

#ifndef DERIVATIONFRAMEWORK_PHOTONID_H
#define DERIVATIONFRAMEWORK_PHOTONID_H

#include <string>

#include "AthenaBaseComps/AthAlgTool.h"
#include "xAODEgamma/PhotonContainer.h"
#include "DerivationFrameworkInterfaces/IAugmentationTool.h"
#include "GaudiKernel/ToolHandle.h"


//local includes 
#include "DerivationFrameworkCNNPID/CellsHandler.h"
#include "DerivationFrameworkCNNPID/CNNHandler.h"


namespace DerivationFramework {
  
  /** Apply CNN to photon  **/
  class PhotonID : public AthAlgTool, public IAugmentationTool {
    public:
      PhotonID(const std::string& t, const std::string& n, const IInterface* p);
      ~PhotonID();
      StatusCode initialize();
      StatusCode finalize();
      virtual StatusCode addBranches() const;
      ToolHandle<CaloClusterCollectionProcessor> m_caloFillRectangularTool;
    private:
 
      std::string m_SGKey_photons;
      std::string m_CNN_model_name;
      std::string m_SGKey_caloCells;
      int m_n_eta, m_n_phi;

      CellsHandler* m_Cells_Tool;
      CNNHandler*  m_CNN_Tool;
  };
}

#endif // DERIVATIONFRAMEWORK_PHOTONID_H
