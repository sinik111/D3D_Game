#pragma once

#include "Common/Utility/Singleton.h"
#include <vector>

namespace engine
{
    class Collider;
    class BoxCollider;
    class SphereCollider;
    class CapsuleCollider;
    class CharacterController;

    // ═══════════════════════════════════════════════════════════════
    // PhysicsDebugRenderer - 물리 충돌체 디버그 시각화
    // 
    // 기본값: OFF (m_enabled = false)
    // 활성화: PhysicsDebugRenderer::Get().SetEnabled(true)
    // ═══════════════════════════════════════════════════════════════

    class PhysicsDebugRenderer : public Singleton<PhysicsDebugRenderer>
    {
    private:
        bool m_enabled = false;  // 기본 OFF

        // 색상
        Vector4 m_colliderColor{ 0.0f, 1.0f, 0.0f, 1.0f };      // 초록
        Vector4 m_triggerColor{ 0.0f, 0.5f, 1.0f, 1.0f };       // 파랑
        Vector4 m_controllerColor{ 1.0f, 1.0f, 0.0f, 1.0f };    // 노랑
        Vector4 m_activeColor{ 1.0f, 0.5f, 0.0f, 1.0f };        // 주황 (활성 충돌)

    private:
        PhysicsDebugRenderer() = default;
        ~PhysicsDebugRenderer() = default;

    public:
        // 활성화/비활성화
        void SetEnabled(bool enabled) { m_enabled = enabled; }
        bool IsEnabled() const { return m_enabled; }

        // 색상 설정
        void SetColliderColor(const Vector4& color) { m_colliderColor = color; }
        void SetTriggerColor(const Vector4& color) { m_triggerColor = color; }
        void SetControllerColor(const Vector4& color) { m_controllerColor = color; }

        // 렌더링 (RenderSystem에서 호출)
        void Render();

    private:
        void RenderColliders();
        void RenderControllers();

        void DrawBox(const Vector3& center, const Vector3& halfExtents, 
                     const Quaternion& rotation, const Vector4& color);
        void DrawSphere(const Vector3& center, float radius, const Vector4& color);
        void DrawCapsule(const Vector3& center, float radius, float height, 
                        const Quaternion& rotation, const Vector4& color);
        void DrawWireframe(const std::vector<Vector3>& vertices, 
                          const std::vector<uint32_t>& indices, const Vector4& color);

        friend class Singleton<PhysicsDebugRenderer>;
    };
}
