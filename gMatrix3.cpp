#include "gMatrix3.h"

#define PI 3.14159265

gMatrix3::gMatrix3()
{
	data[0] = gVector3(1, 0, 0);
	data[1] = gVector3(0, 1, 0);
	data[2] = gVector3(0, 0, 1);
}

gMatrix3::gMatrix3(const gVector3& row1, const gVector3& row2, const gVector3& row3)
{
	data[0] = row1;
	data[1] = row2;
	data[2] = row3;
}

gVector3
gMatrix3::operator[](unsigned int index) const
{
	return data[index];
}

gVector3&
gMatrix3::operator[](unsigned int index)
{
	return data[index];
}

gVector3
gMatrix3::getColumn(unsigned int index) const
{
	gVector3 v;
	v[0] = data[0][index];
	v[1] = data[1][index];
	v[2] = data[2][index];
	return v;
}

void
gMatrix3::print(){
	std::cout << "The matrix is: [" << data[0][0] << ", " << data[0][1] << ", " << data[0][2] << "]" << std::endl;
	std::cout << "               [" << data[1][0] << ", " << data[1][1] << ", " << data[1][2] << "]" << std::endl;
	std::cout << "               [" << data[2][0] << ", " << data[2][1] << ", " << data[2][2] << "]" << std::endl;
}

bool
operator==(const gMatrix3& m1, const gMatrix3& m2)
{
	if (m1[0] == m2[0] && m1[1] == m2[1] && m1[2] == m2[2])
		return true;
	else
		return false;
}

bool
operator!=(const gMatrix3& m1, const gMatrix3& m2)
{
	if (m1 == m2)
		return false;
	else
		return true;
}

gMatrix3
gMatrix3::transpose() const
{
	gVector3 temp1 = getColumn(0);
	gVector3 temp2 = getColumn(1);
	gVector3 temp3 = getColumn(2);
	return gMatrix3(temp1, temp2, temp3);
}

gMatrix3
operator+(const gMatrix3& m1, const gMatrix3& m2)
{
	gVector3 v1 = m1[0] + m2[0];
	gVector3 v2 = m1[1] + m2[1];
	gVector3 v3 = m1[2] + m2[2];
	return gMatrix3(v1, v2, v3);
}

gMatrix3
operator-(const gMatrix3& m1, const gMatrix3& m2)
{
	gVector3 v1 = m1[0] - m2[0];
	gVector3 v2 = m1[1] - m2[1];
	gVector3 v3 = m1[2] - m2[2];
	return gMatrix3(v1, v2, v3);
}

gMatrix3
operator*(const gMatrix3& m, float c)
{
	gVector3 v1 = m[0] * c;
	gVector3 v2 = m[1] * c;
	gVector3 v3 = m[2] * c;
	return gMatrix3(v1, v2, v3);
}

gMatrix3
operator*(float c, const gMatrix3& m)
{
	gVector3 v1 = m[0] * c;
	gVector3 v2 = m[1] * c;
	gVector3 v3 = m[2] * c;
	return gMatrix3(v1, v2, v3);
}

gMatrix3
operator/(const gMatrix3& m, float c)
{
	gVector3 v1 = m[0] / c;
	gVector3 v2 = m[1] / c;
	gVector3 v3 = m[2] / c;
	return gMatrix3(v1, v2, v3);
}

gMatrix3
operator*(const gMatrix3& m1, const gMatrix3& m2)
{
	gVector3 v1 = gVector3(m1.getColumn(0)*m2[0], m1.getColumn(1)*m2[0], m1.getColumn(2)*m2[0]);
	gVector3 v2 = gVector3(m1.getColumn(0)*m2[1], m1.getColumn(1)*m2[1], m1.getColumn(2)*m2[1]);
	gVector3 v3 = gVector3(m1.getColumn(0)*m2[2], m1.getColumn(1)*m2[2], m1.getColumn(2)*m2[2]);
	return gMatrix3(v1, v2, v3);
}

gVector3
operator*(const gMatrix3& m, const gVector3& v)
{
	gVector3 x = gVector3(m[0] * v, m[1] * v, m[2] * v);
	return x;
}

gVector3
operator*(const gVector3& v, const gMatrix3& m)
{
	gVector3 x = gVector3(m.getColumn(0)* v, m.getColumn(1)* v, m.getColumn(2) * v);
	return x;
}

gMatrix3
gMatrix3::identity()
{
	gMatrix3 m = gMatrix3();
	return m;
}

gMatrix3
gMatrix3::translation2D(float x, float y)
{
	gMatrix3 t = identity();
	t[0][2] = x;
	t[1][2] = y;
	return t;
}

gMatrix3
gMatrix3::scale2D(float x, float y)
{
	gMatrix3 scalar = identity();
	scalar[0][0] = x;
	scalar[1][1] = y;
	return scalar;
}

gMatrix3
gMatrix3::rotation2D(float angle)
{
	gMatrix3 rot = identity();
	float epsilon = 0.000001f;
	angle = angle*PI / 180;
	rot[0][0] = cos(angle);
	rot[0][1] = -sin(angle);
	rot[1][0] = sin(angle);
	rot[1][1] = cos(angle);
	return rot;
}