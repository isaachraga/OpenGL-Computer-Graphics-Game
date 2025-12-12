#pragma once

// ========== Vector =============
struct Vector3 {
	float x;
	float y;
	float z;

	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3(float ix, float iy, float iz) {
		x = ix;
		y = iy;
		z = iz;
	}

	Vector3 operator+(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 operator*(const float& other) const {
		return Vector3(x * other, y * other, z * other);
	}
	Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
};