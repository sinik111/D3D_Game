#include "EnginePCH.h"
#include "CapsuleCollider.h"

#include "Framework/Physics/PhysicsUtility.h"

namespace engine
{
    void CapsuleCollider::SetRadius(float radius)
    {
        m_radius = std::max(0.001f, radius);
        
        // 높이가 반지름×2보다 작으면 조정
        if (m_height < m_radius * 2.0f)
        {
            m_height = m_radius * 2.0f;
        }
        
        UpdateGeometry();
    }

    void CapsuleCollider::SetHeight(float height)
    {
        // 최소 높이는 반지름×2 (양쪽 반구)
        m_height = std::max(m_radius * 2.0f, height);
        UpdateGeometry();
    }

    void CapsuleCollider::SetDirection(CapsuleDirection direction)
    {
        m_direction = direction;
        UpdateLocalPose();
    }

    physx::PxGeometry* CapsuleCollider::CreateGeometry()
    {
        // PhysX 캡슐: 기본적으로 X축 방향
        // halfHeight = 실린더 부분의 절반 높이
        return new physx::PxCapsuleGeometry(m_radius, GetHalfHeight());
    }

    void CapsuleCollider::UpdateGeometry()
    {
        if (!m_shape)
        {
            return;
        }

        physx::PxCapsuleGeometry capsule(m_radius, GetHalfHeight());
        m_shape->setGeometry(capsule);

        // 로컬 회전 갱신 (direction + rotation 조합)
        UpdateLocalPose();
    }

    physx::PxQuat CapsuleCollider::GetDirectionRotation() const
    {
        // PhysX 캡슐은 기본적으로 X축 방향
        // Y축이나 Z축으로 바꾸려면 회전 필요
        
        switch (m_direction)
        {
        case CapsuleDirection::X:
            // 기본값, 회전 없음
            return physx::PxQuat(physx::PxIdentity);
            
        case CapsuleDirection::Y:
            // X축 → Y축: Z축 기준 90도 회전
            return physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1));
            
        case CapsuleDirection::Z:
            // X축 → Z축: Y축 기준 -90도 회전
            return physx::PxQuat(-physx::PxHalfPi, physx::PxVec3(0, 1, 0));
            
        default:
            return physx::PxQuat(physx::PxIdentity);
        }
    }

    void CapsuleCollider::UpdateLocalPose()
    {
        if (!m_shape)
        {
            return;
        }

        // 사용자 회전 (오일러 각도 → 쿼터니언)
        Vector3 radians = m_rotation * (DirectX::XM_PI / 180.0f);
        Quaternion userRot = Quaternion::CreateFromYawPitchRoll(radians.y, radians.x, radians.z);

        // Direction 회전
        physx::PxQuat dirQuat = GetDirectionRotation();
        Quaternion dirRot(dirQuat.x, dirQuat.y, dirQuat.z, dirQuat.w);

        // Direction 먼저, 그 다음 사용자 회전 적용
        Quaternion finalRot = dirRot * userRot;

        physx::PxTransform localPose(
            PhysicsUtility::ToPxVec3(m_center),
            physx::PxQuat(finalRot.x, finalRot.y, finalRot.z, finalRot.w)
        );
        m_shape->setLocalPose(localPose);
    }

    void CapsuleCollider::OnGui()
    {
        Collider::OnGui();
        
        ImGui::Separator();
        
        // Radius
        float radius = m_radius;
        if (ImGui::DragFloat("Radius", &radius, 0.01f, 0.001f, 1000.0f))
        {
            SetRadius(radius);
        }
        
        // Height
        float height = m_height;
        if (ImGui::DragFloat("Height", &height, 0.01f, m_radius * 2.0f, 1000.0f))
        {
            SetHeight(height);
        }
        
        // Direction
        const char* directions[] = { "X", "Y", "Z" };
        int dir = static_cast<int>(m_direction);
        if (ImGui::Combo("Direction", &dir, directions, 3))
        {
            SetDirection(static_cast<CapsuleDirection>(dir));
        }
    }

    void CapsuleCollider::Save(json& j) const
    {
        Collider::Save(j);
        j["radius"] = m_radius;
        j["height"] = m_height;
        j["direction"] = static_cast<int>(m_direction);
    }

    void CapsuleCollider::Load(const json& j)
    {
        Collider::Load(j);
        if (j.contains("radius"))
        {
            m_radius = j["radius"].get<float>();
        }
        if (j.contains("height"))
        {
            m_height = j["height"].get<float>();
        }
        if (j.contains("direction"))
        {
            m_direction = static_cast<CapsuleDirection>(j["direction"].get<int>());
        }
    }
}
