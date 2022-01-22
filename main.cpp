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
    lines.resize(10);

    string tempLine;

    trainDataset >> tempLine;

    for(int i = 0; i < 10; i++){
        trainDataset >> lines[i];
    }

    vector<double> pixels;
    int index;
    vector<char> buff;
    Matrix<double> targets(1, network.getOutputNodes());
    Matrix<double> inputs(1, network.getInputNodes());
    for(int i = 0; i < 10; i++){
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

        network.train(inputs, targets);
        pixels.clear();
    }

    trainDataset.close();
}

int main(){
    srand(static_cast<unsigned int>(time(0)));
    rand();

    const int inputNodes = 784,
              hidenNodes = 100,
              outputNodes = 10;
    float learningRate = 0.3;

    NeuralNetwork network(inputNodes, hidenNodes, outputNodes, learningRate);

    //trainNetwork(network);

    ifstream testDataset;
    testDataset.open("mnist_dataset/mnist_test.csv", ios::in);

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
        cout << outputs << endl;
    }
    
    testDataset.close();
    return 0;
}



/* 5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,18,18,18,126,136,175,26,166,255,247,127,0,0,0,0,0,0,0,0,0,0,0,0,30,36,94,154,
170,253,253,253,253,253,225,172,253,242,195,64,0,0,0,0,0,0,0,0,0,0,0,49,238,253,253,253,253,253,253,253,253,251,93,82,82,56,39,0,0
,0,0,0,0,0,0,0,0,0,0,18,219,253,253,253,253,253,198,182,247,241,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,80,156,107,253,253,205,11,0,43
,154,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14,1,154,253,90,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,139,253,190,2,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,190,253,70,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,241,225,160,108,1,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,81,240,253,253,119,25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,45,186,253,253,150,27,0
,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,93,252,253,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,249,253,249,64,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,46,130,183,253,253,207,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,39,148,229,253,253,253,250,182,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,114,221,253,253,253,253,201,78,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,66,213,253,253,253,253,198
,81,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,171,219,253,253,253,253,195,80,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,55,172,226,253,253,253,253,
244,133,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,136,253,253,253,212,135,132,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0 */
