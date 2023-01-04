#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
using namespace std;

class Matrix{
private:
	size_t _height;
	size_t _width;
	double* _array;
public:
	Matrix(size_t height, size_t width=1, double filler=0);
	Matrix(size_t height, size_t width, double min, double max);
	Matrix(const Matrix&);
	Matrix(const vector<double>&);
	~Matrix();

	Matrix& operator= (Matrix&);
	Matrix& operator= (const vector<double>&);

	void print(size_t precision = 2);
	double& operator() (size_t row, size_t column = 0);
	double& operator[] (size_t i);
	size_t getWidth() {return _width;}
	size_t getHeight() {return _height;}
	vector<double> toVector();

	Matrix operator+ (double);
	Matrix operator- (double);
	Matrix operator* (double);
	Matrix operator/ (double);

	Matrix operator* (Matrix&);
	//Matrix operator+ (Matrix&);
	//Matrix operator- (Matrix&);

	template <typename Type>
	Matrix(size_t height, size_t width, Type &in): _height(height), _width(width) {
		if (width == 0 || height == 0)
			throw length_error("Matrix constructor has 0");

		_array = new double[height * width];
		for (size_t i = 0; i < height * width; i++)
			in >> _array[i];
	}

	template <typename Type>
	friend Type& operator<< (Type& out, Matrix& matrix) {
		if (matrix._width > 1)
			for (int i = 0; i < matrix._height; i++) {
				for (int j = 0; j < matrix._width; j++)
					out << matrix(i, j) << " ";
				out << "\n";
			}
		else {
			for (int i = 0; i < matrix._height; i++) 
				out << matrix(i) << " ";
			out << "\n";
		}

		return out;
	}

	template <typename Type>
	friend Type& operator>> (Type& out, Matrix& matrix) {
		for (int i = 0; i < matrix._height * matrix._width; i++)
			out >> matrix[i];

		return out;
	}
};

#endif
