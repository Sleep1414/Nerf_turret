#define SUPPRESS_HPP_WARNING
#include "ServoEasing.h"
#include "ESP32Servo.h"

#include "Turret.h"

Turret::Turret(Servo &recoil, ServoEasing &pan, ServoEasing &tilt) : recoil_servo(recoil), pan_servo(pan), tilt_servo(tilt)
{
  //-----attaches servo to pins
  recoil_servo.attach(SHOOT_SERVO);
  recoil_servo.write(recoil_rest);
  pan_servo.attach(PAN_SERVO, pan_start);
  tilt_servo.attach(TILT_SERVO, tilt_start);
  // pan_servo.setEasingType(EASE_SINE_IN_OUT);
  // tilt_servo.setEasingType(EASE_SINE_IN_OUT);

  //-----define motor pin mode
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  this->motorSpeed(false);
}

void Turret::fire()
{

  recoil_servo.write(recoil_pushed);
  Serial.println(recoil_pushed);
  delay(500);
  recoil_servo.write(recoil_rest);
  Serial.println(recoil_rest);
}

void Turret::motorSpeed(byte speed, bool onoff)
{
  speed = map(speed, 0, 5, 0, 230);

  analogWrite(ENA, speed);
  digitalWrite(IN1, onoff);
}

void Turret::motorSpeed(bool onoff)
{
  digitalWrite(IN1, onoff);
}

void Turret::motorSpeed(byte speed)
{
  speed = map(speed, 0, 5, 0, 230);
  analogWrite(ENA, speed);
}

void Turret::targets(byte pan, byte tilt)
{
  pan_servo.easeTo(pan, 50);
  tilt_servo.easeTo(tilt, 50);
  // pan_servo.write(pan);
  // tilt_servo.write(tilt);

  Serial.println("Pan: " + pan);
  Serial.println("Tilt: " + tilt);
}
void Turret::setPan(byte pan)
{
  pan_servo.easeTo(pan, 50);
  // pan_servo.write(pan);

  Serial.println("Pan: " + pan);
}
void Turret::setTilt(byte tilt)
{
  tilt_servo.easeTo(tilt, 50);
  // tilt_servo.write(tilt);

  Serial.println("Tilt: " + tilt);
}

Turret::~Turret()
{
}
