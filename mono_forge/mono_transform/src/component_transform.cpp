#include "mono_transform/src/pch.h"
#include "mono_transform/include/component_transform.h"

#pragma comment(lib, "riaecs.lib")

using namespace DirectX;

mono_transform::ComponentTransform::ComponentTransform()
{
}

mono_transform::ComponentTransform::~ComponentTransform()
{
    isInitialized_ = false;

    pos_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    localPos_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    lastPos_ = XMFLOAT3(0.0f, 0.0f, 0.0f);

    rot_ = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    localRot_ = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    lastRot_ = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

    scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
    localScale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
    lastLocalScale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);

    parent_ = riaecs::Entity();
    childs_.clear();
}

MONO_TRANSFORM_API riaecs::ComponentRegistrar
<mono_transform::ComponentTransform, mono_transform::ComponentTransformMaxCount> mono_transform::ComponentTransformID;

void mono_transform::QuaternionToYawPitchRoll(const XMVECTOR &q, float &yaw, float &pitch, float &roll)
{
    XMFLOAT4 qf;
    XMStoreFloat4(&qf, q);

    // Calculate the angular radians of each axis
    float ysqr = qf.y * qf.y;

    // roll (X)
    float t0 = +2.0f * (qf.w * qf.x + qf.y * qf.z);
    float t1 = +1.0f - 2.0f * (qf.x * qf.x + ysqr);
    roll = std::atan2(t0, t1);

    // pitch (Y)
    float t2 = +2.0f * (qf.w * qf.y - qf.z * qf.x);
    t2 = t2 > 1.0f ? 1.0f : t2;
    t2 = t2 < -1.0f ? -1.0f : t2;
    pitch = std::asin(t2);

    // yaw (Z)
    float t3 = +2.0f * (qf.w * qf.z + qf.x * qf.y);
    float t4 = +1.0f - 2.0f * (ysqr + qf.z * qf.z);
    yaw = std::atan2(t3, t4);
}

void mono_transform::InitializeTransform
(
    ComponentTransform &component, 
    const XMFLOAT3 &position, const XMFLOAT4 &rotation, const XMFLOAT3 &scale, 
    const riaecs::Entity &parent
){
    std::unique_lock<std::shared_mutex> lock(component.mutex_);

    component.pos_ = position;
    component.localPos_ = position;
    component.lastPos_ = position;

    component.rot_ = rotation;
    component.localRot_ = rotation;
    component.lastRot_ = rotation;

    component.scale_ = scale;
    component.localScale_ = scale;
    component.lastLocalScale_ = scale;

    component.parent_ = parent;
    component.childs_.clear();

    component.isInitialized_ = true;
}

XMMATRIX mono_transform::GetWorldMatrix(const ComponentTransform &component)
{
    XMMATRIX scale = XMMatrixScaling(component.scale_.x, component.scale_.y, component.scale_.z);
    XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&component.rot_));
    XMMATRIX pos = XMMatrixTranslation(component.pos_.x, component.pos_.y, component.pos_.z);

    return scale * rot * pos;
}

XMMATRIX mono_transform::GetWorldMatrixNoRot(const ComponentTransform &component)
{
    XMMATRIX scale = XMMatrixScaling(component.scale_.x, component.scale_.y, component.scale_.z);
    XMMATRIX pos = XMMatrixTranslation(component.pos_.x, component.pos_.y, component.pos_.z);

    return scale * pos;
}

XMMATRIX mono_transform::GetWorldMatrixNoScale(const ComponentTransform &component)
{
    XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&component.rot_));
    XMMATRIX pos = XMMatrixTranslation(component.pos_.x, component.pos_.y, component.pos_.z);

    return rot * pos;
}

XMMATRIX mono_transform::GetLocalMatrix(const ComponentTransform &component)
{
    XMMATRIX scale = XMMatrixScaling(component.localScale_.x, component.localScale_.y, component.localScale_.z);
    XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&component.localRot_));
    XMMATRIX pos = XMMatrixTranslation(component.localPos_.x, component.localPos_.y, component.localPos_.z);

    return scale * rot * pos;
}

DirectX::XMMATRIX mono_transform::GetLocalMatrixNoRot(const ComponentTransform &component)
{
    XMMATRIX scale = XMMatrixScaling(component.localScale_.x, component.localScale_.y, component.localScale_.z);
    XMMATRIX pos = XMMatrixTranslation(component.localPos_.x, component.localPos_.y, component.localPos_.z);

    return scale * pos;
}

DirectX::XMMATRIX mono_transform::GetLocalMatrixNoScale(const ComponentTransform &component)
{
    XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&component.localRot_));
    XMMATRIX pos = XMMatrixTranslation(component.localPos_.x, component.localPos_.y, component.localPos_.z);

    return rot * pos;
}

DirectX::XMMATRIX mono_transform::GetLastWorldMatrix(const ComponentTransform &component)
{
    XMMATRIX scale = XMMatrixScaling(component.lastLocalScale_.x, component.lastLocalScale_.y, component.lastLocalScale_.z);
    XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&component.lastRot_));
    XMMATRIX pos = XMMatrixTranslation(component.lastPos_.x, component.lastPos_.y, component.lastPos_.z);

    return scale * rot * pos;
}

DirectX::XMMATRIX mono_transform::GetLastWorldMatrixNoRot(const ComponentTransform &component)
{
    XMMATRIX scale = XMMatrixScaling(component.lastLocalScale_.x, component.lastLocalScale_.y, component.lastLocalScale_.z);
    XMMATRIX pos = XMMatrixTranslation(component.lastPos_.x, component.lastPos_.y, component.lastPos_.z);

    return scale * pos;
}

DirectX::XMMATRIX mono_transform::GetLastWorldMatrixNoScale(const ComponentTransform &component)
{
    XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&component.lastRot_));
    XMMATRIX pos = XMMatrixTranslation(component.lastPos_.x, component.lastPos_.y, component.lastPos_.z);

    return rot * pos;
}

void mono_transform::UpdateRootTransform(ComponentTransform &component)
{
    // Store last transform
    component.lastPos_ = component.pos_;
    component.lastRot_ = component.rot_;
    component.lastLocalScale_ = component.localScale_;

    // Update world transform from local transform
    component.pos_ = component.localPos_;
    component.rot_ = component.localRot_;
    component.scale_ = component.localScale_;
}

void mono_transform::UpdateChildTransform(ComponentTransform &component, const XMMATRIX &parentMatrix)
{
    // Apply parent matrix to local matrix to get world matrix
    XMMATRIX localMatrix = GetLocalMatrix(component);
    XMMATRIX worldMatrix = localMatrix * parentMatrix;

    // Decompose world matrix to position, rotation, scale
    XMVECTOR pos, rot, scale;
    XMMatrixDecompose(&scale, &rot, &pos, worldMatrix);

    XMStoreFloat3(&component.pos_, pos);
    XMStoreFloat4(&component.rot_, rot);
    XMStoreFloat3(&component.scale_, scale);
}