#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H

#include "Matrix.h"
#include <vector>

// Вот совершенно не знаю, как это нормально реализовать...
// Оно должно уметь:
// - Хранить нужную функцию и производную к ней 
// - Сохранять себя в файл и доставать назад (поэтому нельзя сделать через наследование)
// 
// По сути softmax сюда не относится, но я не знаю куда его в таком случае...

class ActivationFunction {
private:
	int _number;

public:
	ActivationFunction(size_t = 0);

	static ActivationFunction linear()     { return ActivationFunction(0); };
	static ActivationFunction sigmoid()    { return ActivationFunction(1); };
	static ActivationFunction tanh()       { return ActivationFunction(2); };
	static ActivationFunction reLu()       { return ActivationFunction(3); };
	static ActivationFunction softsign()   { return ActivationFunction(4); };
	static ActivationFunction mySoftsign() { return ActivationFunction(5); };

	static ActivationFunction softmax()    { return ActivationFunction(-1); };

	double operator() (double) const;
	double operator[] (double) const;
	
	Matrix operator()(const Matrix&) const;
	Matrix operator[](const Matrix&) const;

	double error(const Matrix& activation, const Matrix& target) const;
	Matrix errorDerivative(const Matrix& activation, const Matrix& target) const;

	template <typename Type>
	friend Type& operator<< (Type& out, ActivationFunction& function) { out << function._number; return out; }

	template <typename Type>
	friend Type& operator>> (Type& in, ActivationFunction& function) { in >> function._number; return in; }

private:
	
	static std::vector<double(*)(double)> _functions;
	static std::vector<double(*)(double)> _derivatives;

	static double linearFunction(double  x) { return x; }
	static double linearDerivative(double) { return 1; }

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

	static Matrix softmaxFunction(const Matrix&);


	//Cost functions


	static double crossEntropyErrorFunction(const Matrix& activation, const Matrix& target);
	static Matrix CESoftmaxErrorDerivative(const Matrix& activation, const Matrix& target);

	static double meanErrorFunction(const Matrix& activation, const Matrix& target);
	static Matrix meanErrorDerivative(const Matrix& activation, const Matrix& target);
	
};

#endif\
