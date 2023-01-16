#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <initializer_list>
#include <fstream>

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
	Matrix(ifstream& file, size_t height, size_t width = 1);
	~Matrix();

    Matrix& operator= (const Matrix&);

	Matrix& setSize(size_t height, size_t width);
	void writeToBinFile(ofstream& file);
	void random(double min, double max);
	void print(size_t precision = 2) const;
	double& operator() (size_t row, size_t column = 0);
	double operator() (size_t row, size_t column = 0) const;
	double& operator[] (size_t i);
	double operator[] (size_t i) const;
	size_t getWidth() const {return _width;}
	size_t getHeight() const {return _height;}
	size_t size() const { return _width * _height; };
	Matrix map(double (*)(double)) const;
	Matrix& Matrix::insert(const Matrix&);
	double sum() const;
	vector<double> toVector() const;

	Matrix operator+ (double) const;
	Matrix operator- (double) const;
	Matrix operator* (double) const;
	Matrix operator/ (double) const;

	Matrix operator* (const Matrix&) const;
	Matrix operator+ (const Matrix&) const;
	Matrix operator- (const Matrix&) const;

	void operator+= (const Matrix&);
	void operator-= (const Matrix&);

	Matrix& hadamarProduct(const Matrix&);
	Matrix& add(const Matrix&);
	Matrix transpose() const;
	Matrix cut(size_t height, size_t width = 1) const;


	

	template <typename Type>
	Matrix(size_t height, size_t width, Type &in): _height(height), _width(width), _size(width*height) {

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
			for (int i = 0; i < matrix._size; i++) 
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
