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

//_byteswap_ulong()

int main() {
	int magicNumber;
	int numberOfImages;
	int rows;
	int cols;

	ifstream file("train_labels.bin", ios_base::binary);
	int test;
	file.read((char*)&magicNumber, sizeof(int));


	
}
