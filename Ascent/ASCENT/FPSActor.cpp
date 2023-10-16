#include "FPSActor.h"
#include "MoveComponent.h"
#include "AudioComponent.h"
#include "Game.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"

FPSActor::FPSActor() : 
	Actor(), 
	moveComponent(nullptr), 
	audioComponent(nullptr), 
	meshComponent(nullptr),
	cameraComponent(nullptr),
	lastFootstep(0.0f)
{
	setPosition(Vector3{ 0.0f, 0.0f, 250.0f });

	moveComponent = new MoveComponent(this);
	audioComponent = new AudioComponent(this);
	cameraComponent = new FPSCameraComponent(this);

	footstep = audioComponent->playEvent("event:/Footstep");
	footstep.setPaused(true);

	FPSModel = new Actor();
	FPSModel->setScale(0.75f);
	meshComponent = new MeshComponent(FPSModel);
	// meshComponent->setMesh(Assets::getMesh("Mesh_Rifle"));
}

void FPSActor::updateActor(float dt)
{
	Actor::updateActor(dt);

	// Play the footstep if we're moving and haven't recently
	lastFootstep -= dt;
	if (!Maths::nearZero(moveComponent->getForwardSpeed()) && lastFootstep <= 0.0f)
	{
		footstep.setPaused(false);
		footstep.restart();
		lastFootstep = 0.5f;
	}

	// Update position and rotation of model relatively to position
	Vector3 modelPosition = getPosition();
	modelPosition += getForward() * MODEL_OFFSET.x;
	modelPosition += getRight() * MODEL_OFFSET.y;
	modelPosition.z += MODEL_OFFSET.z;
	FPSModel->setPosition(modelPosition);
	Quaternion q = getRotation();
	q = Quaternion::concatenate(q, Quaternion(getRight(), cameraComponent->getPitch()));
	FPSModel->setRotation(q);
}

void FPSActor::actorInput(const InputState& inputState)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	// wasd movement
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_W))
	{
		forwardSpeed += 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_S))
	{
		forwardSpeed -= 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_A))
	{
		strafeSpeed -= 400.0f;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		strafeSpeed += 400.0f;
	}
	moveComponent->setForwardSpeed(forwardSpeed);
	moveComponent->setStrafeSpeed(strafeSpeed);
	// Mouse mouvement
	Vector2 mousePosition = inputState.mouse.getPosition();
	float x = mousePosition.x;
	float y = mousePosition.y;
	const int maxMouseSpeed = 500;
	const float maxAngularSpeed = Maths::pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0)
	{
		angularSpeed = x / maxMouseSpeed;
		angularSpeed *= maxAngularSpeed;
	}
	moveComponent->setAngularSpeed(angularSpeed);
	const float maxPitchSpeed = Maths::pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		pitchSpeed = y / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	cameraComponent->setPitchSpeed(pitchSpeed);
}

void FPSActor::setFootstepSurface(float value)
{
	// Pause here because the way I setup the parameter in FMOD
	// changing it will play a footstep
	footstep.setPaused(true);
	footstep.setParameter("Surface", value);
}

void FPSActor::setVisible(bool isVisible)
{
	meshComponent->setVisible(isVisible);
}
