#pragma once

#include <PxPhysicsAPI.h>
#include "Framework/Physics/CollisionTypes.h"

namespace engine
{
    class CollisionSystem;

    // ═══════════════════════════════════════════════════════════════
    // PhysX 시뮬레이션 이벤트 콜백
    // PhysX에서 충돌/트리거 이벤트를 받아 CollisionSystem에 전달
    // ═══════════════════════════════════════════════════════════════

    class PhysicsEventCallback : public physx::PxSimulationEventCallback
    {
    public:
        PhysicsEventCallback() = default;
        virtual ~PhysicsEventCallback() = default;

        // ═══════════════════════════════════════
        // PxSimulationEventCallback 구현
        // ═══════════════════════════════════════

        // 물리 충돌 이벤트
        void onContact(
            const physx::PxContactPairHeader& pairHeader,
            const physx::PxContactPair* pairs,
            physx::PxU32 nbPairs
        ) override;

        // 트리거 이벤트
        void onTrigger(
            physx::PxTriggerPair* pairs,
            physx::PxU32 count
        ) override;

        // 제약 조건 파괴 (Joint 등)
        void onConstraintBreak(
            physx::PxConstraintInfo* constraints,
            physx::PxU32 count
        ) override;

        // Actor 깨어남
        void onWake(
            physx::PxActor** actors,
            physx::PxU32 count
        ) override;

        // Actor 수면
        void onSleep(
            physx::PxActor** actors,
            physx::PxU32 count
        ) override;

        // 연속 충돌 감지 (CCD)
        void onAdvance(
            const physx::PxRigidBody* const* bodyBuffer,
            const physx::PxTransform* poseBuffer,
            physx::PxU32 count
        ) override;

    private:
        // 접촉점 추출
        void ExtractContactPoints(
            const physx::PxContactPair& pair,
            std::vector<ContactPoint>& outContacts
        );
    };

    // ═══════════════════════════════════════════════════════════════
    // PhysX 충돌 필터 셰이더
    // 레이어 기반 충돌 필터링
    // ═══════════════════════════════════════════════════════════════

    physx::PxFilterFlags PhysicsFilterShader(
        physx::PxFilterObjectAttributes attributes0,
        physx::PxFilterData filterData0,
        physx::PxFilterObjectAttributes attributes1,
        physx::PxFilterData filterData1,
        physx::PxPairFlags& pairFlags,
        const void* constantBlock,
        physx::PxU32 constantBlockSize
    );
}
