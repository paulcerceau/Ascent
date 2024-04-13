#pragma once

#include "Component.h"

#include "Vector3.h"

class FlyingComponent :
    public Component
{
public:
    FlyingComponent(Actor* ownerP, int updateOrderP = 10);

    FlyingComponent() = delete;
    FlyingComponent(const FlyingComponent&) = delete;
    FlyingComponent& operator=(const FlyingComponent&) = delete;

    //v Accessors ====================================================

    void setInputsDirection(Vector3 inputsDirectionP);

    void setForwardSpeed(float forwardSpeedP);
    void setStrafeSpeed(float strafeSpeedP);
    void setElevationSpeed(float elevationSpeedP);

    void setRollSpeed(float rollSpeedP);
    void setPitchSpeed(float pitchSpeedP);
    void setYawSpeed(float yawSpeedP);

    //^ Accessors ====================================================

    void update(float dt) override;

private:
    void updateTargetVelocity();
    void lerpDirections(float dt);

    Vector3 inputsDirection;

    float forwardSpeed;
    float strafeSpeed;
    float elevationSpeed;

    const Vector3 MAX_SPEEDS{ 1000.0f, 500.0f, 500.0f };
    const Vector3 MIN_SPEEDS{ -500.0f, -500.0f, -500.0f };
    const Vector3 ACCELERATION_FACTORS{ 20.0f, 25.0f, 25.0f };
    const Vector3 DECELERATION_FACTORS{ 10.0f, 12.0f, 12.0f };

    Vector3 currentVelocity;
    Vector3 lastStoredVelocity;
    Vector3 targetVelocity;
    float velocityLerp;

    Vector3 forwardVelocity;
    Vector3 strafeVelocity;
    Vector3 elevationVelocity;

    Vector3 forwardDirection;
    Vector3 rightDirection;
    Vector3 upDirection;

    Vector3 ownerForwardVectorOnInputTarget;

    float rollSpeed;
    float pitchSpeed;
    float yawSpeed;
};

