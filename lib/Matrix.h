#pragma once
#define MatrixT Matrix<T>

#include <iostream>
#include <cmath>
#include <assert.h>
#include <vector>

using namespace std;

template<typename T>
class Matrix;

template<typename T>
ostream& operator<<(ostream &out, MatrixT &matrix){
    int max = 0;
    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            if((log10(static_cast<double>(matrix(i, j))) + 1 / 1) > max){
                max = log10(static_cast<double>(matrix(i, j))) + 1 / 1;
            }
        }
    }

    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            out.width(max);
            out << matrix(i, j) << " ";
        }
        out << endl;
    }
    return out;
}

template<typename T>
MatrixT operator*(const MatrixT &matrix1, const MatrixT &matrix2){
    assert(matrix1.getCols() == matrix2.getRows());

    MatrixT targetMatrix(matrix1.getRows(), matrix2.getCols());
    for (int i = 0; i < matrix1.getRows(); i++){
        for (int j = 0; j < matrix2.getCols(); j++){
            for (int k = 0; k < matrix1.getCols(); k++){
                targetMatrix(i, j) += matrix1(i, k) * matrix2(k, j);
            }
        }
    }
    return targetMatrix;
}

template<typename T>
MatrixT operator*(MatrixT &matrix, const T value){
    MatrixT targetMatrix(matrix.getRows(), matrix.getCols());

    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            targetMatrix(i, j) = matrix(i, j) * value; 
        }
    }
    return targetMatrix;
}

template<typename T>
MatrixT operator*(const T value, MatrixT &matrix){
    MatrixT targetMatrix(matrix.getRows(), matrix.getCols());
    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            targetMatrix(i, j) = matrix(i, j) * value; 
        }
    }
    return targetMatrix;
}

template<typename T>
MatrixT operator+(MatrixT &matrix1, MatrixT &matrix2){
    assert(matrix1.getRows() == matrix2.getRows() && matrix1.getCols() == matrix2.getCols());

    MatrixT targetMatrix(matrix1.getRows(), matrix1.getRows());
    for (int i = 0; i < matrix1.getRows(); i++){
        for (int j = 0; j < matrix1.getCols(); j++){
            targetMatrix(i, j) = matrix1(i, j) + matrix2(i, j); 
        }
    }
    return targetMatrix;
}

template<typename T>
MatrixT operator+(MatrixT &matrix, const T value){
    MatrixT targetMatrix(matrix.getRows(), matrix.getRows());
    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            targetMatrix(i, j) = matrix(i, j) + value; 
        }
    }
    return targetMatrix;
}

template<typename T>
MatrixT operator+(const T value, MatrixT &matrix){
    MatrixT targetMatrix(matrix.getRows(), matrix.getRows());
    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            targetMatrix(i, j) = matrix(i, j) + value; 
        }
    }
    return targetMatrix;
}

template<typename T>
MatrixT operator-(MatrixT &matrix1, MatrixT &matrix2){
    assert(matrix1.getRows() == matrix2.getRows() && matrix1.getCols() == matrix2.getCols());

    MatrixT targetMatrix(matrix1.getRows(), matrix1.getCols());
    for (int i = 0; i < matrix1.getRows(); i++){
        for (int j = 0; j < matrix1.getCols(); j++){
            targetMatrix(i, j) = matrix1(i, j) - matrix2(i, j); 
        }
    }
    return targetMatrix;
}

template<typename T>
MatrixT operator-(MatrixT &matrix, const T value){
    MatrixT targetMatrix(matrix.getRows(), matrix.getRows());
    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            targetMatrix(i, j) = matrix(i, j) - value; 
        }
    }
    return targetMatrix;
}

template<typename T>
MatrixT operator-(const T value, MatrixT &matrix){
    MatrixT targetMatrix(matrix.getRows(), matrix.getRows());
    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            targetMatrix(i, j) = matrix(i, j) - value; 
        }
    }
    return targetMatrix;
}

template<typename T>
class Matrix{
private:
    T **matrix = nullptr;
    int rows = 0;
    int cols = 0;

    void memoryAlloc();
    void memoryClear();
public:
    Matrix();
    Matrix(int rows, int cols);
    Matrix(const MatrixT &matrix);
    Matrix(const initializer_list<initializer_list<T>> &list);

    int getRows() const;
    int getCols() const;
    int getSize() const;
    void transposition();
    void random(const T min, const T max);
    void resize(int rows, int cols);
    MatrixT multiply(MatrixT matrix);

    ~Matrix();

    friend ostream& operator<<<T>(ostream &out, MatrixT &matrix);
    T& operator()(int rows, int cols) const;
    void operator()();
    MatrixT& operator=(const MatrixT &matrix);
    friend MatrixT operator*<T>(const MatrixT &matrix1, const MatrixT &matrix2);
    friend MatrixT operator*<T>(MatrixT &matrix, const T value);
    friend MatrixT operator*<T>(const T value, MatrixT &matrix);
    friend MatrixT operator+<T>(MatrixT &matrix1, MatrixT &matrix2);
    friend MatrixT operator+<T>(MatrixT &matrix, const T value);
    friend MatrixT operator+<T>(const T value, MatrixT &matrix);
    friend MatrixT operator-<T>(MatrixT &matrix1, MatrixT &matrix2);
    friend MatrixT operator-<T>(MatrixT &matrix, const T value);
    friend MatrixT operator-<T>(const T value, MatrixT &matrix);
};

//Constructors
template<typename T>
MatrixT::Matrix(){}

template<typename T>
MatrixT::Matrix(int rows, int cols): rows(rows), cols(cols){
    memoryAlloc();
    
    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = 0;
        } 
    }
}

template<typename T>
MatrixT::Matrix(const MatrixT &matrix): MatrixT(matrix.getRows(), matrix.getCols()){
    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = matrix(i, j);
        }
    }
}

template<typename T>
MatrixT::Matrix(const initializer_list<initializer_list<T>> &list): MatrixT(list.size(), list.begin()->size()){
    int listRows = list.size(),
        listCols = list.begin()->size();
    
    int sizeList = 0;
    for (int i = 0; i < listRows; i++){
        sizeList += (list.begin()+i)->size();
    }
    
    vector<int> countCols;
    for (int i = 0; i < listRows; i++){
       countCols.push_back((list.begin()+i)->size());
    }

    if(sizeList < this->getSize()){
        for(int i = 0; i < listRows; i++){
            for(int j = 0; j < countCols[i]; j++){
                this->matrix[i][j] = *((list.begin()+i)->begin()+j);
            }
        }
    }else{
        for(int i = 0; i < this->rows; i++){
            for(int j = 0; j < this->cols; j++){
                this->matrix[i][j] = *((list.begin()+i)->begin()+j);
            }
        }
    }
}

//Methods
template<typename T>
void MatrixT::memoryAlloc(){
    this->matrix = new T*[this->rows];
    for (int i = 0; i < this->rows; i++){
        this->matrix[i] = new T[this->cols];
    }
}

template<typename T>
void MatrixT::memoryClear(){
    for (int i = 0; i < this->rows; i++){
        delete[] this->matrix[i];
    }
    delete[] this->matrix;
    this->matrix = nullptr;
}

template<typename T>
int MatrixT::getRows() const{
    return this->rows;
}

template<typename T>
int MatrixT::getCols() const{
    return this->cols;
}

template<typename T>
int MatrixT::getSize() const{
    return this->rows * this->cols;
}

template<typename T>
void MatrixT::transposition(){
    MatrixT tempMatrix = *this;

    memoryClear();
    int temp = this->rows;
    this->rows = this->cols;
    this->cols = temp;
    memoryAlloc();

    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = tempMatrix(j, i);
        }
    }
}

template<typename T>
MatrixT MatrixT::multiply(MatrixT matrix){
    assert(matrix.getRows() == this->rows && matrix.getCols() == this->cols);

    MatrixT targetMatrix(this->getRows(), this->getCols());
    for (size_t i = 0; i < targetMatrix.getRows(); i++){
        for (size_t j = 0; j < targetMatrix.getCols(); i++){
            targetMatrix(i, j) = this->matrix[i][j] * matrix(i, j);
        }
        
    }
    
    return targetMatrix;
}

template<typename T>
void MatrixT::random(const T min, const T max){
    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
            this->matrix[i][j] = static_cast<T>(rand() * fraction * (max - min + 1) + min);
        }
    }
}

template<typename T>
void MatrixT::resize(const int rows, const int cols){
    assert(!(rows == this->rows && cols == this->cols));

    int tempRows = this->rows;
    int tempCols = this->cols;
    T **temp = new T*[tempRows];
    for (int i = 0; i < tempRows; i++){
        temp[i] = new T[tempCols];
    }

    for (int i = 0; i < tempRows; i++){
        for (int j = 0; j < tempCols; j++){
            temp[i][j] = this->matrix[i][j];
        }
    }
    memoryClear();
    this->rows = rows;
    this->cols = cols;
    memoryAlloc();

    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = 0;
        } 
    }
    if(this->getSize() < tempRows * tempCols){
        for (int i = 0; i < this->rows; i++){
            for (int j = 0; j < this->cols; j++){
                this->matrix[i][j] = temp[i][j];
            }
        }
    }else{
        for (int i = 0; i < tempRows; i++){
            for (int j = 0; j < tempCols; j++){
                this->matrix[i][j] = temp[i][j];
            }
        }
    }

    for (int i = 0; i < tempRows; i++){
        delete[] temp[i];
    }
    delete[] temp;
    temp = nullptr;
}

//Overloading operations
template<typename T>
MatrixT& MatrixT::operator=(const MatrixT &matrix){
    if(this == &matrix) return *this;

    memoryClear();

    this->rows = matrix.getRows();
    this->cols = matrix.getCols();

    memoryAlloc();

    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = 0;
        } 
    }

    for(int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = matrix(i, j); 
        }
    }
    return *this;
}

template<typename T>
T& MatrixT::operator()(int rows, int cols) const{
    assert(rows >= 0 && rows < this->rows);
    assert(cols >= 0 && cols < this->cols);

    return this->matrix[rows][cols];
}

template<typename T>
void MatrixT::operator()(){
    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = 0;
        } 
    }
}
//Destructors
template<typename T>
MatrixT::~Matrix(){
    memoryClear();
}