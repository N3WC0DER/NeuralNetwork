#pragma once
#include "Eigen/Eigen/Dense"

int getRandomNumber(int min, int max);

float getRandomFloatNumber(float min, float max);

Eigen::MatrixXf randomMatrix(Eigen::MatrixXf matrix, int rows = 0, int cols = 0);

Eigen::MatrixXf sigmoida(Eigen::MatrixXf matrix);