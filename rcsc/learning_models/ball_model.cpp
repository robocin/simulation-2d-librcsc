#include "ball_model.h"

//! Example of sub class for an ONNX model.
/*!
    You must call the constructor of the super class first and then implement whatever you like.
*/
BallModel::BallModel(const char *modelPath, Ort::Env *env, int numberofThreads)
    : LearningModels::OnnxModel(modelPath, env)
{
}

//! Function used to get a pass probability given world model and current action.
/*!
    Use this function to get the pass probability for an action. This function must return the expected output
    of a model, you can create any post-processing function that you want.
*/
rcsc::Vector2D BallModel::getBallError(const rcsc::Vector2D selfPos, const rcsc::Vector2D ballPos)
{
    std::vector<double> frame = this->createFrame(selfPos, ballPos);
    std::array<double, 2> output;
    Ort::Value outputTensor = this->forward<double, std::array<double, 2>>(frame, input_shape_, output, output_shape_);
    rcsc::Vector2D result = rcsc::Vector2D(output[0], output[1]);
    return result;
}

//! Function to create a frame to be infered by the pass model.
/*!
    This function creates a frame to be used by the model. You must create a function like this for a model,
    to handle any preprocessing step of a model.
*/
std::vector<double> BallModel::createFrame(const rcsc::Vector2D selfPos, const rcsc::Vector2D ballPos)
{
    double distance = (ballPos - selfPos).length();
    double frame[3];

    frame[0] = ballPos.x;
    frame[1] = ballPos.y;
    frame[2] = distance;

    return std::vector<double>(frame, frame + 3);
}
