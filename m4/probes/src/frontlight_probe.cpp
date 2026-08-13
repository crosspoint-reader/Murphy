// Murphy M4 frontlight probe — sweeps candidate GPIOs for cool and warm LED channels.
// M4 has dual-tone frontlight (frontlightCool + frontlightWarm), unlike M3's single channel.
// Watch the screen: one GPIO will brighten the cool LEDs, another the warm LEDs.
#include <Arduino.h>
#include <driver/ledc.h>

// GPIOs to sweep. Exclude USB (19/20), flash (26-32), BOOT (0), USB strap (45/46).
// M3 used GPIO48; try that first, then common alternatives.
static const uint8_t CANDIDATES[] = {
    48, 47, 38, 21, 14, 15, 16, 17, 18,
    33, 34, 35, 36, 37, 39, 40, 41, 42, 43, 44
};
static const int NUM_CANDIDATES = sizeof(CANDIDATES) / sizeof(CANDIDATES[0]);

static const uint32_t PWM_FREQ = 25000;
static const ledc_timer_bit_t PWM_RES = LEDC_TIMER_10_BIT;
static const uint32_t DUTY_MAX = (1UL << 10) - 1;

static void pulseGpio(uint8_t gpio) {
    ledc_timer_config_t timer = {};
    timer.speed_mode       = LEDC_LOW_SPEED_MODE;
    timer.duty_resolution  = PWM_RES;
    timer.timer_num        = LEDC_TIMER_0;
    timer.freq_hz          = PWM_FREQ;
    timer.clk_cfg          = LEDC_AUTO_CLK;
    ledc_timer_config(&timer);

    ledc_channel_config_t ch = {};
    ch.gpio_num   = gpio;
    ch.speed_mode = LEDC_LOW_SPEED_MODE;
    ch.channel    = LEDC_CHANNEL_0;
    ch.timer_sel  = LEDC_TIMER_0;
    ch.duty       = 0;
    ledc_channel_config(&ch);

    Serial.printf("[%lus] GPIO%d: off (0)\n", millis()/1000, gpio);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    delay(800);

    Serial.printf("[%lus] GPIO%d: half (%lu)\n", millis()/1000, gpio, DUTY_MAX/2);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, DUTY_MAX / 2);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    delay(1500);

    Serial.printf("[%lus] GPIO%d: full (%lu)\n", millis()/1000, gpio, DUTY_MAX);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, DUTY_MAX);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    delay(1500);

    ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    pinMode(gpio, INPUT);
    delay(300);
}

void setup() {
    Serial.begin(115200);
    delay(2500);
    Serial.println("\n=== Murphy M4 Frontlight Probe ===");
    Serial.println("Watch the screen edge LEDs. Note which GPIO number lights cool vs warm.");
    Serial.println("Each GPIO gets 3 steps: off / half / full brightness, then off again.\n");

    for (int i = 0; i < NUM_CANDIDATES; i++) {
        uint8_t gpio = CANDIDATES[i];
        Serial.printf("\n--- GPIO%d (%d/%d) ---\n", gpio, i+1, NUM_CANDIDATES);
        Serial.println("OBSERVE: does this change the frontlight? cool or warm?");
        pulseGpio(gpio);
    }

    Serial.println("\n=== Sweep done. ===");
    Serial.println("Note which GPIO numbers changed cool and which changed warm.");
}

void loop() {
    delay(10000);
    Serial.println("idle — note your observations above");
}
