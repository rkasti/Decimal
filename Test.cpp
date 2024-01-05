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
	Decimal a(99999999999999999, 1);
	Decimal b(1, 0);
	Decimal c;
	c = a - b;
	// auto start = std::chrono::system_clock::now();
	// for (size_t counter = 0; counter < 1000000; ++counter) {
	//	c = a + b;
	// }
	// auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start);
	// std::cout << duration.count() / 1000000 * 4.3 << " cycles on average" << std::endl;
	// std::cout << "result: " << c.to_string() << std::endl;
}
