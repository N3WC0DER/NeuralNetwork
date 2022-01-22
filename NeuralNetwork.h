#pragma once

class NeuralNetwork{
private:
    int inputNodes,
        hiddenNodes,
        outputNodes;
    double learningRate;

    Matrix<double> weightsInputHidden;
    Matrix<double> weightsHiddenOutput;
public:
    NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes, double learningRate);

    void train(Matrix<double> &inputs, Matrix<double> &targets);
    Matrix<double> query(Matrix<double> &inputs);
    void print();
    int getInputNodes() const;
    int getHiddenNodes() const;
    int getOutputNodes() const;
};