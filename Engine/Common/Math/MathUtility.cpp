#include "pch.h"
#include "MathUtility.h"

namespace engine
{
    Vector3 ToEulerAngles(const Quaternion& q)
    {
        Vector3 angles;
        
        double sinrCosp = 2 * (q.w * q.x + q.y * q.z);
        double cosrCosp = 1 - 2 * (q.x * q.x + q.y * q.y);
        angles.x = static_cast<float>(std::atan2(sinrCosp, cosrCosp));
        
        double sinp = 2 * (q.w * q.y - q.z * q.x);
        if (std::abs(sinp) >= 1)
        {
            angles.y = static_cast<float>(std::copysign(DirectX::XM_PI / 2, sinp));
        }
        else
        {
            angles.y = static_cast<float>(std::asin(sinp));
        }

        double sinyCosp = 2 * (q.w * q.z + q.x * q.y);
        double cosyCosp = 1 - 2 * (q.y * q.y + q.z * q.z);
        angles.z = static_cast<float>(std::atan2(sinyCosp, cosyCosp));

        return angles;
    }
}