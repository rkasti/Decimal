#include "Decimal.h"

const int64_t Decimal::powers_of_ten[] = {
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
	1000000000000000,
	10000000000000000,
	100000000000000000,
};


Decimal::Decimal()
{
	_val = 0;
	_exp = 0;
}

Decimal::Decimal(int64_t value)
{
	if (std::abs(value) > DECIMAL_VALUE_MAX) std::cout << "Error: Decimal value overflow";
	_exp = 0;
	_val = value;
}

Decimal::Decimal(int64_t value, int16_t exp)
{
	if (std::abs(value) > DECIMAL_VALUE_MAX) std::cout << "Error: Decimal value overflow";
	if (std::abs(exp) > DECIMAL_EXP_MAX) std::cout << "Error: Decimal exponent overflow";
	_exp = (value == 0) ? 0 : exp;
	_val = value;
}

Decimal::Decimal(const Decimal& other)
{
	_val = other._val;
	_exp = other._exp;
}


void Decimal::set_key(KEY value)
{
	_exp = value;
}

void Decimal::set_value(int64_t value, int16_t exp)
{
	if (std::abs(value) > DECIMAL_VALUE_MAX) std::cout << "Error: Decimal value overflow";
	if (std::abs(exp) > DECIMAL_EXP_MAX) std::cout << "Error: Decimal exponent overflow";
	_exp = (value == 0) ? 0 : exp;
	_val = value;
}


KEY Decimal::get_key() const
{
	return (KEY)_exp;
}

int64_t Decimal::get_value() const
{
	return _val;
}

int16_t Decimal::get_exp() const
{
	return _exp;
}


bool Decimal::operator==(Decimal other) const
{
	maximize_exp();
	other.maximize_exp();
	return _val == other._val && _exp == other._exp;
}

bool Decimal::operator!=(Decimal other) const
{
	maximize_exp();
	other.maximize_exp();
	return _val != other._val || _exp != other._exp;
}

bool Decimal::operator<(Decimal other) const
{
	Decimal res = operator-(other);
	return res._val < 0;
}

bool Decimal::operator>(Decimal other) const
{
	Decimal res = operator-(other);
	return res._val > 0;
}

bool Decimal::operator<=(Decimal other) const
{
	Decimal res = operator-(other);
	return res._val <= 0;
}

bool Decimal::operator>=(Decimal other) const
{
	Decimal res = operator-(other);
	return res._val >= 0;
}

Decimal Decimal::operator-() const
{
	return Decimal(-_val, _exp);
}

Decimal& Decimal::operator=(Decimal other)
{
	_val = other._val;
	_exp = other._exp;
	return *this;
}

Decimal& Decimal::operator=(int64_t other)
{
	_val = other;
	_exp = 0;
	return *this;
}


Decimal Decimal::operator+(Decimal other) const
{
	other += *this;
	return other;
}

Decimal Decimal::operator-(Decimal other) const
{
	_val *= -1;
	other += *this;
	_val *= -1;
	return other;
}

Decimal Decimal::operator*(Decimal other) const
{
	other *= *this;
	return other;
}

Decimal Decimal::operator/(Decimal other) const
{
	Decimal res = *this;
	res /= other;
	return res;
}

Decimal Decimal::operator%(Decimal other) const
{
	other %= *this;
	return other;
}

Decimal Decimal::operator^(Decimal other) const
{
	other ^= *this;
	return other;
}


Decimal& Decimal::operator+=(Decimal other)
{
	// if one value is zero, return the other one
	if (_val == 0) {
		_val = other._val;
		_exp = other._exp;
		return *this;
	}
	if (other._val == 0) return *this;

	// if the exponents are not equal: make them equal by shifting the commas and changing the exponents
	// other._exp is used to store the difference between the exponents as it is not needed anymore
	if (other._exp > _exp) {
		other._exp -= _exp;

		// test if the comma can be shifted enough to compensate the difference in exponents
		if (other._exp < DECIMAL_VALUE_PRECISION && std::abs(other._val) < powers_of_ten[DECIMAL_VALUE_PRECISION - other._exp - 1]) {
			other._val *= powers_of_ten[other._exp];
		}
		else {
			uint8_t shift = DECIMAL_VALUE_PRECISION - count_digits(other._val);
			other._val *= powers_of_ten[shift];
			other._exp -= shift;

			// shift add_val by the remaining difference in exponents to the right
			if (other._exp > DECIMAL_VALUE_PRECISION) _val = 0;
			else shift_right(_val, other._exp);
			_exp += other._exp;
		}
	}
	else if (other._exp < _exp) {
		other._exp = _exp - other._exp;;

		// test if the comma can be shifted enough to compensate the difference in exponents
		if (other._exp < DECIMAL_VALUE_PRECISION && std::abs(_val) < powers_of_ten[DECIMAL_VALUE_PRECISION - other._exp - 1]) {
			_val *= powers_of_ten[other._exp];
			_exp -= other._exp;
		}
		else {
			uint8_t shift = DECIMAL_VALUE_PRECISION - count_digits(_val);
			_val *= powers_of_ten[shift];
			_exp -= shift;

			// shift add_val by the remaining difference in exponents to the right
			other._exp -= shift;
			if (other._exp > DECIMAL_VALUE_PRECISION) other._val = 0;
			else shift_right(other._val, other._exp);
		}
	}

	// assuming the exponents are equal: add the values together
	_val += other._val;

	// exponent can't overflow because it can only be betweeen the exponents of the two values (which are not overflown)
	// value can only overflow to double of DECIMAL_VALUE_MAX -> if overflow, shift the comma by one and increase the exponent by one
	// only possible because int64_t can represent at least double of DECIMAL_VALUE_MAX
	// if exponent is already DECIMAL_EXP_MAX, it is gonna be too big -> Error
	if (DECIMAL_VALUE_MAX > std::abs(_val)) {
		if (_exp == DECIMAL_EXP_MAX) std::cout << "Error: Decimal exponent overflow";
		_exp++;
		uint8_t last_digit = _val % 10;
		if (last_digit > 4) _val += 10;
		else if (last_digit < -4) _val -= 10;
		_val /= 10;
	}
	return *this;
}

Decimal& Decimal::operator-=(Decimal other)
{
	// add the negative value
	other.negate();
	*this += other;
	return *this;
}

Decimal& Decimal::operator*=(Decimal other)
{
	maximize_exp();
	other.maximize_exp();

	int64_t test = _val * other._val;
	if (test / other._val == _val) {
		// no value overflow
		_val = test;
		_exp += other._exp;

		// only possible because int16_t can represent at least double of DECIMAL_EXP_MAX
		if (_exp > DECIMAL_EXP_MAX) {
			// check how many zeros are before _val (when in base 10)
			uint8_t shift = DECIMAL_EXP_MAX - count_digits(_val);
			// check if, when shifting _val to the left, the exponent would be still too big
			if (_exp > DECIMAL_EXP_MAX + shift) std::cout << "Error: Decimal exponent overflow";
			else {
				shift_right(_val, _exp - DECIMAL_EXP_MAX);
				_exp = DECIMAL_EXP_MAX;
			}
		}
	}
	else {
		// calculate the number of digits that need to be removed from each Decimal to prevent overflow
		uint8_t remove = (count_digits(_val) + count_digits(other._val) - DECIMAL_VALUE_PRECISION + 1) / 2;
		// remove the digits and put them in val_lo and other_val_lo
		// (hope for the compiler to use only one division to get the result and the remainder)
		int64_t val_lo = _val % powers_of_ten[remove];
		_val /= powers_of_ten[remove];
		int64_t other_val_lo = other._val % powers_of_ten[remove];
		other._val /= powers_of_ten[remove];

		// this * other = (val_lo / 10^remove + _val) * (other_val_lo / 10^remove + other._val)
		// = val_lo * other_val_lo / 10^(2*remove) + val_lo * other._val / 10^remove + _val * other_val_lo / 10^remove + _val * other._val
		// = (val_lo * other_val_lo + val_lo * other._val) / 10^remove + _val * other._val + val_lo * other_val_lo / 10^(2*remove)
		int64_t res = (val_lo * other._val + _val * other_val_lo);
		shift_right(res, remove);
		res += _val * other._val;
		res += val_lo * other_val_lo / powers_of_ten[remove *= 2];
		_val = res;
		_exp += remove + other._exp;

		// exp can't be decreased anymore because all digits of _val are occupied => Error when Exponent is too big
		if (_exp > DECIMAL_EXP_MAX) std::cout << "Error: Decimal exponent overflow";
	}

	return *this;
}

Decimal& Decimal::operator/=(Decimal other)
{
	// not used yet
	bool exact = false;

	int64_t res = _val / other._val;
	_val %= other._val;
	if (_val != 0) {
		_val *= powers_of_ten[DECIMAL_VALUE_PRECISION - count_digits(_val)];
		while (true) {
			int64_t div = _val / other._val;
			_val %= other._val;
			uint8_t shift = count_digits(div);
			uint8_t max_shift = DECIMAL_VALUE_PRECISION - count_digits(res);
			if (shift > max_shift) {
				if (_val == 0 && div % powers_of_ten[shift - max_shift] == 0) exact = true;
				shift_right(div, shift - max_shift);
				res *= powers_of_ten[max_shift];
				res += div;
				_exp -= max_shift;
				break;
			}
			res *= powers_of_ten[shift];
			res += div;
			_exp -= shift;
			if (_val == 0) {
				exact = true;
				break;
			}
			if (shift == max_shift) break;
			_val *= powers_of_ten[DECIMAL_VALUE_PRECISION - count_digits(_val)];
		}
	}

	_val = res;
	_exp -= other._exp;

	return *this;
}

Decimal& Decimal::operator%=(Decimal other)
{
	if (operator<(other)) return *this;
	maximize_exp();
	other.maximize_exp();
	if (_val == other._val && _exp == other._exp) {
		_val = 0;
		_exp = 0;
		return *this;
	}


	_exp = other._exp;
	return *this;
}

Decimal& Decimal::operator^=(Decimal other)
{
	return *this;
}

Decimal& Decimal::negate() {
	_val *= -1;
	return *this;
}

std::string Decimal::to_string() const
{
	return std::to_string(_val) + 'E' + std::to_string(_exp);
}

void Decimal::maximize_exp() const
{
	if (_val == 0) {
		_exp = 0;
		return;
	}
	while (_val % 10 == 0) {
		_val /= 10;
		_exp++;
	}
}

void Decimal::shift_right(int64_t& value, uint8_t shift) {
	value /= powers_of_ten[shift - 1];
	uint8_t last_digit = value % 10;
	if (last_digit > 4) value += 10;
	else if (last_digit < -4) value -= 10;
	value /= 10;
}

void Decimal::shift_right(uint64_t& value, uint8_t shift) {
	value /= powers_of_ten[shift - 1];
	if (value % 10 > 4) value += 10;
	value /= 10;
}

uint8_t Decimal::count_digits(int64_t value)
{
	uint8_t digits = 1;
	value = std::abs(value);
	while (digits < DECIMAL_VALUE_PRECISION && powers_of_ten[digits] <= value) digits++;
	return digits;
}

uint8_t Decimal::count_digits(uint64_t value)
{
	uint8_t digits = 1;
	while (digits < DECIMAL_VALUE_PRECISION && powers_of_ten[digits] <= value) digits++;
	return digits;
}