#include "Decimal.h"
#include <chrono>
#include <iostream>
#include <string>


int main()
{
	Decimal a(-456, 3);
	Decimal b(6, 4);
	Decimal c;
	c = a % b;
	std::cout << c.to_string() << std::endl;
}
