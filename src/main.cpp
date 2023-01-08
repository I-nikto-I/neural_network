#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "Matrix.h"
#include "Network.h"
#include "ActivationFunction.h"

int main() {
	Network test(4, { 2, 4, 3, 2 }, { 1, 1, 1, 0 }, { ActivationFunction::reLu(), ActivationFunction::reLu(), ActivationFunction::sigmoid() }, -1, 1, "Network.txt");

	for (int i = 0; i < 25000; i++) {
		auto gradient1 = test.backpropagation(Matrix{ 1, 1 }, Matrix{ 1, 1 });
		auto gradient2 = test.backpropagation(Matrix{ 1, 0 }, Matrix{ 0, 0 });
		auto gradient3 = test.backpropagation(Matrix{ 0, 1 }, Matrix{ 1, 1 });
		auto gradient4 = test.backpropagation(Matrix{ 0, 0 }, Matrix{ 1, 0 });
		for (int i = 0; i < gradient1.size(); i++) 
			test._weights[i] = test._weights[i] - (gradient1[2 - i] + gradient2[2 - i] + gradient3[2 - i] + gradient4[2 - i]) / 20;
	}
	
	test.feedForward(Matrix{ 1, 1 }).print();
	test.feedForward(Matrix{ 1, 0 }).print();
	test.feedForward(Matrix{ 0, 1 }).print();
	test.feedForward(Matrix{ 0, 0 }).print();
	
}
