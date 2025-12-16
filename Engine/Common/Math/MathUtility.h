#pragma once

namespace engine
{
	float ToRadian(float degree);
	float ToDegree(float radian);
	Vector3 ToEulerAngles(const Quaternion& q);
}
