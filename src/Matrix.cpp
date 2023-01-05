#include <random>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "Matrix.h"

using namespace std;

Matrix::Matrix(size_t height, size_t width, double filler): _height(height), _width(width) {
	if (width == 0 || height == 0)
		throw length_error("Matrix constructor has 0");

	_array = new double[height * width];
	for (size_t i = 0; i < height * width; i++)
		_array[i] = filler;
}

Matrix::Matrix(size_t height, size_t width, double min, double max) : _height(height), _width(width) {
	if (width == 0 || height == 0)
		throw length_error("Matrix constructor has 0");

	_array = new double[height * width];

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dist(min, max);

	for (int i = 0; i < width*height; i++)
		_array[i] = dist(gen);

}


Matrix::Matrix(const Matrix& other) : _height(other._height), _width(other._width)
{
	_array = new double[_height * _width];
	for (int i = 0; i < _height * _width; i++) {
		_array[i] = other._array[i];
	}
}

Matrix::Matrix(const vector<double>& vec): _height(vec.size()), _width(1)
{
	_array = new double[_height * _width];
	for (int i = 0; i < vec.size(); i++) {
		_array[i] = vec[i];
	}
}


Matrix::~Matrix(){
	delete[] _array;
}

Matrix& Matrix::operator=(Matrix& other){
	if (other._height > _height || other._width > _width) {
		_height = other._height;
		_width = other._width;
		delete[] _array;
		_array = new double[_height * _width];
		for (int i = 0; i < _height * _width; i++) {
			_array[i] = other._array[i];
		}
	}
	else
	{
		for (size_t i = 0; i < other._height; i++)
			for (size_t j = 0; j < other._width; j++)
				(*this)(i, j) = other(i, j);
	}
	return *this;
}

Matrix& Matrix::operator=(const vector<double>& vec)
{
	if (vec.size() > _width * _height)
		throw invalid_argument("Matrix operator= vector size > _array size");
	for (size_t i = 0; i < vec.size(); i++)
		_array[i] = vec[i];
	
}


vector<double> Matrix::toVector()
{
	vector<double> result(_width * _height);
	for (size_t i = 0; i < result.size(); i++) {
		result[i] = _array[i];
	}
	return result;
}

Matrix Matrix::operator+(double number){
	Matrix result(_height, _width);
	for (size_t i = 0; i < _height * _width; i++) {
		result._array[i] = _array[i] + number;
	}
	return result;
}


Matrix Matrix::operator-(double number){
	Matrix result(_height, _width);
	for (size_t i = 0; i < _height * _width; i++) {
		result._array[i] = _array[i] - number;
	}
	return result;
}

Matrix Matrix::operator*(double number){
	Matrix result(_height, _width);
	for (size_t i = 0; i < _height * _width; i++) {
		result._array[i] = _array[i] * number;
	}
	return result;
}


Matrix Matrix::operator/(double number){
	if (number == 0)
		throw runtime_error("Matrix operator/ divide by zero error");
	Matrix result(_height, _width);
	for (size_t i = 0; i < _height * _width; i++) {
		result._array[i] = _array[i] / number;
	}
	return result;
}


void Matrix::print(size_t precision)
{
	cout << setprecision(precision) << fixed;

	bool negative = false;
	int max = abs(_array[0]) + 0.5;
	for (int i = 1; i < _width * _height; i++) {
		if (max < abs(_array[i]) + 0.5)
			max = abs(_array[i]) + 0.5;

		if (_array[i] < 0)
			negative = true;
	}

	size_t width = precision + int(precision > 0) + int(negative);
	do {
		width++;
		max /= 10;
	} while (max > 0);

	cout << " _" << setw(_width * (width + 1) + 1) << "_\n";
	cout << "| " << setw(_width * (width + 1) + 2) << "|\n";

	for (size_t i = 0; i < _height; i++) {
		cout << "| ";
		for (size_t j = 0; j < _width; j++)
			cout << setw(width) << (*this)(i, j) << " ";
		cout << "|\n";
	}
	cout << "|_" << setw(_width * (width + 1)+1) << "_|" << endl;
}

double& Matrix::operator() (size_t row, size_t column){
	if (row >= _height || column >= _width)
		throw out_of_range("Matrix index out of range");
	return _array[row * _width + column];
}

double& Matrix::operator[](size_t i)
{
	if (i > _width*_height)
		throw out_of_range("Matrix index out of range");
	return _array[i];
}

Matrix Matrix::operator*(Matrix& other)
{
	if (_width != other._height)
		throw invalid_argument("_width != other._height");
	Matrix result(_height, other._width);
	for (size_t i = 0; i < _height; i++)
		for (size_t j = 0; j < other._width; j++)
			for (size_t k = 0; k < _width; k++) {
				result(i, j) += (*this)(i, k) * other(k, j);
			}
	return result;
}


