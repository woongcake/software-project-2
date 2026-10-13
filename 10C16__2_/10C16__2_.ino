/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/faq/how-to-control-speed-of-servo-motor
 */

#include <Servo.h>
#include <math.h>

#define PIN_SERVO 10
#define PIN_TRIG  12
#define PIN_ECHO  13

#define DETECT_DIST 150.0  // 차량 감지 기준 거리 (150mm = 15cm)
#define BAR_DOWN 0         // 차단기 내려진 각도 (0도)
#define BAR_UP   90        // 차단기 올려진 각도 (90도)

Servo myServo;
unsigned long MOVING_TIME = 2000; // moving time is 2 seconds (2000ms)
unsigned long moveStartTime;
int startAngle = BAR_DOWN;
int stopAngle  = BAR_UP;
int currentAngle = BAR_DOWN;

bool isGateOpen = false;
bool isMoving = false;

// 초음파 센서 거리 측정 함수 (mm)
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) return 400.0;
  return duration * 0.001 * 0.5 * 346.0;
}

// Cosine Ease-In-Out 함수
float get_cosine_ratio(float t) {
  return 0.5 * (1.0 - cos(M_PI * t));
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  myServo.attach(PIN_SERVO);
  moveStartTime = millis(); // start moving

  myServo.write(startAngle); // Set position
  delay(500);

  Serial.begin(57600);
}

void loop() {
  float dist = USS_measure(PIN_TRIG, PIN_ECHO);

  if (dist < DETECT_DIST && !isGateOpen) {
    isGateOpen = true;
    startAngle = currentAngle;
    stopAngle = BAR_UP;
    moveStartTime = millis();
    isMoving = true;
  } 
  else if (dist >= DETECT_DIST && isGateOpen) {
    isGateOpen = false;
    startAngle = currentAngle;
    stopAngle = BAR_DOWN;
    moveStartTime = millis();
    isMoving = true;
  }

  if (isMoving) {
    unsigned long progress = millis() - moveStartTime;

    if (progress <= MOVING_TIME) {
      // while moving
      float t = (float)progress / MOVING_TIME;
      float ratio = get_cosine_ratio(t);

      currentAngle = startAngle + (stopAngle - startAngle) * ratio;
      myServo.write(currentAngle); 
    } else {
      // movement finished -> reverse direction
      currentAngle = stopAngle;
      myServo.write(currentAngle);
      isMoving = false;
    }
  }

  Serial.print("Dist:"); Serial.print(dist);
  Serial.print(", Angle:"); Serial.println(currentAngle);

  delay(25);
}
