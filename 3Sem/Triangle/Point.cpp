#include "Point.h"
#include <iostream>

Point::Point(float x , float y )
{
	x_ = x;
	y_ = y;
}

void Point::ShowPoint()
{
	printf("(%f,%f)", x_, y_);
}

void Point::operator=(const Point &a)
{
	x_ = a.x_;
	y_ = a.y_;
}

void Point::operator+=(Point& a)
{
	x_ += a.x_;
	y_ += a.y_;
}
