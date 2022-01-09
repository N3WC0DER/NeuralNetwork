#include <iostream>
#include <ctime>
#include "lib/Matrix.h"
//#include "eigen/Eigen/Dense"

using namespace std;

int main(){
    srand(static_cast<unsigned int>(time(0)));
    rand();
    Matrix<int> m1(3, 3);
    m1.random(0, 10);
    cout << m1 << endl;
    m1.resize(2, 2);
    cout << m1;
    /* srand(static_cast<unsigned int>(time(0)));
    rand();

    

    int inputNodes = 3,
        hidenNodes = 3,
        outputNodes = 3;
    float learningRate = 0.3;

    NeuralNetwork network(inputNodes, hidenNodes, outputNodes, learningRate);

    Eigen::Matrix<float, 1, 3> inputs {{1.0, 0.5, -1.5}};
    Eigen::MatrixXf matrix = network.query(inputs);
    cout << matrix << endl; */
    return 0;
}