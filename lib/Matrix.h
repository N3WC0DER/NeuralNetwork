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
ostream& operator<<(ostream& out, MatrixT& matrix) {
	int max = 0;
	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			if ((log10(matrix(i, j)) + 1 / 1) > max) {
				max = log10(matrix(i, j)) + 1 / 1;
			}
		}
	}

	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			out.width(max);
			out << matrix(i, j) << " ";
		}
		out << endl;
	}
	return out;
}

template<typename T>
MatrixT operator*(const MatrixT& matrix1, const MatrixT& matrix2) {
	assert(matrix1.getCols() == matrix2.getRows());

	MatrixT targetMatrix(matrix1.getRows(), matrix2.getCols());
	for (int i = 0; i < matrix1.getRows(); i++) {
		for (int j = 0; j < matrix2.getCols(); j++) {
			for (int k = 0; k < matrix1.getCols(); k++) {
				targetMatrix(i, j) += matrix1(i, k) * matrix2(k, j);
			}
		}
	}
	return targetMatrix;
}

template<typename T>
MatrixT operator*(const MatrixT& matrix, const T value) {
	MatrixT targetMatrix(matrix.getRows(), matrix.getCols());

	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			targetMatrix(i, j) = matrix(i, j) * value;
		}
	}
	return targetMatrix;
}

template<typename T>
MatrixT operator*(const T value, const MatrixT& matrix) {
	MatrixT targetMatrix(matrix.getRows(), matrix.getCols());
	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			targetMatrix(i, j) = matrix(i, j) * value;
		}
	}
	return targetMatrix;
}

template<typename T>
MatrixT operator+(const MatrixT& matrix1, const MatrixT& matrix2) {
	assert(matrix1.getRows() == matrix2.getRows() && matrix1.getCols() == matrix2.getCols());

	MatrixT targetMatrix(matrix1.getRows(), matrix1.getCols());
	for (int i = 0; i < matrix1.getRows(); i++) {
		for (int j = 0; j < matrix1.getCols(); j++) {
			targetMatrix(i, j) = matrix1(i, j) + matrix2(i, j);
		}
	}
	return targetMatrix;
}

template<typename T>
MatrixT operator+(const MatrixT& matrix, const T value) {
	MatrixT targetMatrix(matrix.getRows(), matrix.getCols());
	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			targetMatrix(i, j) = matrix(i, j) + value;
		}
	}
	return targetMatrix;
}

template<typename T>
MatrixT operator+(const T value, const MatrixT& matrix) {
	MatrixT targetMatrix(matrix.getRows(), matrix.getCols());
	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			targetMatrix(i, j) = matrix(i, j) + value;
		}
	}
	return targetMatrix;
}

template<typename T>
MatrixT operator-(const MatrixT& matrix1, const MatrixT& matrix2) {
	assert(matrix1.getRows() == matrix2.getRows() && matrix1.getCols() == matrix2.getCols());

	MatrixT targetMatrix(matrix1.getRows(), matrix1.getCols());
	for (int i = 0; i < matrix1.getRows(); i++) {
		for (int j = 0; j < matrix1.getCols(); j++) {
			targetMatrix(i, j) = matrix1(i, j) - matrix2(i, j);
		}
	}
	return targetMatrix;
}

template<typename T>
MatrixT operator-(const MatrixT& matrix, const T value) {
	MatrixT targetMatrix(matrix.getRows(), matrix.getCols());
	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			targetMatrix(i, j) = matrix(i, j) - value;
		}
	}
	return targetMatrix;
}

template<typename T>
MatrixT operator-(const T value, const MatrixT& matrix) {
	MatrixT targetMatrix(matrix.getRows(), matrix.getCols());
	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			targetMatrix(i, j) = matrix(i, j) - value;
		}
	}
	return targetMatrix;
}

template<typename T>
MatrixT operator%(const MatrixT& matrix1, const MatrixT& matrix2) {
	assert(matrix1.getRows() == matrix2.getRows() && matrix1.getCols() == matrix2.getCols());

	MatrixT targetMatrix(matrix1.getRows(), matrix1.getCols());
	for (int i = 0; i < targetMatrix.getRows(); i++) {
		for (int j = 0; j < targetMatrix.getCols(); j++) {
			targetMatrix(i, j) = matrix1(i, j) * matrix2(i, j);
		}
	}
	return targetMatrix;
}

template<typename T>
void operator+=(MatrixT& matrix1, const MatrixT& matrix2) {
	assert(matrix1.getRows() == matrix2.getRows() && matrix1.getCols() == matrix2.getCols());

	for (int i = 0; i < matrix1.getRows(); i++) {
		for (int j = 0; j < matrix1.getCols(); j++) {
			matrix1(i, j) = matrix1(i, j) + matrix2(i, j);
		}
	}
}

template<typename T>
void operator+=(MatrixT& matrix, const T value) {
	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			matrix(i, j) = matrix(i, j) + value;
		}
	}
}

template<typename T>
void operator-=(MatrixT& matrix1, const MatrixT& matrix2) {
	assert(matrix1.getRows() == matrix2.getRows() && matrix1.getCols() == matrix2.getCols());

	for (int i = 0; i < matrix1.getRows(); i++) {
		for (int j = 0; j < matrix1.getCols(); j++) {
			matrix1(i, j) = matrix1(i, j) - matrix2(i, j);
		}
	}
}

template<typename T>
void operator-=(MatrixT& matrix, const T value) {
	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			matrix(i, j) = matrix(i, j) - value;
		}
	}
}

template<typename T>
void operator*=(MatrixT& matrix1, const MatrixT& matrix2) {
	assert(matrix1.getCols() == matrix2.getRows());

	MatrixT targetMatrix(matrix1.getRows(), matrix2.getCols());
	for (int i = 0; i < matrix1.getRows(); i++) {
		for (int j = 0; j < matrix2.getCols(); j++) {
			for (int k = 0; k < matrix1.getCols(); k++) {
				targetMatrix(i, j) += matrix1(i, k) * matrix2(k, j);
			}
		}
	}
	matrix1 = targetMatrix;
}

template<typename T>
void operator*=(MatrixT& matrix, const T value) {
	for (int i = 0; i < matrix.getRows(); i++) {
		for (int j = 0; j < matrix.getCols(); j++) {
			matrix(i, j) = matrix(i, j) * value;
		}
	}
}

template<typename T>
void operator%=(MatrixT& matrix1, const MatrixT& matrix2) {
	assert(matrix1.getRows() == matrix2.getRows() && matrix1.getCols() == matrix2.getCols());

	for (int i = 0; i < matrix1.getRows(); i++) {
		for (int j = 0; j < matrix1.getCols(); j++) {
			matrix1(i, j) = matrix1(i, j) * matrix2(i, j);
		}
	}
}

template<typename T>
class Matrix {
private:
	T** matrix = nullptr;
	int rows = 0;
	int cols = 0;

	void memoryAlloc();
	void memoryClear();
public:
	Matrix();
	Matrix(const int rows, const int cols);
	Matrix(const MatrixT& matrix);
	Matrix(T** array, int rows, int cols);
	Matrix(const initializer_list<initializer_list<T>>& list);

	int getRows() const;
	int getCols() const;
	int getSize() const;
	MatrixT transposition() const;
	void random(const T min, const T max);
	void resize(int rows, int cols);

	~Matrix();

	friend ostream& operator<<<T>(ostream& out, MatrixT& matrix);
	T& operator()(int rows, int cols) const;
	void operator()();
	MatrixT& operator=(const MatrixT& matrix);
	friend MatrixT operator*<T>(const MatrixT& matrix1, const MatrixT& matrix2);
	friend MatrixT operator*<T>(const MatrixT& matrix, const T value);
	friend MatrixT operator*<T>(const T value, const MatrixT& matrix);
	friend MatrixT operator+<T>(const MatrixT& matrix1, const MatrixT& matrix2);
	friend MatrixT operator+<T>(const MatrixT& matrix, const T value);
	friend MatrixT operator+<T>(const T value, const MatrixT& matrix);
	friend MatrixT operator-<T>(const MatrixT& matrix1, const MatrixT& matrix2);
	friend MatrixT operator-<T>(const MatrixT& matrix, const T value);
	friend MatrixT operator-<T>(const T value, const MatrixT& matrix);
	friend MatrixT operator%<T>(const MatrixT& matrix1, const MatrixT& matrix2);
	friend void operator+=<T>(MatrixT& matrix1, const MatrixT& matrix2);
	friend void operator+=<T>(MatrixT& matrix, const T value);
	friend void operator-=<T>(MatrixT& matrix1, const MatrixT& matrix2);
	friend void operator-=<T>(MatrixT& matrix, const T value);
	friend void operator*=<T>(MatrixT& matrix1, const MatrixT& matrix2);
	friend void operator*=<T>(MatrixT& matrix, const T value);
	friend void operator%=<T>(MatrixT& matrix1, const MatrixT& matrix2);
};

//Constructors
template<typename T>
MatrixT::Matrix() {}

template<typename T>
MatrixT::Matrix(const int rows, const int cols) : rows(rows), cols(cols) {
	memoryAlloc();

	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			this->matrix[i][j] = 0;
		}
	}
}

template<typename T>
MatrixT::Matrix(const MatrixT& matrix) : MatrixT(matrix.getRows(), matrix.getCols()) {
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			this->matrix[i][j] = matrix(i, j);
		}
	}
}

template<typename T>
MatrixT::Matrix(T** array, int rows, int cols) : MatrixT(rows, cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			this->matrix[i][j] = array[i][j];
		}
	}
}

template<typename T>
MatrixT::Matrix(const initializer_list<initializer_list<T>>& list) : MatrixT(list.size(), list.begin()->size()) {
	for (int i = 0; i < list.size(); i++) {
		for (int j = 0; j < (list.begin() + i)->size(); j++) {
			this->matrix[i][j] = *((list.begin() + i)->begin() + j);
		}
	}
}

//Methods
template<typename T>
void MatrixT::memoryAlloc() {
	if (this->rows != 0 && this->cols != 0 && this->matrix == nullptr) {
		this->matrix = new T * [this->rows];
		for (int i = 0; i < this->rows; i++) {
			this->matrix[i] = new T[this->cols];
		}
	}
}

template<typename T>
void MatrixT::memoryClear() {
	if (this->rows != 0 && this->cols != 0 && this->matrix != nullptr) {
		for (int i = 0; i < this->rows; i++) {
			delete[] this->matrix[i];
		}
		delete[] this->matrix;
		this->matrix = nullptr;
	}
}

template<typename T>
int MatrixT::getRows() const {
	return this->rows;
}

template<typename T>
int MatrixT::getCols() const {
	return this->cols;
}

template<typename T>
int MatrixT::getSize() const {
	return this->rows * this->cols;
}

template<typename T>
MatrixT MatrixT::transposition() const {
	MatrixT tempMatrix(this->getCols(), this->getRows());

	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			tempMatrix(j, i) = this->matrix[i][j];
		}
	}

	return tempMatrix;
}

template<typename T>
void MatrixT::random(const T min, const T max) {
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
			this->matrix[i][j] = static_cast<T>(rand() * fraction * (max - min + 1) + min);
		}
	}
}

template<typename T>
void MatrixT::resize(const int rows, const int cols) {
	assert(!(rows == this->rows && cols == this->cols));

	int tempRows = this->rows;
	int tempCols = this->cols;
	T** temp = new T*[tempRows];
	for (int i = 0; i < tempRows; i++) {
		temp[i] = new T[tempCols];
	}

	for (int i = 0; i < tempRows; i++) {
		for (int j = 0; j < tempCols; j++) {
			temp[i][j] = this->matrix[i][j];
		}
	}
	memoryClear();
	this->rows = rows;
	this->cols = cols;
	memoryAlloc();

	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			this->matrix[i][j] = 0;
		}
	}
	if (this->getSize() < tempRows * tempCols) {
		for (int i = 0; i < this->rows; i++) {
			for (int j = 0; j < this->cols; j++) {
				this->matrix[i][j] = temp[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < tempRows; i++) {
			for (int j = 0; j < tempCols; j++) {
				this->matrix[i][j] = temp[i][j];
			}
		}
	}

	for (int i = 0; i < tempRows; i++) {
		delete[] temp[i];
	}
	delete[] temp;
	temp = nullptr;
}

//Overloading operations
template<typename T>
MatrixT& MatrixT::operator=(const MatrixT& matrix) {
	if (this == &matrix) return *this;

	memoryClear();

	this->rows = matrix.getRows();
	this->cols = matrix.getCols();

	memoryAlloc();

	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			this->matrix[i][j] = 0;
		}
	}

	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			this->matrix[i][j] = matrix(i, j);
		}
	}
	return *this;
}

template<typename T>
T& MatrixT::operator()(int rows, int cols) const {
	assert(rows >= 0 && rows < this->rows);
	assert(cols >= 0 && cols < this->cols);

	return this->matrix[rows][cols];
}

template<typename T>
void MatrixT::operator()() {
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			this->matrix[i][j] = 0;
		}
	}
}
//Destructors
template<typename T>
MatrixT::~Matrix() {
	memoryClear();
}