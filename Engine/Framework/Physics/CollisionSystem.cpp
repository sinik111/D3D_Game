#include "EnginePCH.h"
#include "CollisionSystem.h"

#include "Framework/Object/Component/Collider.h"
#include "Framework/Object/Component/Rigidbody.h"
#include "Framework/Object/Component/Script.h"
#include "Framework/Object/GameObject/GameObject.h"

namespace engine
{
    // ═══════════════════════════════════════════════════════════════
    // 이벤트 큐잉
    // ═══════════════════════════════════════════════════════════════

    void CollisionSystem::QueueCollisionEvent(const CollisionEvent& event)
    {
        m_pendingCollisionEvents.push_back(event);
    }

    void CollisionSystem::QueueTriggerEvent(const TriggerEvent& event)
    {
        m_pendingTriggerEvents.push_back(event);
    }

    // ═══════════════════════════════════════════════════════════════
    // 이벤트 처리
    // ═══════════════════════════════════════════════════════════════

    void CollisionSystem::ProcessEvents()
    {
        ProcessCollisionEvents();
        ProcessTriggerEvents();
    }

    void CollisionSystem::ProcessCollisionEvents()
    {
        if (m_pendingCollisionEvents.empty())
        {
            return;
        }

        // 우선순위로 정렬 (높은 것 먼저)
        std::sort(m_pendingCollisionEvents.begin(), m_pendingCollisionEvents.end(),
            [](const CollisionEvent& a, const CollisionEvent& b)
            {
                return static_cast<int32_t>(a.priority) > static_cast<int32_t>(b.priority);
            });

        // 순서대로 처리
        for (CollisionEvent& event : m_pendingCollisionEvents)
        {
            if (!event.colliderA || !event.colliderB)
            {
                continue;
            }

            // 우선순위 시스템: 공격 인스턴스 체크
            if (event.sourceId != 0)
            {
                AttackInstance* attack = GetAttack(event.sourceId);
                if (attack)
                {
                    // 이미 더 높은 우선순위에 의해 소비됨?
                    if (!attack->CanProcessWith(event.priority))
                    {
                        continue;
                    }

                    // 이 대상을 이미 처리함?
                    GameObject* targetA = event.colliderA->GetGameObject();
                    GameObject* targetB = event.colliderB->GetGameObject();
                    
                    if (attack->HasHit(targetA) || attack->HasHit(targetB))
                    {
                        continue;
                    }
                }
            }

            // 이벤트 타입에 따라 디스패치
            switch (event.type)
            {
            case CollisionEventType::Enter:
                DispatchCollisionEnter(event.colliderA, event.colliderB, event.contacts);
                break;

            case CollisionEventType::Stay:
                DispatchCollisionStay(event.colliderA, event.colliderB, event.contacts);
                break;

            case CollisionEventType::Exit:
                DispatchCollisionExit(event.colliderA, event.colliderB);
                break;
            }

            // 공격 인스턴스에 히트 기록
            if (event.sourceId != 0)
            {
                AttackInstance* attack = GetAttack(event.sourceId);
                if (attack)
                {
                    attack->RecordHit(event.colliderA->GetGameObject());
                    attack->RecordHit(event.colliderB->GetGameObject());

                    // 높은 우선순위(패링 등)면 공격 소비
                    if (event.priority >= CollisionPriority::Block)
                    {
                        attack->Consume(event.priority);
                    }
                }
            }
        }

        m_pendingCollisionEvents.clear();
    }

    void CollisionSystem::ProcessTriggerEvents()
    {
        // Enter/Exit 이벤트 처리
        for (const TriggerEvent& event : m_pendingTriggerEvents)
        {
            if (!event.trigger || !event.other)
            {
                continue;
            }

            if (event.type == TriggerEventType::Enter)
            {
                m_activeTriggerPairs.insert({ event.trigger, event.other });
                DispatchTriggerEnter(event.trigger, event.other);
            }
            else if (event.type == TriggerEventType::Exit)
            {
                m_activeTriggerPairs.erase({ event.trigger, event.other });
                DispatchTriggerExit(event.trigger, event.other);
            }
        }

        m_pendingTriggerEvents.clear();

        // Stay 이벤트 발생 (매 프레임 활성 쌍에 대해)
        for (const TriggerPair& pair : m_activeTriggerPairs)
        {
            if (pair.trigger && pair.other)
            {
                DispatchTriggerStay(pair.trigger, pair.other);
            }
        }
    }

    // ═══════════════════════════════════════════════════════════════
    // 공격 인스턴스 관리
    // ═══════════════════════════════════════════════════════════════

    uint64_t CollisionSystem::CreateAttack(GameObject* attacker)
    {
        uint64_t id = m_nextAttackId++;
        m_activeAttacks.emplace(id, AttackInstance(id, attacker));
        return id;
    }

    void CollisionSystem::EndAttack(uint64_t attackId)
    {
        m_activeAttacks.erase(attackId);
    }

    AttackInstance* CollisionSystem::GetAttack(uint64_t attackId)
    {
        auto it = m_activeAttacks.find(attackId);
        if (it != m_activeAttacks.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    // ═══════════════════════════════════════════════════════════════
    // Collider 정리
    // ═══════════════════════════════════════════════════════════════

    void CollisionSystem::OnColliderDestroyed(Collider* collider)
    {
        // Trigger 쌍에서 제거
        for (auto it = m_activeTriggerPairs.begin(); it != m_activeTriggerPairs.end(); )
        {
            if (it->trigger == collider || it->other == collider)
            {
                it = m_activeTriggerPairs.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // 대기 중인 이벤트에서 제거
        m_pendingCollisionEvents.erase(
            std::remove_if(m_pendingCollisionEvents.begin(), m_pendingCollisionEvents.end(),
                [collider](const CollisionEvent& e)
                {
                    return e.colliderA == collider || e.colliderB == collider;
                }),
            m_pendingCollisionEvents.end()
        );

        m_pendingTriggerEvents.erase(
            std::remove_if(m_pendingTriggerEvents.begin(), m_pendingTriggerEvents.end(),
                [collider](const TriggerEvent& e)
                {
                    return e.trigger == collider || e.other == collider;
                }),
            m_pendingTriggerEvents.end()
        );
    }

    void CollisionSystem::ClearPendingEvents()
    {
        m_pendingCollisionEvents.clear();
        m_pendingTriggerEvents.clear();
    }

    // ═══════════════════════════════════════════════════════════════
    // 콜백 디스패치
    // ═══════════════════════════════════════════════════════════════

    void CollisionSystem::DispatchCollisionEnter(
        Collider* a, Collider* b, const std::vector<ContactPoint>& contacts)
    {
        if (!a || !b) return;

        GameObject* goA = a->GetGameObject();
        GameObject* goB = b->GetGameObject();

        if (!goA || !goB) return;

        // A에게 전달할 정보
        CollisionInfo infoForA;
        infoForA.collider = b;
        infoForA.rigidbody = b->GetAttachedRigidbody();
        infoForA.gameObject = goB;
        infoForA.contacts = contacts;

        // B에게 전달할 정보 (노말 반전)
        CollisionInfo infoForB;
        infoForB.collider = a;
        infoForB.rigidbody = a->GetAttachedRigidbody();
        infoForB.gameObject = goA;
        infoForB.contacts = FlipContactNormals(contacts);

        // A의 모든 Script에 알림
        // TODO: Script 시스템과 연동 필요
        // 현재는 주석 처리된 OnCollisionEnter가 활성화되면 호출

        // B의 모든 Script에 알림
    }

    void CollisionSystem::DispatchCollisionStay(
        Collider* a, Collider* b, const std::vector<ContactPoint>& contacts)
    {
        // Enter와 유사하게 구현
    }

    void CollisionSystem::DispatchCollisionExit(Collider* a, Collider* b)
    {
        if (!a || !b) return;

        GameObject* goA = a->GetGameObject();
        GameObject* goB = b->GetGameObject();

        if (!goA || !goB) return;

        CollisionInfo infoForA;
        infoForA.collider = b;
        infoForA.rigidbody = b->GetAttachedRigidbody();
        infoForA.gameObject = goB;

        CollisionInfo infoForB;
        infoForB.collider = a;
        infoForB.rigidbody = a->GetAttachedRigidbody();
        infoForB.gameObject = goA;

        // Script 콜백 호출
    }

    void CollisionSystem::DispatchTriggerEnter(Collider* trigger, Collider* other)
    {
        if (!trigger || !other) return;

        // Trigger 측 Script들에 알림
        // Other 측 Script들에도 알림
    }

    void CollisionSystem::DispatchTriggerStay(Collider* trigger, Collider* other)
    {
        // Enter와 유사
    }

    void CollisionSystem::DispatchTriggerExit(Collider* trigger, Collider* other)
    {
        // Enter와 유사
    }

    std::vector<ContactPoint> CollisionSystem::FlipContactNormals(
        const std::vector<ContactPoint>& contacts)
    {
        std::vector<ContactPoint> flipped = contacts;
        for (ContactPoint& cp : flipped)
        {
            cp.normal = -cp.normal;
        }
        return flipped;
    }
}
