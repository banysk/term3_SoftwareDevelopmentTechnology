#include "ComplexNumber.h"
#include <cmath>
#define M_PI 3.14159265358979323846  /* pi */

ComplexNumber::ComplexNumber() {}

ComplexNumber::ComplexNumber(double re, double im) {
	real = RationalNumber(re * 1000, 1000);
	imagine = RationalNumber(im * 1000, 1000);
}

ComplexNumber::ComplexNumber(RationalNumber re, RationalNumber im) {
	real = re;
	imagine = im;
}

ComplexNumber operator+(const ComplexNumber &left, const ComplexNumber &right) {
	return ComplexNumber(left.real + right.real, left.imagine + right.imagine);
}

ComplexNumber& ComplexNumber::operator+=(const ComplexNumber &right) {
	this->real = this->real + right.real;
	this->imagine = this->imagine + right.imagine;
	return *this;
}

ComplexNumber operator-(const ComplexNumber &left, const ComplexNumber &right) {
	return ComplexNumber(left.real - right.real, left.imagine - right.imagine);
}

ComplexNumber& ComplexNumber::operator-=(const ComplexNumber &right) {
	this->real = this->real - right.real;
	this->imagine = this->imagine - right.imagine;
	return *this;
}

ComplexNumber operator*(const ComplexNumber &left, const ComplexNumber &right) {
	return ComplexNumber(left.real * right.real - left.imagine * right.imagine, left.real * right.imagine + left.imagine * right.real);
}

ComplexNumber& ComplexNumber::operator*=(const ComplexNumber &right) {
	RationalNumber re = this->real, im = this->imagine;
	this->real = re * right.real - im * right.imagine;
	this->imagine = re * right.imagine + im * right.real;
	return *this;
}

ComplexNumber operator/(const ComplexNumber &left, const ComplexNumber &right) {
	RationalNumber re = ((left.real * right.real + left.imagine * right.imagine) / (right.real * right.real + right.imagine * right.imagine));
	RationalNumber im = ((right.real * left.imagine - left.real * right.imagine) / (right.real * right.real + right.imagine * right.imagine));
	return ComplexNumber(re, im);
}

ComplexNumber& ComplexNumber::operator/=(const ComplexNumber &right) {
	RationalNumber re = this->real, im = this->imagine;
	this->real = ((re * right.real + im * right.imagine) / (right.real * right.real + right.imagine * right.imagine));
	this->imagine = ((right.real * im - re * right.imagine) / (right.real * right.real + right.imagine * right.imagine));
	return *this;
}

ComplexNumber& ComplexNumber::operator=(const ComplexNumber &right) {
	this->real = right.real;
	this->imagine = right.imagine;
	return *this;
}

ComplexNumber& ComplexNumber::operator=(const double &right) {
	this->real = RationalNumber(right);
	this->imagine = RationalNumber(0);
	return *this;
}

bool operator==(const ComplexNumber &left, const ComplexNumber &right) {
	return (left.real == right.real && left.imagine == right.imagine);
}

bool operator!=(const ComplexNumber &left, const ComplexNumber &right) {
	return (left.real != right.real || left.imagine != right.imagine);
}

ComplexNumber ComplexNumber::operator-() {
	RationalNumber re = this->real, im = this->imagine;
	re.signReverse();
	im.signReverse();
	return ComplexNumber(re, im);
}

ComplexNumber ComplexNumber::Pow(int p = 2) {
	RationalNumber re = real;
	RationalNumber im = imagine;
	ComplexNumber a(re, im);
	for (int i = 1; i < p; i++) {
		a *= ComplexNumber(re, im);
	}
	return a;
}

double ComplexNumber::arg() {
	double x, y;
	real.isNegative() ? x = real.toDouble() * (-1) : x = real.toDouble();
	imagine.isNegative() ? y = imagine.toDouble() * (-1) : y = imagine.toDouble();
	return atan2(y, x);
}

double ComplexNumber::abs() {
	RationalNumber middle = real * real + imagine * imagine;
	return sqrt((double)middle.get_num()/(double)middle.get_den());
}

RationalNumber ComplexNumber::getReal() {
	return real;
}

RationalNumber ComplexNumber::getImagine() {
	return imagine;
}

void ComplexNumber::setReal(long long in) {
	real = RationalNumber(in);
}

void ComplexNumber::setReal(RationalNumber in) {
	real = in;
}

void ComplexNumber::setImagine(long long in) {
	imagine = RationalNumber(in);
}

void ComplexNumber::setImagine(RationalNumber in) {
	imagine = in;
}

std::string ComplexNumber::toString() {
	std::string out;
	if (real.isNegative()) out += '-';
	out += std::to_string(real.get_num());
	if (real.get_den() != 1) out += '/' + std::to_string(real.get_den());
	imagine.isNegative() ? out += '-' : out += '+';
	out += std::to_string(imagine.get_num());
	if (imagine.get_den() != 1) out += '/' + std::to_string(imagine.get_den());
	out += 'i';
	return out;
}

std::ostream& operator<<(std::ostream &out, ComplexNumber &complexnumber) {
	out << complexnumber.toString();
	return out;
}