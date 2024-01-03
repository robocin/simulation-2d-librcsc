#ifndef BALLMODEL_H
#define BALLMODEL_H

#include <rcsc/geom.h>

#include <rcsc/learning_models/onnx_model.h>

class BallModel : LearningModels::OnnxModel
{
  public:
    BallModel(const char *modelPath, Ort::Env *env, int numberofThreads = 1);
    rcsc::Vector2D getBallError(const rcsc::Vector2D selfPos, const rcsc::Vector2D ballPos);

  private:
    std::vector<float> createFrame(const rcsc::Vector2D selfPos, const rcsc::Vector2D ballPos);
};

#endif