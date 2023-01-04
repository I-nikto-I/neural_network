#include <iostream>
#include <fstream>
#include "Matrix.h"
#include "Network.h"

int main() {
	Network test("Network.txt");

	for (double n : test.feedForward({ 1, 2 }))
		cout << n << "\n";

}