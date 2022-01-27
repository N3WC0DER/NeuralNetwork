#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
#include <cstring>

#include "lib/Matrix.h"
#include "NeuralNetwork.h"
#include "lib/random.h"

using namespace std;

void trainNetwork(NeuralNetwork &network){
    ifstream trainDataset;
    trainDataset.open("mnist_dataset/mnist_train.csv", ios::in);
    vector<string> lines;
    lines.resize(1);

    string tempLine;

    trainDataset >> tempLine;

    for(int i = 0; i < 1; i++){
        trainDataset >> lines[i];
    }

    vector<double> pixels;
    int index;
    vector<char> buff;
    Matrix<double> targets(network.getOutputNodes(), 1);
    Matrix<double> inputs(network.getInputNodes(), 1);
    for(int i = 0; i < 1; i++){
        for(int j = 0; j < lines[i].size(); j++){
            if(isdigit(lines[i][j])){
                if(j == 0){
                    index = lines[i][j] - '0';
                    continue;
                }
                buff.push_back(lines[i][j]);
                if(!(j == lines[i].size()-1)) continue;
            }
            if(buff.size() == 3) pixels.push_back((buff[0] - '0') * 100 + (buff[1] - '0') * 10 + (buff[2] - '0'));
            else if(buff.size() == 2) pixels.push_back((buff[0] - '0') * 10 + buff[1] - '0');
            else if(buff.size() == 1) pixels.push_back(buff[0] - '0');
            
            buff.clear();
        }

        for(int j = 0; j < pixels.size(); j++){
            pixels[j] = (pixels[j] / 255 * 0.99) + 0.01;
        }

        for(int j = 0; j < network.getOutputNodes(); j++){
            targets(j, 0) = 0.01;
            if(j == index) targets(j, 0) = 0.99;
        }

        for(int j = 0; j < network.getInputNodes(); j++){
            inputs(j, 0) = pixels[j];
        }

        network.train(inputs, targets);
        pixels.clear();
    }

    trainDataset.close();
}

void testNetwork(NeuralNetwork &network){
    ifstream testDataset;
    testDataset.open("mnist_dataset/mnist_train.csv", ios::in);

    vector<string> lines;
    lines.resize(1);

    string tempLine;

    testDataset >> tempLine;

    for(int i = 0; i < 1; i++){
        testDataset >> lines[i];
    }

    vector<double> pixels;
    int index;
    vector<char> buff;
    Matrix<double> targets(1, network.getOutputNodes());
    Matrix<double> inputs(1, network.getInputNodes());
    for(int i = 0; i < 1; i++){
        for(int j = 0; j < lines[i].size(); j++){
            if(isdigit(lines[i][j])){
                if(j == 0){
                    index = lines[i][j] - '0';
                    continue;
                }
                buff.push_back(lines[i][j]);
                if(!(j == lines[i].size()-1)) continue;
            }
            if(buff.size() == 3) pixels.push_back((buff[0] - '0') * 100 + (buff[1] - '0') * 10 + (buff[2] - '0'));
            else if(buff.size() == 2) pixels.push_back((buff[0] - '0') * 10 + buff[1] - '0');
            else if(buff.size() == 1) pixels.push_back(buff[0] - '0');
            
            buff.clear();
        }

        for(int j = 0; j < pixels.size(); j++){
            pixels[j] = (pixels[j] / 255 * 0.99) + 0.01;
        }

        for(int j = 0; j < network.getOutputNodes(); j++){
            targets(0, j) = 0.01;
            if(j == index) targets(0, j) = 0.99;
        }

        for(int j = 0; j < network.getInputNodes(); j++){
            inputs(0, j) = pixels[j];
        }

        Matrix<double> outputs = network.query(inputs);
        pixels.clear();
        cout << "------------" << i+1 << " attempts: " << endl;
        cout << outputs << endl << endl;
    }
    testDataset.close();
}

int main(){
    srand(static_cast<unsigned int>(time(0)));
    rand();

    const int inputNodes = 784,
              hidenNodes = 100,
              outputNodes = 10;
    double learningRate = 0.5;

    NeuralNetwork network(inputNodes, hidenNodes, outputNodes, learningRate);

    trainNetwork(network);
    testNetwork(network);
    return 0;
}