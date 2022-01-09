#include <cstdlib>
#include "Eigen/Eigen/Dense"

int getRandomNumber(int min, int max){
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

float getRandomFloatNumber(float min, float max){
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
    return static_cast<float>(rand() * fraction * (max - min + 1) + min);
}

Eigen::MatrixXf randomMatrix(Eigen::MatrixXf matrix, int rows = 0, int cols = 0){
    if (rows == 0 && cols == 0){
        for (int i = 0; i < matrix.rows(); i++){
            for (int j = 0; j < matrix.cols(); j++){
                matrix(i, j) = (static_cast<float>(rand() % 100) / 100) - 0.5;
            }
        }
    }else{
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                matrix(i, j) = (static_cast<float>(rand() % 100) / 100) - 0.5;
            }
        }
    }

    return matrix;
}

Eigen::MatrixXf sigmoida(Eigen::MatrixXf matrix){
    for (int i = 0; i < matrix.rows(); i++){
        for (int j = 0; j < matrix.cols(); j++){
            matrix(i, j) = 1 / (1 + exp(-(matrix(i, j))));
        }
    }
    return matrix;
}