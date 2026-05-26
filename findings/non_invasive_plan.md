# Non-Invasive Recovery Plan

Current practical constraints:

- OEM USB runtime logs are not reliably accessible.
- The device cannot be opened.
- No internal pads, FPC pins, test points, or IC markings can be probed.

That means the remaining recovery path is software-only plus visible/audible behavior:

- static firmware analysis,
- public reference hardware comparison,
- USB-flashed custom probe firmware,
- screen changes,
- front-light changes,
- touch/button behavior,
- headphone/audio output,
- refresh sound/timing.

## Consequences

Without pad access, we cannot directly prove a GPIO-to-net mapping with a meter, scope, or logic analyzer. Pin recovery has to come from one of these:

1. OEM binary static analysis eventually reveals app-level constants.
2. A public reference board exactly matches the Murphy wiring.
3. A custom probe finds a visible effect.

The first option is slow because the OEM app is stripped and the current Ghidra xrefs mostly land in framework/string tables. The second option is currently disproven for display bring-up because the public CrowPanel, Good Display sample, and Waveshare-style mappings did not change the screen. The third option is now the most practical route.

## Safe Probe Strategy

Use narrow, reversible probes that exercise one subsystem at a time and leave unrelated pins as inputs.

General rules:

- Keep flash/PSRAM pins untouched.
- Keep USB pins untouched so recovery remains possible.
- Avoid random all-GPIO sweeps.
- Only test pin sets that come from OEM strings, public reference boards, ESP32-S3 board variants, or plausible panel/sample mappings.
- Start with low-speed bit-banged buses before hardware peripheral attachment.
- Print probe state over our own firmware serial, even though OEM logs are unavailable.
- Require visible confirmation before promoting any pin into the SDK board profile.

## Display

The display remains blocked. Tested mappings produced no visible change:

- CrowPanel-style: `SCK12 MOSI11 CS45 DC46 RST47 BUSY48`
- Alternate CrowPanel-style: `SCK12 MOSI11 CS10 DC9 RST21 BUSY48`
- Good Display A-alias: `SCK12 MOSI11 CS18 DC17 RST16 BUSY15`
- Good Display raw-numbered: `SCK12 MOSI11 CS17 DC16 RST15 BUSY14`
- Waveshare comparator: `SCK11 MOSI12 CS10 DC9 RST46 BUSY3`

Update: `GPIO48` is now confirmed as the Murphy front-light PWM pin, so any display tuple using `BUSY48` is invalid for this hardware. Keep those rows as historical public-reference tests only.

Non-invasive next steps:

1. Continue static OEM analysis for display init constants and GPIO setup clusters.
2. Run the expanded blind display probe in `/Users/jmitch/GitHub/crosspoint-reader-main/src/murphy_display_probe.cpp`.
3. Watch the screen while monitoring our custom serial output.
4. If the screen changes, record the `OBSERVE NOW` line for the active case.
5. Treat BUSY as advisory only. The expanded probe tests ready-high, ready-low, and ignore-BUSY modes because the X3/X4 family has already shown BUSY polarity differences. Since current BUSY candidates are stuck high/low and `GPIO48` is not BUSY, use visible screen change as the success signal.

Do not spend time on random pin brute force unless static analysis yields a bounded candidate list.

## Front Light

Front light is now confirmed non-invasively.

Custom probe result:

- Pin: `GPIO48`
- Polarity: active high
- PWM: 25 kHz works visibly
- Probe resolution: 10-bit duty, `0..1023`
- Negative controls: GPIO42 and GPIO41 did not turn the light on

Porting next steps:

1. Promote `GPIO48` into the Murphy board profile as the front-light PWM pin.
2. Use 25 kHz PWM to avoid audible PWM.
3. Start with a conservative 10-level brightness table and adjust by visual testing.
4. Exclude GPIO48 from future display BUSY, touch I2C, IRQ, RTC, and generic GPIO sweeps.

## Touch

Touch can be tested non-invasively with I2C scan firmware.

Good Display `GDEY037T03-FT21` points to FT6336U, commonly at `0x38`. The current custom probes have not found a valid touch bus. `SDA=13/SCL=12 @ 0x38` ACKs but returns static invalid data and does not respond to taps. Sweeps involving GPIO48 produced false positives because GPIO48 is front-light control.

Non-invasive next steps:

1. Run bounded I2C scans on plausible SDA/SCL pairs from public references and ESP32-S3 examples, excluding GPIO48.
2. Probe for `0x38` first.
3. If found, read FT6336-compatible registers and require tap-correlated register changes before accepting it as touch.
4. Do not assume touch shares the audio/RTC I2C bus until proven.

## Audio / RTC

The headphone jack and OEM firmware prove audio features exist, but the codec path remains unknown.

Non-invasive options:

- Run I2C scans for codec/RTC candidates on bounded pin pairs.
- If an ES8311-like codec at `0x18` is found, try the Waveshare comparator I2S pins only as a hypothesis.
- If a PCF85063-like RTC at `0x51` is found, document the I2C pair and test timekeeping.

Do not make audio a blocker for first display/input bring-up.

## Porting Impact

For `community-sdk` and CrossPoint:

- Keep Murphy display support behind an experimental board flag.
- Enable front light through the confirmed `GPIO48` active-high PWM backend.
- Implement digital buttons and app-level touch-target translation independently of display recovery where possible.
- Make all Murphy pins configurable in one board profile so probe results can be promoted without rewriting drivers.

The first usable port milestone should be:

1. Boots on Murphy M3.
2. Serial logs from custom firmware work.
3. Buttons are readable.
4. Front light is controlled on GPIO48.
5. Display remains experimental until a visible refresh succeeds.
