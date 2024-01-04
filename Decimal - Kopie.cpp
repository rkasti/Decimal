#include "Decimal.h"

Decimal::Decimal()
{
	_value = 0;
	_exp = 0;
}

Decimal::Decimal(double value)
{
	_exp = (value == 0) ? 0 : 1 + (int16_t)std::floor(std::log10(std::fabs(value)));
	_value = value * std::pow(10, -_exp);
}

Decimal::Decimal(int64_t value, int16_t exp)
{
	if (std::abs(value) > DECIMAL_VALUE_MAX) std::cout << "Error: Decimal value overflow";
	if (std::abs(exp) > DECIMAL_EXP_MAX) std::cout << "Error: Decimal exponent overflow";
	_exp = (value == 0) ? 0 : exp;
	_value = value;
}

bool Decimal::operator==(const Decimal& other) const
{
	return _value == other._value && _exp == other._exp;
}

bool Decimal::operator!=(const Decimal& other) const
{
	return _value != other._value || _exp != other._exp;
}

bool Decimal::operator<(const Decimal& other) const
{
	if (sign() > other.sign()) return false;
	if (sign() < other.sign()) return true;
	if (sign() == 0) return false;
	if (sign() == 1) {
		if (_exp > other._exp + DECIMAL_VALUE_PRECISION) return false;
		if (_exp + DECIMAL_VALUE_PRECISION < other._exp) return true;
		if (_value * std::pow(10, _exp) < other._value * std::pow(10, other._exp)) return true;
		else return false;
	}
	else {
		if (_exp > other._exp + DECIMAL_VALUE_PRECISION) return true;
		if (_exp + DECIMAL_VALUE_PRECISION < other._exp) return false;
		if (_value * std::pow(10, _exp) < other._value * std::pow(10, other._exp)) return false;
		else return true;
	}
}

bool Decimal::operator>(const Decimal& other) const
{
	return *this != other && !(*this < other);
}

bool Decimal::operator<=(const Decimal& other) const
{
	return *this == other || *this < other;
}

bool Decimal::operator>=(const Decimal& other) const
{
	return !(*this > other);
}

Decimal Decimal::operator-() const
{
	return Decimal(-_value, _exp);
}

Decimal& Decimal::operator=(const Decimal& other)
{
	_value = other._value;
	_exp = other._exp;
	return *this;
}

Decimal& Decimal::operator+=(const Decimal& other)
{
	// if one value is zero, return the other one
	if (_value == 0) { *this = other; return *this; }
	if (other._value == 0) return *this;

	int8_t exp_diff = _exp - other._exp;
	int64_t add_value = other._value;

	// if the exponents are not equal: make them equal by shifting the comma and changing the exponent
	if (exp_diff != 0) {
		// place the value with the smaller exponent in add_value and the value with the bigger exponent in _value
		if (exp_diff < 0) {
			exp_diff = -exp_diff;
			add_value = _value;
			*this = other;
		}

		// decrease the exponent of the value with the bigger exponent until the exponents are equal or no more increasing with loss of digits is possible
		uint8_t value_i = 1;
		int64_t abs_value = std::abs(_value);
		while (value_i < DECIMAL_VALUE_PRECISION && powers_of_ten[value_i] < abs_value) value_i++;
		value_i = DECIMAL_VALUE_PRECISION - value_i;
		if (value_i + 1 > exp_diff) {
			_value *= powers_of_ten[exp_diff];
			_exp -= exp_diff;
		}
		else {
			uint8_t add_value_i = 1;
			int64_t abs_add_value = std::abs(add_value);
			while (add_value_i < DECIMAL_VALUE_PRECISION && powers_of_ten[add_value_i] < abs_add_value) add_value_i++;
			exp_diff -= value_i;

			if (add_value_i <= exp_diff) return *this;

			_value *= powers_of_ten[value_i];
			add_value /= powers_of_ten[exp_diff];
			_exp -= value_i;
		}
	}

	// assuming the exponents are equal: add the values together
	_value += add_value;
	// if overflow: shift the comma and increase the exponent
	if (DECIMAL_VALUE_MAX - std::abs(_value) < 0) {
		_exp++;
		if (_value % 10 > 4) _value += 10;
		_value /= 10;
	}
	return *this;
}

Decimal& Decimal::operator-=(const Decimal& other)
{
	*this += -other;
	return *this;
}

Decimal& Decimal::operator*=(const Decimal& other)
{
	// if one value is zero, return zero
	if (_value == 0) return *this;
	if (other._value == 0) { *this = other; return *this; }

	int64_t add_value = other._value;
	_exp += other._exp;
	minimize_value(_value, _exp);
	minimize_value(add_value, _exp);
	_value *= add_value;


	// if overflow: shift the comma and increase the exponent
	if (DECIMAL_VALUE_MAX - std::abs(_value) < 0) {
		_exp++;
		if (_value % 10 > 4) _value += 10;
		_value /= 10;
	}
	return *this;
}

Decimal Decimal::operator+(const Decimal& other) const
{
	Decimal result = *this;
	result += other;
	return result;
}

Decimal Decimal::operator-(const Decimal& other) const
{
	Decimal result = *this;
	result += -other;
	return result;
}

uint8_t Decimal::sign() const
{
	if (_value > 0) return 1;
	else if (_value < 0) return -1;
	else return 0;
}

void Decimal::minimize_value(int64_t& value, int16_t& exp)
{
	if (value == 0) {
		exp = 0;
		return;
	}
	while (value > 0 && value % 10 == 0) {
		value *= 10;
		exp--;
	}
}

int64_t Decimal::get_value() const
{
	return _value;
}

int16_t Decimal::get_exp() const
{
	return _exp;
}