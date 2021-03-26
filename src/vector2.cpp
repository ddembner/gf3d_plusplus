#include "vector2.h"
#include <cmath>
#include <cfloat>
#define CMP(x, y) ( fabsf(x - y)) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y)))
#define ADD(x, y) (x + y)



float Dot(const Vector2& l, const Vector2& r) {

	return l.x * r.x + l.y * r.y;
}

float Vector2::magnitude() {

	return sqrtf(Dot(*this, *this));
}

float Vector2::magnitudesq() {

	return Dot(*this, *this);
}

float Magnitude(const Vector2& vec) {

	return sqrtf(Dot(vec, vec));
}

float Magnitudesq(const Vector2& vec) {

	return Dot(vec, vec);
}

float Distance(const Vector2& l, const Vector2& r) {

	Vector2 vec = l - r;
	return Magnitude(vec);
}

Vector2 Vector2::normalized() {
	return *this * (1.0f / Magnitude(*this));
}

void Normalize(Vector2& vec) {
	vec = vec * (1.0f / Magnitude(vec));
}

float Angle(const Vector2& l, const Vector2& r) {

	float magnitude = sqrtf(Magnitudesq(l) * Magnitudesq(r));
	return acos(Dot(l, r) / magnitude);
}

Vector2 Project(const Vector2& len, const Vector2& dir) {

	float dot = Dot(len, dir);
	float magnitudeSq = Magnitudesq(dir);
	return dir * (dot / magnitudeSq);
}

Vector2 Perpendicular(const Vector2& len, const Vector2& dir) {

	return len - Project(len, dir);
}

Vector2 Reflection(const Vector2& vec, const Vector2& normal) {

	float dot = Dot(vec, normal);
	return vec - normal * (dot * 2.0f);
}

Vector2 operator+ (const Vector2& l, const Vector2& r) {

	Vector2 vec;
	vec.x = l.x + r.x;
	vec.y = l.y + r.y;
	return vec;
}

Vector2 operator- (const Vector2& l, const Vector2& r) {

	Vector2 vec;
	vec.x = l.x - r.x;
	vec.y = l.y - r.y;
	return vec;
}

Vector2 operator* (const Vector2& l, const Vector2& r) {

	Vector2 vec;
	vec.x = l.x * r.x;
	vec.y = l.y * r.y;
	return vec;
}

Vector2 operator* (const Vector2& l, const float r) {

	Vector2 vec;
	vec.x = l.x * r;
	vec.y = l.y * r;
	return vec;
}

Vector2 operator/ (const Vector2& l, const Vector2& r) {

	Vector2 vec;
	vec.x = l.x / r.x;
	vec.y = l.y / r.y;
	return vec;
}

Vector2 operator/ (const Vector2& l, const float r) {

	Vector2 vec;
	vec.x = l.x / r;
	vec.y = l.y / r;
	return vec;
}

bool operator==(const Vector2& l, const Vector2& r) {

	return  (CMP(l.x, r.x) && CMP(l.y, r.y));
}

bool operator!=(const Vector2& l, const Vector2& r) {

	return !(l == r);
}

std::ostream& operator<<(std::ostream& os, const Vector2& vec) {

	os << '(' << vec.x << ", " << vec.y << ')';
	return os;
}