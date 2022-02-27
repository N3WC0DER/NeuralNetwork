#pragma once

class NeuralNetwork{
private:
	int inputNodes,
		hiddenNodes,
		outputNodes;
	double learningRate;

	Matrix<double> weightsInputHidden;
	Matrix<double> weightsHiddenOutput;

	void train(const Matrix<double> &inputs, const Matrix<double> &targets);
public:
	NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes, double learningRate);
	~NeuralNetwork();

	void trainNetwork(const int countIteration);
	void testNetwork(const int countIteration);
	Matrix<double> prepareValues(Matrix<double> &pixels) const;

	Matrix<double> query(const Matrix<double> &inputs);

	int getInputNodes() const;
	int getHiddenNodes() const;
	int getOutputNodes() const;

	void saveWeightsInFile() const;
	void receiveWeightsInFile();
};