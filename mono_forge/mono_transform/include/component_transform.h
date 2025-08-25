#pragma once
#include "mono_transform/include/dll_config.h"
#include "riaecs/riaecs.h"

#include <shared_mutex>
#include <DirectXMath.h>

namespace mono_transform
{
    constexpr size_t ComponentTransformMaxCount = 10000;
    class MONO_TRANSFORM_API ComponentTransform
    {
    public:
        ComponentTransform();
        ~ComponentTransform();

        std::shared_mutex mutex_;
        bool isInitialized_ = false;

        DirectX::XMFLOAT3 pos_;
        DirectX::XMFLOAT3 localPos_;
        DirectX::XMFLOAT3 lastPos_;

        DirectX::XMFLOAT4 rot_;
        DirectX::XMFLOAT4 localRot_;
        DirectX::XMFLOAT4 lastRot_;

        DirectX::XMFLOAT3 scale_;
        DirectX::XMFLOAT3 localScale_;
        DirectX::XMFLOAT3 lastLocalScale_;

        riaecs::Entity parent_;
        std::vector<riaecs::Entity> childs_;
    };
    extern MONO_TRANSFORM_API riaecs::ComponentRegistrar<ComponentTransform, ComponentTransformMaxCount> ComponentTransformID;

    void QuaternionToYawPitchRoll(const DirectX::XMVECTOR& q, float& yaw, float& pitch, float& roll);

    void InitializeTransform
    (
        ComponentTransform &component, 
        const DirectX::XMFLOAT3 &position, const DirectX::XMFLOAT4 &rotation, const DirectX::XMFLOAT3 &scale,
        const riaecs::Entity &parent = riaecs::Entity()
    );

    DirectX::XMMATRIX GetWorldMatrix(const ComponentTransform& component);
    DirectX::XMMATRIX GetWorldMatrixNoRot(const ComponentTransform& component);
    DirectX::XMMATRIX GetWorldMatrixNoScale(const ComponentTransform& component);

    DirectX::XMMATRIX GetLocalMatrix(const ComponentTransform& component);
    DirectX::XMMATRIX GetLocalMatrixNoRot(const ComponentTransform& component);
    DirectX::XMMATRIX GetLocalMatrixNoScale(const ComponentTransform& component);

    DirectX::XMMATRIX GetLastWorldMatrix(const ComponentTransform& component);
    DirectX::XMMATRIX GetLastWorldMatrixNoRot(const ComponentTransform& component);
    DirectX::XMMATRIX GetLastWorldMatrixNoScale(const ComponentTransform& component);

    void UpdateRootTransform(ComponentTransform& component);
    void UpdateChildTransform(ComponentTransform& component, const DirectX::XMMATRIX& parentMatrix);

} // namespace mono_transform