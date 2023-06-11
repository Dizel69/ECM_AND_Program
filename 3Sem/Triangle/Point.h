#pragma once

class Point
{
	public:
	float x_;
	float y_;

	Point(float x=0, float y=0);
	void ShowPoint();
	void operator+=(Point &point);
	void operator=(const Point& point);
};