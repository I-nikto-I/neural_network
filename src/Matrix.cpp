#include <random>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "Matrix.h"


Matrix::Matrix(int height, int width = 1) {
	if (width <= 0 || height <= 0)
		throw std::length_error("Matrix constructor has 0 or negative size");
	_height = height;
	_width = width;
	_array = new double[height * width]{};
}


Matrix::Matrix(const Matrix& other):Matrix(other._height, other._width)
{
	for (int i = 0; i < _height * _width; i++) {
		_array[i] = other._array[i];
	}
}


Matrix::~Matrix(){
	delete[] _array;
}


Matrix Matrix::operator+(double number){
	Matrix result(_height, _width);
	for (int i = 0; i < _height * _width; i++) {
		result._array[i] = _array[i] + number;
	}
	return result;
}


Matrix Matrix::operator-(double number){
	Matrix result(_height, _width);
	for (int i = 0; i < _height * _width; i++) {
		result._array[i] = _array[i] - number;
	}
	return result;
}

Matrix Matrix::operator*(double number){
	Matrix result(_height, _width);
	for (int i = 0; i < _height * _width; i++) {
		result._array[i] = _array[i] * number;
	}
	return result;
}


Matrix Matrix::operator/(double number){
	if (number == 0)
		throw std::runtime_error("Matrix operator/ divide by zero error");
	Matrix result(_height, _width);
	for (int i = 0; i < _height * _width; i++) {
		result._array[i] = _array[i] / number;
	}
	return result;
}


void Matrix::print(int precision)
{
	if (precision < 0)
		throw std::invalid_argument("Matrix::print precision is negative");
	std::cout << std::setprecision(precision) << std::fixed;

	bool negative = false;
	int max = abs(_array[0]) + 0.5;
	for (int i = 1; i < _width * _height; i++) {
		if (max < abs(_array[i]) + 0.5)
			max = abs(_array[i]) + 0.5;

		if (_array[i] < 0)
			negative = true;
	}

	int width = precision + int(precision > 0) + int(negative);
	do {
		width++;
		max /= 10;
	} while (max > 0);

	std::cout << " _" << std::setw(_width * (width + 1) + 1) << "_\n";
	std::cout << "| " << std::setw(_width * (width + 1) + 2) << "|\n";

	for (int i = 0; i < _height; i++) {
		std::cout << "| ";
		for (int j = 0; j < _width; j++)
			std::cout << std::setw(width) << (*this)(i, j) << " ";
		std::cout << "|\n";
	}
	std::cout << "|_" << std::setw(_width * (width + 1)+1) << "_|" << std::endl;
}

double& Matrix::operator() (int row, int column){
	if (row < 0 || column < 0)
		throw std::out_of_range("Matrix index is negative");
	if (row >= _height || column >= _width)
		throw std::out_of_range("Matrix index out of range");
	return _array[row * _width + column];
}

Matrix Matrix::operator*(Matrix& other)
{
	if (_width != other._height)
		throw std::invalid_argument("_width != other._height");
	Matrix result(_height, other._width);
	for (int i = 0; i < _height; i++)
		for (int j = 0; j < other._width; j++)
			for (int k = 0; k < _width; k++) {
				result(i, j) += (*this)(i, k) * other(k, j);
			}
	return result;
}

Matrix Matrix::Random(int height, int width, int min=0, int max=1)
{
	Matrix matrix(height, width);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(min, max);

	for (int i = 0; i < matrix._width * matrix._height; i++)
		matrix._array[i] = dist(gen);
	return matrix;
}


