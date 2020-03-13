#pragma once
#include <algorithm>
class Fuzzy
{
public:
	static float FuzzyGrade(float value, float x0, float x1);
	static float FuzzyTriangle(float value, float x0, float x1, float x2);
	static float FuzzyTrapezoid(float value, float x0, float x1, float x2, float x3);
	static float AND(float A, float B);
	static float OR(float A, float B);
	static float NOT(float A);
private:
};

