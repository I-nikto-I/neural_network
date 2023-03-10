#include "Dataset.h"
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <random>

using namespace std;

Dataset::Dataset(const vector<Matrix>& inputs, const vector<Matrix>& outputs){
	if (inputs.size() == 0 || outputs.size() == 0)
		throw length_error("Dataset constructor inputs/ouputs size = 0");
	if (inputs.size() != outputs.size())
		throw length_error("Dataset constructor inputs lenght != outputs lenght");

	_size = inputs.size();
	_inputSize = inputs[0].size();
	_outputSize = outputs[0].size();
	_indexes.resize(_size);

	for (size_t i = 0; i < _size; i++) {
		_indexes[i] = i;
		if (inputs[i].size()!=_inputSize || outputs[i].size() != _outputSize)
			throw length_error("Dataset constructor inputs/outputs sizes is different");
	}

	_inputs = inputs;
	_outputs = outputs;
}

Dataset::Dataset(string filename, bool binary){
	ifstream file;

	if (binary) {
		file.open(filename, ios_base::binary);
		if (!file.is_open())
			throw runtime_error("Dataset constructor: failed to open file");

		file.read((char*)&_size, sizeof(_size));
		file.read((char*)&_inputSize, sizeof(_inputSize));
		file.read((char*)&_outputSize, sizeof(_outputSize));

		_indexes.resize(_size);
		for (size_t i = 0; i < _size; i++) {
			_inputs.push_back(Matrix(file, _inputSize));
			_outputs.push_back(Matrix(file, _outputSize));
			_indexes[i] = i;
		}
	}
	else {
		file.open(filename);
		if (!file.is_open())
			throw runtime_error("Dataset constructor: failed to open file");

		file >> _size;
		file >> _inputSize >> _outputSize;

		_indexes.resize(_size);
		for (size_t i = 0; i < _size; i++) {
			_inputs.push_back(Matrix(_inputSize, 1, file));
			_outputs.push_back(Matrix(_outputSize, 1, file));
			_indexes[i] = i;
		}
	}
	
	
	file.close();
}

void Dataset::saveToFile(string filename, bool binary){
	ofstream file;

	if (binary) {
		file.open(filename, ios_base::binary);
		if (!file.is_open())
			throw runtime_error("Dataset saveToFile: failed to open file");

		file.write((char*)&_size, sizeof(_size));
		file.write((char*)&_inputSize, sizeof(_inputSize));
		file.write((char*)&_outputSize, sizeof(_outputSize));

		for (size_t i = 0; i < _size; i++) {
			_inputs[i].writeToBinFile(file);
			_outputs[i].writeToBinFile(file);
		}
	}
	else {
		file.open(filename);
		if (!file.is_open())
			throw runtime_error("Dataset saveToFile: failed to open file");

		file << _size << "\n";
		file << _inputSize << " " << _outputSize << "\n\n\n";

		for (size_t i = 0; i < _size; i++) {
			file << _inputs[i];
			file << _outputs[i] << "\n";
		}
	}

	file.close();
}

void Dataset::add(const Matrix& input, const Matrix& output) {
	if (input.size() != _inputSize || output.size() != _outputSize)
		throw length_error("Dataset add() invalid input/output size");
	_inputs.push_back(input);
	_outputs.push_back(output);
	_size++;
	_indexes.push_back(_indexes.size());
}

void Dataset::shuffle(){
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(_indexes.begin(), _indexes.end(), g);
}

void Dataset::next(){
	_index++;
	if (_index >= _size)
		_index = 0;
}

const Matrix& Dataset::getInput(int index){
	if (index >= _size)
		throw length_error("Dataset getInput index out of range");

	if (index < 0)
		return _inputs[_indexes[_index]];
	return _inputs[_indexes[index]];
}

const Matrix& Dataset::getOuput(int index){
	if (index >= _size)
		throw length_error("Dataset getOutput index out of range");

	if (index < 0)
		return _outputs[_indexes[_index]];
	return _outputs[_indexes[index]];
}

void Dataset::setIndex(size_t index)
{
	if (index < 0 || index >= _size)
		throw length_error("Dataset setIndex index out of range");
}
