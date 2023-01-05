#include "ActivationFunction.h"
#include <cmath>

using namespace std;

std::vector<double(*)(double)> ActivationFunction::_functions = { sigmoidFunction, tanhFunction, reLuFunction, softsignFunction, mySoftsignFunction };
std::vector<double(*)(double)> ActivationFunction::_derivatives = { sigmoidDerivative, tanhDerivative, reLuDerivative, softsignDerivative, mySoftsignDerivative };

ActivationFunction::ActivationFunction(size_t number): _number(number){

}

double ActivationFunction::operator()(double x)
{
	return _functions[_number](x);
}

double ActivationFunction::operator[](double x)
{
	return _derivatives[_number](x);
}

Matrix ActivationFunction::operator()(Matrix& matrix){
	Matrix result(matrix._height, matrix._width);
	for (size_t i = 0; i < matrix._width * matrix._height; i++) {
		result._array[i] = _functions[_number](matrix._array[i]);
	}
	return result;
}

Matrix ActivationFunction::operator[](Matrix& matrix) {
	Matrix result(matrix._height, matrix._width);
	for (size_t i = 0; i < matrix._width * matrix._height; i++) {
		result._array[i] = _derivatives[_number](matrix._array[i]);
	}
	return result;
}

double ActivationFunction::sigmoidFunction(double x)
{
	return 1/(1+exp(-x));
}

double ActivationFunction::sigmoidDerivative(double x)
{
	x = 1 / (1 + exp(-x));
	return x*(1-x);
}

double ActivationFunction::tanhFunction(double x)
{
	return std::tanh(x);
}

double ActivationFunction::tanhDerivative(double x)
{
	x = std::tanh(x);
	return 1 - x*x;
}

double ActivationFunction::reLuFunction(double x)
{
	return x * (x < 0);
}
double ActivationFunction::reLuDerivative(double x)
{
	return x < 0;
}

double ActivationFunction::softsignFunction(double x)
{
	return x / (1 + abs(x));
}

double ActivationFunction::softsignDerivative(double x)
{
	x = 1 + abs(x);
	return 1 / (x*x);
}

double ActivationFunction::mySoftsignFunction(double x)
{
	return (x / (abs(x) + 1) + 1) / 2;
}

double ActivationFunction::mySoftsignDerivative(double x)
{
	x = 1 + abs(x);
	return 0.5 / (x * x);
}
