#include "Network.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

void Network::init(double filler){
	for (size_t i = 0; i < _size - 1; i++) {
		_weights.push_back(Matrix(_layers[i + 1], _layers[i] + _biases[i], filler));
		_neuronsActivation.push_back(Matrix(_layers[i + 1] + _biases[i + 1]));
	}

	for (size_t i = 0; i < _size; i++) {
		_neuronsLinear.push_back(Matrix(_layers[i] + _biases[i]));

		for (size_t j = _layers[i]; j < _biases[i] + _layers[i]; j++) {
			_neuronsLinear.back()(j) = 1;
		}
	}
}

Network::Network(size_t size, vector<size_t> layers, vector<size_t> biases, vector<ActivationFunction> functions, double filler, string filename):
	_size(size), _layers(layers), _biases(biases), _functions(functions), _filename(filename), _saveToFile(filename.length()>0)
{
	if (size < 2)
		throw invalid_argument("Network constructor size < 2");
	if (size != layers.size() || size != biases.size() || size-1 != functions.size())
		throw invalid_argument("Network constructor invalid vector size");

	init(filler);
}

Network::Network(size_t size, vector<size_t> layers, vector<size_t> biases, vector<ActivationFunction> functions, double min, double max, string filename):
	_size(size), _layers(layers), _biases(biases), _functions(functions), _filename(filename), _saveToFile(filename.length() > 0)
{
	if (size < 2)
		throw invalid_argument("Network constructor size < 2");
	if (size != layers.size() || size != biases.size() || size-1 != functions.size())
		throw invalid_argument("Network constructor invalid vector size");

	init();
	for (auto& weight : _weights)
		weight.random(min, max);

}

Network::Network(string filename, bool save): _filename(filename), _saveToFile(save)
{
	ifstream file;
	file.open(filename);

	file >> _size;

	_layers.resize(_size);
	for (size_t i = 0; i < _size; i++)
		file >> _layers[i];

	_biases.resize(_size);
	for (size_t i = 0; i < _size; i++)
		file >> _biases[i];

	_functions.resize(_size-1);
	for (size_t i = 0; i < _size-1; i++)
		file >> _functions[i];

	for (size_t i = 0; i < _size - 1; i++) {
		_weights.push_back(Matrix(_layers[i + 1], _layers[i] + _biases[i], file));
		_neuronsActivation.push_back(Matrix(_layers[i + 1] + _biases[i + 1]));
	}

	for (size_t i = 0; i < _size; i++) {
		_neuronsLinear.push_back(Matrix(_layers[i] + _biases[i]));

		for (size_t j = _layers[i]; j < _biases[i] + _layers[i]; j++) {
			_neuronsLinear.back()(j) = 1;
		}
	}

	file.close();
}

Network::~Network()
{
	if (_filename.length() > 0 && _saveToFile) {
		ofstream file;
		file.open(_filename);

		file << _size << "\n\n";
		for (auto& layer : _layers)
			file << layer << " ";
		file << "\n\n";
		for (auto& bias : _biases)
			file << bias << " ";
		file << "\n\n";
		for (auto& function : _functions)
			file << function << " ";
		file << "\n\n";
		for (auto& weights : _weights) 
			file << weights << "\n";

		file.close();
	}
}

Matrix Network::feedForward(Matrix& inputLayer){
	if (inputLayer.size() != _layers[0])
		throw invalid_argument("Network feedForward inputLayer size != first layer size");
	_neuronsLinear[0].insert(inputLayer);
	for (size_t i = 0; i < _size-1; i++) {
		_neuronsLinear[i + 1].insert(_weights[i] * _neuronsLinear[i]);
		_neuronsActivation[i].insert(_functions[i](_neuronsLinear[i + 1]));
	}
	return _neuronsActivation[_size-2];
}

vector<Matrix> Network::backpropagation(Matrix& input, Matrix& targetOutput){
	if (input.size() != _layers[0] || targetOutput.size() != _layers[_size - 1])
		throw invalid_argument("Network backpropagation invalid input/output lenght");
	vector<Matrix> gradients;
	Matrix activations = feedForward(input);
	Matrix errorDerivatives = _functions[_size - 2].errorDerivative(activations, targetOutput);
	for (int layer = _size - 2; layer >= 0; layer--) {   //size_t тут переполняется
		errorDerivatives = errorDerivatives.hadamarProduct(_functions[layer][_neuronsLinear[layer + 1]]).cut(_layers[layer + 1]);
		Matrix gradient =   errorDerivatives * _neuronsLinear[layer].transponse();
		errorDerivatives = (_weights[layer].transponse() * errorDerivatives);
		gradients.insert(gradients.begin(), gradient);
	}

	return gradients;
}

void Network::train(Dataset& dataset, double learningRate, size_t epochs, int batchSize, int repeats){
	if (dataset.getSize() == 0)
		throw invalid_argument("Network train() dataset size = 0");
	if (dataset.getInputSize() != _layers[0] || dataset.getOutputSize() != _layers[_size - 1])
		throw invalid_argument("Network train invalid input/output lenght");

	if (batchSize <= 0 || batchSize > dataset.getSize())
		batchSize = dataset.getSize();
	if (repeats <= 0 || repeats > dataset.getSize() / batchSize)
		repeats = dataset.getSize() / batchSize;

	vector<Matrix> emptyGradient;
	for (size_t i = 0; i < _size - 1; i++)
		emptyGradient.push_back(Matrix(_weights[i].getHeight(), _weights[i].getWidth()));

	for (size_t epoch = 0; epoch < epochs; epoch++) {
		dataset.shuffle();
		for (size_t repeat = 0; repeat < repeats; repeat++) {
			vector<Matrix> gradient = emptyGradient;

			for (size_t batch = 0; batch < batchSize; batch++) {
				auto batchGradient = backpropagation(dataset.getInput(), dataset.getOuput());
				for (size_t i = 0; i < batchGradient.size(); i++)
					gradient[i] += batchGradient[i] / batchSize;
				dataset.next();
			}

			for (size_t i = 0; i < _size - 1; i++)
				_weights[i] -= gradient[i] * learningRate;
		}
	}
}