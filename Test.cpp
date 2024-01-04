#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include <bitset>
#include "Decimal.h"
#include <chrono>


void speed_test() {
	auto start = std::chrono::system_clock::now();
	for (size_t counter = 0; counter < 1000000; ++counter)
		auto duration = std::chrono::duration_cast<TimeT>
		(std::chrono::system_clock::now() - start) / 1000000;
	return duration.count();
}


int main()
{
	Decimal a(1, 1);
	Decimal b(3, 0);
	Decimal c = a / b;
	std::cout << c.to_string() << std::endl;
}
