#include "onnx_model.h"

//! ONNXModel constructor.
/*!
    This constructor creates a session for a given modelPath, Ort::Env* and numberOfThreads. Due to competition
   regulations, it is recomended to use only 1 thread.
*/
#ifdef ONNX_ENABLED
LearningModels::OnnxModel::OnnxModel(const char *modelPath, Ort::Env *env, int numberOfThreads)
{
    // Set environment for our session
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(
        numberOfThreads); // number of threads should be a define, we may experience issues to change that in the future
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    // Load session
    this->session = new Ort::Session(*env, modelPath, session_options);

    // Get the properties of our model
    this->numInputNodes = session->GetInputCount();
    this->inputNodeNames = std::vector<char *>(this->numInputNodes);

    for (int i = 0; i < numInputNodes; i++)
    {
        char *input_name = session->GetInputName(i, allocator);
        this->inputNodeNames[i] = input_name;
        Ort::TypeInfo typeInfo = session->GetInputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        ONNXTensorElementDataType type = tensorInfo.GetElementType();
        this->inputNodeDims = tensorInfo.GetShape();
    }
}
#endif

LearningModels::OnnxModel::OnnxModel()
{
}

//! ONNXModel destructor.
/*!
    This destructor must destroy every dynamic created array/object during any step of the class life cycle.
*/
LearningModels::OnnxModel::~OnnxModel()
{
#ifdef ONNX_ENABLED
    for (size_t i = 0; i < this->numInputNodes; i++)
        allocator.Free(this->inputNodeNames[i]);
#endif
}

#ifdef TORCH_ENABLED
void LearningModels::TorchModel::printTensor()
{
    std::cout << this->tensor << std::endl;
}
#endif
