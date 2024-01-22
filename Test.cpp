#include "Decimal.h"
#include <chrono>
#include <iostream>
#include <string>


int main()
{
	// for (int64_t i = 100; i < 1000; i++) {
	//	Decimal c = Decimal(i, 0);
	//	std::cout << "exp of " << i << " is:" << std::endl;
	//	c.exp();
	//	std::cout << c.to_string() << std::endl;
	// }
	Decimal a = Decimal(1333418256, 0);
	Decimal b = Decimal(3, 0);
	a.root(5);
	std::cout << a.to_string() << std::endl;
}
