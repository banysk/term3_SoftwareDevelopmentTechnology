#pragma once
#include <string>

long long gcd(const long long a, const long long b);
long long lcm(const long long a, const long long b);

class RationalNumber {
public:
	RationalNumber();
	RationalNumber(const double &num, const double &den = 1);
	RationalNumber& operator=(const RationalNumber &right);
	friend RationalNumber operator+(const RationalNumber &left, const RationalNumber &right);
	friend RationalNumber operator-(const RationalNumber &left, const RationalNumber &right);
	friend RationalNumber operator*(const RationalNumber &left, const RationalNumber &right);
	friend RationalNumber operator/(const RationalNumber &left, const RationalNumber &right);
	friend bool operator==(const RationalNumber &left, const RationalNumber &right);
	friend bool operator!=(const RationalNumber &left, const RationalNumber &right);
	long long get_num();
	long long get_den();
	bool isNegative();
	void signReverse();
	double toDouble();
private:
	long long numerator;
	long long denominator;
	bool is_negative;
};