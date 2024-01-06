#include "Decimal.h"
#include <algorithm>
#include <bitset>
#include <chrono>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>


int main()
{
	Decimal a(23, 0);
	Decimal b(45, 0);
	Decimal c;
	// c = a * b;
	// std::cout << c.to_string() << std::endl;
	auto start = std::chrono::system_clock::now();
	for (size_t counter = 0; counter < 1000000; ++counter) {
		c = a * b;
	}
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start);
	std::cout << duration.count() / 1000000 * 4.3 << " cycles on average" << std::endl;
	std::cout << "result: " << c.to_string() << std::endl;
}
