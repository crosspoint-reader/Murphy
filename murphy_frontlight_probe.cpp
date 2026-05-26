#include <Arduino.h>
#include <driver/ledc.h>

namespace {
constexpr const char* VERSION = "murphy_frontlight_probe_v3_gpio48_pwm";
constexpr int FRONTLIGHT_PIN = 48;
constexpr uint32_t PWM_FREQ_HZ = 25000;
constexpr ledc_mode_t PWM_MODE = LEDC_LOW_SPEED_MODE;
constexpr ledc_timer_t PWM_TIMER = LEDC_TIMER_0;
constexpr ledc_channel_t PWM_CHANNEL = LEDC_CHANNEL_0;
constexpr ledc_timer_bit_t PWM_RES = LEDC_TIMER_10_BIT;
constexpr uint32_t DUTY_MAX = (1UL << 10) - 1;

bool setupPwm() {
  ledc_timer_config_t timer = {};
  timer.speed_mode = PWM_MODE;
  timer.duty_resolution = PWM_RES;
  timer.timer_num = PWM_TIMER;
  timer.freq_hz = PWM_FREQ_HZ;
  timer.clk_cfg = LEDC_AUTO_CLK;
  if (ledc_timer_config(&timer) != ESP_OK) {
    Serial.printf("[%lu] LEDC timer setup failed\n", millis());
    return false;
  }

  ledc_channel_config_t ch = {};
  ch.gpio_num = FRONTLIGHT_PIN;
  ch.speed_mode = PWM_MODE;
  ch.channel = PWM_CHANNEL;
  ch.timer_sel = PWM_TIMER;
  ch.intr_type = LEDC_INTR_DISABLE;
  ch.duty = 0;
  ch.hpoint = 0;
  if (ledc_channel_config(&ch) != ESP_OK) {
    Serial.printf("[%lu] LEDC channel setup failed for GPIO%d\n", millis(), FRONTLIGHT_PIN);
    return false;
  }
  return true;
}

void setDuty(uint32_t duty) {
  if (duty > DUTY_MAX) duty = DUTY_MAX;
  Serial.printf("[%lu] GPIO%d PWM duty=%lu/%lu\n", millis(), FRONTLIGHT_PIN, static_cast<unsigned long>(duty),
                static_cast<unsigned long>(DUTY_MAX));
  ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
  ledc_update_duty(PWM_MODE, PWM_CHANNEL);
}

void digitalStep(const char* label, int level, uint32_t ms) {
  ledc_stop(PWM_MODE, PWM_CHANNEL, 0);
  pinMode(FRONTLIGHT_PIN, OUTPUT);
  digitalWrite(FRONTLIGHT_PIN, level);
  Serial.printf("[%lu] GPIO%d digital %s (%d)\n", millis(), FRONTLIGHT_PIN, label, level);
  delay(ms);
}

void releasePin() {
  ledc_stop(PWM_MODE, PWM_CHANNEL, 0);
  pinMode(FRONTLIGHT_PIN, INPUT);
  Serial.printf("[%lu] GPIO%d released to INPUT\n", millis(), FRONTLIGHT_PIN);
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(2500);
  Serial.printf("\n[%lu] Murphy front-light probe boot: %s\n", millis(), VERSION);
  Serial.printf("Testing GPIO%d: digital LOW/HIGH, then %lu Hz PWM brightness steps.\n", FRONTLIGHT_PIN,
                static_cast<unsigned long>(PWM_FREQ_HZ));

  pinMode(FRONTLIGHT_PIN, INPUT);
  delay(1000);

  digitalStep("LOW/off?", LOW, 2500);
  digitalStep("HIGH/on?", HIGH, 2500);
  digitalStep("LOW/off?", LOW, 2500);

  Serial.printf("[%lu] Starting PWM sequence on GPIO%d\n", millis(), FRONTLIGHT_PIN);
  if (setupPwm()) {
    constexpr uint32_t duties[] = {0, DUTY_MAX / 64, DUTY_MAX / 32, DUTY_MAX / 16, DUTY_MAX / 8,
                                   DUTY_MAX / 4, DUTY_MAX / 2, DUTY_MAX, 0};
    for (uint32_t duty : duties) {
      setDuty(duty);
      delay(2200);
    }
  }

  releasePin();
  Serial.printf("[%lu] front-light GPIO48 probe done\n", millis());
}

void loop() {
  delay(5000);
  Serial.printf("[%lu] alive; GPIO%d is INPUT\n", millis(), FRONTLIGHT_PIN);
}
