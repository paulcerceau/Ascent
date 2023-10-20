#pragma once
#include "CameraComponent.h"


class FpFlyingCameraComponent :
    public CameraComponent
{
public:
    FpFlyingCameraComponent(class Actor* ownerP);

    void update(float dt) override;

    //v Accessors ====================================================
    // -- GETTERS --
    float getPitch() const { return pitch; }
    float getPitchSpeed() const { return pitchSpeed; }

    float getRoll() const { return roll; }
    float getRollSpeed() const { return rollSpeed; }

    float getYaw() const { return yaw; }
    float getYawSpeed() const { return yawSpeed; }

    // -- SETTERS --
    void setPitchSpeed(float speed);
    void setRollSpeed(float speed);
    void setYawSpeed(float speed);

    //^ Accessors ====================================================

private:
    float pitch;
    float pitchSpeed;

    float roll;
    float rollSpeed;

    float yaw;
    float yawSpeed;
};

