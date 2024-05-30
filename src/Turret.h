#ifndef SERVOMOVE_H
#define SERVOMOVE_H

#define PAN_SERVO 18
#define TILT_SERVO 19

#define SHOOT_SERVO 5

#define ENA 25
#define IN1 26
#define IN2 27

class ServoEasing;
class Servo;

class Turret
{

private:
    Servo &recoil_servo;
    ServoEasing &pan_servo;
    ServoEasing &tilt_servo;

    // targets and time variabels
    const byte pan_start = 90;
    const byte tilt_start = 100;

    const byte recoil_rest = 180;   // Angle of the servo when at rest
    const byte recoil_pushed = 125; // Angle the servo need to reach to push the dart

public:
    Turret(Servo &recoil, ServoEasing &pan, ServoEasing &tilt);
    ~Turret();

    void motorSpeed(byte speed, bool onoff);
    void motorSpeed(byte speed);
    void motorSpeed(bool onoff);

    void fire();

    void targets(byte pan, byte tilt);
    void setPan(byte pan);
    void setTilt(byte tilt);
};

#endif