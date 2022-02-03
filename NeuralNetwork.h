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

	void train(const Matrix<double> &inputs, const Matrix<double> &targets);
	Matrix<double> query(const Matrix<double> &inputs);
	int getInputNodes() const;
	int getHiddenNodes() const;
	int getOutputNodes() const;

	void saveWeightsInFile() const;
	void receiveWeightsInFile();
};