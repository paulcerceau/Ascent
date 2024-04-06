#include "ShipActor.h"

#include <SDL_scancode.h>

#include "AudioComponent.h"
#include "FpFlyingCameraComponent.h"
#include "FlyingComponent.h"

#include "MeshComponent.h"
#include "SpriteComponent.h"

#include "AudioSystem.h"
#include "InputSystem.h"
#include "Assets.h"

#include <iostream>


ShipActor::ShipActor() :
	Actor(),
	flyingComponent{ nullptr },
	audioComponent{ nullptr },
	flyingCameraComponent{ nullptr },
	spriteComponent{ nullptr },
	meshComponent{ nullptr }
{
	flyingComponent = new FlyingComponent(this);
	audioComponent = new AudioComponent(this);
	flyingCameraComponent = new FpFlyingCameraComponent(this);
	spriteComponent = new SpriteComponent(this, Assets::getTexture(""));
	meshComponent = new MeshComponent(this);
	// meshComponent->setMesh(Assets::getMesh("Mesh_Car"));
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

	Vector3 inputsVector = Vector3::zero;

	// Forward 
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_W))
	{
		inputsVector.x += 1;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_S))
	{
		inputsVector.x -= 1;
	}
	// Strafe
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_A))
	{
		inputsVector.y -= 1;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		inputsVector.y += 1;
	}
	// Elevation
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_SPACE))
	{
		inputsVector.z += 1;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_LSHIFT))
	{
		inputsVector.z -= 1;
	}
	flyingComponent->setInputsDirection(inputsVector);

	// Roll
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_Q))
	{
		rollSpeed += Maths::piOver2;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_E))
	{
		rollSpeed -= Maths::piOver2;
	}

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
	//^ Movement (mouse) =============================================
}
