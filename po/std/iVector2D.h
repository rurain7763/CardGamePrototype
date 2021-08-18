#pragma once

#define iVector2DMakeZero iVector2DMake(0,0)

struct iVector2D
{
public:
	iVector2D operator+(const iVector2D& v);
	iVector2D operator-(const iVector2D& v);
	iVector2D operator*(const iVector2D& v);
	iVector2D operator/(const iVector2D& v);

	iVector2D& operator+=(const iVector2D& v);
	iVector2D& operator-=(const iVector2D& v);
	iVector2D& operator*=(const iVector2D& v);
	iVector2D& operator/=(const iVector2D& v);

	iVector2D operator+(const float& f);
	iVector2D operator-(const float& f);
	iVector2D operator*(const float& f);
	iVector2D operator/(const float& f);

	iVector2D& operator+=(const float& f);
	iVector2D& operator-=(const float& f);
	iVector2D& operator*=(const float& f);
	iVector2D& operator/=(const float& f);

	bool operator==(const iVector2D& v);
	bool operator!=(const iVector2D& v);

	void normalize();
	float length();
public:
	float x, y;
};

iVector2D iVector2DMake(float x, float y);
float dotProduct(iVector2D v1, iVector2D v2);
iVector2D linear(iVector2D s, iVector2D e, float r);
iVector2D easeIn(iVector2D s, iVector2D e, float r);
iVector2D iVector2DRotate(iVector2D p, iVector2D t, float degree);


