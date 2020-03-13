#include "Fuzzy.h"

float Fuzzy::FuzzyGrade(float value, float x0, float x1)
{
	if (value <= x0)
	{
		return 0;
	}
	else if (value > x1)
	{
		return 1;
	}
	else
	{
		return (value - x0) / (x1 - x0);
	}
}

float Fuzzy::FuzzyTriangle(float value, float x0, float x1, float x2)
{
	if (value <= x0 || value >= x2)
	{
		return 0;
	}
	else if (value == x1)
	{
		return 1;
	}
	else if (value > x0 && value < x1)
	{
		return (value - x0) / (x1 - x0);
	}
	else
	{
		return  (x2 - value) / (x2 - x1);
	}
}

float Fuzzy::FuzzyTrapezoid(float value, float x0, float x1, float x2, float x3)
{
	if (value <= x0 || value >= x3)
	{
		return 0;
	}
	else if (value >= x1 && value <= x2)
	{
		return 1;
	}
	else if (value > x0 && value < x1)
	{
		return (value - x0) / (x1 - x0);
	}
	else
	{
		return (x3 - value) / (x3 - x2);
	}
}

float Fuzzy::AND(float A, float B)
{
	return std::min(A, B);
}

float Fuzzy::OR(float A, float B)
{
	return std::max(A, B);
}

float Fuzzy::NOT(float A)
{
	return 1.0 - A;
}
