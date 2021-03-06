#include "ComplexNumber.h"
#include <cmath>
#define M_PI 3.14159265358979323846  /* pi */

ComplexNumber::ComplexNumber() {}

ComplexNumber::ComplexNumber(const double &re, const double &im) {
	real = RationalNumber(re * 1000, 1000);
	imagine = RationalNumber(im * 1000, 1000);
}

ComplexNumber::ComplexNumber(const RationalNumber &re, const RationalNumber &im) {
	real = re;
	imagine = im;
}

ComplexNumber operator+(const ComplexNumber &left, const ComplexNumber &right) {
	return ComplexNumber(left.real + right.real, left.imagine + right.imagine);
}

ComplexNumber& ComplexNumber::operator+=(const ComplexNumber &right) {
	*this = *this + right;
	return *this;
}

ComplexNumber operator-(const ComplexNumber &left, const ComplexNumber &right) {
	return ComplexNumber(left.real - right.real, left.imagine - right.imagine);
}

ComplexNumber& ComplexNumber::operator-=(const ComplexNumber &right) {
	*this = *this - right;
	return *this;
}

ComplexNumber operator*(const ComplexNumber &left, const ComplexNumber &right) {
	return ComplexNumber(left.real * right.real - left.imagine * right.imagine, left.real * right.imagine + left.imagine * right.real);
}

ComplexNumber& ComplexNumber::operator*=(const ComplexNumber &right) {
	*this = *this * right;
	return *this;
}

ComplexNumber operator/(const ComplexNumber &left, const ComplexNumber &right) {
	RationalNumber re = ((left.real * right.real + left.imagine * right.imagine) / (right.real * right.real + right.imagine * right.imagine));
	RationalNumber im = ((right.real * left.imagine - left.real * right.imagine) / (right.real * right.real + right.imagine * right.imagine));
	return ComplexNumber(re, im);
}

ComplexNumber& ComplexNumber::operator/=(const ComplexNumber &right) {
	*this = *this / right;
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

ComplexNumber ComplexNumber::operator-() const{
	RationalNumber re = this->real, im = this->imagine;
	return ComplexNumber(-re, -im);
}

ComplexNumber ComplexNumber::Pow(const int &p = 2) const{
	RationalNumber re = real;
	RationalNumber im = imagine;
	ComplexNumber a(re, im);
	for (int i = 1; i < p; i++) {
		a *= ComplexNumber(re, im);
	}
	return a;
}

double ComplexNumber::arg() const{
	double x, y;
	x = real.toDouble() * real.getSign();
	y = imagine.toDouble() * imagine.getSign();
	return atan2(y, x);
}

double ComplexNumber::abs() const{
	RationalNumber middle = real * real + imagine * imagine;
	return sqrt((double)middle.get_num()/(double)middle.get_den());
}

RationalNumber ComplexNumber::getReal() const{
	return real;
}

RationalNumber ComplexNumber::getImagine() const{
	return imagine;
}

void ComplexNumber::setReal(const long long &in) {
	real = RationalNumber(in);
}

void ComplexNumber::setReal(const RationalNumber &in) {
	real = in;
}

void ComplexNumber::setImagine(const long long &in) {
	imagine = RationalNumber(in);
}

void ComplexNumber::setImagine(const RationalNumber &in) {
	imagine = in;
}

std::string ComplexNumber::toString() const{
	std::string out;
	if (real.getSign() == -1) out += '-';
	out += std::to_string(real.get_num());
	if (real.get_den() != 1) out += '/' + std::to_string(real.get_den());
	imagine.getSign() == -1 ? out += '-' : out += '+';
	out += std::to_string(imagine.get_num());
	if (imagine.get_den() != 1) out += '/' + std::to_string(imagine.get_den());
	out += 'i';
	return out;
}

std::ostream& operator<<(std::ostream &out, const ComplexNumber &complexnumber) {
	out << complexnumber.toString();
	return out;
}