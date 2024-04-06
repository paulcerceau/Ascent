#include "FlyingComponent.h"

#include <fmod_studio.h>

#include "Actor.h"

#include <iostream>


FlyingComponent::FlyingComponent(Actor* ownerP, int updateOrderP)
	: Component(ownerP, updateOrderP),
	forwardSpeed{ 0.0f },
	strafeSpeed{ 0.0f },
	elevationSpeed{ 0.0f },
	forwardVelocity{ Vector3::zero },
	strafeVelocity{ Vector3::zero },
	elevationVelocity{ Vector3::zero },
	rollSpeed{ 0.0f },
	pitchSpeed{ 0.0f },
	yawSpeed{ 0.0f }
{
}

#pragma region Accessors
//v ==============================================================
//v Accessors                                                        

void FlyingComponent::setInputsDirection(Vector3 inputsDirectionP)
{
	inputsDirection = inputsDirectionP;
}

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

	updateLocalVelocity();

	// Make sure that velocity is different than zero
	// so that we don't calculate the position for nothing
	if (!Vector3::nearZero(localVelocity))
	{
		const Vector3 ownerPosition = owner.getPosition();

		forwardVelocity = owner.getForward() * localVelocity.x * dt;
		strafeVelocity = owner.getRight() * localVelocity.y * dt;
		elevationVelocity = owner.getUp() * localVelocity.z * dt;

		const Vector3 actorVelocity = forwardVelocity + strafeVelocity + elevationVelocity;
		const Vector3 newPosition = ownerPosition + actorVelocity;

		owner.setPosition(newPosition);
	}
	//^ Set owner position ===========================================
}

void FlyingComponent::updateLocalVelocity()
{
	// -- ACCELERATION --
	// Ignore acceleration calculation if no inputs were made
	if (!Vector3::nearZero(inputsDirection)) 
	{
		// Update forward component
		if (inputsDirection.x != 0) {
			localVelocity.x += ACCELERATION_FACTORS.x * inputsDirection.x;
			localVelocity.x = Maths::clamp(localVelocity.y, MIN_SPEEDS.x, MAX_SPEEDS.x);
		}

		// Update strafe component
		if (inputsDirection.y != 0) {
			localVelocity.y += ACCELERATION_FACTORS.y * inputsDirection.y;
			localVelocity.y = Maths::clamp(localVelocity.y, MIN_SPEEDS.y, MAX_SPEEDS.y);
		}

		// Update elevation component
		if (inputsDirection.z != 0) {
			localVelocity.z += ACCELERATION_FACTORS.z * inputsDirection.z;
			localVelocity.z = Maths::clamp(localVelocity.z, MIN_SPEEDS.z, MAX_SPEEDS.z);
		}
	}

	// -- DECELERATION --
	// Actually is damping, should be improved later on
	// Forward 
	if (inputsDirection.x == 0 && !Maths::nearZero(localVelocity.x)) {
		localVelocity.x *= 0.99f;
	}
	// Strafe 
	if (inputsDirection.y == 0 && !Maths::nearZero(localVelocity.y)) {
		localVelocity.y *= 0.99f;
	}
	// Elevation 
	if (inputsDirection.z == 0 && !Maths::nearZero(localVelocity.z)) {
		localVelocity.z *= 0.99f;
	}

	std::cout << "inputsDirection: " << inputsDirection.x << " " << inputsDirection.y << " " << inputsDirection.z << " | local velocity: " << localVelocity.x << " " << localVelocity.y << " " << localVelocity.z << std::endl;
}

