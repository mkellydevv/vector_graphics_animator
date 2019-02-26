#ifndef GVECTOR3_H
#define GVECTOR3_H

#include <iostream>
#include <fstream>
#include <string>

class gVector3
{
private:
	float data[3];

public:
	gVector3();
	gVector3(float x, float y, float z);
	gVector3(std::string str);

	// Getters/Setters //

	// Returns the value at index
	float operator[](unsigned int index) const;

	// Returns a reference to the value at index
	float& operator[](unsigned int index);

	// Prints the vector to standard output in a nice format
	void print();

	// Vector Operations //

	// Returns the geometric length of the vector
	float length() const;

	// Friend Functions //

	// Checks if v1 == v2
	friend bool operator==(const gVector3& v1, const gVector3& v2);

	// Checks if v1 != v2
	friend bool operator!=(const gVector3& v1, const gVector3& v2);

	// Vector Addition (v1 + v2)
	friend gVector3 operator+ (const gVector3& v1, const gVector3& v2);

	// Vector Subtraction (v1 - v2)
	friend gVector3 operator- (const gVector3& v1, const gVector3& v2);

	// Scalar Multiplication (v * c)
	friend gVector3 operator* (const gVector3& v, float c);

	// Scalar Multiplication (c * v)
	friend gVector3 operator* (float c, const gVector3& v);

	// Scalar Division (v/c)
	friend gVector3 operator/ (const gVector3& v, float c);

	// Dot Product (v1 * v2)
	friend float operator* (const gVector3& v1, const gVector3& v2);

	// Cross Product (v1 % v2)
	friend gVector3 operator% (const gVector3& v1, const gVector3& v2);

	// Generates a null vector = {-1,-1,-1}
	static gVector3 nullVector();
};

#endif