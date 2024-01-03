///////////////////////// -*- C++ -*- /////////////////////////////
/*
  Copyright (C) 2019-2020 CERN for the benefit of the ATLAS collaboration
*/
// CNNHandler.h
// Header file for class CNNHandler
// Author: BELFKIR Mohamed 
// Email : mohamed.belfkir@cern.ch
/////////////////////////////////////////////////////////////////// 

#ifndef DERIVATIONFRAMEWORK_CNNHANDLER_H
#define DERIVATIONFRAMEWORK_CNNHANDLER_H

#include <string>

#include "AthenaBaseComps/AthAlgTool.h"
#include "xAODEgamma/PhotonContainer.h"
#include "GaudiKernel/ToolHandle.h"

#include <core/session/onnxruntime_cxx_api.h>


namespace DerivationFramework {

	class CNNHandler {
	  public:
	  CNNHandler(std::string modelname);
	  ~CNNHandler();
	  StatusCode initialize();
          StatusCode finalize();
	  Double_t predict(std::vector<std::vector<float>> Image_Lr1, std::vector<std::vector<float>> Image_Lr2, std::vector<std::vector<float>> Image_Lr3);

	  private:

	  std::string m_modelName; 
	  std::string m_model;

	  size_t NInputs;
          std::vector<const char*> InputsNames;
          std::vector<std::vector<int64_t>> InputsDim;
	  size_t NOutputs;
          std::vector<const char*> OutputsNames;
	  std::vector<int64_t> OutputDim;


	  size_t Input1_size = 1*2*56*1;
          size_t Input2_size = 1*11*7*1;
	  size_t Input3_size = 1*11*4*1;

          std::vector<int64_t> Lr1Dim = {2,56,1};
          std::vector<int64_t> Lr2Dim = {11,7,1};
	  std::vector<int64_t> Lr3Dim = {11,4,1};

	  
	};
}

#endif // DERIVATIONFRAMEWORK_CNNHANDLER_H
