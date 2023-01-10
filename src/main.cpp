#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>
#include "Matrix.h"
#include "Network.h"
#include "ActivationFunction.h"

int main() {
	Network test(2, { 2, 2 }, { 1, 0 }, { 1 }, -1, 1, "Network.txt");
	Dataset dataset("Test.ds");

	test.train(dataset, 0.1, 1000);

	cout << setprecision(0) << fixed;
	for (size_t i = 0; i < dataset.getSize(); i++)
		cout << test.feedForward(dataset.getInput(i));

	
}
