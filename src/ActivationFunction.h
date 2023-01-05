#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H
#include "Matrix.h"

#include <vector>

class ActivationFunction {
private:
	size_t _number;

public:
	ActivationFunction(size_t = 0);

	static ActivationFunction sigmoid()    { return ActivationFunction(0); };
	static ActivationFunction tanh()       { return ActivationFunction(1); };
	static ActivationFunction ReLU()       { return ActivationFunction(2); };
	static ActivationFunction softsign()   { return ActivationFunction(3); };
	static ActivationFunction mySoftsign() { return ActivationFunction(4); };

	double operator() (double);
	double operator[] (double);
	
	Matrix operator()(Matrix&);
	Matrix operator[](Matrix&);

	template <typename Type>
	friend Type& operator<< (Type& out, ActivationFunction& function) { out << function._number; return out; }

	template <typename Type>
	friend Type& operator>> (Type& in, ActivationFunction& function) { in >> function._number; return in; }

private:
	
	static std::vector<double(*)(double)> _functions;
	static std::vector<double(*)(double)> _derivatives;

	static double sigmoidFunction(double);
	static double sigmoidDerivative(double);

	static double tanhFunction(double);
	static double tanhDerivative(double);

	static double reLuFunction(double);
	static double reLuDerivative(double);

	static double softsignFunction(double);
	static double softsignDerivative(double);

	static double mySoftsignFunction(double);
	static double mySoftsignDerivative(double);
	
};

#endif\
