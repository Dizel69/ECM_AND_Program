#include "Triangle.h"
#include <iostream>
//#include "main.cpp"
#pragma warning(disable:4996)
float triangle_area(Point& a, Point& b, Point& c);

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3): v1_(p1),v2_(p2),v3_(p3)
{
	name_ = new char[32];
	//sprintf(name_, "Треугольник_%d", count++);
	//printf("ctoe_%s", name_);
	//Вычисление длин сторон
	a_ = sqrt((v1_.x_ - v2_.x_) * (v1_.x_ - v2_.x_) + (v1_.y_ - v2_.y_) * (v1_.y_ - v2_.y_));
	b_ = sqrt((v2_.x_ - v3_.x_) * (v2_.x_ - v3_.x_) + (v2_.y_ - v3_.y_) * (v2_.y_ - v3_.y_));
	c_ = sqrt((v1_.x_ - v3_.x_) * (v1_.x_ - v3_.x_) + (v1_.y_ - v3_.y_) * (v1_.y_ - v3_.y_));
	
}
	
Triangle::~Triangle()
{
	delete[] name_;
}

Triangle::Triangle(const Triangle& obj)
{
	strcpy(name_, obj.name_);
	v1_ = obj.v1_;
	v2_ = obj.v2_;
	v3_ = obj.v3_;
	a_ = obj.a_;
	b_ = obj.b_;
	c_ = obj.c_;
}

bool Triangle::operator>(Triangle &a)
{
	if (this->GetArea()>a.GetArea())
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Triangle::GetArea()
{
	float area = 0.5 * ((v1_.x_ - v3_.x_) * (v2_.y_ - v3_.y_) - (v1_.y_ - v3_.y_) * (v2_.x_ - v3_.x_));
	if (area>0)
	{
		return area;
	}
	return area * (-1);
}

void Triangle::show_triangle()
{
	printf("A=(%.1f %.1f), B=(%.1f %.1f), C=(%.1f %.1f), AB = %.1f, BC = %.1f, CA = %.1f, Area = %.1f\n", v1_.x_, v1_.y_, v2_.x_, v2_.y_, v3_.x_, v3_.y_, a_, b_, c_, GetArea());
}

void Triangle::move(Point a)
{
	v1_.x_ += a.x_;
	v2_.x_ += a.x_;
	v3_.x_ += a.x_;
	v1_.y_ += a.y_;
	v2_.y_ += a.y_;
	v3_.y_ += a.y_;
}

bool Triangle::check_include(Triangle& a)
{
	if (GetArea()==(triangle_area(v1_,v2_,a.v1_)+ triangle_area(v3_, v2_, a.v1_)+ triangle_area(v1_, v3_, a.v1_)))
	{
		if (GetArea() == (triangle_area(v1_, v2_, a.v2_) + triangle_area(v3_, v2_, a.v2_) + triangle_area(v1_, v3_, a.v2_)))
		{
			if (GetArea() == (triangle_area(v1_, v2_, a.v3_) + triangle_area(v3_, v2_, a.v3_) + triangle_area(v1_, v3_, a.v3_)))
			{
				return true;
			}
		}
	}
	return false;
}

void Triangle::triangle_fill()
{
	printf("Введите первую точку\n");
	scanf_s("%f%f", &v1_.x_, &v1_.y_);
	printf("Введите вторую точку\n");
	scanf_s("%f%f", &v2_.x_, &v2_.y_);
	printf("Введите третью точку\n");
	scanf_s("%f%f", &v3_.x_, &v3_.y_);
}

bool Triangle::operator<(Triangle& a)
{
	if (this->GetArea() < a.GetArea())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Triangle::operator==(Triangle& a)
{
	if (this->GetArea() == a.GetArea())
	{
		return true;
	}
	else
	{
		return false;
	}
}

Triangle::Triangle() : v1_(), v2_(), v3_()
{
	a_ = 0;
	b_ = 0;
	c_ = 0;
}