#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <functional>

#include "Common/Utility/Singleton.h"
#include "Framework/Physics/CollisionTypes.h"

namespace engine
{
    class Collider;
    class GameObject;

    // ═══════════════════════════════════════════════════════════════
    // 공격 인스턴스 (우선순위 시스템용)
    // 하나의 "공격"에 의한 여러 충돌을 그룹화
    // ═══════════════════════════════════════════════════════════════

    class AttackInstance
    {
    private:
        uint64_t m_id;
        GameObject* m_attacker = nullptr;
        std::unordered_set<GameObject*> m_hitTargets;
        
        bool m_isConsumed = false;
        CollisionPriority m_consumedBy = CollisionPriority::Default;

    public:
        AttackInstance(uint64_t id, GameObject* attacker)
            : m_id(id), m_attacker(attacker) {}

        uint64_t GetId() const { return m_id; }
        GameObject* GetAttacker() const { return m_attacker; }

        bool HasHit(GameObject* target) const
        {
            return m_hitTargets.find(target) != m_hitTargets.end();
        }

        void RecordHit(GameObject* target)
        {
            m_hitTargets.insert(target);
        }

        bool IsConsumed() const { return m_isConsumed; }

        void Consume(CollisionPriority by)
        {
            m_isConsumed = true;
            m_consumedBy = by;
        }

        bool CanProcessWith(CollisionPriority priority) const
        {
            if (!m_isConsumed) return true;
            return static_cast<int32_t>(priority) >= static_cast<int32_t>(m_consumedBy);
        }
    };

    // ═══════════════════════════════════════════════════════════════
    // CollisionSystem - 충돌 이벤트 처리 및 콜백 디스패치
    // ═══════════════════════════════════════════════════════════════

    class CollisionSystem : public Singleton<CollisionSystem>
    {
    private:
        // 이벤트 큐
        std::vector<CollisionEvent> m_pendingCollisionEvents;
        std::vector<TriggerEvent> m_pendingTriggerEvents;

        // Trigger Stay 추적 (PhysX가 제공하지 않음)
        struct TriggerPair
        {
            Collider* trigger;
            Collider* other;

            bool operator==(const TriggerPair& rhs) const
            {
                return trigger == rhs.trigger && other == rhs.other;
            }
        };

        struct TriggerPairHash
        {
            size_t operator()(const TriggerPair& p) const
            {
                size_t h1 = std::hash<void*>()(p.trigger);
                size_t h2 = std::hash<void*>()(p.other);
                return h1 ^ (h2 << 1);
            }
        };

        std::unordered_set<TriggerPair, TriggerPairHash> m_activeTriggerPairs;

        // 공격 인스턴스 관리 (우선순위 시스템)
        std::unordered_map<uint64_t, AttackInstance> m_activeAttacks;
        uint64_t m_nextAttackId = 1;

    private:
        CollisionSystem() = default;
        ~CollisionSystem() = default;

    public:
        // ═══════════════════════════════════════
        // 이벤트 큐잉 (PhysicsCallback에서 호출)
        // ═══════════════════════════════════════
        void QueueCollisionEvent(const CollisionEvent& event);
        void QueueTriggerEvent(const TriggerEvent& event);

        // ═══════════════════════════════════════
        // 이벤트 처리 (프레임 끝에서 호출)
        // ═══════════════════════════════════════
        void ProcessEvents();

        // ═══════════════════════════════════════
        // 공격 인스턴스 관리 (전투 시스템용)
        // ═══════════════════════════════════════
        uint64_t CreateAttack(GameObject* attacker);
        void EndAttack(uint64_t attackId);
        AttackInstance* GetAttack(uint64_t attackId);

        // ═══════════════════════════════════════
        // Collider 정리 (파괴 시)
        // ═══════════════════════════════════════
        void OnColliderDestroyed(Collider* collider);

        // ═══════════════════════════════════════
        // 이벤트 큐 클리어
        // ═══════════════════════════════════════
        void ClearPendingEvents();

    private:
        void ProcessCollisionEvents();
        void ProcessTriggerEvents();

        void DispatchCollisionEnter(Collider* a, Collider* b, const std::vector<ContactPoint>& contacts);
        void DispatchCollisionStay(Collider* a, Collider* b, const std::vector<ContactPoint>& contacts);
        void DispatchCollisionExit(Collider* a, Collider* b);

        void DispatchTriggerEnter(Collider* trigger, Collider* other);
        void DispatchTriggerStay(Collider* trigger, Collider* other);
        void DispatchTriggerExit(Collider* trigger, Collider* other);

        // 접촉점 노말 반전 (상대방에게 전달할 때)
        std::vector<ContactPoint> FlipContactNormals(const std::vector<ContactPoint>& contacts);

        friend class Singleton<CollisionSystem>;
    };
}
