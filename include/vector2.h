#ifndef __VECTOR_2__
#define __VECTOR_2__

#include <iostream>

#ifndef RAD2DEG
#define RAD2DEG(x) ((x) * 57.295754f)
#endif // !RAD2DEG

#ifndef DEG2RAD
#define DEG2RAD(x) ((x) * 0.0174533f)
#endif // !DEG2RAD

struct Vector2 {

	union {
		struct {
			float x, y;
		};
		float asArray[2];
	};


	Vector2() : x(0.0f), y(0.0f) { }
	Vector2(float _x, float _y) : x(_x), y(_y) { }

	~Vector2() { }

	float& operator[] (const int i) {
		return asArray[i];
	}

	/**
	* @brief Get a Vector2 with all components set to 0
	* @return Vector2(0, 0, 0)
	*/
	static Vector2 zero() { return Vector2(); }

	/**
	* @brief Gets a Vector2 with all components set to 1
	* @return Vector2(1, 1, 1)
	*/
	static Vector2 one() { return Vector2(1, 1); }

	/**
	* @brief Gets a Vector2 with direction to the right length 1
	* @return Vector2(1, 0, 0)
	*/
	static Vector2 right() { return Vector2(1, 0); }

	/**
	* @brief Gets a Vector2 with direction to the left length 1
	* @return Vector2(-1, 0, 0)
	*/
	static Vector2 left() { return Vector2(-1, 0); }

	/**
	* @brief Gets a Vector2 with direction to the upward length 1
	* @return Vector2(0, 1, 0)
	*/
	static Vector2 up() { return Vector2(0, 1); }

	/**
	* @brief Gets a Vector2 with direction to the downward length 1
	* @return Vector2(0, -1, 0)
	*/
	static Vector2 down() { return Vector2(0, -1); }

	/**
	* @brief Gets a Vector2 with direction to the forward length 1
	* @return Vector2(0, 0, 1)
	*/
	static Vector2 forward() { return Vector2(1, 0); }

	/**
	* @brief Gets a Vector2 with direction to the backward length 1
	* @return Vector2(0, 0, -1)
	*/
	static Vector2 back() { return Vector2(1, 0); }

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
	Vector2 normalized();
};

/**
* @brief Dot product of two Vector2
* @param l - Vector2
* @param r - Vector2
* @return float calculated value from the dot product
*/
float Dot(const Vector2& l, const Vector2& r);

/**
* @brief Gets the length of this vector
* @param vec - The vector to get the length of
* @return The vector's length
*/
float Magnitude(const Vector2& vec);

/**
* @brief Gets the length squared of this vector
* @param vec - The vector to get the length sqaured of
* @return The vector's length sqaured
*/
float Magnitudesq(const Vector2& vec);

/**
* @brief Gets the distance between two vectors
* @param l - Position of point 1
* @param r - Position of point 2
* @return float distance
*/
float Distance(const Vector2& l, const Vector2& r);

/**
* @brief Changes the vector to a length of 1
* @param vec - The vector to normalize
*/
void Normalize(Vector2& vec);

/**
* @brief Gets the angle between two vectors
* @param l - Position of point 1
* @param r - Position of point 2
* @return Vector2 angle
*/
float Angle(const Vector2& l, const Vector2& r);

/**
* @brief Projects a length vector to get length parallel to another vector
* @param len - Vector length
* @param dir - Vector Direction
* @return Vector2 length in direction of vec2
*/
Vector2 Project(const Vector2& len, const Vector2& dir);

/**
* @brief Projects a length vector to get length perpendicular to another vector
* @param len - Vector to project
* @param dir - Vector to be projected on
* @return Vector2 length in perpendicular direction
*/
Vector2 Perpendicular(const Vector2& len, const Vector2& dir);

/**
* @brief Bounces a vector on a normal vector
* @param vec - Vector to reflect
* @param normal - Surface normal
* @return Vector2 the reflected vector
*/
Vector2 Reflection(const Vector2& vec, const Vector2& normal);

Vector2 operator+ (const Vector2& l, const Vector2& r);
Vector2 operator- (const Vector2& l, const Vector2& r);
Vector2 operator* (const Vector2& l, const Vector2& r);
Vector2 operator* (const Vector2& l, const float r);
Vector2 operator/ (const Vector2& l, const Vector2& r);
Vector2 operator/ (const Vector2& l, const float r);

bool operator==(const Vector2& l, const Vector2& r);
bool operator!=(const Vector2& l, const Vector2& r);
std::ostream& operator<<(std::ostream& os, const Vector2& vec);

#endif // !__VECTOR_3D__
