#include "gVector3.h"

gVector3::gVector3()
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
}

gVector3::gVector3(float x, float y, float z)
{
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

gVector3::gVector3(std::string str)
{
	std::string s = str;
	std::string delimiter = ",";
	size_t pos = 0;
	std::string token;

	// X Value
	pos = s.find(delimiter);
	token = s.substr(0, pos);
	data[0] = stof(token);
	s.erase(0, pos + delimiter.length());

	// Y Value
	pos = s.find(delimiter);
	token = s.substr(0, pos);
	data[1] = stof(token);
	s.erase(0, pos + delimiter.length());

	// Z Value
	if ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		data[2] = stof(token);
		s.erase(0, pos + delimiter.length());
	}
	else
		data[2] = 1;
}

float
gVector3::operator[](unsigned int index) const
{
	return data[index];
}

float&
gVector3::operator[](unsigned int index)
{
	return data[index];
}

void
gVector3::print(){
	std::cout << "The vector is: [" << data[0] << ", " << data[1] << ", " << data[2] << "]" << std::endl;
}

float
gVector3::length() const
{
	float length;
	length = sqrt(pow(data[0], 2) + pow(data[1], 2) + pow(data[2], 2));
	return length;
}

bool
operator==(const gVector3& v1, const gVector3& v2)
{
	float epsilon = 0.000001f;
	float temp = abs(v1[0] - v2[0]);
	if (temp < epsilon)
	{
		temp = abs(v1[1] - v2[1]);
		if (temp < epsilon)
		{
			temp = abs(v1[2] - v2[2]);
			if (temp < epsilon)
			{
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

bool
operator!=(const gVector3& v1, const gVector3& v2)
{
	if (v1 == v2)
		return false;
	else
		return true;
}

gVector3
operator+(const gVector3& v1, const gVector3& v2)
{
	return gVector3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

gVector3
operator-(const gVector3& v1, const gVector3& v2)
{
	return gVector3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

gVector3
operator*(const gVector3& v, float c)
{
	return gVector3(v[0] * c, v[1] * c, v[2] * c);
}

gVector3
operator*(float c, const gVector3& v)
{
	return gVector3(v[0] * c, v[1] * c, v[2] * c);
}

gVector3
operator/(const gVector3& v, float c)
{
	return gVector3(v[0] / c, v[1] / c, v[2] / c);
}

float
operator*(const gVector3& v1, const gVector3& v2)
{
	float dot = (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
	return dot;
}

gVector3
operator%(const gVector3& v1, const gVector3& v2)
{
	gVector3 cross = gVector3();
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return cross;
}

gVector3
gVector3::nullVector()
{
	gVector3 v = gVector3(-1, -1, -1);
	return v;
}