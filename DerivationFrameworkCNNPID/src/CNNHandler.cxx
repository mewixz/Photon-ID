///////////////////////// -*- C++ -*- /////////////////////////////
/*
  Copyright (C) 2019-2020 CERN for the benefit of the ATLAS collaboration
*/
// CNNHandler.cxx
// Source file for class CNNHandler
// Author: BELFKIR Mohamed 
// Email : mohamed.belfkir@cern.ch
/////////////////////////////////////////////////////////////////// 


#include "DerivationFrameworkCNNPID/CNNHandler.h"
#include "PathResolver/PathResolver.h"

DerivationFramework::CNNHandler::CNNHandler(std::string modelname) : m_modelName(modelname){
	
}

DerivationFramework::CNNHandler::~CNNHandler() {

}

StatusCode DerivationFramework::CNNHandler::initialize() {

	m_model = PathResolverFindCalibFile(m_modelName);
	if(m_model == "") {
		std::cout<< "No model is founded" << std::endl;
		return StatusCode::FAILURE; 
	}
	std::cout<<"Model " << m_model << " is loaded!"<< std::endl;
	return StatusCode::SUCCESS;
}

StatusCode DerivationFramework::CNNHandler::finalize() {

        return StatusCode::SUCCESS;
}

Double_t DerivationFramework::CNNHandler::predict(std::vector<std::vector<float>> Image_Lr1, std::vector<std::vector<float>> Image_Lr2, std::vector<std::vector<float>> Image_Lr3) {


	Ort::SessionOptions sessionOptions;
      	Ort::AllocatorWithDefaultOptions allocator;
      	Ort::Env env;  // new envirement for each photon
	
	sessionOptions.SetIntraOpNumThreads( 1 );
      	sessionOptions.SetGraphOptimizationLevel( ORT_ENABLE_BASIC );

        std::unique_ptr< Ort::Session > m_session = std::make_unique< Ort::Session >( env, m_model.c_str(), sessionOptions ); // new session for each photon
	
	// get inputs and output properties (number, names, dimesions)
      	NInputs = m_session->GetInputCount();
      	InputsNames = std::vector<const char*>(NInputs);
	for(std::size_t i = 0; i<NInputs; i++){ 
		InputsNames[i] = m_session->GetInputName(i, allocator); 
		Ort::TypeInfo InputsInfo = m_session->GetInputTypeInfo(i);
        	auto InputTensorInfo = InputsInfo.GetTensorTypeAndShapeInfo();
        	std::vector<int64_t> InputDim = InputTensorInfo.GetShape();
		for( std::size_t j = 0 ; j< InputDim.size(); j++) { 
			if(InputDim[j]<0) InputDim[j] = 1; 
	 	}
		InputsDim.push_back(InputDim);		
	}

        NOutputs = m_session->GetOutputCount();
        OutputsNames = std::vector<const char*>(NOutputs);
	for(std::size_t i = 0; i<NOutputs; i++){ OutputsNames[i] = m_session->GetOutputName(i, allocator); }

	Ort::TypeInfo OutputsInfo = m_session->GetOutputTypeInfo(0);
        auto OutputTensorInfo = OutputsInfo.GetTensorTypeAndShapeInfo();
        OutputDim = OutputTensorInfo.GetShape();

	auto meminfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

	// create layer 1 input tensor
	std::vector<float> Input1_Img(Input1_size);
	Input1_Img = Image_Lr1[0];
	Ort::Value Input1 = Ort::Value::CreateTensor<float>(meminfo, Input1_Img.data(), Input1_size, InputsDim[0].data(), Lr1Dim.size()+1);
     	assert(Input1.IsTensor());

	// create layer 2 input tensor 
	std::vector<float> Input2_Img(Input2_size);
	Input2_Img = Image_Lr2[0];
	Ort::Value Input2 = Ort::Value::CreateTensor<float>(meminfo, Input2_Img.data(), Input2_size, InputsDim[1].data(), Lr2Dim.size()+1);
     	assert(Input2.IsTensor());


	// create layer 3 input tensor
	std::vector<float> Input3_Img(Input3_size);
	Input3_Img = Image_Lr3[0];
	Ort::Value Input3 = Ort::Value::CreateTensor<float>(meminfo, Input3_Img.data(), Input3_size, InputsDim[2].data(), Lr3Dim.size()+1);
     	assert(Input3.IsTensor());


	// create CNN model input tensor layer 1 + layer 2 + layer 3
	std::vector<Ort::Value> Inputs;
	Inputs.push_back(std::move(Input1));
	Inputs.push_back(std::move(Input2));
	Inputs.push_back(std::move(Input3));

	std::vector<Ort::Value> CNNOutputs = m_session->Run(Ort::RunOptions{nullptr}, InputsNames.data(), Inputs.data(), Inputs.size(), OutputsNames.data(), 1); // expect 1 output  
	assert(CNNOutputs.size() == 1 && CNNOutputs.front().IsTensor());

	float* CNNProba = CNNOutputs.front().GetTensorMutableData<float>();

	//clear everything 
	m_session.reset();
	InputsNames.clear();
	InputsDim.clear();
	OutputsNames.clear();
	OutputDim.clear();
	
	//return the predicted probabilty
	return CNNProba[0];
}

