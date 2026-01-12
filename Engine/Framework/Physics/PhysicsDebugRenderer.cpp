#include "EnginePCH.h"
#include "PhysicsDebugRenderer.h"

#include "Framework/Physics/PhysicsSystem.h"
#include "Framework/Object/Component/Collider.h"
#include "Framework/Object/Component/BoxCollider.h"
#include "Framework/Object/Component/SphereCollider.h"
#include "Framework/Object/Component/CapsuleCollider.h"
#include "Framework/Object/Component/CharacterController.h"
#include "Framework/Object/Component/Transform.h"
#include "Framework/Scene/SceneManager.h"

namespace engine
{
    void PhysicsDebugRenderer::Render()
    {
        if (!m_enabled)
        {
            return;
        }

        RenderColliders();
        RenderControllers();
    }

    void PhysicsDebugRenderer::RenderColliders()
    {
        // TODO: 실제 렌더링 시스템과 연동
        // 현재는 구조만 작성
        
        /*
        Scene* scene = SceneManager::Get().GetScene();
        if (!scene) return;

        // PhysicsSystem에서 콜라이더 목록 가져오기
        // 각 콜라이더 타입에 맞게 와이어프레임 그리기
        
        for (Collider* collider : colliders)
        {
            Vector4 color = collider->IsTrigger() ? m_triggerColor : m_colliderColor;
            
            Transform* transform = collider->GetTransform();
            Vector3 worldPos = transform->GetWorldPosition();
            Quaternion rotation = transform->GetLocalRotation();

            if (BoxCollider* box = dynamic_cast<BoxCollider*>(collider))
            {
                DrawBox(worldPos + box->GetCenter(), box->GetHalfExtents(), rotation, color);
            }
            else if (SphereCollider* sphere = dynamic_cast<SphereCollider*>(collider))
            {
                DrawSphere(worldPos + sphere->GetCenter(), sphere->GetRadius(), color);
            }
            else if (CapsuleCollider* capsule = dynamic_cast<CapsuleCollider*>(collider))
            {
                DrawCapsule(worldPos + capsule->GetCenter(), 
                           capsule->GetRadius(), capsule->GetHeight(), rotation, color);
            }
        }
        */
    }

    void PhysicsDebugRenderer::RenderControllers()
    {
        // TODO: CharacterController 시각화
        /*
        for (CharacterController* controller : controllers)
        {
            Vector3 pos = controller->GetPosition();
            float radius = controller->GetRadius();
            float height = controller->GetHeight();
            
            DrawCapsule(pos, radius, height, Quaternion::Identity, m_controllerColor);
        }
        */
    }

    void PhysicsDebugRenderer::DrawBox(
        const Vector3& center, 
        const Vector3& halfExtents, 
        const Quaternion& rotation, 
        const Vector4& color)
    {
        // TODO: 기존 렌더링 시스템의 디버그 드로우 기능 사용
        // 예: DebugDraw::DrawWireBox(center, halfExtents, rotation, color);
    }

    void PhysicsDebugRenderer::DrawSphere(
        const Vector3& center, 
        float radius, 
        const Vector4& color)
    {
        // TODO: DebugDraw::DrawWireSphere(center, radius, color);
    }

    void PhysicsDebugRenderer::DrawCapsule(
        const Vector3& center, 
        float radius, 
        float height, 
        const Quaternion& rotation, 
        const Vector4& color)
    {
        // TODO: DebugDraw::DrawWireCapsule(center, radius, height, rotation, color);
    }

    void PhysicsDebugRenderer::DrawWireframe(
        const std::vector<Vector3>& vertices, 
        const std::vector<uint32_t>& indices, 
        const Vector4& color)
    {
        // TODO: 범용 와이어프레임 렌더링
    }
}
