#include "EnginePCH.h"
#include "PhysicsUtility.h"

#include "Framework/Object/Component/Transform.h"

namespace engine
{
    namespace PhysicsUtility
    {
        physx::PxTransform ToPxTransform(const Transform* transform)
        {
            if (!transform)
            {
                return physx::PxTransform(physx::PxIdentity);
            }

            // 월드 위치와 회전 사용
            // 주의: GetWorldPosition()은 월드 행렬 계산이 필요할 수 있음
            Vector3 worldPos = const_cast<Transform*>(transform)->GetWorldPosition();
            Quaternion localRot = transform->GetLocalRotation();

            // TODO: 부모가 있는 경우 월드 회전 계산 필요
            // 현재는 로컬 회전 사용 (단순화)

            return ToPxTransform(worldPos, localRot);
        }

        void ApplyPxTransformToTransform(const physx::PxTransform& pxTransform, Transform* transform)
        {
            if (!transform)
            {
                return;
            }

            Vector3 position = ToVector3(pxTransform.p);
            Quaternion rotation = ToQuaternion(pxTransform.q);

            // 부모가 있는 경우 로컬 좌표로 변환 필요
            // TODO: 월드 → 로컬 변환 구현
            // 현재는 부모가 없다고 가정

            transform->SetLocalPosition(position);
            transform->SetLocalRotation(rotation);
        }
    }
}
