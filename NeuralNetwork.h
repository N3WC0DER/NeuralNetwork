#pragma once

#include "Eigen/Eigen/Dense"

class NeuralNetwork{
private:
    int inputNodes,
        hiddenNodes,
        outputNodes;
    float learningRate;

    Eigen::Matrix3f weightsInputHidden;
    Eigen::Matrix3f weightsHiddenOutput;
public:
    NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes, float learningRate);
    void train();
    Eigen::MatrixXf query(Eigen::MatrixXf inputs);
};