#include "FlyingComponent.h"

#include <fmod_studio.h>

#include "Actor.h"
#include "SimpleLog.h"

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
	yawSpeed{ 0.0f },

	forwardDirection{ Vector3::zero },
	rightDirection{ Vector3::zero },
	upDirection{ Vector3::zero }
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
		// TODO: lerp rotations

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

		owner.setRotation(newRotation);
	}

	//^ Set owner rotation ===========================================
	//v Set owner position ===========================================

	updateTargetVelocity();

	// Make sure that velocity is different than zero
	// so that we don't calculate the position for nothing
	if (!Vector3::nearZero(targetVelocity))
	{
		const Vector3 ownerPosition = owner.getPosition();

		const Vector3 newVelocity = targetVelocity * dt;

		const Vector3 newPosition = ownerPosition + newVelocity.x * forwardDirection + newVelocity.y * rightDirection + newVelocity.z * upDirection;

		owner.setPosition(newPosition);
	}
	//^ Set owner position ===========================================
}


void FlyingComponent::updateTargetVelocity()
{
	// TODO: smarter acceleration
	#pragma region Acceleration
	// -- ACCELERATION --
	// Ignore acceleration calculation if no inputs were made
	if (!Vector3::nearZero(inputsDirection)) 
	{
		// On input store owner's vectors to improve 
		// the feeling of controlling a flying vessel

		// Update forward component
		if (inputsDirection.x != 0) {
			forwardDirection = owner.getForward();

			if (targetVelocity.x <= MAX_SPEEDS.x && targetVelocity.x >= MIN_SPEEDS.x) {

				const float thrustPower = ACCELERATION_FACTORS.x * inputsDirection.x;
				targetVelocity.x += thrustPower;
			}

		//	const Vector3 currentForwardVelocity = localVelocity * forwardDirection;
		//	const float magnitude = currentForwardVelocity.length();

		//	if (magnitude <= MAX_SPEEDS.x) {

		//		const float thrustPower = ACCELERATION_FACTORS.x;
		//		localVelocity += forwardDirection * thrustPower;
		//	}
		}

		// Update strafe component
		if (inputsDirection.y != 0) {
			rightDirection = owner.getRight();

			if (targetVelocity.y <= MAX_SPEEDS.y && targetVelocity.y >= MIN_SPEEDS.y) {

				const float thrustPower = ACCELERATION_FACTORS.y * inputsDirection.y;
				targetVelocity.y += thrustPower;
			}
		}

		// Update elevation component
		if (inputsDirection.z != 0) {
			upDirection = owner.getUp();

			if (targetVelocity.z <= MAX_SPEEDS.z && targetVelocity.z >= MIN_SPEEDS.z) {

				const float thrustPower = ACCELERATION_FACTORS.z * inputsDirection.z;
				targetVelocity.z += thrustPower;
			}
		}
	}
	#pragma endregion Acceleration
	#pragma region Deceleration
	// -- DECELERATION --
	// Forward 
	if (inputsDirection.x == 0 && !Maths::nearZero(targetVelocity.x)) {
	//	const Vector3 forwardComponent = localVelocity * Vector3::absolute(forwardDirection);

	//	if (!Vector3::nearZero(forwardComponent)) {

	//		const Vector3 deceleration = forwardDirection * -1.0f * DECELERATION_FACTORS.x;

	//		const float dot = Vector3::dot(Vector3::normalize(forwardComponent), Vector3::normalize(deceleration));

	//		if (dot <= 0.95f) {
	//			localVelocity += deceleration;

	//			std::cout << "FORWARD" << std::endl;
	//		}
	//	}

		targetVelocity.x *= 0.98f;
	}
	// Strafe 
	if (inputsDirection.y == 0 && !Maths::nearZero(targetVelocity.y)) {
		targetVelocity.y *= 0.98f;
	}
	// Elevation 
	if (inputsDirection.z == 0 && !Maths::nearZero(targetVelocity.z)) {
		targetVelocity.z *= 0.98f;
	}	

	//SimpleLog::logVector("localVelocity", localVelocity);
	#pragma endregion Deceleration
}

void FlyingComponent::lerpDirections(float dt)
{

}
