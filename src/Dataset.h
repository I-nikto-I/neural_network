#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <string>
#include "Matrix.h"

using namespace std;

class Dataset {
private:
	int _size = 0;
	int _index = 0;
	vector<size_t> _indexes;

	vector<Matrix> _inputs;
	vector<Matrix> _outputs;
	size_t _inputSize;
	size_t _outputSize;
public:
	Dataset(const vector<Matrix>& inputs, const vector<Matrix>& outputs);
	Dataset(size_t inputSize, size_t outputSize) : _inputSize(inputSize), _outputSize(outputSize) {};
	Dataset(string filename, bool binary = true);

	void safeToFile(string filename, bool binary = true);

	void add(Matrix& input, Matrix& output);

	void shuffle();
	void next();
	Matrix& getInput(int index = -1);
	Matrix& getOuput(int index = -1);

	size_t getSize() { return _size; }
	size_t getIndex() { return _index; }
	size_t getInputSize() { return _inputSize; }
	size_t getOutputSize() { return _outputSize; }

	void setIndex(size_t index);
};

#endif // !DATASET_H

