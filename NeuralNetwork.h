#pragma once

using namespace std;

class NeuralNetwork{
private:
	int inputNodes,
		hiddenNodes,
		outputNodes;
	double learningRate;
	string file = "mnist_dataset/mnist_train.csv";

	Matrix<double> weightsInputHidden;
	Matrix<double> weightsHiddenOutput;
public:
	NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes, double learningRate);
	~NeuralNetwork();

	void train(const Matrix<double> &inputs, const Matrix<double> &targets);
	Matrix<double> query(const Matrix<double> &inputs);
	void trainNetwork(const int countIteration, const int countEpochs);
	void testNetwork(const int countIteration);
	int getInputNodes() const;
	int getHiddenNodes() const;
	int getOutputNodes() const;

	void saveWeightsInFile() const;
	void receiveWeightsInFile();
};