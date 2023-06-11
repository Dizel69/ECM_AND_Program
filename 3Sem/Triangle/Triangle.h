#pragma once
#include "Point.h"

class Triangle
{
	char* name_;
	Point v1_, v2_, v3_;
	double a_, b_, c_;

public:
	static int count;
	Triangle(const Point& p1, const Point& p2, const Point& p3);
	Triangle(const Triangle& obj);
	Triangle();
	float GetArea();
	bool operator>(Triangle &a);
	bool operator<(Triangle& a);
	bool operator==(Triangle& a);
	~Triangle();
	void show_triangle();
	void move(Point a);
	bool check_include(Triangle& a);
	void triangle_fill();
};
