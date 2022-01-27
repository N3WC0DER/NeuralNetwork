#include <cstdlib>
#include "random.h"
#include "Matrix.h"

Matrix<double> sigmoida(const Matrix<double> &matrix){
    Matrix<double> tempMatrix(matrix.getRows(), matrix.getCols());

    for (int i = 0; i < tempMatrix.getRows(); i++){
        for (int j = 0; j < tempMatrix.getCols(); j++){
            tempMatrix(i, j) = 0.5 * (matrix(i, j) / (abs(matrix(i, j)) + 10)) + 0.5;
        }
    }
    return tempMatrix;
}