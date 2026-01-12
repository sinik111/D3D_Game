#pragma once

#include <PxPhysicsAPI.h>

namespace engine
{
    class Transform;

    // ═══════════════════════════════════════════════════════════════
    // 좌표계 변환 유틸리티
    // 
    // DirectX: 왼손 좌표계 (Z+ 전방, Y+ 위)
    // PhysX:   오른손 좌표계 (Z- 전방, Y+ 위)
    // 
    // 변환: Z축 부호 반전
    // ═══════════════════════════════════════════════════════════════

    namespace PhysicsUtility
    {
        // ═══════════════════════════════════════
        // Vector3 변환
        // ═══════════════════════════════════════

        inline physx::PxVec3 ToPxVec3(const Vector3& v)
        {
            return physx::PxVec3(v.x, v.y, -v.z);  // Z 반전
        }

        inline Vector3 ToVector3(const physx::PxVec3& v)
        {
            return Vector3(v.x, v.y, -v.z);  // Z 반전
        }

        // 방향 벡터 변환 (위치와 동일하게 Z 반전)
        inline physx::PxVec3 ToPxDirection(const Vector3& dir)
        {
            return physx::PxVec3(dir.x, dir.y, -dir.z);
        }

        inline Vector3 ToDirection(const physx::PxVec3& dir)
        {
            return Vector3(dir.x, dir.y, -dir.z);
        }

        // ═══════════════════════════════════════
        // Quaternion 변환
        // ═══════════════════════════════════════

        inline physx::PxQuat ToPxQuat(const Quaternion& q)
        {
            // 좌표계 변환 시 회전축의 Z가 반전되므로
            // Quaternion의 x, y 성분 부호 반전
            return physx::PxQuat(-q.x, -q.y, q.z, q.w);
        }

        inline Quaternion ToQuaternion(const physx::PxQuat& q)
        {
            return Quaternion(-q.x, -q.y, q.z, q.w);
        }

        // ═══════════════════════════════════════
        // Transform 변환
        // ═══════════════════════════════════════

        inline physx::PxTransform ToPxTransform(const Vector3& position, const Quaternion& rotation)
        {
            return physx::PxTransform(ToPxVec3(position), ToPxQuat(rotation));
        }

        physx::PxTransform ToPxTransform(const Transform* transform);

        inline void FromPxTransform(const physx::PxTransform& pxTransform, Vector3& outPosition, Quaternion& outRotation)
        {
            outPosition = ToVector3(pxTransform.p);
            outRotation = ToQuaternion(pxTransform.q);
        }

        void ApplyPxTransformToTransform(const physx::PxTransform& pxTransform, Transform* transform);

        // ═══════════════════════════════════════
        // Extended Vector (CharacterController용)
        // ═══════════════════════════════════════

        inline physx::PxExtendedVec3 ToPxExtendedVec3(const Vector3& v)
        {
            return physx::PxExtendedVec3(
                static_cast<physx::PxReal>(v.x),
                static_cast<physx::PxReal>(v.y),
                static_cast<physx::PxReal>(-v.z)
            );
        }

        inline Vector3 FromPxExtendedVec3(const physx::PxExtendedVec3& v)
        {
            return Vector3(
                static_cast<float>(v.x),
                static_cast<float>(v.y),
                static_cast<float>(-v.z)
            );
        }

        // ═══════════════════════════════════════
        // Scale 변환 (Z 반전 없음, 크기는 동일)
        // ═══════════════════════════════════════

        inline physx::PxVec3 ToPxScale(const Vector3& scale)
        {
            return physx::PxVec3(scale.x, scale.y, scale.z);
        }

        inline Vector3 ToScale(const physx::PxVec3& scale)
        {
            return Vector3(scale.x, scale.y, scale.z);
        }

        // ═══════════════════════════════════════
        // ForceMode 변환
        // ═══════════════════════════════════════

        inline physx::PxForceMode::Enum ToPxForceMode(int mode)
        {
            // 0: Force, 1: Impulse, 2: VelocityChange, 3: Acceleration
            switch (mode)
            {
            case 0: return physx::PxForceMode::eFORCE;
            case 1: return physx::PxForceMode::eIMPULSE;
            case 2: return physx::PxForceMode::eVELOCITY_CHANGE;
            case 3: return physx::PxForceMode::eACCELERATION;
            default: return physx::PxForceMode::eFORCE;
            }
        }
    }
}
