#include"iVector2D.h"
#include"iStd.h"

iVector2D iVector2D::operator+(const iVector2D& v)
{
	return iVector2D{ x + v.x,y + v.y };
}

iVector2D iVector2D::operator-(const iVector2D& v)
{
	return iVector2D{ x - v.x,y - v.y };
}

iVector2D iVector2D::operator*(const iVector2D& v)
{
	return iVector2D{ x * v.x,y * v.y };
}

iVector2D iVector2D::operator/(const iVector2D& v)
{
	return iVector2D{ x / v.x,y / v.y };
}

iVector2D& iVector2D::operator+=(const iVector2D& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

iVector2D& iVector2D::operator-=(const iVector2D& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

iVector2D& iVector2D::operator*=(const iVector2D& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

iVector2D& iVector2D::operator/=(const iVector2D& v)
{
	x /= v.x;
	y /= v.y;

	return *this;
}

iVector2D iVector2D::operator+(const float& f)
{
	return iVector2D{ x + f,y + f };
}

iVector2D iVector2D::operator-(const float& f)
{
	return iVector2D{ x - f , y - f };
}

iVector2D iVector2D::operator*(const float& f)
{
	return iVector2D{ x * f,y * f };
}

iVector2D iVector2D::operator/(const float& f)
{
	return iVector2D{ x / f,y / f };
}

iVector2D& iVector2D::operator+=(const float& f)
{
	x += f;
	y += f;

	return *this;
}

iVector2D& iVector2D::operator-=(const float& f)
{
	x -= f;
	y -= f;

	return *this;
}

iVector2D& iVector2D::operator*=(const float& f)
{
	x *= f;
	y *= f;

	return *this;
}

iVector2D& iVector2D::operator/=(const float& f)
{
	x /= f;
	y /= f;
		 
	return *this;
}

bool iVector2D::operator==(const iVector2D& v)
{
	return x == v.x && y == v.y;
}

bool iVector2D::operator!=(const iVector2D& v)
{
	return x != v.x || y != v.y;
}

void iVector2D::normalize()
{
	int length = sqrt(x * x + y * y);
	if (length == 0.f)
	{
		x = 0; y = 0;
		return;
	}

	x /= length;
	y /= length;
}

float iVector2D::length()
{
	return sqrt(x * x + y * y);
}

iVector2D iVector2DMake(float x, float y)
{
	return iVector2D{ x,y };
}

float dotProduct(iVector2D v1, iVector2D v2)
{
	return v1.x*v2.x + v1.y* v2.y;
}

iVector2D linear(iVector2D s, iVector2D e, float r)
{
	r = clamp(r, 0, 1);
	return s + (e - s) * r;
}

iVector2D easeIn(iVector2D s, iVector2D e, float r)
{
	r = clamp(r, 0, 1);
	return s + (e - s) * _sin(90 * r);
}

iVector2D iVector2DRotate(iVector2D p, iVector2D t, float degree)
{
	float c = _cos(degree), s = _sin(degree);
	iVector2D dir = iVector2DMake(p.x - t.x , t.y - p.y);
	iVector2D r;
	r.x = dir.x * c - dir.y * s;
	r.y = dir.x * s + dir.y * c;
	
	r.x = t.x + r.x;
	r.y = t.y - r.y;

	return r;
}

