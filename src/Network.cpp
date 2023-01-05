#include "Network.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

Network::Network(size_t size, vector<size_t> layers, vector<size_t> biases, vector<ActivationFunction> functions, double filler, string filename):
	_size(size), _layers(layers), _biases(biases), _functions(functions), _filename(filename), _saveToFile(filename.length()>0)
{
	if (size != layers.size() || size != biases.size() || size-1 != functions.size())
		throw invalid_argument("Network constructor invalid vector size");

	for (size_t i = 0; i < size - 1; i++)
		_weights.push_back(Matrix(_layers[i + 1], _layers[i] + _biases[i], filler));

	for (size_t i = 0; i < size; i++) {
		_neurons.push_back(Matrix(layers[i] + biases[i]));

		for (size_t j = layers[i]; j < biases[i] + layers[i]; j++) {
			_neurons.back()(j) = 1;
		}
	}
	
}

Network::Network(size_t size, vector<size_t> layers, vector<size_t> biases, vector<ActivationFunction> functions, double min, double max, string filename):
	_size(size), _layers(layers), _biases(biases), _functions(functions), _filename(filename), _saveToFile(filename.length() > 0)
{
	if (size != layers.size() || size != biases.size() || size-1 != functions.size())
		throw invalid_argument("Network constructor invalid vector size");

	for (size_t i = 0; i < size - 1; i++)
		_weights.push_back(Matrix(_layers[i + 1], _layers[i] + _biases[i], min, max));

	for (size_t i = 0; i < size; i++) {
		_neurons.push_back(Matrix(layers[i] + biases[i]));

		for (size_t j = layers[i]; j < biases[i] + layers[i]; j++) {
			_neurons.back()(j) = 1;
		}
	}

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

	for (size_t i = 0; i < _size - 1; i++)
		_weights.push_back(Matrix(_layers[i + 1], _layers[i] + _biases[i], file));

	for (size_t i = 0; i < _size; i++) {
		_neurons.push_back(Matrix(_layers[i] + _biases[i]));

		for (size_t j = _layers[i]; j < _biases[i] + _layers[i]; j++) {
			_neurons.back()(j) = 1;
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

vector<double> Network::feedForward(const vector<double>& inputLayer){
	if (inputLayer.size() != _layers[0])
		throw invalid_argument("Network feedForward inputLayer size != first layer size");
	_neurons[0] = inputLayer;
	for (size_t i = 0; i < _size-1; i++) {
		_neurons[i + 1] = _functions[i](_weights[i] * _neurons[i]);
	}
	return _neurons[_size-1].toVector();
}


