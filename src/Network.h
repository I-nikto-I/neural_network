#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include "Matrix.h"
#include "ActivationFunction.h"

using namespace std;

class Network {
public: //��������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	string _filename;
	bool _saveToFile;

	size_t _size;                          // ���������� ����

	vector<size_t> _layers;                // ���������� �������� � �����
	vector<size_t> _biases;                // ���������� �������� �������� � �����
	vector<ActivationFunction> _functions; // ������� ��������� ��� ����

	vector<Matrix> _neuronsLinear;         // �������� �������� �� ������� ���������
	vector<Matrix> _neuronsActivation;     // �������� �������� ����� ������� ���������
	vector<Matrix> _weights;               // �������� �����

public:
	Network(size_t size, vector<size_t> layers, vector<size_t> biases, vector<ActivationFunction> functions, double filler = 0, string filename = "");
	Network(size_t size, vector<size_t> layers, vector<size_t> biases, vector<ActivationFunction> functions, double min, double max, string filename="");
	Network(string filename, bool save = true);
	~Network();

	Matrix feedForward(Matrix&);
	vector<Matrix> backpropagation(Matrix& input, Matrix& targetOutput);

};

#endif
