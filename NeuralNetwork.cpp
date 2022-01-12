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

void NeuralNetwork::train(Matrix<double> &inputs, Matrix<double> &targets){
    targets.transposition();

    Matrix<double> hiddenInputs = this->weightsInputHidden * inputs;
    cout << hiddenInputs;
    Matrix<double> hiddenOutputs = sigmoida(hiddenInputs);
    cout << hiddenOutputs;

    Matrix<double> finalInputs = this->weightsHiddenOutput * hiddenOutputs;
    cout << finalInputs;
    Matrix<double> finalOutputs = sigmoida(finalInputs);
    cout << finalOutputs;

    Matrix<double> outputErrors = targets - finalOutputs;
    cout << outputErrors;
    
    this->weightsHiddenOutput.transposition();
    cout << weightsHiddenOutput;
    Matrix<double> hiddenErrors = this->weightsHiddenOutput * outputErrors;
    cout << hiddenErrors;

    this->weightsHiddenOutput = this->weightsHiddenOutput + this->learningRate * (1.0 - finalOutputs).multiply(outputErrors.multiply(finalOutputs)) * hiddenOutputs;
    this->weightsInputHidden = this->weightsHiddenOutput + this->learningRate * (1.0 - hiddenOutputs).multiply(hiddenErrors.multiply(hiddenOutputs)) * inputs;

}

Matrix<double> NeuralNetwork::query(Matrix<double> &inputs){
    inputs.transposition();
    Matrix<double> hiddenInputs = this->weightsInputHidden * inputs;
    Matrix<double> hiddenOutputs = sigmoida(hiddenInputs);

    Matrix<double> finalInputs = this->weightsHiddenOutput * hiddenOutputs;
    Matrix<double> finalOutputs = sigmoida(finalInputs);

    return finalOutputs;
}

void NeuralNetwork::print(){
    cout << "IH:" << endl << this->weightsInputHidden << endl;
    cout << "HO:" << endl << this->weightsHiddenOutput << endl;
}