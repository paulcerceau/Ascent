#pragma once
#include "Actor.h"


class ShipActor : public Actor
{
public:
	ShipActor();

	ShipActor(const ShipActor&) = delete;
	ShipActor& operator=(const ShipActor&) = delete;

	void updateActor(float dt) override;
	void actorInput(const InputState& inputState) override;

private:
	class FlyingComponent* flyingComponent;
	class AudioComponent* audioComponent;
	class FpFlyingCameraComponent* flyingCameraComponent;
};

