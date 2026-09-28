#define PIN_LED 7

int g_period = 10000;
int g_duty = 0;

void set_period(int period) {
  g_period = period;
}

void set_duty(int duty) {
  g_duty = duty;
}

void pwm_pulse() {
  int on_time = (g_period * g_duty) / 100;
  int off_time = g_period - on_time;

  if (on_time > 0) {
    digitalWrite(PIN_LED, LOW);
    delayMicroseconds(on_time);
  }
  if (off_time > 0) {
    digitalWrite(PIN_LED, HIGH);
    delayMicroseconds(off_time);
  }
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);
  set_period(10000);
}

void loop() {
  for (int d = 0; d <= 100; d++) {
    set_duty(d);
    unsigned long start = millis();
    while (millis() - start < 5) {
      pwm_pulse();
    }
  }

  for (int d = 100; d >= 0; d--) {
    set_duty(d);
    unsigned long start = millis();
    while (millis() - start < 5) {
      pwm_pulse();
    }
  }
}
