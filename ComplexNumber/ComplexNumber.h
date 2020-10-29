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
	ComplexNumber operator-() const;
	ComplexNumber Pow(const int &p) const;
	double arg() const;
	double abs() const;
	RationalNumber getReal() const;
	RationalNumber getImagine() const;
	void setReal(const long long &in);
	void setReal(const RationalNumber &in);
	void setImagine(const long long &in);
	void setImagine(const RationalNumber &in);
	std::string toString() const;
	friend std::ostream& operator<<(std::ostream &out, const ComplexNumber &complexnumber);
private:
	RationalNumber real;
	RationalNumber imagine;
};