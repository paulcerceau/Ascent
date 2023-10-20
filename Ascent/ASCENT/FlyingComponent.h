#pragma once
#include "Component.h"

class FlyingComponent :
    public Component
{
public:
    FlyingComponent(Actor* ownerP, int updateOrderP = 10);

    FlyingComponent() = delete;
    FlyingComponent(const FlyingComponent&) = delete;
    FlyingComponent& operator=(const FlyingComponent&) = delete;

    //v Accessors ====================================================
    void setForwardSpeed(float forwardSpeedP);
    void setStrafeSpeed(float strafeSpeedP);
    void setElevationSpeed(float elevationSpeedP);

    void setRollSpeed(float rollSpeedP);
    void setPitchSpeed(float pitchSpeedP);
    void setYawSpeed(float yawSpeedP);

    //^ Accessors ====================================================

    void update(float dt) override;

private:
    float forwardSpeed;
    float strafeSpeed;
    float elevationSpeed;

    float rollSpeed;
    float pitchSpeed;
    float yawSpeed;
};

