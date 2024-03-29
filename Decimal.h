#pragma once
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#define DECIMAL_VALUE_PRECISION 18
#define DECIMAL_EXP_PRECISION 4
// int64_t needs to be able to represent at least double the value of DECIMAL_VALUE_MAX
#define DECIMAL_VALUE_MAX 999999999999999999
#define DECIMAL_EXP_MAX 9999

/// <summary>
/// stores either a decimal of the form: value = _val * 10 ^ _exp<para/>
/// </summary>
class Decimal
{
	public:
	Decimal();
	Decimal(int64_t value);
	Decimal(int64_t value, int16_t exp);
	Decimal(const Decimal& other);

	void set(int64_t value, int16_t exp);
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

	Decimal& ln();
	Decimal& log(Decimal other);
	Decimal& exp();
	Decimal& sqrt();
	Decimal& root(Decimal other);
	Decimal& factorial();

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
	Decimal& to_int();
	Decimal& abs();
	Decimal& negate();
	Decimal& ran();
	Decimal& ran_int(Decimal start, Decimal end);

	std::string to_string() const;

	/// <summary>
	/// approximation of pi
	/// </summary>
	static const Decimal PI;
	/// <summary>
	/// approximation of e
	/// </summary>
	static const Decimal EULER;
	/// <summary>
	/// approximation of ln(10)
	/// </summary>
	static const Decimal LN10;
	/// <summary>
	/// contains all values for 10^n with n from 0 to DECIMAL_VALUE_PRECISION-1
	/// </summary>
	static const int64_t powers_of_ten[19];

	private:
	/// <summary>
	/// count the number of digits after the first non-zero digit (base 10)
	/// </summary>
	static uint8_t count_digits(int64_t value);
	/// <summary>
	/// count the number of digits after the first non-zero digit (base 10)
	/// </summary>
	static uint8_t count_digits_unsigned(uint64_t value);
	/// <summary>
	/// shifts the value to the right by shift and rounds the last digit
	/// </summary>
	/// <param name="shift">range: 1 to DECIMAL_VALUE_PRECISION (inclusive)</param>
	static void shift_right(int64_t& value, uint8_t shift);
	/// <summary>
	/// shifts the value to the right by shift
	/// </summary>
	/// <param name="shift">range: 1 to DECIMAL_VALUE_PRECISION (inclusive)</param>
	static void shift_right(uint64_t& value, uint8_t shift);
	/// <summary>
	/// shifts the value to the right by one and rounds the last digit
	/// </summary>
	/// <param name="value"></param>
	static void shift_right_one(int64_t& value);
	/// <summary>
	/// maximize the exponent as much as possible without loosing precision
	/// </summary>
	void maximize_exp() const;
	/// <summary>
	/// add other to this while adding the next digit to error if necessary and returning whether the value or error changed
	/// </summary>
	bool add_changed(Decimal other);

	/// <summary>
	/// value of the decimal: value = _val * 10^_exp
	/// </summary>
	mutable int64_t _val;
	/// <summary>
	/// exponent of the decimal: value = _val * 10^_exp
	/// </summary>
	mutable int16_t _exp;
};