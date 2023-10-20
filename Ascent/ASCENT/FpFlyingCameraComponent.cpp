#include "FpFlyingCameraComponent.h"

#include "Actor.h"


FpFlyingCameraComponent::FpFlyingCameraComponent(Actor* ownerP) :
	CameraComponent(ownerP),
	pitch(0.0f),
	pitchSpeed(0.0f),
	roll(0.0f),
	rollSpeed(0.0f),
	yaw(0.0f),
	yawSpeed(0.0f)
{
}

void FpFlyingCameraComponent::update(float dt)
{
	CameraComponent::update(dt);

	const float tempClamp = Maths::pi / 3.0f;

	Vector3 cameraPosition = owner.getPosition();
	pitch += pitchSpeed * dt;
	pitch = Maths::clamp(pitch, -tempClamp, tempClamp); // TODO: define clamp values
	Quaternion q{ owner.getRight(), pitch };
	Vector3 viewForward = Vector3::transform(owner.getForward(), q);

	Vector3 target = cameraPosition + viewForward * 100.0f;
	Vector3 up = Vector3::transform(owner.getUp(), q);
	Matrix4 view = Matrix4::createLookAt(cameraPosition, target, up);
	setViewMatrix(view);
}

#pragma region Accessors
//v ==============================================================
//v Accessors                                                        

void FpFlyingCameraComponent::setPitchSpeed(float speed)
{
	pitchSpeed = speed;
}
void FpFlyingCameraComponent::setRollSpeed(float speed)
{
	rollSpeed = speed;
}
void FpFlyingCameraComponent::setYawSpeed(float speed)
{
	yawSpeed = speed;
}

//^ Accessors                                                        
//^ ==============================================================
#pragma endregion

