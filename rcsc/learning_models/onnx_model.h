// *************************************************************************
//  This program is implemented by RoboCIn
//  Copyright 2019, Cristiano Oliveira, Mateus Gonçalves, Walber Rodrigues
//  Centro de Informática - Universidade Federal de Pernambuco, Brazil
//  All rights reserved.
// *************************************************************************

#ifndef LEARNINGMODELS_H
#define LEARNINGMODELS_H

// #include "../lib/PY_ENV/include/python3.6m/Python.h"
#include <iostream>
#include <string>
#include <vector>

#define ONNX_ENABLED

#ifdef TORCH_ENABLED
#include <torch/torch.h>
#endif
#ifdef ONNX_ENABLED
#include <onnxruntime_cxx_api.h>
#endif

namespace LearningModels
{
void initEmbeddedPython();
void finalizeEmbeddedPython();
void loadAllModels();

//!  Class to handle ONNX models and sessions
/*!
OnnxModel is the class that handles every step for ONNX inference models, you can export a model trained in any other
framework (ie. Pytorch, Tensorflow, etc), read specific documentation in each of these frameworks for more details.

The objective of this class is to be used as super class. Your sub class should not override implement
OnnxModel::forward function as it will be used afeter you handling any data processing and frame creation. A frame is an
instance of your data and its size must be the number of features from a sample.
*/
class OnnxModel
{
public:
#ifdef ONNX_ENABLED
    OnnxModel(const char *modelPath, Ort::Env *env, int numberofThreads = 1);
    std::vector<int64_t> input_shape_;
#endif
    OnnxModel();
    ~OnnxModel();

protected:
#ifdef ONNX_ENABLED
    template <typename I, typename R> Ort::Value forward(std::vector<I> inputFrame, std::vector<int64_t> inputShape, R output, std::vector<int64_t> outputShape)
    {
        auto memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        Ort::Value inputTensor =
            Ort::Value::CreateTensor<I>(memoryInfo, inputFrame.data(), inputFrame.size(), inputShape.data(), inputShape.size());
        Ort::Value outputTensor =
            Ort::Value::CreateTensor<I>(memoryInfo, output.data(), output.size(), outputShape.data(), outputShape.size());

        // Get our model output
        std::vector<const char *> outputNodeNames;
        for (size_t i = 0; i < this->session->GetOutputCount(); i++)
        {
            outputNodeNames.push_back(this->session->GetOutputName(i, allocator));
        }

        this->session->Run(
            Ort::RunOptions().UnsetTerminate(),
            this->inputNodeNames.data(), 
            &inputTensor, 
            1, 
            outputNodeNames.data(), 
            &outputTensor, 
            1
        );

        return outputTensor;
    }

    Ort::Session *session;
    Ort::Env *env;
    size_t numInputNodes;
    size_t outputNodeDims;
    std::vector<int64_t> inputNodeDims;
    Ort::AllocatorWithDefaultOptions allocator;
    std::vector<char *> inputNodeNames;
#endif
};

#ifdef TORCH_ENABLED
class TorchModel
{
  public:
    void printTensor();

  private:
    torch::Tensor tensor = torch::eye(3);
};
#endif
} // namespace LearningModels

#endif