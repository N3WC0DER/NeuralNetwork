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

	//this->weightsInputHidden.random(-0.3, 0.3);
	//this->weightsHiddenOutput.random(-0.3, 0.3);

	this->receiveWeightsInFile();
}

NeuralNetwork::~NeuralNetwork() {
	this->saveWeightsInFile();
}

void NeuralNetwork::train(const Matrix<double> &inputs, const Matrix<double> &targets) {
	Matrix<double> hiddenInputs = this->weightsInputHidden * inputs;
	Matrix<double> hiddenOutputs = sigmoida(hiddenInputs);

	Matrix<double> finalInputs = this->weightsHiddenOutput * hiddenOutputs;
	Matrix<double> finalOutputs = sigmoida(finalInputs);

	Matrix<double> outputErrors = finalOutputs - targets;
	
	Matrix<double> hiddenErrors = this->weightsHiddenOutput.transposition() * outputErrors;

	this->weightsHiddenOutput += this->learningRate * (1.0 - finalOutputs) % outputErrors % finalOutputs * hiddenOutputs.transposition();
	this->weightsInputHidden += this->learningRate * (1.0 - hiddenOutputs) % hiddenErrors % hiddenOutputs * inputs.transposition();
}

Matrix<double> NeuralNetwork::query(const Matrix<double> &inputs) {
	Matrix<double> hiddenInputs = this->weightsInputHidden * inputs.transposition();
	Matrix<double> hiddenOutputs = sigmoida(hiddenInputs);

	Matrix<double> finalInputs = this->weightsHiddenOutput * hiddenOutputs;
	Matrix<double> finalOutputs = sigmoida(finalInputs);

	return finalOutputs;
}

void NeuralNetwork::trainNetwork(const int countIteration) {
	ifstream trainDataset;
	trainDataset.open(file, ios::in);
	vector<string> lines;
	lines.resize(countIteration);

	string tempLine;

	trainDataset >> tempLine;

	vector<double> pixels;
	int index = 0;
	Matrix<double> targets(this->getOutputNodes(), 1);
	Matrix<double> inputs(this->getInputNodes(), 1);
	for (int i = 0; i < countIteration; i++) {

		string line = "";
		getline(trainDataset, line, ',');
		index = stoi(line);

		line = "";
		for (int j = 0; j < this->getInputNodes(); j++) {
			if (j == this->getInputNodes() - 1) {
				getline(trainDataset, line, '\n');
				pixels.push_back(stoi(line));
				continue;
			}
			getline(trainDataset, line, ',');
			pixels.push_back(stoi(line));
		}

		Matrix<int> temp(28, 28);

		for (int i = 0; i < temp.getRows(); i++) {
			for (int j = 0; j < temp.getCols(); j++) {
				temp(j, i) = pixels.at(i * 28 + j);
			}
		}

		for (int j = 0; j < pixels.size(); j++) {
			pixels[j] = (pixels[j] / 255 * 0.99) + 0.01;
		}

		for (int j = 0; j < this->getOutputNodes(); j++) {
			targets(j, 0) = 0.01;
			if (j == index) targets(j, 0) = 0.99;
		}

		for (int j = 0; j < this->getInputNodes(); j++) {
			inputs(j, 0) = pixels[j];
		}

		this->train(inputs, targets);
		pixels.clear();
	}

	cout << "Train completed!" << endl;

	trainDataset.close();
}

void NeuralNetwork::testNetwork(const int countIteration) {
	ifstream testDataset;
	testDataset.open("mnist_dataset/mnist_test.csv", ios::in);

	vector<string> lines;
	lines.resize(countIteration);

	string tempLine;

	testDataset >> tempLine;

	vector<double> pixels;
	int result = 0;
	int index = 0;
	Matrix<double> inputs(this->getInputNodes(), 1);
	for (int i = 0; i < countIteration; i++) {

		string line = "";
		getline(testDataset, line, ',');
		index = stoi(line);

		line = "";
		for (int j = 0; j < this->getInputNodes(); j++) {
			if (j == this->getInputNodes() - 1) {
				getline(testDataset, line, '\n');
				pixels.push_back(stoi(line));
				continue;
			}
			getline(testDataset, line, ',');
			pixels.push_back(stoi(line));
		}

		Matrix<int> temp(28, 28);

		for (int i = 0; i < temp.getRows(); i++) {
			for (int j = 0; j < temp.getCols(); j++) {
				temp(i, j) = pixels.at(i * 28 + j);
			}
		}

		for (int j = 0; j < pixels.size(); j++) {
			pixels[j] = (pixels[j] / 256 * 0.99) + 0.01;
		}

		for (int j = 0; j < this->getInputNodes(); j++) {
			inputs(j, 0) = pixels[j];
		};

		Matrix<double> outputs = this->query(inputs);
		pixels.clear();
		cout << "------------" << i + 1 << " attempts: " << endl;

		int maxIndex = 0;
		for (int j = 0; j < this->getOutputNodes(); j++) {
			if (outputs(maxIndex, 0) < outputs(j, 0)) maxIndex = j;
		}

		cout << "Output network: " << maxIndex << endl;
		cout << "Target output: " << index << endl;
		cout << outputs(maxIndex, 0) << endl;
		if (maxIndex == index) result++;
	}

	cout << "Result: " << endl;
	cout << (double)result / (double)countIteration * 100 << "% (" << result << "/" << countIteration << ")" << endl;
	testDataset.close();
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