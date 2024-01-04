#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#define DECIMAL_VALUE_PRECISION 18
#define DECIMAL_EXP_PRECISION 4
// int64_t needs to be able to represent at least double the value of DECIMAL_VALUE_MAX
#define DECIMAL_VALUE_MAX 999999999999999999
#define DECIMAL_EXP_MAX 9999

using KEY = uint16_t;

/// <summary>
/// stores either a KEY or a decimal: value = _val * 10 ^ _exp<para/>
/// IMPORTANT: don't use any operations if the decimal represents a KEY
/// </summary>
class Decimal
{
public:
	Decimal();
	Decimal(int64_t value);
	Decimal(int64_t value, int16_t exp);
	Decimal(const Decimal& other);

	void set_key(KEY value);
	void set_value(int64_t value, int16_t exp);

	KEY get_key() const;
	int64_t get_value() const;
	int16_t get_exp() const;

	bool operator==(Decimal other) const;
	bool operator!=(Decimal other) const;
	bool operator<(Decimal other) const;
	bool operator>(Decimal other) const;
	bool operator<=(Decimal other) const;
	bool operator>=(Decimal other) const;

	Decimal operator-() const;
	Decimal& operator=(Decimal other);
	Decimal& operator=(int64_t other);

	Decimal operator+(Decimal other) const;
	Decimal operator-(Decimal other) const;
	Decimal operator*(Decimal other) const;
	Decimal operator/(Decimal other) const;
	Decimal operator%(Decimal other) const;
	Decimal operator^(Decimal other) const;

	Decimal& operator+=(Decimal other);
	Decimal& operator-=(Decimal other);
	Decimal& operator*=(Decimal other);
	Decimal& operator/=(Decimal other);
	Decimal& operator%=(Decimal other);
	Decimal& operator^=(Decimal other);

	Decimal& log();
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
	Decimal& factorial();

	Decimal& pow(const Decimal& other);
	Decimal& root(const Decimal& other);

	std::string to_string() const;

private:
	/// <summary>
	/// contains all values for 10^n with n from 0 to DECIMAL_VALUE_PRECISION-1
	/// </summary>
	static const int64_t powers_of_ten[DECIMAL_VALUE_PRECISION];
	/// <summary>
	/// count the number of digits after the first non-zero digit (base 10)
	/// </summary>
	static uint8_t count_digits(int64_t value);
	/// <summary>
	/// count the number of digits after the first non-zero digit (base 10)
	/// </summary>
	static uint8_t count_digits(uint64_t value);
	/// <summary>
	/// shifts the value to the right by shift
	/// </summary>
	/// <param name="shift">range: 1 to DECIMAL_VALUE_PRECISION (inclusive)</param>
	static void shift_right(int64_t& value, uint8_t shift);
	/// <summary>
	/// shifts the value to the right by shift
	/// </summary>
	/// <param name="shift">range: 1 to DECIMAL_VALUE_PRECISION (inclusive)</param>
	static void shift_right(uint64_t& value, uint8_t shift);
	/// <summary>
	/// maximize the exponent as much as possible without loosing precision
	/// </summary>
	void maximize_exp() const;

	/// <summary>
	/// value of the decimal: value = _val * 10^_exp
	/// </summary>
	mutable int64_t _val;
	/// <summary>
	/// exponent of the decimal: value = _val * 10^_exp
	/// </summary>
	mutable int16_t _exp;
};