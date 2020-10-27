#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "ComplexNumber.h"
#include <fstream>

TEST_CASE("Test") {

	SECTION("RationalNumber.cpp", "[constructor]")
	{
		RationalNumber a;
		REQUIRE(a.get_num() == 0);
		REQUIRE(a.get_den() == 1);
		REQUIRE(a.isNegative() == false);
	}
	{
		RationalNumber a(100, 1000);
		REQUIRE(a.get_num() == 1);
		REQUIRE(a.get_den() == 10);
		REQUIRE(a.isNegative() == false);
	}
	{
		RationalNumber a(83492, 72790);
		REQUIRE(a.get_num() == 41746);
		REQUIRE(a.get_den() == 36395);
		REQUIRE(a.isNegative() == false);
	}
	{
		RationalNumber a(-83492, 72790);
		REQUIRE(a.get_num() == 41746);
		REQUIRE(a.get_den() == 36395);
		REQUIRE(a.isNegative() == true);
	}
	{
		RationalNumber a(72790, -83492);
		REQUIRE(a.get_num() == 36395);
		REQUIRE(a.get_den() == 41746);
		REQUIRE(a.isNegative() == true);
	}

	SECTION("RationalNumber.cpp", "[operator=]")
	{
		RationalNumber a(33, 100), b;
		b = a;
		REQUIRE(a.get_num() == b.get_num());
		REQUIRE(a.get_den() == b.get_den());
		REQUIRE(a.isNegative() == b.isNegative());
	}
	
	SECTION("RationalNumber.cpp", "[operator+]")
	{
		RationalNumber a1(-1000, 333), b1(341, 631), c1;
		RationalNumber a2(-8923, 124), b2(-922, 100), c2;
		RationalNumber a3(1, 100), b3(199, 100), c3;
		c1 = a1 + b1;
		REQUIRE(c1.get_num() == 517447);
		REQUIRE(c1.get_den() == 210123);
		REQUIRE(c1.isNegative() == true);
		c2 = a2 + b2;
		REQUIRE(c2.get_num() == 251657);
		REQUIRE(c2.get_den() == 3100);
		REQUIRE(c2.isNegative() == true);
		c3 = a3 + b3;
		REQUIRE(c3.get_num() == 2);
		REQUIRE(c3.get_den() == 1);
		REQUIRE(c3.isNegative() == false);
	}

	SECTION("RationalNumber.cpp", "[operator-]")
	{
		RationalNumber a1(-1000, 333), b1(341, 631), c1;
		RationalNumber a2(-8923, 124), b2(-922, 100), c2;
		RationalNumber a3(1, 100), b3(199, 100), c3;
		c1 = a1 - b1;
		REQUIRE(c1.get_num() == 744553);
		REQUIRE(c1.get_den() == 210123);
		REQUIRE(c1.isNegative() == true);
		c2 = a2 - b2;
		REQUIRE(c2.get_num() == 194493);
		REQUIRE(c2.get_den() == 3100);
		REQUIRE(c2.isNegative() == true);
		c3 = a3 - b3;
		REQUIRE(c3.get_num() == 99);
		REQUIRE(c3.get_den() == 50);
		REQUIRE(c3.isNegative() == true);
	}

	SECTION("RationalNumber.cpp", "[operator*]")
	{
		RationalNumber a1(-1000, 333), b1(341, 631), c1;
		RationalNumber a2(-8923, 124), b2(-922, 100), c2;
		RationalNumber a3(1, 100), b3(199, 100), c3;
		c1 = a1 * b1;
		REQUIRE(c1.get_num() == 341000);
		REQUIRE(c1.get_den() == 210123);
		REQUIRE(c1.isNegative() == true);
		c2 = a2 * b2;
		REQUIRE(c2.get_num() == 4113503);
		REQUIRE(c2.get_den() == 6200);
		REQUIRE(c2.isNegative() == false);
		c3 = a3 * b3;
		REQUIRE(c3.get_num() == 199);
		REQUIRE(c3.get_den() == 10000);
		REQUIRE(c3.isNegative() == false);
	}

	SECTION("RationalNumber.cpp", "[operator/]")
	{
		RationalNumber a1(-1000, 333), b1(341, 631), c1;
		RationalNumber a2(-8923, 124), b2(-922, 100), c2;
		RationalNumber a3(1, 100), b3(199, 100), c3;
		c1 = a1 / b1;
		REQUIRE(c1.get_num() == 631000);
		REQUIRE(c1.get_den() == 113553);
		REQUIRE(c1.isNegative() == true);
		c2 = a2 / b2;
		REQUIRE(c2.get_num() == 223075);
		REQUIRE(c2.get_den() == 28582);
		REQUIRE(c2.isNegative() == false);
		c3 = a3 / b3;
		REQUIRE(c3.get_num() == 1);
		REQUIRE(c3.get_den() == 199);
		REQUIRE(c3.isNegative() == false);
	}

	SECTION("RationalNumber.cpp", "[operator==]")
	{
		RationalNumber a1(1, 10), b1(1, 10);
		RationalNumber a2(-100, 1000), b2(10, -100);
		RationalNumber a3(-2, -6), b3(4, -12);
		REQUIRE((a1 == b1) == true);
		REQUIRE((a2 == b2) == true);
		REQUIRE((a3 == b3) == false);
	}

	SECTION("ComplexNumber.cpp", "[operator+ +=]")
	{
		ComplexNumber a1(1.5, 4), b1(0, 6), c1(1.5, 4);
		ComplexNumber a2(-6.3, 5), b2(10, -3), c2(-6.3, 5);
		ComplexNumber a3(6.3, -5), b3(-10, 3), c3(6.3, -5);
		REQUIRE((a1 + b1).toString() == "3/2+10i");
		REQUIRE((a2 + b2).toString() == "37/10+2i");
		REQUIRE((a3 + b3).toString() == "-37/10-2i");
		c1 += b1;
		c2 += b2;
		c3 += b3;
		REQUIRE((a1 + b1).toString() == c1.toString());
		REQUIRE((a2 + b2).toString() == c2.toString());
		REQUIRE((a3 + b3).toString() == c3.toString());
	}

	SECTION("ComplexNumber.cpp", "[operator- -=]")
	{
		ComplexNumber a1(1.5, 4), b1(0, 6), c1(1.5, 4);
		ComplexNumber a2(-6.3, 5), b2(10, -3), c2(-6.3, 5);
		ComplexNumber a3(6.3, -5), b3(-10, 3), c3(6.3, -5);
		REQUIRE((a1 - b1).toString() == "3/2-2i");
		REQUIRE((a2 - b2).toString() == "-163/10+8i");
		REQUIRE((a3 - b3).toString() == "163/10-8i");
		c1 -= b1;
		c2 -= b2;
		c3 -= b3;
		REQUIRE((a1 - b1).toString() == c1.toString());
		REQUIRE((a2 - b2).toString() == c2.toString());
		REQUIRE((a3 - b3).toString() == c3.toString());
	}

	SECTION("ComplexNumber.cpp", "[operator* *=]")
	{
		ComplexNumber a1(1.5, 4), b1(0, 6), c1(1.5, 4);
		ComplexNumber a2(-6.3, 5), b2(10, -3), c2(-6.3, 5);
		ComplexNumber a3(6.3, -5), b3(-10, 3), c3(6.3, -5);
		REQUIRE((a1 * b1).toString() == "-24+9i");
		REQUIRE((a2 * b2).toString() == "-48+689/10i");
		REQUIRE((a3 * b3).toString() == "-48+689/10i");
		c1 *= b1;
		c2 *= b2;
		c3 *= b3;
		REQUIRE((a1 * b1).toString() == c1.toString());
		REQUIRE((a2 * b2).toString() == c2.toString());
		REQUIRE((a3 * b3).toString() == c3.toString());
	}

	SECTION("ComplexNumber.cpp", "[operator/ /=]")
	{
		ComplexNumber a1(1.5, 4), b1(0, 6), c1(1.5, 4);
		ComplexNumber a2(-6.3, 5), b2(10, -3), c2(-6.3, 5);
		ComplexNumber a3(6.3, -5), b3(-10, 3), c3(6.3, -5);
		REQUIRE((a1 / b1).toString() == "2/3-1/4i");
		REQUIRE((a2 / b2).toString() == "-78/109+311/1090i");
		REQUIRE((a3 / b3).toString() == "-78/109+311/1090i");
		c1 /= b1;
		c2 /= b2;
		c3 /= b3;
		REQUIRE((a1 / b1).toString() == c1.toString());
		REQUIRE((a2 / b2).toString() == c2.toString());
		REQUIRE((a3 / b3).toString() == c3.toString());
	}

	SECTION("ComplexNumber.cpp", "[operator== !=]")
	{
		ComplexNumber a1(1, 4), b1(1, 4);
		ComplexNumber a2(4, 1), b2(1, 4);
		ComplexNumber a3(-1, 1), b3(-1, -1);
		ComplexNumber a4(-1, 1), b4(1, -1);
		REQUIRE((a1 == b1) == true);
		REQUIRE((a1 != b1) == false);
		REQUIRE((a2 == b2) == false);
		REQUIRE((a2 != b2) == true);
		REQUIRE((a3 == b3) == false);
		REQUIRE((a3 != b3) == true);
		REQUIRE((a4 == b4) == false);
	}

	SECTION("ComplexNumber.cpp", "[operator-()]") {
		ComplexNumber a1(1, 4), b1(-1, -4);
		ComplexNumber a2(4, 1), b2(-4, -1);
		ComplexNumber a3(-1, 1), b3(1, -1);
		ComplexNumber a4(1, -1), b4(-1, 1);
		REQUIRE((-a1 == b1) == true);
		REQUIRE((-a2 == b2) == true);
		REQUIRE((-a3 == b3) == true);
		REQUIRE((-a4 == b4) == true);
	}

	SECTION("ComplexNumber.cpp", "[Pow()]") {
		ComplexNumber a(1, 4), b(-1.3, 2.5), c(7.1, 3);
		REQUIRE(a.Pow(3).toString() == "-47-52i");
		REQUIRE(b.Pow(2).toString() == "-114/25-13/2i");
		REQUIRE(c.Pow(4).toString() == "-999719/10000+882033/250i");
	}

	SECTION("ComplexNumber.cpp", "[arg()]")
	{
		ComplexNumber a(1, 4), b(1.3, -2.5), c(0, 3);
		REQUIRE(round(a.arg() * 10000) / 10000 == 1.3258);
		REQUIRE(round(b.arg() * 10000) / 10000 == -1.0913);
		REQUIRE(round(c.arg() * 10000) / 10000 == 1.5708);
	}

	SECTION("ComplexNumber.cpp", "[abs()]")
	{
		ComplexNumber a(1, 4), b(1.3, -2.5), c(0, 3);
		REQUIRE(a.abs() == sqrt(17));
		REQUIRE(b.abs() == sqrt(pow(1.3, 2) + pow(-2.5, 2)));
		REQUIRE(c.abs() == sqrt(9));
	}

	SECTION("ComplexNumber.cpp", "[operator<<]") 
	{	
		ComplexNumber a;
		std::ofstream fout("out.txt");
		fout << a << std::endl;
		a = ComplexNumber(1, 1);
		fout << a << std::endl;
		a.setReal(5);
		fout << a << std::endl;
		a.setImagine(-5);
		fout << a << std::endl;
		a.setReal(RationalNumber(10, 2));
		fout << a << std::endl;
		a.setImagine(RationalNumber(-2, 5));
		fout << a << std::endl;
		a = 5;
		REQUIRE(a.getReal() == RationalNumber(5, 1));
		REQUIRE(a.getImagine() == RationalNumber(0, 1));
	}
}
