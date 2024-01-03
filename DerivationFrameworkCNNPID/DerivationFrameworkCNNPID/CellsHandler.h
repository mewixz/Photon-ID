///////////////////////// -*- C++ -*- /////////////////////////////
/*
  Copyright (C) 2019-2020 CERN for the benefit of the ATLAS collaboration
*/
// CellsHandler.h
// Header file for class CellsHandler
// Author: BELFKIR Mohamed 
// Email : mohamed.belfkir@cern.ch
/////////////////////////////////////////////////////////////////// 


#ifndef DERIVATIONFRAMEWORK_CELLSHANDLER_H
#define DERIVATIONFRAMEWORK_CELLSHANDLER_H

#include <string>

#include "xAODEgamma/Egamma.h"
#include "xAODEgamma/EgammaxAODHelpers.h"
#include "CaloIdentifier/CaloCell_ID.h"
#include "CaloUtils/CaloCellDetPos.h"
#include "CaloUtils/CaloClusterStoreHelper.h"
#include "CaloClusterCorrection/CaloFillRectangularCluster.h"

#include "GaudiKernel/ToolHandle.h"

class CaloFillRectangularCluster;

namespace DerivationFramework {
  
    class CellsHandler {
      public:
      CellsHandler(std::string t, int eta, int phi);
      ~CellsHandler();
      StatusCode initialize();
      StatusCode finalize();
      int getImages(const xAOD::Egamma* egamma, const CaloCellContainer*  CellContainer, std::vector<std::vector<float>>& Img_Lr1, std::vector<std::vector<float>>& Img_Lr2, std::vector<std::vector<float>>& Img_Lr3);

    private:
     std::string m_name;
     int m_n_eta;
     int m_n_phi;
     ToolHandle<CaloClusterCollectionProcessor> m_caloFillRectangularTool;
     CaloFillRectangularCluster *m_tool;
     void Normalize(std::vector<float>& cells_LrX);
     void FixMissingCellsAndImageSize(std::vector<float>& cells_LrX, unsigned int Exact_size);
     void CreateImage(std::vector<float> cells_LrX, int ETA, int PHI, std::vector<std::vector<float>>& Img_LrX);
  };
}

#endif // DERIVATIONFRAMEWORK_CELLSHANDLER_H
