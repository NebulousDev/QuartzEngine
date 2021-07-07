#include "Transform.h"

namespace Quartz
{
    TransformComponent::TransformComponent() :
        position({ 0.0f, 0.0f, 0.0f }),
        rotation(Quaternion().SetEuler({ 0.0f, 0.0f, 0.0f })),
        scale({ 1.0f, 1.0f, 1.0f })
    {
        // Nothing
    }

    TransformComponent::TransformComponent(const Vector3& position, const Quaternion& rotation, const Vector3& scale) :
        position(position),
        rotation(rotation),
        scale(scale)
    {
        // Nothing
    }

    Vector3 TransformComponent::GetForward()
    {
        return rotation * Vector3( 0.0f, 0.0f, 1.0f );
    }

    Vector3 TransformComponent::GetBackward()
    {
        return rotation * Vector3(0.0f, 0.0f, -1.0f);
    }

    Vector3 TransformComponent::GetLeft()
    {
        return rotation * Vector3(0.0f, -1.0f, 0.0f);
    }

    Vector3 TransformComponent::GetRight()
    {
        return rotation * Vector3(0.0f, 1.0f, 1.0f);
    }

    Vector3 TransformComponent::GetUp()
    {
        return rotation * Vector3(0.0f, 1.0f, 1.0f);
    }

    Vector3 TransformComponent::GetDown()
    {
        return rotation * Vector3(0.0f, -1.0f, 1.0f);
    }

    Matrix4 TransformComponent::GetMatrix()
    {
        return 
            Matrix4().SetTranslation(position) *
            Matrix4().SetRotation(rotation) *
            Matrix4().SetScale(scale);
    }
}

