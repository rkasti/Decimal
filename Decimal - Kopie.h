#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#define DECIMAL_VALUE_PRECISION 15
#define DECIMAL_EXP_PRECISION 3
#define DECIMAL_VALUE_MAX 999999999999999
#define DECIMAL_EXP_MAX 999

class Decimal
{
public:
	Decimal();
	Decimal(double value);
	Decimal(int64_t value, int16_t exp);

	bool operator==(const Decimal& other) const;
	bool operator!=(const Decimal& other) const;
	bool operator<(const Decimal& other) const;
	bool operator>(const Decimal& other) const;
	bool operator<=(const Decimal& other) const;
	bool operator>=(const Decimal& other) const;

	Decimal operator-() const;
	Decimal& operator=(const Decimal& other);

	Decimal operator+(const Decimal& other) const;
	Decimal operator-(const Decimal& other) const;
	Decimal operator*(const Decimal& other) const;
	Decimal operator/(const Decimal& other) const;
	Decimal operator%(const Decimal& other) const;
	Decimal operator^(const Decimal& other) const;

	Decimal& operator+=(const Decimal& other);
	Decimal& operator-=(const Decimal& other);
	Decimal& operator*=(const Decimal& other);
	Decimal& operator/=(const Decimal& other);
	Decimal& operator%=(const Decimal& other);
	Decimal& operator^=(const Decimal& other);

	uint8_t sign() const;

	Decimal& log();
	Decimal& log(const double& other);
	Decimal& log(const Decimal& other);
	Decimal& ln();

	Decimal& sin();
	Decimal& cos();
	Decimal& tan();
	Decimal& asin();
	Decimal& acos();
	Decimal& atan();
	Decimal& sinh();
	Decimal& cosh();
	Decimal& tanh();
	Decimal& asinh();
	Decimal& acosh();
	Decimal& atanh();

	Decimal& round();
	Decimal& floor();
	Decimal& ceil();
	Decimal& abs();
	Decimal& to_int();
	Decimal& negate();

	Decimal& pow(const Decimal& other);
	Decimal& pow(const double& other);
	Decimal& factorial();

	Decimal& sqrt();
	Decimal& root(const Decimal& other);
	Decimal& root(const double& other);


	int64_t get_value() const;
	int16_t get_exp() const;

private:
	static void minimize_value(int64_t& value, int16_t& exp);
	static constexpr int64_t powers_of_ten[DECIMAL_VALUE_PRECISION] = {
		1,
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000,
		100000000,
		1000000000,
		10000000000,
		100000000000,
		1000000000000,
		10000000000000,
		100000000000000,
	};
	int64_t _value;
	int16_t _exp;
};
