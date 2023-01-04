#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include "Matrix.h"

using namespace std;

class Network {
private:
	string _filename;
	bool _saveToFile;

	size_t _size;             // Количество слоёв

	vector<size_t> _layers;   // Количество нейронов в слоях
	vector<size_t> _biases;   // Количество нейронов смещения в слоях

	vector<Matrix> _neurons;    // Значения нейронов [Не уверен, что это нужно вообще]
	vector<Matrix> _weights;    // Значения весов

public:
	Network(size_t size, vector<size_t> layers, vector<size_t> biases, double filler = 0, string filename = "");
	Network(size_t size, vector<size_t> layers, vector<size_t> biases, double min, double max, string filename="");
	Network(string filename, bool save = true);
	~Network();

	vector<double> feedForward(const vector<double>&);
};

#endif
