#include "ShipActor.h"

#include <SDL_scancode.h>

#include "AudioComponent.h"
#include "FpFlyingCameraComponent.h"
#include "FlyingComponent.h"

#include "AudioSystem.h"
#include "InputSystem.h"


ShipActor::ShipActor() :
	Actor(),
	flyingComponent{ nullptr },
	audioComponent{ nullptr },
	flyingCameraComponent{ nullptr }
{
	flyingComponent = new FlyingComponent(this);
	audioComponent = new AudioComponent(this);
	flyingCameraComponent = new FpFlyingCameraComponent(this);
}

void ShipActor::updateActor(const float dt)
{
	Actor::updateActor(dt);

}

void ShipActor::actorInput(const InputState& inputState)
{
	Actor::actorInput(inputState);

	//v Movement (keyboard) ==========================================
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	float elevationSpeed = 0.0f;

	float rollSpeed = 0.0f;

	// Forward 
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_W))
	{
		forwardSpeed += 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_S))
	{
		forwardSpeed -= 400.0f;
	}
	// Strafe
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_A))
	{
		strafeSpeed -= 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		strafeSpeed += 400.0f;
	}
	// Elevation
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_SPACE))
	{
		elevationSpeed += 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_LSHIFT))
	{
		elevationSpeed -= 400.0f;
	}
	// Roll
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_Q))
	{
		rollSpeed += Maths::piOver2;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_E))
	{
		rollSpeed -= Maths::piOver2;
	}

	flyingComponent->setForwardSpeed(forwardSpeed);
	flyingComponent->setStrafeSpeed(strafeSpeed);
	flyingComponent->setElevationSpeed(elevationSpeed);

	flyingComponent->setRollSpeed(rollSpeed);

	//^ Movement (keyboard) ==========================================
	//v Movement (mouse) =============================================
	const Vector2 mousePosition = inputState.mouse.getPosition();
	const float x = mousePosition.x;
	const float y = mousePosition.y;

	const int maxMouseSpeed = 500;

	// x component ==========================
	const float maxYawSpeed = Maths::pi * 8;
	float yawSpeed = 0.0f;

	if (x != 0)
	{
		yawSpeed = x / maxMouseSpeed;
		yawSpeed *= maxYawSpeed;
	}
	flyingComponent->setYawSpeed(yawSpeed);

	// y component ==========================
	const float maxPitchSpeed = Maths::pi * 8;
	float pitchSpeed = 0.0f;

	if (y != 0)
	{
		pitchSpeed = y / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	flyingComponent->setPitchSpeed(pitchSpeed);
	flyingCameraComponent->setPitchSpeed(pitchSpeed);
	//^ Movement (mouse) =============================================
}
