#include <iostream>
#include "Eigen/Eigen/Dense"

#include "random.h"

using namespace std;

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

NeuralNetwork::NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes, float learningRate){
    this->inputNodes = inputNodes;
    this->hiddenNodes = hiddenNodes;
    this->outputNodes = outputNodes;
    this->learningRate = learningRate;

    this->weightsInputHidden = randomMatrix(this->weightsInputHidden, this->hiddenNodes, this->inputNodes);
    this->weightsHiddenOutput = randomMatrix(this->weightsHiddenOutput, this->outputNodes, this->hiddenNodes);
}

void NeuralNetwork::train(){

}

Eigen::MatrixXf NeuralNetwork::query(Eigen::MatrixXf inputs){
    Eigen::MatrixXf hiddenInputs = this->weightsInputHidden * inputs;
    Eigen::MatrixXf hiddenOutputs = sigmoida(hiddenInputs);

    Eigen::MatrixXf finalInputs = this->weightsHiddenOutput * hiddenOutputs;
    Eigen::MatrixXf finalOutputs = sigmoida(finalInputs);

    return finalOutputs;
}