#include <random>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "Matrix.h"

using namespace std;

Matrix::Matrix(size_t height, size_t width, double filler): 
	_height(height), _width(width), _size(height * width) {
	if (width == 0 || height == 0)
		throw length_error("Matrix constructor has 0");

	_array = new double[_size];
	for (size_t i = 0; i < _size; i++)
		_array[i] = filler;
}

Matrix::Matrix(size_t height, size_t width, double min, double max):
	_height(height), _width(width), _size(height * width) {
	if (width == 0 || height == 0)
		throw length_error("Matrix constructor has 0");

	_array = new double[_size];

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dist(min, max);

	for (int i = 0; i < _size; i++)
		_array[i] = dist(gen);

}


Matrix::Matrix(const Matrix& other):
	_height(other._height), _width(other._width), _size(other._size){
	_array = new double[_size];
	for (int i = 0; i < _size; i++) {
		_array[i] = other._array[i];
	}
}

Matrix::Matrix(const vector<double>& vec): _height(vec.size()), _width(1)
{
	_size = _height * _width;
	_array = new double[_size];
	for (int i = 0; i < vec.size(); i++) {
		_array[i] = vec[i];
	}
}

Matrix::Matrix(initializer_list<double> array) : _height(array.size()), _width(1), _size(array.size()) {
	_array = new double[_size];
	for (size_t i = 0; i < _size; i++) 
		_array[i] = array.begin()[i];
}

Matrix::Matrix(ifstream& file, size_t height, size_t width): _height(height), _width(width), _size(width*height){
	_array = new double[_size];
	int sizeOfDouble = sizeof(double);
	for (size_t i = 0; i < _size; i++)
		file.read((char*)&_array[i], sizeOfDouble);
}


Matrix::~Matrix(){
	delete[] _array;
}

Matrix& Matrix::insert(Matrix& other){
	if (other._height > _height || other._width > _width) {
		_height = other._height;
		_width = other._width;
		_size = other._size;
		delete[] _array;
		_array = new double[_size];
		for (int i = 0; i < _size; i++) {
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

Matrix& Matrix::operator=(Matrix& other){
	_height = other._height;
	_width = other._width;
	_size = other._size;
	delete[] _array;

	_array = new double[_size];
	for (int i = 0; i < _size; i++) {
		_array[i] = other._array[i];
	}
	return *this;
}


Matrix Matrix::map(double(*function)(double))
{
	Matrix result(_height, _width);
	for (size_t i = 0; i < _size; i++) {
		result._array[i] = function(_array[i]);
	}
	return result;
}

double Matrix::sum()
{
	double result = 0;
	for (size_t i = 0; i < _size; i++) {
		result += _array[i];
	}
	return result;
}

vector<double> Matrix::toVector()
{
	vector<double> result(_size);
	for (size_t i = 0; i < result.size(); i++) {
		result[i] = _array[i];
	}
	return result;
}

Matrix Matrix::operator+(double number){
	Matrix result(_height, _width);
	for (size_t i = 0; i < _size; i++) {
		result._array[i] = _array[i] + number;
	}
	return result;
}


Matrix Matrix::operator-(double number){
	Matrix result(_height, _width);
	for (size_t i = 0; i < _size; i++) {
		result._array[i] = _array[i] - number;
	}
	return result;
}

Matrix Matrix::operator*(double number){
	Matrix result(_height, _width);
	for (size_t i = 0; i < _size; i++) {
		result._array[i] = _array[i] * number;
	}
	return result;
}


Matrix Matrix::operator/(double number){
	if (number == 0)
		throw runtime_error("Matrix operator/ divide by zero error");
	Matrix result(_height, _width);
	for (size_t i = 0; i < _size; i++) {
		result._array[i] = _array[i] / number;
	}
	return result;
}


void Matrix::writeToBinFile(ofstream& file){
	int sizeOfDouble = sizeof(double);
	for (size_t i = 0; i < _size; i++)
		file.write((char*)&_array[i], sizeOfDouble);
}

void Matrix::random(double min, double max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dist(min, max);

	for (int i = 0; i < _size; i++)
		_array[i] = dist(gen);
}

void Matrix::print(size_t precision)
{
	cout << setprecision(precision) << fixed;

	bool negative = false;
	int max = abs(_array[0]) + 0.5;
	for (int i = 1; i < _size; i++) {
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
	if (i > _size)
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

Matrix Matrix::operator+(Matrix& other)
{
	if (_width < other._width || _height < other._height)
		throw invalid_argument("Matrix operator+ size < other size");
	Matrix result(*this);
	for (size_t i = 0; i < other._height; i++)
		for (size_t j = 0; j < other._width; j++)
			result(i, j) += other(i, j);
	return result;
}

Matrix Matrix::operator-(Matrix& other)
{
	if (_width < other._width || _height < other._height)
		throw invalid_argument("Matrix operator- size < other size");
	Matrix result(*this);
	for (size_t i = 0; i < other._height; i++)
		for (size_t j = 0; j < other._width; j++)
			result(i, j) -= other(i, j);
	return result;
}

Matrix Matrix::operator+=(Matrix& other)
{
	if (_width < other._width || _height < other._height)
		throw invalid_argument("Matrix operator+= size < other size");

	for (size_t i = 0; i < other._height; i++)
		for (size_t j = 0; j < other._width; j++)
			(*this)(i, j) += other(i, j);
	return*this;
}

Matrix Matrix::operator-=(Matrix& other)
{
	if (_width < other._width || _height < other._height)
		throw invalid_argument("Matrix operator-= size < other size");

	for (size_t i = 0; i < other._height; i++)
		for (size_t j = 0; j < other._width; j++)
			(*this)(i, j) -= other(i, j);
	return*this;
}

Matrix Matrix::hadamarProduct(Matrix& other)
{
	if (_width < other._width || _height < other._height)
		throw invalid_argument("Matrix operator- size < other size");
	Matrix result(*this);
	for (size_t i = 0; i < other._height; i++)
		for (size_t j = 0; j < other._width; j++)
			result(i, j) *= other(i, j);
	return result;
}

Matrix Matrix::transponse(){
	Matrix result(_width, _height);
	for (size_t i = 0; i < _height; i++)
		for (size_t j = 0; j < _width; j++)
			result(j, i) = (*this)(i, j);
	return result;
}

Matrix Matrix::cut(size_t height, size_t width)
{
	if (_height < height || _width < width)
		throw invalid_argument("Matrix::cut size error");
	Matrix result(height, width);
	for (size_t i = 0; i < height; i++)
		for (size_t j = 0; j < width; j++)
			result(i, j) = (*this)(i, j);
	return result;
}


