#include "ActivationFunction.h"
#include <cmath>
#include <stdexcept>

using namespace std;

std::vector<double(*)(double)> ActivationFunction::_functions = { linearFunction, sigmoidFunction, tanhFunction, reLuFunction, softsignFunction, mySoftsignFunction };
std::vector<double(*)(double)> ActivationFunction::_derivatives = { linearDerivative, sigmoidDerivative, tanhDerivative, reLuDerivative, softsignDerivative, mySoftsignDerivative };

ActivationFunction::ActivationFunction(size_t number): _number(number){}

double ActivationFunction::operator()(double x){
	return _functions[_number](x);
}

double ActivationFunction::operator[](double x){
	return _derivatives[_number](x);
}

Matrix ActivationFunction::operator()(Matrix& matrix){
	if (_number == -1)
		return softmaxFunction(matrix);
	return matrix.map(_functions[_number]);
}

Matrix ActivationFunction::operator[](Matrix& matrix) {
	if (_number == -1)
		return Matrix(matrix._height, matrix._width, 1);
	return matrix.map(_derivatives[_number]);
}

double ActivationFunction::error(Matrix& activation, Matrix& target)
{
	if (_number > 0)
		return meanSquearedErrorFunction(activation, target);
	return crossEntropyErrorFunction(activation, target);
}

Matrix ActivationFunction::errorDerivative(Matrix& activation, Matrix& target)
{
	if (_number > 0)
		return meanSquearedErrorDerivative(activation, target);
	return CESoftmaxErrorDerivative(activation, target);
}

double ActivationFunction::sigmoidFunction(double x){
	return 1/(1+exp(-x));
}

double ActivationFunction::sigmoidDerivative(double x){
	x = 1 / (1 + exp(-x));
	return x*(1-x);
}

double ActivationFunction::tanhFunction(double x){
	return std::tanh(x);
}

double ActivationFunction::tanhDerivative(double x){
	x = std::tanh(x);
	return 1 - x*x;
}

double ActivationFunction::reLuFunction(double x){
	return x * (x < 0);
}
double ActivationFunction::reLuDerivative(double x){
	return x < 0;
}

double ActivationFunction::softsignFunction(double x){
	return x / (1 + abs(x));
}

double ActivationFunction::softsignDerivative(double x){
	x = 1 + abs(x);
	return 1 / (x*x);
}

double ActivationFunction::mySoftsignFunction(double x){
	return (x / (abs(x) + 1) + 1) / 2;
}

double ActivationFunction::mySoftsignDerivative(double x){
	x = 1 + abs(x);
	return 0.5 / (x * x);
}

Matrix ActivationFunction::softmaxFunction(Matrix& matrix)
{
	Matrix e = matrix.map(exp);
	double sum = e.sum();
	if (sum == 0)
		return e;
	return e / sum;
}

double ActivationFunction::crossEntropyErrorFunction(Matrix& activation, Matrix& target)
{
	double result = 0;
	for (size_t i = 0; i < target._width * target._height; i++) {
		result += -target._array[i] * log(activation._array[i]);
	}
	return result;
}

Matrix ActivationFunction::CESoftmaxErrorDerivative(Matrix& activation, Matrix& target)
{
	return activation - target;
}

double ActivationFunction::meanSquearedErrorFunction(Matrix& activation, Matrix& target)
{
	double result = 0;
	for (size_t i = 0; i < target._width * target._height; i++) {
		double x = activation._array[i] - target._array[i];
		result += x*x;
	}
	return result;
}

Matrix ActivationFunction::meanSquearedErrorDerivative(Matrix& activation, Matrix& target)
{
	return (activation-target)*2;
}

