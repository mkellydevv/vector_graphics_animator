#ifndef GMATRIX3_H
#define GMATRIX3_H

#include "gVector3.h"

class gMatrix3
{
private:
	gVector3 data[3];

public:
	gMatrix3();																	// Default matrix of all 0s
	gMatrix3(const gVector3& row1, const gVector3& row2, const gVector3& row3);	// Each vector represents a row in the matrix

	// Getters //
	// Returns the values of the row at the index
	gVector3 operator[](unsigned int index) const;

	// Returns a reference to the row at the index
	gVector3& operator[](unsigned int index);

	// Returns a column of the matrix
	gVector3 getColumn(unsigned int index) const;

	// Prints the matrix to standard output in a nice format
	void print();

	// Matrix Operations //
	// Returns the transpose of the matrix (v_ij == v_ji)
	gMatrix3 transpose() const;

	// Static Initializers //
	// Creates a 3-D rotation matrix.
	// Takes an angle in degrees and outputs a 3x3 rotation matrix
	static gMatrix3 rotation2D(float angle);

	// Takes an x and y displacement and outputs a 3x3 translation matrix
	static gMatrix3 translation2D(float x, float y);

	// Takes an x and y scale and outputs a 3x3 scale matrix
	static gMatrix3 scale2D(float x, float y);

	// Generates a 3x3 identity matrix
	static gMatrix3 identity();

	// Friend Functions //
	// Checks if m1 == m2
	friend bool     operator==(const gMatrix3& m1, const gMatrix3& m2);

	// Checks if m1 != m2
	friend bool     operator!=(const gMatrix3& m1, const gMatrix3& m2);

	// Matrix addition (m1 + m2)
	friend gMatrix3 operator+ (const gMatrix3& m1, const gMatrix3& m2);

	// Matrix subtraction (m1 - m2)
	friend gMatrix3 operator- (const gMatrix3& m1, const gMatrix3& m2);

	// Scalar multiplication (m * c)
	friend gMatrix3 operator* (const gMatrix3& m, float c);

	// Scalar multiplication (c * m)
	friend gMatrix3 operator* (float c, const gMatrix3& m);

	// Scalar division (m / c)
	friend gMatrix3 operator/ (const gMatrix3& m, float c);

	// Matrix multiplication (m1 * m2)
	friend gMatrix3 operator* (const gMatrix3& m1, const gMatrix3& m2);

	// Matrix/vector multiplication (m * v)
	// Assume v is a column vector (ie. a 3x1 matrix)
	friend gVector3 operator* (const gMatrix3& m, const gVector3& v);

	// Vector/matrix multiplication (v * m)
	// Assume v is a row vector (ie. a 1x3 matrix)
	friend gVector3 operator* (const gVector3& v, const gMatrix3& m);
};

#endif