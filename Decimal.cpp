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
	1000000000000000000
};


Decimal::Decimal()
{
	_val = 0;
	_exp = 0;
}

Decimal::Decimal(int64_t value)
{
	if (std::abs(value) > DECIMAL_VALUE_MAX) std::cout << "Error: Decimal value overflow" << std::endl;
	_exp = 0;
	_val = value;
}

Decimal::Decimal(int64_t value, int16_t exp)
{
	if (std::abs(value) > DECIMAL_VALUE_MAX) std::cout << "Error: Decimal value overflow" << std::endl;
	if (std::abs(exp) > DECIMAL_EXP_MAX) std::cout << "Error: Decimal exponent overflow" << std::endl;
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
	if (std::abs(value) > DECIMAL_VALUE_MAX) std::cout << "Error: Decimal value overflow" << std::endl;
	if (std::abs(exp) > DECIMAL_EXP_MAX) std::cout << "Error: Decimal exponent overflow" << std::endl;
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
	other.negate();
	other += *this;
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
	Decimal res = *this;
	res %= other;
	return res;
}

Decimal Decimal::operator^(Decimal other) const
{
	Decimal res = *this;
	res ^= other;
	return res;
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
	// this is done by shifting the bigger value to the left until the exponents are equal, if no more
	// shifting is possible, shift the smaller value to the right until the exponents are equal
	// other._exp is used to store the difference between the exponents as it is not needed anymore
	if (other._exp > _exp) {
		other._exp -= _exp;

		// test if the the bigger value can be shifted enough to the left to compensate the difference in exponents
		if (other._exp < DECIMAL_VALUE_PRECISION && std::abs(other._val) < powers_of_ten[DECIMAL_VALUE_PRECISION - other._exp]) {
			other._val *= powers_of_ten[other._exp];
		} else {
			// when it can't be shifted enough, shift it as much as possible and shift the smaller value to the right
			uint8_t max_shift = DECIMAL_VALUE_PRECISION - count_digits(other._val);
			other._val *= powers_of_ten[max_shift];
			other._exp -= max_shift;

			if (other._exp > DECIMAL_VALUE_PRECISION) _val = 0;
			else shift_right(_val, other._exp);
			_exp += other._exp;
		}
	} else if (other._exp < _exp) {
		other._exp = _exp - other._exp;

		// test if the the bigger value can be shifted enough to the left to compensate the difference in exponents
		if (other._exp < DECIMAL_VALUE_PRECISION && std::abs(_val) < powers_of_ten[DECIMAL_VALUE_PRECISION - other._exp]) {
			_val *= powers_of_ten[other._exp];
			_exp -= other._exp;
		} else {
			// when it can't be shifted enough, shift it as much as possible and shift the smaller value to the right
			uint8_t shift = DECIMAL_VALUE_PRECISION - count_digits(_val);
			_val *= powers_of_ten[shift];
			_exp -= shift;

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
	if (std::abs(_val) > DECIMAL_VALUE_MAX) {
		if (_exp == DECIMAL_EXP_MAX) std::cout << "Error: Decimal overflow" << std::endl;
		_exp++;
		shift_right_one(_val);
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

	// test if the value can be multiplied without overflow
	int64_t test = _val * other._val;
	if (test / other._val == _val) {
		_val = test;
		_exp += other._exp;

		// value can still be bigger than DECIMAL_VALUE_MAX -> shift the value to the left by one and increase the exponent by one
		if (std::abs(_val) > DECIMAL_VALUE_MAX) {
			_exp++;
			if (std::abs(_exp) > DECIMAL_EXP_MAX) std::cout << "Error: Decimal overflow" << std::endl;
			shift_right_one(_val);
		} else if (std::abs(_exp) > DECIMAL_EXP_MAX) {
			// check how many zeros are before _val (when in base 10)
			uint8_t shift = DECIMAL_EXP_MAX - count_digits(_val);
			// check if, when shifting the value to the left, the exponent would be still too big
			if (_exp > DECIMAL_EXP_MAX + shift) std::cout << "Error: Decimal overflow" << std::endl;
			else {
				shift_right(_val, _exp - DECIMAL_EXP_MAX);
				_exp = DECIMAL_EXP_MAX;
			}
		}
	} else {
		if (std::abs(_val) > std::abs(other._val)) std::swap(_val, other._val);
		uint8_t remove_this;
		uint8_t remove_other = 1;
		int64_t val_lo;
		int64_t other_val_lo;
		int64_t abs_val = std::abs(_val);
		int64_t abs_other_val = std::abs(other._val);
		if (abs_val < powers_of_ten[9]) {
			remove_this = 0;
			val_lo = 0;
		} else {
			remove_this = count_digits(_val) - 9;
			val_lo = _val % powers_of_ten[remove_this];
			_val /= powers_of_ten[remove_this];
		}
		abs_other_val /= 10;
		abs_val = DECIMAL_VALUE_MAX / abs_val;
		while (abs_other_val > abs_val) {
			abs_other_val /= 10;
			remove_other++;
		}
		other_val_lo = other._val % powers_of_ten[remove_other];
		other._val /= powers_of_ten[remove_other];

		// this * other
		// = (val_lo + _val * 10^remove_this) * (other_val_lo + other._val * 10^remove_other)
		// = (val_lo / 10^remove_this + _val) * (other_val_lo / 10^remove_other + other._val)    when increasing _exp of the result by (remove_this + remove_other)
		// = (val_lo * other_val_lo / 10^(remove_this + remove_other)) + (val_lo * other._val / 10^remove_this) + (_val * other_val_lo / 10^remove_other) + (_val * other._val)

		int8_t last_digit = 0;
		// _val * other._val
		int64_t res = _val * other._val;
		// val_lo * other._val / 10^remove_this
		int64_t part_res = val_lo * other._val;
		if (remove_this != 0) {
			part_res /= powers_of_ten[remove_this - 1];
			last_digit = part_res % 10;
			part_res /= 10;
		}
		res += part_res;
		// _val * other_val_lo / 10^remove_other
		part_res = _val * other_val_lo;
		part_res /= powers_of_ten[remove_other - 1];
		last_digit += part_res % 10;
		res += part_res / 10;
		// val_lo * other_val_lo / 10^(2*remove)
		part_res = val_lo * other_val_lo;
		part_res /= powers_of_ten[(remove_this += remove_other) - 1];
		last_digit += part_res % 10;
		res += part_res / 10;

		res += last_digit / 10;
		last_digit %= 10;
		if (last_digit > 4) res += 1;
		else if (last_digit < -4) res -= 1;

		_val = res;
		_exp += remove_this + other._exp;

		// exp can't be decreased anymore because all digits of _val are occupied => Error when Exponent is too big
		if (std::abs(_exp) > DECIMAL_EXP_MAX) std::cout << "Error: Decimal overflow" << std::endl;
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
		while (true) {
			int64_t shift = DECIMAL_VALUE_PRECISION - count_digits(_val);
			_val *= powers_of_ten[shift];
			int64_t div = _val / other._val;
			_val %= other._val;
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

Decimal& Decimal::round()
{
	return *this;
}

Decimal& Decimal::floor()
{
	return *this;
}

Decimal& Decimal::ceil()
{
	return *this;
}

Decimal& Decimal::to_int()
{
	return *this;
}

Decimal& Decimal::abs()
{
	_val = std::abs(_val);
	return *this;
}

Decimal& Decimal::negate()
{
	_val *= -1;
	return *this;
}

Decimal& Decimal::pow(const Decimal& other)
{
	return *this;
}

Decimal& Decimal::root(const Decimal& other)
{
	return *this;
}

Decimal& Decimal::factorial()
{
	maximize_exp();
	if (_exp != 0 || _val < 0 || _val > 3253) std::cout << "Error: Decimal overflow" << std::endl;
	else {
		Decimal res = _val;
		_val = 1;
		for (; res._val > 1; res._val--)
			operator*=(res);
	}
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

void Decimal::shift_right(int64_t& value, uint8_t shift)
{
	value /= powers_of_ten[shift - 1];
	int8_t last_digit = value % 10;
	if (last_digit > 4) value += 10;
	else if (last_digit < -4) value -= 10;
	value /= 10;
}

void Decimal::shift_right(uint64_t& value, uint8_t shift)
{
	value /= powers_of_ten[shift - 1];
	if (value % 10 > 4) value += 10;
	value /= 10;
}

void Decimal::shift_right_one(int64_t& value)
{
	int8_t last_digit = value % 10;
	value /= 10;
	if (last_digit > 4) value += 1;
	else if (last_digit < -4) value -= 1;
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