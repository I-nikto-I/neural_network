#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <bitset>
#include <random>
#include <intrin.h>
#include "Matrix.h"
#include "Network.h"
#include "ActivationFunction.h"

double test(Network net, Dataset dataset, int tasks=10000) {
	double success = 0;

	for (int i = 0; i < tasks; i++) {
		auto predicted = net.feedForward(dataset.getInput());
		auto expectied = dataset.getOuput();
		dataset.next();
		int maxPred = 0, maxExp = 0;

		for (int j = 0; j < 10; j++) {
			if (predicted[j] > predicted[maxPred])
				maxPred = j;
			if (expectied[j] > expectied[maxExp])
				maxExp = j;
		}
		success += (maxExp == maxPred);
		
	}
	return success / tasks;
}

int main() {
	Network net("MaxSuccess", true);
	Dataset dataset("TrainMnist.ds");
	Dataset TestDataset("TestMnist.ds");

	double maxSuccees = test(net, TestDataset);

	
	cout << "Epoch 0. Success: " << maxSuccees << "\n\n";

	for (int epoch = 1; epoch <= 20; epoch++) {
		double learningRate = 0.01 * exp(-epoch / 20.);
		cout << "Epoch " << epoch << ". " << learningRate << "\n";
		net.SGD(dataset, learningRate, 1000);

		double success = test(net, TestDataset);
		cout << "Success: " << success << "\n\n";
		if (success > maxSuccees) {
			net.saveToFile("MaxSuccess", true);
			maxSuccees = success;
		}

	}
}
