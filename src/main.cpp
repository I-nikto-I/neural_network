#include <iostream>
#include "Matrix.h"

int main(){
	Matrix first = Matrix::Random(4, 4, 0, 10);
	Matrix second = first;
	first(1, 3) = 999999;
	

	//�����
	first.print(5);
	second.print(0);
}