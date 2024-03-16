#include "FlyingComponent.h"

#include <fmod_studio.h>

#include "Actor.h"
#include "Maths.h"
#include "Vector3.h"
#include <iostream>


FlyingComponent::FlyingComponent(Actor* ownerP, int updateOrderP)
	: Component(ownerP, updateOrderP),
	forwardSpeed(0.0f),
	strafeSpeed(0.0f),
	elevationSpeed(0.0f),
	rollSpeed(0.0f),
	pitchSpeed(0.0f),
	yawSpeed(0.0f)
{
}

#pragma region Accessors
//v ==============================================================
//v Accessors                                                        

void FlyingComponent::setForwardSpeed(float forwardSpeedP)
{
	forwardSpeed = forwardSpeedP;
}
void FlyingComponent::setStrafeSpeed(float strafeSpeedP)
{
	strafeSpeed = strafeSpeedP;
}
void FlyingComponent::setElevationSpeed(float elevationSpeedP)
{
	elevationSpeed = elevationSpeedP;
}

void FlyingComponent::setRollSpeed(float rollSpeedP)
{
	rollSpeed = rollSpeedP;
}
void FlyingComponent::setPitchSpeed(float pitchSpeedP)
{
	pitchSpeed = pitchSpeedP;
}
void FlyingComponent::setYawSpeed(float yawSpeedP)
{
	yawSpeed = yawSpeedP;
}

//^ Accessors                                                        
//^ ==============================================================
#pragma endregion

void FlyingComponent::update(float dt)
{
	Component::update(dt);

	//v Set owner rotation ===========================================
	if (!Maths::nearZero(rollSpeed) || !Maths::nearZero(pitchSpeed) || !Maths::nearZero(yawSpeed))
	{
		Quaternion newRotation = owner.getRotation();

		const float rollAngle = rollSpeed * dt;
		const float pitchAngle = pitchSpeed * dt;
		const float yawAngle = yawSpeed * dt;

		const Quaternion rollIncrement(owner.getForward(), rollAngle);
		newRotation = Quaternion::concatenate(newRotation, rollIncrement);

		const Quaternion pitchIncrement(owner.getRight(), pitchAngle);
		newRotation = Quaternion::concatenate(newRotation, pitchIncrement);

		const Quaternion yawIncrement(owner.getUp(), yawAngle);
		newRotation = Quaternion::concatenate(newRotation, yawIncrement);

		//std::cout << "Roll angle: " << rollAngle << std::endl;

		owner.setRotation(newRotation);
	}
	//^ Set owner rotation ===========================================
	//v Set owner position ===========================================
	// Make sure that at least one position-related variable is different than zero
	// so that we don't calculate the position for nothing
	if (!Maths::nearZero(forwardSpeed) || !Maths::nearZero(elevationSpeed) || !Maths::nearZero(strafeSpeed))
	{
		const Vector3 ownerPosition = owner.getPosition();
		const Vector3 forwardVector = owner.getForward() * forwardSpeed * dt;
		const Vector3 upVector = owner.getUp() * elevationSpeed * dt;
		const Vector3 rightVector = owner.getRight() * strafeSpeed * dt;

		const Vector3 newPosition = ownerPosition + forwardVector + upVector + rightVector;
		owner.setPosition(newPosition);
	}
	//^ Set owner position ===========================================
}

