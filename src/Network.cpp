#include "Network.h"
#include <fstream>
#include <stdexcept>

Network::Network(vector<size_t> layersSize, vector<bool> hasBiases, vector<ActivationFunction> functions, double min, double max) :
	_layersSize(layersSize), _biasesSize(hasBiases), _functions(functions) {
	if (layersSize.size() != hasBiases.size() || layersSize.size() != functions.size())
		throw invalid_argument("Netword constructor: sizes error");

	_size = _layersSize.size();

	for (size_t i = 0; i < _size; i++) {
		_neuronLinear.push_back(Matrix(_layersSize[i]));
		_biases.push_back(Matrix(_layersSize[i], _biasesSize[i], min, max));
	}
	_neuronActivation = _neuronLinear;

	for (size_t i = 0; i < _size - 1; i++)
		_weights.push_back(Matrix(_layersSize[i + 1], _layersSize[i], min, max));
}

// сделать что-то с файловыми операциями

Network::Network(string filename, bool binary){
	ifstream file;
	if (binary) {
		file.open(filename, ios_base::binary);
		if (!file.is_open())
			throw runtime_error("Network constructor: failed to open file");

		file.read((char*)&_size, sizeof(_size));

		_layersSize.resize(_size);
		for (size_t i = 0; i < _size; i++)
			file.read((char*)&_layersSize[i], sizeof(_layersSize[i]));

		_biasesSize.resize(_size);
		for (size_t i = 0; i < _size; i++) {
			short biases;
			file.read((char*)&biases, sizeof(biases));
			_biasesSize[i] = biases;
		}

		_functions.resize(_size);
		for (size_t i = 0; i < _size; i++)
			file.read((char*)&_functions[i], sizeof(_functions[i]));

		for (size_t i = 0; i < _size; i++)
			_biases.push_back(Matrix(file, _layersSize[i], _biasesSize[i]));

		for (size_t i = 0; i < _size - 1; i++)
			_weights.push_back(Matrix(file, _layersSize[i + 1], _layersSize[i]));
	}
	else {
		file.open(filename);
		if (!file.is_open())
			throw runtime_error("Network constructor: failed to open file");

		file >> _size;
		
		_layersSize.resize(_size);
		for (size_t i = 0; i < _size; i++)
			file >> _layersSize[i];

		_biasesSize.resize(_size);
		for (size_t i = 0; i < _size; i++) {
			short biases;
			file >> biases;
			_biasesSize[i] = biases;
		}

		_functions.resize(_size);
		for (size_t i = 0; i < _size; i++)
			file >> _functions[i];

		for (size_t i = 0; i < _size; i++)
			_biases.push_back(Matrix(_layersSize[i], _biasesSize[i], file));

		for (size_t i = 0; i < _size - 1; i++)
			_weights.push_back(Matrix(_layersSize[i + 1], _layersSize[i], file));

	}

	file.close();

	for (size_t i = 0; i < _size; i++) 
		_neuronLinear.push_back(Matrix(_layersSize[i]));

	_neuronActivation = _neuronLinear;
}

void Network::saveToFile(string filename, bool binary){
	ofstream file;
	if (binary) {
		file.open(filename, ios_base::binary);
		if (!file.is_open())
			throw runtime_error("Network constructor: failed to open file");

		file.write((char*)&_size, sizeof(_size));

		for (size_t i = 0; i < _size; i++)
			file.write((char*)&_layersSize[i], sizeof(_layersSize[i]));

		for (size_t i = 0; i < _size; i++) {
			short biasesSize = _biasesSize[i];
			file.write((char*)&biasesSize, sizeof(biasesSize));
		}

		for (size_t i = 0; i < _size; i++)
			file.write((char*)&_functions[i], sizeof(_functions[i]));

		for (size_t i = 0; i < _size; i++)
			_biases[i].writeToBinFile(file);

		for (size_t i = 0; i < _size - 1; i++)
			_weights[i].writeToBinFile(file);

	}
	else {

		file.open(filename);
		if (!file.is_open())
			throw runtime_error("Network constructor: failed to open file");

		file << _size << "\n\n";

		for (size_t i = 0; i < _size; i++)
			file << _layersSize[i] << " ";
		file << "\n\n";

		for (size_t i = 0; i < _size; i++)
			file << static_cast<short>(_biasesSize[i]) << " ";
		file << "\n\n";

		for (size_t i = 0; i < _size; i++)
			file << _functions[i] << " ";
		file << "\n\n";

		for (size_t i = 0; i < _size; i++)
			file << _biases[i] << "\n";
		file << "\n\n";

		for (size_t i = 0; i < _size - 1; i++)
			file << _weights[i] << "\n";
	}
}

const Matrix& Network::feedForward(const Matrix& input){
	if (input.size() != _layersSize[0])
		throw invalid_argument("Network feedForward: input size != first layer size");

	_neuronLinear[0] = input + _biases[0];
	_neuronActivation[0] = _functions[0](_neuronLinear[0]);

	for (size_t i = 1; i < _size; i++) {
		_neuronLinear[i] = _weights[i - 1] *_neuronActivation[i - 1] + _biases[i];
		_neuronActivation[i] = _functions[i](_neuronLinear[i]);
	}

	return _neuronActivation[_size - 1];
}

// Избавится от пар

pair<vector<Matrix>, vector<Matrix>> Network::backpropagation(const Matrix& input, const Matrix& expectedOutput) {
	vector<Matrix> gradientWeights;
	vector<Matrix> gradientBiases;
	Matrix layerError = _functions[_size - 1].errorDerivative(feedForward(input), expectedOutput);

	for (size_t i = _size - 1; i > 0; i--) {
		layerError.hadamarProduct(_functions[i][_neuronLinear[i]]);
		gradientBiases.insert(gradientBiases.begin(), layerError.cut(_layersSize[i], _biasesSize[i]));                                             // biases gradient
		gradientWeights.insert(gradientWeights.begin(), layerError * _neuronActivation[i - 1].transpose());                                        // weigth gradient
		layerError = _weights[i - 1].transpose() * layerError;
	}
	gradientBiases.insert(gradientBiases.begin(), layerError.hadamarProduct(_functions[0][_neuronLinear[0]]).cut(_layersSize[0], _biasesSize[0])); // first layer biases

	return pair<vector<Matrix>, vector<Matrix>>(gradientWeights, gradientBiases);
}

void Network::SGD(Dataset& dataset, double learningRate, size_t batchSize, size_t repeats, size_t epochs){
	if (dataset.getSize() == 0)
		throw invalid_argument("Network train() dataset size = 0");
	if (dataset.getInputSize() != _layersSize[0] || dataset.getOutputSize() != _layersSize[_size - 1])
		throw invalid_argument("Network train invalid input/output lenght");

	if (batchSize == 0 || batchSize > dataset.getSize())
		batchSize = dataset.getSize();
	if (repeats == 0 || repeats > dataset.getSize() / batchSize)
		repeats = dataset.getSize() / batchSize;

	pair<vector<Matrix>, vector<Matrix>> emptyGradient;
	for (size_t i = 0; i < _size; i++) 
		emptyGradient.second.push_back(Matrix(_layersSize[i], _biasesSize[i], 0));
	for (size_t i = 0; i < _size - 1; i++)
		emptyGradient.first.push_back(Matrix(_layersSize[i + 1], _layersSize[i], 0));

	for (size_t epoch = 0; epoch < epochs; epoch++) {
		dataset.shuffle();
		for (size_t repeat = 0; repeat < repeats; repeat++) {
			auto gradient = emptyGradient;

			for (size_t batch = 0; batch < batchSize; batch++) {
				auto batchGradient = backpropagation(dataset.getInput(), dataset.getOuput());
				for (int i = 0; i < batchGradient.first.size(); i++)
					gradient.first[i] += batchGradient.first[i];

				for (int i = 0; i < batchGradient.second.size(); i++)
					gradient.second[i] += batchGradient.second[i];
				dataset.next();
			}

			for (int i = 0; i < gradient.first.size(); i++)
				_weights[i] -= gradient.first[i] * learningRate / batchSize;

			for (int i = 0; i < gradient.second.size(); i++)
				_biases[i] -= gradient.second[i] * learningRate / batchSize;

		}
	}
}


