#ifndef __VECTOR_3__
#define __VECTOR_3__

#include <iostream>

#ifndef RAD2DEG
#define RAD2DEG(x) ((x) * 57.295754f)
#endif // !RAD2DEG

#ifndef DEG2RAD
#define DEG2RAD(x) ((x) * 0.0174533f)
#endif // !DEG2RAD

struct Vector3 {

	union {
		struct {
			float x, y, z;
		};
		float asArray[3];
	};
	
	
	Vector3() : x(0.0f), y(0.0f), z(0.0f) { }
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

	~Vector3() { }

	float& operator[] (const int i) {
		return asArray[i];
	}

	/**
	* @brief Get a vector3 with all components set to 0
	* @return Vector3(0, 0, 0)
	*/
	static Vector3 zero() { return Vector3(); }

	/**
	* @brief Gets a vector3 with all components set to 1
	* @return Vector3(1, 1, 1)
	*/
	static Vector3 one() { return Vector3(1, 1, 1); }

	/**
	* @brief Gets a vector3 with direction to the right length 1
	* @return Vector3(1, 0, 0)
	*/
	static Vector3 right() { return Vector3(1, 0, 0); }

	/**
	* @brief Gets a vector3 with direction to the left length 1
	* @return Vector3(-1, 0, 0)
	*/
	static Vector3 left() { return Vector3(-1, 0, 0); }

	/**
	* @brief Gets a vector3 with direction to the upward length 1
	* @return Vector3(0, 1, 0)
	*/
	static Vector3 up() { return Vector3(0, 1, 0); }

	/**
	* @brief Gets a vector3 with direction to the downward length 1
	* @return Vector3(0, -1, 0)
	*/
	static Vector3 down() { return Vector3(0, -1, 0); }

	/**
	* @brief Gets a vector3 with direction to the forward length 1
	* @return Vector3(0, 0, 1)
	*/
	static Vector3 forward() { return Vector3(1, 0, 0); }

	/**
	* @brief Gets a vector3 with direction to the backward length 1
	* @return Vector3(0, 0, -1)
	*/
	static Vector3 back() { return Vector3(1, 0, 0); }

	/**
	* @brief Gets the length of this vector
	* @return The vector's length
	*/
	float magnitude();

	/**
	* @brief Gets the length squared of this vector
	* @return The vector's length sqaured
	*/
	float magnitudesq();

	/**
	* @brief Gets the vector with a length of 1
	* @return The vector's length normalized
	*/
	Vector3 normalized();
};

/**
* @brief Dot product of two Vector3
* @param l - Vector3
* @param r - Vector3
* @return float calculated value from the dot product
*/
float Dot(const Vector3& l, const Vector3& r);

/**
* @brief Gets the length of this vector
* @param vec - The vector to get the length of
* @return The vector's length
*/
float Magnitude(const Vector3& vec);

/**
* @brief Gets the length squared of this vector
* @param vec - The vector to get the length sqaured of
* @return The vector's length sqaured
*/
float Magnitudesq(const Vector3& vec);

/**
* @brief Gets the distance between two vectors
* @param l - Position of point 1
* @param r - Position of point 2
* @return float distance
*/
float Distance(const Vector3& l, const Vector3& r);

/**
* @brief Changes the vector to a length of 1
* @param vec - The vector to normalize
*/
void Normalize(Vector3& vec);

/**
* @brief Gets the cross product of two vectors
* @param l - Position of point 1
* @param r - Position of point 2
* @return Vector3 result
*/
Vector3 Cross(const Vector3& l, const Vector3& r);

/**
* @brief Gets the angle between two vectors
* @param l - Position of point 1
* @param r - Position of point 2
* @return Vector3 angle
*/
float Angle(const Vector3& l, const Vector3& r);

/**
* @brief Projects a length vector to get length parallel to another vector
* @param len - Vector length
* @param dir - Vector Direction
* @return Vector3 length in direction of vec2
*/
Vector3 Project(const Vector3& len, const Vector3& dir);

/**
* @brief Projects a length vector to get length perpendicular to another vector
* @param len - Vector to project
* @param dir - Vector to be projected on
* @return Vector3 length in perpendicular direction
*/
Vector3 Perpendicular(const Vector3& len, const Vector3& dir);

/**
* @brief Bounces a vector on a normal vector
* @param vec - Vector to reflect
* @param normal - Surface normal
* @return Vector3 the reflected vector
*/
Vector3 Reflection(const Vector3& vec, const Vector3& normal);

Vector3 operator+ (const Vector3& l, const Vector3& r);
Vector3 operator- (const Vector3& l, const Vector3& r);
Vector3 operator* (const Vector3& l, const Vector3& r);
Vector3 operator* (const Vector3& l, const float r);
Vector3 operator/ (const Vector3& l, const Vector3& r);
Vector3 operator/ (const Vector3& l, const float r);

bool operator==(const Vector3& l, const Vector3& r);
bool operator!=(const Vector3& l, const Vector3& r);
std::ostream& operator<<(std::ostream& os, const Vector3& vec);

#endif // !__VECTOR_3D__
