#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include "Matrix.h"
#include "ActivationFunction.h"

using namespace std;

class Network {
public: //Временно!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	string _filename;
	bool _saveToFile;

	size_t _size;                          // Количество слоёв

	vector<size_t> _layers;                // Количество нейронов в слоях
	vector<size_t> _biases;                // Количество нейронов смещения в слоях
	vector<ActivationFunction> _functions; // Функции активации для слоёв

	vector<Matrix> _neuronsLinear;         // Значения нейронов до функции активации
	vector<Matrix> _neuronsActivation;     // Значения нейронов после функции активации
	vector<Matrix> _weights;               // Значения весов

public:
	Network(size_t size, vector<size_t> layers, vector<size_t> biases, vector<ActivationFunction> functions, double filler = 0, string filename = "");
	Network(size_t size, vector<size_t> layers, vector<size_t> biases, vector<ActivationFunction> functions, double min, double max, string filename="");
	Network(string filename, bool save = true);
	~Network();

	Matrix feedForward(Matrix&);
	vector<Matrix> backpropagation(Matrix& input, Matrix& targetOutput);

};

#endif
