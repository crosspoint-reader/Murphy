// Murphy M4 SD_MMC probe v7 — hyper-targeted.
//
// v6 finding: 1036 send_op_cond errors = SDMMC clock works for ALL CLK candidates.
// Clock is fine. Card is not responding to ACMD41. Two gaps in v6:
//   1. GPIO15 was reserved as power, NEVER tried as CMD.
//   2. GPIO39 was never tested as active-HIGH power enable.
//
// M3 analogy: CLK=16, CMD=17, D0=15, D1=14, D2=21→11?, D3=18, power=GPIO10→??
// CLK=16 is confirmed (SDMMC clock setup succeeded every time).
// CMD=15 is the most likely miss — try it now.
//
// Strategy:
//   For each power config (none, GPIO39 HIGH, GPIO16 HIGH, GPIO15 HIGH):
//     CLK=16 (confirmed), CMD from {11,14,15,17,18}, D0 from remaining.
//   Then expand CLK to all other candidates with CMD=15 specifically.

#include <Arduino.h>
#include <SD_MMC.h>
#include <driver/gpio.h>

static int s_pwr = -1;
static int s_prev[6] = {-1,-1,-1,-1,-1,-1};

static void resetAll() {
    SD_MMC.end();
    for (int i = 0; i < 6; i++) {
        if (s_prev[i] >= 0) { gpio_reset_pin((gpio_num_t)s_prev[i]); s_prev[i] = -1; }
    }
    if (s_pwr >= 0) { gpio_reset_pin((gpio_num_t)s_pwr); s_pwr = -1; }
    delay(30);
}

static void setPower(int pin, int level) {
    if (pin < 0) return;
    gpio_config_t cfg = {};
    cfg.pin_bit_mask  = (1ULL << pin);
    cfg.mode          = GPIO_MODE_OUTPUT;
    cfg.pull_up_en    = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en  = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type     = GPIO_INTR_DISABLE;
    gpio_config(&cfg);
    gpio_set_level((gpio_num_t)pin, level);
    s_pwr = pin;
}

static void listRoot() {
    File root = SD_MMC.open("/");
    if (!root || !root.isDirectory()) return;
    File f = root.openNextFile(); int n = 0;
    while (f && n++ < 8) {
        Serial.printf("  %s %s\n", f.isDirectory()?"DIR":"FILE", f.name());
        f = root.openNextFile();
    }
}

static bool try1bit(int pwr_pin, int pwr_level, int clk, int cmd, int d0) {
    resetAll();
    setPower(pwr_pin, pwr_level);
    s_prev[0]=clk; s_prev[1]=cmd; s_prev[2]=d0;
    delay(80);
    SD_MMC.setPins(clk, cmd, d0);
    for (int freq : {400, 1000, 4000}) {
        if (SD_MMC.begin("/sd", true, false, freq, 5)) {
            Serial.printf("*** 1-bit MOUNTED! PWR=GPIO%d(%s) CLK=%d CMD=%d D0=%d @ %dkHz\n",
                pwr_pin, pwr_level?"HIGH":"LOW", clk, cmd, d0, freq);
            return true;
        }
        SD_MMC.end(); delay(15);
    }
    return false;
}

static void try4bit(int pwr_pin, int pwr_level, int clk, int cmd, int d0) {
    static const int ALL_SD[] = {11, 14, 15, 16, 17, 18};
    int remaining[8]; int nr = 0;
    for (int p : ALL_SD)
        if (p!=clk && p!=cmd && p!=d0 && p!=pwr_pin) remaining[nr++] = p;

    Serial.printf("  4-bit: D1/D2/D3 from {");
    for (int i=0;i<nr;i++) Serial.printf("%d%s",remaining[i],i<nr-1?",":"");
    Serial.println("}");

    for (int a=0;a<nr;a++)
    for (int b=0;b<nr;b++) { if(b==a) continue;
    for (int c=0;c<nr;c++) { if(c==a||c==b) continue;
        resetAll();
        setPower(pwr_pin, pwr_level);
        s_prev[0]=clk; s_prev[1]=cmd; s_prev[2]=d0;
        s_prev[3]=remaining[a]; s_prev[4]=remaining[b]; s_prev[5]=remaining[c];
        delay(80);
        SD_MMC.setPins(clk, cmd, d0, remaining[a], remaining[b], remaining[c]);
        for (int freq : {400, 1000, 4000}) {
            if (SD_MMC.begin("/sd", false, false, freq, 5)) {
                Serial.printf("*** 4-bit MOUNTED! PWR=GPIO%d(%s) CLK=%d CMD=%d D0=%d D1=%d D2=%d D3=%d @ %dkHz\n",
                    pwr_pin, pwr_level?"HIGH":"LOW", clk, cmd, d0,
                    remaining[a], remaining[b], remaining[c], freq);
                listRoot();
                Serial.println("\n*** STOP — write down these pins! ***");
                while(true){delay(5000);Serial.println("holding...");}
            }
            SD_MMC.end(); delay(15);
        }
    }}
}

static const int SD_PINS[] = {11, 14, 15, 16, 17, 18};
static const int N_SD = 6;

// Power configurations to try: {pin, level} pairs. -1 = no power control.
struct PwrCfg { int pin; int level; const char* label; };
static const PwrCfg PWR_CFGS[] = {
    {-1,  0, "none (always-on)"},
    {39,  1, "GPIO39 HIGH"},
    {16,  1, "GPIO16 HIGH"},
    {15,  1, "GPIO15 HIGH"},
    {15,  0, "GPIO15 LOW"},
    {16,  0, "GPIO16 LOW"},
};
static const int N_PWR = sizeof(PWR_CFGS)/sizeof(PWR_CFGS[0]);

static int found_clk=-1, found_cmd=-1, found_d0=-1, found_pwr=-99, found_lvl=-1;

static bool sweep(const PwrCfg& pwr) {
    // Priority: CLK=16 + CMD=15 (the gap in v6)
    Serial.printf("\n[PWR=%s] CLK=16, CMD=15 priority sweep:\n", pwr.label);
    int d0_opts[] = {11, 14, 17, 18};
    for (int d0 : d0_opts) {
        if (try1bit(pwr.pin, pwr.level, 16, 15, d0)) {
            found_clk=16; found_cmd=15; found_d0=d0;
            found_pwr=pwr.pin; found_lvl=pwr.level;
            return true;
        }
    }
    // Full sweep: CLK=16 + all CMD/D0 from SD_PINS
    Serial.printf("[PWR=%s] CLK=16, full CMD/D0 sweep:\n", pwr.label);
    for (int ai=0;ai<N_SD;ai++) {
        int cmd=SD_PINS[ai]; if(cmd==16||cmd==pwr.pin) continue;
        for (int bi=0;bi<N_SD;bi++) {
            int d0=SD_PINS[bi]; if(d0==16||d0==cmd||d0==pwr.pin) continue;
            if (try1bit(pwr.pin, pwr.level, 16, cmd, d0)) {
                found_clk=16; found_cmd=cmd; found_d0=d0;
                found_pwr=pwr.pin; found_lvl=pwr.level;
                return true;
            }
        }
    }
    return false;
}

void setup() {
    Serial.begin(115200);
    delay(2500);
    Serial.println("\n=== Murphy M4 SD_MMC Probe v7 (hyper-targeted) ===");
    Serial.println("CLK=16 confirmed. Trying CMD=15 (missed in v6) + all power configs.\n");

    for (int i = 0; i < N_PWR && found_clk < 0; i++) {
        if (sweep(PWR_CFGS[i])) break;
    }

    if (found_clk >= 0) {
        Serial.printf("\n1-bit OK: CLK=%d CMD=%d D0=%d PWR=GPIO%d(%s)\n",
            found_clk, found_cmd, found_d0, found_pwr, found_lvl?"HIGH":"LOW");
        Serial.println("Searching 4-bit combos...");
        try4bit(found_pwr, found_lvl, found_clk, found_cmd, found_d0);
    } else {
        Serial.println("\n=== 1-bit not found. CLK may not be 16, or more power configs needed. ===");
    }
}

void loop() { delay(10000); Serial.println("idle"); }
