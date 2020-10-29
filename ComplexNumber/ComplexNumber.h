#pragma once
#include "RationalNumber.h"
#include <cmath>

class ComplexNumber {
public:
	ComplexNumber();
	ComplexNumber(const double &re, const double &im = 0);
	ComplexNumber(const RationalNumber &re, const RationalNumber &im);
	friend ComplexNumber operator+(const ComplexNumber &left, const ComplexNumber &right);
	ComplexNumber& operator+=(const ComplexNumber &right);
	friend ComplexNumber operator-(const ComplexNumber &left, const ComplexNumber &right);
	ComplexNumber& operator-=(const ComplexNumber &right);
	friend ComplexNumber operator*(const ComplexNumber &left, const ComplexNumber &right);
	ComplexNumber& operator*=(const ComplexNumber &right);
	friend ComplexNumber operator/(const ComplexNumber &left, const ComplexNumber &right);
	ComplexNumber& operator/=(const ComplexNumber &right);
	friend bool operator==(const ComplexNumber &left, const ComplexNumber &right);
	friend bool operator!=(const ComplexNumber &left, const ComplexNumber &right);
	ComplexNumber& operator=(const ComplexNumber &right);
	ComplexNumber& operator=(const double &right);
	ComplexNumber operator-();
	ComplexNumber Pow(const int &p);
	double arg();
	double abs();
	RationalNumber getReal() const;
	RationalNumber getImagine() const;
	void setReal(const long long &in);
	void setReal(const RationalNumber &in);
	void setImagine(const long long &in);
	void setImagine(const RationalNumber &in);
	std::string toString();
	friend std::ostream& operator<<(std::ostream &out, ComplexNumber &complexnumber);
private:
	RationalNumber real;
	RationalNumber imagine;
};
