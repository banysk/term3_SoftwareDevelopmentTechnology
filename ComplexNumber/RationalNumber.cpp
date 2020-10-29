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
	sign = 1;
}

RationalNumber::RationalNumber(const double &num, const double &den) {
	if (den == 0)
		throw std::invalid_argument("div by zero");
	bool num_sign = (num < 0), den_sign = (den < 0);
	(num_sign + den_sign) % 2 ? sign = -1 : sign = 1;
	num > 0 ? numerator = (long long)num : numerator = (long long)num * (-1);
	den > 0 ? denominator = (long long)den : denominator = (long long)den * (-1);
	long long devider = gcd(numerator, denominator);
	numerator /= devider;
	denominator /= devider;
}


RationalNumber RationalNumber::operator-() const {
	if (sign == 1)
		return RationalNumber(-numerator, denominator);
	else
		return RationalNumber(numerator, denominator);
}

RationalNumber& RationalNumber::operator=(const RationalNumber &right) {
	this->numerator = right.numerator;
	this->denominator = right.denominator;
	this->sign = right.sign;
	return *this;
}

RationalNumber operator+(const RationalNumber &left, const RationalNumber &right) {
	long long multiple = lcm(left.denominator, right.denominator);
	long long l, r;
	l = multiple / left.denominator * left.numerator * left.sign;
	r = multiple / right.denominator * right.numerator * right.sign;
	return RationalNumber(l + r, multiple);
}

RationalNumber operator-(const RationalNumber &left, const RationalNumber &right) {
	long long multiple = lcm(left.denominator, right.denominator);
	long long l, r;
	l = multiple / left.denominator * left.numerator * left.sign;
	r = multiple / right.denominator * right.numerator * right.sign;
	return RationalNumber(l - r, multiple);
}

RationalNumber operator*(const RationalNumber &left, const RationalNumber &right) {
	long long l, r;
	l = left.numerator * left.sign;
	r = right.numerator * right.sign;
	return RationalNumber(l * r, left.denominator * right.denominator);
}

RationalNumber operator/(const RationalNumber &left, const RationalNumber &right) {
	long long l, r;
	l = left.numerator * left.sign;
	r = right.numerator * right.sign;
	return RationalNumber(l * right.denominator, left.denominator * r);
}

bool operator==(const RationalNumber &left, const RationalNumber &right) {
	long long multiple = lcm(left.denominator, right.denominator);
	long long l, r;
	l = multiple / left.denominator * left.numerator * left.sign;
	r = multiple / right.denominator * right.numerator * right.sign;
	return (l == r);
}

bool operator!=(const RationalNumber &left, const RationalNumber &right) {
	return !(left == right);
}

long long RationalNumber::get_num() const{
	return numerator;
}

long long RationalNumber::get_den() const{
	return denominator;
}

short int RationalNumber::getSign() const{
	return sign;
}

double RationalNumber::toDouble() const{
	return (((double)numerator) / (double)denominator);
}