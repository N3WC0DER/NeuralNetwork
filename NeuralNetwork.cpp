#include <fstream>
#include <string>
#include "lib/Matrix.h"
#include "NeuralNetwork.h"
#include "lib/sigmoida.h"

using namespace std;

NeuralNetwork::NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes, double learningRate){
	this->inputNodes = inputNodes;
	this->hiddenNodes = hiddenNodes;
	this->outputNodes = outputNodes;
	this->learningRate = learningRate;

	this->weightsInputHidden.resize(this->hiddenNodes, this->inputNodes);
	this->weightsHiddenOutput.resize(this->outputNodes, this->hiddenNodes);

	this->receiveWeightsInFile();
}

NeuralNetwork::~NeuralNetwork() {
	saveWeightsInFile();
}

void NeuralNetwork::train(const Matrix<double> &inputs, const Matrix<double> &targets){
	Matrix<double> hiddenInputs = this->weightsInputHidden * inputs;
	Matrix<double> hiddenOutputs = sigmoida(hiddenInputs);

	Matrix<double> finalInputs = this->weightsHiddenOutput * hiddenOutputs;
	Matrix<double> finalOutputs = sigmoida(finalInputs);

	Matrix<double> outputErrors = finalOutputs - targets;
	
	Matrix<double> hiddenErrors = this->weightsHiddenOutput.transposition() * outputErrors;

	this->weightsHiddenOutput += this->learningRate * (1.0 - finalOutputs) % outputErrors % finalOutputs * hiddenOutputs.transposition();
	this->weightsInputHidden += this->learningRate * (1.0 - hiddenOutputs) % hiddenErrors % hiddenOutputs * inputs.transposition();
}

Matrix<double> NeuralNetwork::query(const Matrix<double> &inputs){
	Matrix<double> hiddenInputs = this->weightsInputHidden * inputs.transposition();
	Matrix<double> hiddenOutputs = sigmoida(hiddenInputs);

	Matrix<double> finalInputs = this->weightsHiddenOutput * hiddenOutputs;
	Matrix<double> finalOutputs = sigmoida(finalInputs);

	return finalOutputs;
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

void NeuralNetwork::saveWeightsInFile() const{
	ofstream weights;
	weights.open("weights.csv");
	if (!weights.is_open()) {
		exit(EXIT_FAILURE);
	}

	weights.clear();

	string line = "";
	for (int i = 0; i < this->weightsInputHidden.getRows(); i++) {
		for (int j = 0; j < this->weightsInputHidden.getCols(); j++) {
			line += to_string(this->weightsInputHidden(i, j));
			line.push_back(',');
		}
	}
	weights << line;
	weights << '\n';
	line = "";

	for (int i = 0; i < this->weightsHiddenOutput.getRows(); i++) {
		for (int j = 0; j < this->weightsHiddenOutput.getCols(); j++) {
			line += to_string(this->weightsHiddenOutput(i, j));
			line.push_back(',');
		}
	}
	
	weights << line;
	weights.close();
}

void NeuralNetwork::receiveWeightsInFile() {
	ifstream weights;
	weights.open("weights.csv");
	if (!weights.is_open()) {
		exit(EXIT_FAILURE);
	}

	string line = "";
	vector<double> vecWeights;
	for (int i = 0; i < this->weightsInputHidden.getSize(); i++) {
		getline(weights, line, ',');
		vecWeights.push_back(stod(line));
	}

	for (int j = 0; j < this->weightsInputHidden.getRows(); j++) {
		for (int k = 0; k < this->weightsInputHidden.getCols(); k++) {
			this->weightsInputHidden(j, k) = vecWeights[k + j * this->weightsInputHidden.getCols()];
		}
	}

	line = "";
	vecWeights.clear();

	for (int i = 0; i < this->weightsHiddenOutput.getSize(); i++) {
		getline(weights, line, ',');
		vecWeights.push_back(stod(line));
	}

	for (int j = 0; j < this->weightsHiddenOutput.getRows(); j++) {
		for (int k = 0; k < this->weightsHiddenOutput.getCols(); k++) {
			this->weightsHiddenOutput(j, k) = vecWeights[k + j * this->weightsHiddenOutput.getCols()];
		}
	}
	weights.close();
}