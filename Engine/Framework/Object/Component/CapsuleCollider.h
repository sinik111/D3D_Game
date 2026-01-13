#pragma once

#include "Framework/Object/Component/Collider.h"
#include "Framework/Object/Component/ComponentFactory.h"

namespace engine
{
    // 캡슐 방향
    enum class CapsuleDirection
    {
        X = 0,  // X축 방향
        Y = 1,  // Y축 방향 (기본)
        Z = 2   // Z축 방향
    };

    // ═══════════════════════════════════════════════════════════════
    // CapsuleCollider - 캡슐 충돌체
    // ═══════════════════════════════════════════════════════════════

    class CapsuleCollider : public Collider
    {
        REGISTER_COMPONENT(CapsuleCollider)

    private:
        float m_radius = 0.5f;
        float m_height = 2.0f;  // 총 높이 (양쪽 반구 포함)
        CapsuleDirection m_direction = CapsuleDirection::Y;

    public:
        CapsuleCollider() = default;
        virtual ~CapsuleCollider() = default;

    public:
        float GetRadius() const { return m_radius; }
        void SetRadius(float radius);

        float GetHeight() const { return m_height; }
        void SetHeight(float height);

        CapsuleDirection GetDirection() const { return m_direction; }
        void SetDirection(CapsuleDirection direction);

        // PhysX 스타일: 실린더 부분의 절반 높이
        float GetHalfHeight() const 
        { 
            return std::max(0.0f, (m_height - m_radius * 2.0f) * 0.5f); 
        }

    protected:
        physx::PxGeometry* CreateGeometry() override;
        void UpdateGeometry() override;

        // Direction과 Rotation을 조합하기 위해 오버라이드
        void UpdateLocalPose();

    public:
        void OnGui() override;
        void Save(json& j) const override;
        void Load(const json& j) override;
        std::string GetType() const override { return "CapsuleCollider"; }

    private:
        physx::PxQuat GetDirectionRotation() const;
    };
}
