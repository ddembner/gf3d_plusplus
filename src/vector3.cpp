#include "vector3.h"
#include <cmath>
#include <cfloat>
#define CMP(x, y) ( fabsf(x - y)) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y)))
#define ADD(x, y) (x + y)



float Dot(const Vector3& l, const Vector3& r) {

	return l.x * r.x + l.y * r.y + l.z * r.z;
}

float Vector3::magnitude() {

	return sqrtf(Dot(*this, *this));
}

float Vector3::magnitudesq() {

	return Dot(*this, *this);
}

float Magnitude(const Vector3& vec) {

	return sqrtf(Dot(vec, vec));
}

float Magnitudesq(const Vector3& vec) {

	return Dot(vec, vec);
}

float Distance(const Vector3& l, const Vector3& r) {

	Vector3 vec = l - r;
	return Magnitude(vec);
}

Vector3 Vector3::normalized() {
	return *this * (1.0f / Magnitude(*this));
}

void Normalize(Vector3& vec) {
	vec = vec * (1.0f / Magnitude(vec));
}

Vector3 Cross(const Vector3& l, const Vector3& r) {
	Vector3 result;
	result.x = l.y * r.z - l.z * r.y;
	result.y = l.z * r.x - l.x * r.z;
	result.z = l.x * r.y - l.y * r.x;
	return result;
}

float Angle(const Vector3& l, const Vector3& r) {

	float magnitude = sqrtf(Magnitudesq(l) * Magnitudesq(r));
	return acos(Dot(l, r) / magnitude);
}

Vector3 Project(const Vector3& len, const Vector3& dir) {

	float dot = Dot(len, dir);
	float magnitudeSq = Magnitudesq(dir);
	return dir * (dot / magnitudeSq);
}

Vector3 Perpendicular(const Vector3& len, const Vector3& dir) {

	return len - Project(len, dir);
}

Vector3 Reflection(const Vector3& vec, const Vector3& normal) {

	float dot = Dot(vec, normal);
	return vec - normal * (dot * 2.0f);
}

Vector3 operator+ (const Vector3& l, const Vector3& r) {

	Vector3 vec;
	vec.x = l.x + r.x;
	vec.y = l.y + r.y;
	vec.z = l.z + r.z;
	return vec;
}

Vector3 operator- (const Vector3& l, const Vector3& r) {

	Vector3 vec;
	vec.x = l.x - r.x;
	vec.y = l.y - r.y;
	vec.z = l.z - r.z;
	return vec;
}

Vector3 operator* (const Vector3& l, const Vector3& r) {

	Vector3 vec;
	vec.x = l.x * r.x;
	vec.y = l.y * r.y;
	vec.z = l.z * r.z;
	return vec;
}

Vector3 operator* (const Vector3& l, const float r) {

	Vector3 vec;
	vec.x = l.x * r;
	vec.y = l.y * r;
	vec.z = l.z * r;
	return vec;
}

Vector3 operator/ (const Vector3& l, const Vector3& r) {

	Vector3 vec;
	vec.x = l.x / r.x;
	vec.y = l.y / r.y;
	vec.z = l.z / r.z;
	return vec;
}

Vector3 operator/ (const Vector3& l, const float r) {

	Vector3 vec;
	vec.x = l.x / r;
	vec.y = l.y / r;
	vec.z = l.z / r;
	return vec;
}

bool operator==(const Vector3& l, const Vector3& r) {

	return  ( CMP(l.x, r.x)  && CMP(l.y, r.y)  && CMP(l.z, r.z) );
}

bool operator!=(const Vector3& l, const Vector3& r) {

	return !(l == r);
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec) {

	os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
	return os;
}