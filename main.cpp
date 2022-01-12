#include <iostream>
#include <ctime>
#include "lib/Matrix.h"
#include "NeuralNetwork.h"
#include "lib/random.h"

using namespace std;

int main(){
    srand(static_cast<unsigned int>(time(0)));
    rand();

    int inputNodes = 3,
        hidenNodes = 3,
        outputNodes = 3;
    float learningRate = 0.3;

    NeuralNetwork network(inputNodes, hidenNodes, outputNodes, learningRate);

    Matrix<double> inputs {{1.0, 0.5, -1.5}};
    Matrix<double> outputs = network.query(inputs);
    cout << outputs << endl;

    Matrix<double> targets {{0.32, 0.25, 0.44}};
    network.train(inputs, targets);
    network.print();
    outputs = network.query(inputs);
    cout << outputs << endl;
    return 0;
}