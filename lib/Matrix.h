#ifndef MATRIX_H
#define MATRIX_H

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
    MatrixT targetMatrix(matrix1.getRows(), matrix1.getRows());
    for (int i = 0; i < matrix1.getRows(); i++){
        for (int j = 0; j < matrix1.getCols(); j++){
            targetMatrix(i, j) = matrix1(i, j) + matrix2(i, j); 
        }
    }
    return targetMatrix;
}

template<typename T>
MatrixT operator-(MatrixT &matrix1, MatrixT &matrix2){
    MatrixT targetMatrix;
    for (int i = 0; i < matrix.getRows(); i++){
        for (int j = 0; j < matrix.getCols(); j++){
            targetMatrix(i, j) = matrix1(i, j) - matrix2(i, j); 
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
public:
    Matrix(int rows, int cols);
    Matrix(const MatrixT &matrix);
    Matrix(const initializer_list<initializer_list<T>> &list);

    int getRows() const;
    int getCols() const;
    int getSize() const;
    void transposition(); // TODO
    void random(const T min, const T max);
    void resize(int rows, int cols); // TODO

    ~Matrix();

    friend ostream& operator<<<T>(ostream &out, MatrixT &matrix);
    T& operator()(int rows, int cols) const;
    MatrixT& operator=(const MatrixT &matrix);
    friend MatrixT operator*<T>(MatrixT &matrix, const T value); // TODO
    friend MatrixT operator*<T>(const T value, MatrixT &matrix); // TODO
    friend MatrixT operator+<T>(MatrixT &matrix1, MatrixT &matrix2); // TODO
    friend MatrixT operator-<T>(MatrixT &matrix1, MatrixT &matrix2); // TODO
    /* template<int K> // TODO
    friend MatrixT operator*(const MatrixT &matrix1, const MatrixT &matrix2){
        MatrixT targetMatrix;
        for (int i = 0; i < matrix1.getRows(); i++){
            for (int j = 0; j < K; j++){
                for (int k = 0; k < cols; k++){
                    targetMatrix(i, j) += matrix1(i, k) * matrix2(k, j);
                }
            }
        }
        return targetMatrix;
    } */
};

//Constructors
template<typename T>
MatrixT::Matrix(int rows, int cols): rows(rows), cols(cols){
    cout << "Constructor begin" << this << endl;
    this->matrix = new T*[rows];
    for (int i = 0; i < rows; i++){
        this->matrix[i] = new T[cols];
    }

    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = 0;
        }
        
    }
    
    cout << "Constructor end" << this << endl << *this << endl;
}

template<typename T>
MatrixT::Matrix(const MatrixT &matrix): MatrixT(matrix.getRows(), matrix.getCols()){
    cout << "Constructor-copy begin" << this << endl;
    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = matrix(i, j);
        }
    }
    cout << "Constructor-copy end" << this << endl << *this << endl;
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

    if(sizeList < this->rows * this->cols){
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
    int temp = this->rows;
    this->rows = this->cols;
    this->cols = temp;

    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = tempMatrix(j, i);
        }
    }
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
    T **tempArray = new T*[this->rows];
    for (int i = 0; i < this->rows; i++){
        tempArray[i] = new T[this->cols];
    }
    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            tempArray[i][j] = this->matrix[i][j];
        }
    }
    for (int i = 0; i < this->rows; i++){
        delete[] this->matrix[this->rows];
    }
    MatrixT matrix(rows, cols);
    matrix = tempArray;

    for (int i = 0; i < this->rows; i++){
        delete[] tempArray[this->rows];
    }
}

//Overloading operations
template<typename T>
MatrixT& MatrixT::operator=(const MatrixT &matrix){
    cout << "Operator= " << this << endl;
    if(this == &matrix) return *this;

    for (int i = 0; i < this->rows; i++){
        delete[] this->matrix[rows];
    }
    delete[] this->matrix;

    this->rows = matrix.getRows();
    this->cols = matrix.getCols();

    this->matrix = new T*[rows];
    for (int i = 0; i < rows; i++){
        this->matrix[i] = new T[cols];
    }

    for (int i = 0; i < this->rows; i++){
        delete[] this->matrix[rows];
    }
    delete[] this->matrix;

    cout << "Object deleted" << endl;

    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->cols; j++){
            this->matrix[i][j] = matrix(i, j); 
        }
    }
    cout << "Operator= " << endl << *this << endl;
    return *this;
}

template<typename T>
T& MatrixT::operator()(int rows, int cols) const{
    assert(rows >= 0 && rows < this->rows);
    assert(cols >= 0 && cols < this->cols);

    return this->matrix[rows][cols];
}

//Destructors
template<typename T>
MatrixT::~Matrix(){
    cout << "Destructor begin" << this << endl;
    for (int i = 0; i < this->rows; i++){
        delete[] this->matrix[rows];
    }
    delete[] this->matrix;
    this->matrix = nullptr;
    cout << "Destructor end" << this << endl;
}

#endif