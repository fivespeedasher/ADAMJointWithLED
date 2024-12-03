/**
 * @file ControlRequest.h
 * @author Jushawn
 * @brief 作为ADAM控制的上层封装
 * @version 0.1
 * @date 2024-11-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef CONTROLREQUEST_H
#define CONTROLREQUEST_H
#include "BulbConstants.h"
#include "ADAM.h"

class ControlRequest {
public:
    ControlRequest(ADAM4068& adam_1, ADAM4068& adam_2, ADAM4068& adam_3, ADAM4051& adam_4, ADAM4051& adam_5, ADAM4168& adam_6);
    ~ControlRequest();
    int controlWarningLight(bool state);
    int controlStatusLight(int color);
    int controlCabinLight(bool state);
    int stopStatusLight();
    int RollingToLeft(bool whichRoller);
    int RollingToRight(bool whichRoller);
    int stopRolling();
    int controlDeliverToLeft(bool whichRoller);
    int controlDeliverToRight(bool whichRoller);
    int blockerDown();
    int stopDeliver();
    int riseLeftRollingDoor();
    int riseRightRollingDoor();
    int downLeftRollingDoor();
    int downRightRollingDoor();
    int stopRollingDoor();
    int controlHorn(int horn);
    int stopHorn();
    vector<bool> readPositionSensor();
    int controlDrivingLight(bool state);
    int controlTurningLight(bool direction);
    int turnOffTurningLight();
    int controlNearLight(bool state);
    int controlFarLight(bool state);
    int controlReversingLight(bool state);
    int controlBrakeLight(bool state);
    
private:
    ADAM4068& adam_1;
    ADAM4068& adam_2;
    ADAM4068& adam_3;
    ADAM4051& adam_4;
    ADAM4051& adam_5;
    ADAM4168& adam_6;
    int waitForPositionSensorState(int PositionSensor, bool state_shouldBe);
};
#endif // CONTROLREQUEST_H