#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include "Matrix.h"
#include "ActivationFunction.h"
#include "Dataset.h"

using namespace std;

class Network {
private:

	size_t _size;                           // Number of layers
	vector<size_t> _layersSize;             // Number of neurons in layers               
	vector<bool> _biasesSize;	            // Number of biases in layers              [_size]
	vector<ActivationFunction> _functions;  // Activation functions for layers         [_size]

	vector<Matrix> _neuronLinear;       // Value of neurons before activation function [_size]     (_layersSize)                                
	vector<Matrix> _neuronActivation;   // Value of neurons after activation function  [_size]     (_layersSize)
	vector<Matrix> _biases;             // Value of biases                             [_size]     (_layersSize, _biasesSize)
	vector<Matrix> _weights;            // Value of weights                            [_size - 1] (_layersSize[+1], _layersSize)

public:

	Network(vector<size_t> layersSize, vector<bool> biasesSize, vector<ActivationFunction> functions, double min = 0, double max = 1);
	Network(string filename, bool binary = false);

	void saveToFile(string filename, bool binary = false);

	const Matrix& feedForward(const Matrix& input);
	pair<vector<Matrix>, vector<Matrix>> backpropagation(const Matrix& input, const Matrix& expectedOutput);
	void SGD(Dataset& dataset, double learningRate, size_t batchSize = 0, size_t repeats = 0, size_t epochs = 1);

};

#endif // !NETWORK_H
