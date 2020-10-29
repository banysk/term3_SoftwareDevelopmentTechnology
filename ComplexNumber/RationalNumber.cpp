#include "RationalNumber.h"
#include <stdexcept>
#include <string>

long long gcd(const long long a, const long long b) {
	return b ? gcd(b, a % b) : a;
}

long long lcm(const long long a, const long long b) {
	return a / gcd(a, b) * b;
}

RationalNumber::RationalNumber() {
	numerator = 0;
	denominator = 1;
	is_negative = false;
}

RationalNumber::RationalNumber(const double &num, const double &den) {
	if (den == 0)
		throw std::invalid_argument("div by zero");
	bool num_sign = (num < 0), den_sign = (den < 0);
	is_negative = (num_sign + den_sign) % 2;
	num > 0 ? numerator = (long long)num : numerator = (long long)num * (-1);
	den > 0 ? denominator = (long long)den : denominator = (long long)den * (-1);
	long long devider = gcd(numerator, denominator);
	numerator /= devider;
	denominator /= devider;
}

RationalNumber& RationalNumber::operator=(const RationalNumber &right) {
	this->numerator = right.numerator;
	this->denominator = right.denominator;
	this->is_negative = right.is_negative;
	return *this;
}

RationalNumber operator+(const RationalNumber &left, const RationalNumber &right) {
	long long multiple = lcm(left.denominator, right.denominator);
	long long l, r;
	left.is_negative ? l = multiple / left.denominator * left.numerator * -1 : l = multiple / left.denominator * left.numerator;
	right.is_negative ? r = multiple / right.denominator * right.numerator * -1: r = multiple / right.denominator * right.numerator;
	return RationalNumber(l + r, multiple);
}

RationalNumber operator-(const RationalNumber &left, const RationalNumber &right) {
	long long multiple = lcm(left.denominator, right.denominator);
	long long l, r;
	left.is_negative ? l = multiple / left.denominator * left.numerator * -1 : l = multiple / left.denominator * left.numerator;
	right.is_negative ? r = multiple / right.denominator * right.numerator * -1 : r = multiple / right.denominator * right.numerator;
	return RationalNumber(l - r, multiple);
}

RationalNumber operator*(const RationalNumber &left, const RationalNumber &right) {
	long long l, r;
	left.is_negative ? l = left.numerator * -1 : l = left.numerator;
	right.is_negative ? r = right.numerator * -1 : r = right.numerator;
	return RationalNumber(l * r, left.denominator * right.denominator);
}

RationalNumber operator/(const RationalNumber &left, const RationalNumber &right) {
	long long l, r;
	left.is_negative ? l = left.numerator * -1 : l = left.numerator;
	right.is_negative ? r = right.numerator * -1 : r = right.numerator;
	return RationalNumber(l * right.denominator, left.denominator * r);
}

bool operator==(const RationalNumber &left, const RationalNumber &right) {
	long long multiple = lcm(left.denominator, right.denominator);
	long long l, r;
	left.is_negative ? l = multiple / left.denominator * left.numerator * (-1) : l = multiple / left.denominator * left.numerator;
	right.is_negative ? r = multiple / right.denominator * right.numerator * (-1) : r = multiple / right.denominator * right.numerator;
	return (l == r);
}

bool operator!=(const RationalNumber &left, const RationalNumber &right) {
	return !(left == right);
}

long long RationalNumber::get_num() {
	return numerator;
}

long long RationalNumber::get_den() {
	return denominator;
}

bool RationalNumber::isNegative() {
	return is_negative;
}

void RationalNumber::signReverse() {
	is_negative = !is_negative;
}

double RationalNumber::toDouble() {
	return (((double)numerator) / (double)denominator);
}
