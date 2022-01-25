#include <iostream>
#include "lib/Matrix.h"
#include "NeuralNetwork.h"
#include "lib/random.h"

using namespace std;

NeuralNetwork::NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes, double learningRate){
    this->inputNodes = inputNodes;
    this->hiddenNodes = hiddenNodes;
    this->outputNodes = outputNodes;
    this->learningRate = learningRate;

    this->weightsInputHidden.resize(this->hiddenNodes, this->inputNodes);
    this->weightsHiddenOutput.resize(this->outputNodes, this->hiddenNodes);

    this->weightsInputHidden.random(-0.5, 0.5);
    this->weightsHiddenOutput.random(-0.5, 0.5);
}

void NeuralNetwork::train(const Matrix<double> &inputs, const Matrix<double> &targets){
    Matrix<double> hiddenInputs = this->weightsInputHidden * inputs.transposition();
    Matrix<double> hiddenOutputs = sigmoida(hiddenInputs);

    Matrix<double> finalInputs = this->weightsHiddenOutput * hiddenOutputs;
    Matrix<double> finalOutputs = sigmoida(finalInputs);

    Matrix<double> outputErrors = targets.transposition() - finalOutputs;
    
    Matrix<double> hiddenErrors = this->weightsHiddenOutput.transposition() * outputErrors;

    this->weightsHiddenOutput = this->weightsHiddenOutput + this->learningRate * (1.0 - finalOutputs).multiply(outputErrors.multiply(finalOutputs)) * hiddenOutputs.transposition();
    this->weightsInputHidden = this->weightsInputHidden + this->learningRate * (1.0 - hiddenOutputs).multiply(hiddenErrors.multiply(hiddenOutputs)) * inputs;
}

Matrix<double> NeuralNetwork::query(const Matrix<double> &inputs){
    Matrix<double> hiddenInputs = this->weightsInputHidden * inputs.transposition();
    Matrix<double> hiddenOutputs = sigmoida(hiddenInputs);

    Matrix<double> finalInputs = this->weightsHiddenOutput * hiddenOutputs;
    Matrix<double> finalOutputs= sigmoida(finalInputs);

    return finalOutputs;
}

void NeuralNetwork::print(){
    cout << "IH:" << endl << this->weightsInputHidden << endl;
    cout << "HO:" << endl << this->weightsHiddenOutput << endl;
}

int NeuralNetwork::getInputNodes() const{
    return this->inputNodes;
}

int NeuralNetwork::getHiddenNodes() const{
    return this->hiddenNodes;
}

int NeuralNetwork::getOutputNodes() const{
    return this->outputNodes;
}