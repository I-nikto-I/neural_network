#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "Matrix.h"
#include "Network.h"
#include "ActivationFunction.h"

int main() {
	Network test("Network.txt");
	for (auto res : test.feedForward({ 1, 1 }))
		cout << res << " ";
	
}
