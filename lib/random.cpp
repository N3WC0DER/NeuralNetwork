#include <cstdlib>
#include "Matrix.h"

int getRandomNumber(int min, int max){
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

float getRandomFloatNumber(float min, float max){
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
    return static_cast<float>(rand() * fraction * (max - min + 1) + min);
}

/* Matrix<double> randomMatrix(Matrix<double> &matrix){
    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            matrix(i, j) = static_cast<double>(rand() % 100) - 0.5;
        }
    }
    return matrix;
} */

Matrix<double> sigmoida(Matrix<double> &matrix){
    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            matrix(i, j) = 1 / (1 + exp(-(matrix(i, j))));
        }
    }
    return matrix;
}