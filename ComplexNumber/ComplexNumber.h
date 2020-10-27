#pragma once
#include "RationalNumber.h"
#include <cmath>

class ComplexNumber {
public:
	ComplexNumber();
	ComplexNumber(double re, double im = 0);
	ComplexNumber(RationalNumber re, RationalNumber im);
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
	ComplexNumber Pow(int p);
	double arg();
	double abs();
	RationalNumber getReal();
	RationalNumber getImagine();
	void setReal(long long in);
	void setReal(RationalNumber in);
	void setImagine(long long in);
	void setImagine(RationalNumber in);
	std::string toString();
	friend std::ostream& operator<<(std::ostream &out, ComplexNumber &complexnumber);
private:
	RationalNumber real;
	RationalNumber imagine;
};