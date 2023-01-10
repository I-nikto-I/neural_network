#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <initializer_list>
using namespace std;

class Matrix{
friend class ActivationFunction;
private:
	size_t _height;
	size_t _width;
	size_t _size;
	double* _array;
public:
	Matrix(size_t height, size_t width=1, double filler=0);
	Matrix(size_t height, size_t width, double min, double max);
	Matrix(const Matrix&);
	Matrix(const vector<double>&);
	Matrix(initializer_list<double>);
	~Matrix();

    Matrix& operator= (Matrix&);

	void random(double min, double max);
	void print(size_t precision = 2);
	double& operator() (size_t row, size_t column = 0);
	double& operator[] (size_t i);
	size_t getWidth() const {return _width;}
	size_t getHeight() const {return _height;}
	size_t size() const { return _width * _height; };
	Matrix map(double (*)(double));
	Matrix& Matrix::insert(Matrix&);
	double sum();
	vector<double> toVector();

	Matrix operator+ (double);
	Matrix operator- (double);
	Matrix operator* (double);
	Matrix operator/ (double);

	Matrix operator* (Matrix&);
	Matrix operator+ (Matrix&);
	Matrix operator- (Matrix&);

	Matrix operator+= (Matrix&);
	Matrix operator-= (Matrix&);

	Matrix hadamarProduct(Matrix&);
	Matrix transponse();
	Matrix cut(size_t height, size_t width = 1);


	

	template <typename Type>
	Matrix(size_t height, size_t width, Type &in): _height(height), _width(width), _size(width*height) {
		if (width == 0 || height == 0)
			throw length_error("Matrix constructor has 0");

		_array = new double[_size];
		for (size_t i = 0; i < _size; i++)
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
