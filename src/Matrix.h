#ifndef MATRIX_H
#define MATRIX_H

class Matrix{
private:
	int _height;
	int _width;
	double* _array;
public:
	Matrix(int, int);
	Matrix(const Matrix&);
	~Matrix();

	void print(int);
	double& operator() (int, int=0);
	int getWidth() {return _width;}
	int getHeight() {return _height;}

	Matrix operator+ (double);
	Matrix operator- (double);
	Matrix operator* (double);
	Matrix operator/ (double);

	Matrix operator* (Matrix&);

	Matrix static Random(int, int, int, int);

};

#endif