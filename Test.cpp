#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include <bitset>
#include "Decimal.h"
#include <chrono>


int main()
{
	Decimal a(74536, 45);
	Decimal b(768, -3);
	Decimal c = a / b;
	std::cout << c.to_string() << std::endl;
	auto start = std::chrono::system_clock::now();
	for (size_t counter = 0; counter < 1000000; ++counter) c = a / b;
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start) / 1000000;
	std::cout << duration.count() << "ns" << std::endl;
}
