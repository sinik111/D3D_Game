#pragma once

namespace engine
{
	float ToRadian(float degree);

	float ToDegree(float radian);

	// radian
	Vector3 ToEulerAngles(const Quaternion& q);
}
