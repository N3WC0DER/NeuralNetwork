#include <cstdlib>
#include "Matrix.h"

Matrix<double> sigmoida(Matrix<double> &matrix){
    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            matrix(i, j) = exp(matrix(i, j)) / (exp(matrix(i, j)) + 1);
        }
    }
    return matrix;
}