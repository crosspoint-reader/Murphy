# Clock, Time Sync, And RTC

## Summary

The firmware has a visible clock/calendar/alarm feature set. The strongest evidence points to network-synchronized system time using NTP, with ESP32-S3 internal RTC/timer domains used for normal ESP-IDF timekeeping and sleep behavior.

There is not yet strong evidence of an external battery-backed RTC chip. The available Elecrow CrowPanel schematic does not show an RTC IC, a 32.768 kHz RTC crystal, or a coin-cell/backup domain dedicated to RTC. Because the physical Murphy M3 differs from that schematic in at least one known way, the absence of RTC evidence in the schematic is not final proof.

## Firmware Evidence

Clock and alarm UI strings:

- `Clock`
- `Calendar`
- `Alarms`
- `Modify Alarm`
- `Clock Ref Interval`
- `Time Format 24H`
- `Adjust Time`
- `Time Font Style`
- `Time direction`
- `Press lock key to stop tomato clock`
- `Use direction keys to adjust tomato time`
- `Lock timeout`

Time synchronization strings:

- `syncTimeTask`
- `updateTime`
- `Connecting NTP server`
- `Time sync successful`
- `Time sync failed`
- `Failed to obtain time`
- `Btime2.cloud.tencent.com`

The `Btime2.cloud.tencent.com` server string is the clearest recovered NTP/server hint. The UI includes manual time adjustment, so the clock may continue operating without WiFi after initial setup.

## Internal ESP32 RTC Evidence

The binary includes ESP-IDF RTC-related symbols and strings such as `rtc_clk`, `rtc_sleep`, `RTCIO`, and `RTCRAM`. These are normal ESP32-S3 platform facilities. They prove the firmware uses the ESP32 RTC/sleep subsystem, but they do not prove an external RTC part is populated.

On ESP32-S3, the internal RTC domain can keep time while the chip is powered and can support deep-sleep wake behavior. It is not the same as a standalone battery-backed wall-clock IC such as DS3231, PCF8563, BM8563, or RX8025.

## External RTC Status

No strong external RTC markers were recovered from the current firmware and vendor-reference pass:

- No clear `DS3231`, `PCF8563`, `BM8563`, `RX8025`, or similar RTC part strings.
- No clear RTC I2C address evidence recovered yet.
- No RTC-specific NVS key names found in the simple NVS string scan.
- The Elecrow CrowPanel reference schematic does not list a dedicated RTC chip or 32.768 kHz RTC crystal.

This leaves three plausible interpretations:

- The clock is NTP-set system time retained while the ESP32-S3 remains powered or asleep.
- The user can manually adjust time, and firmware stores/restores enough state to display a clock after reboot, with drift handled by internal timers while powered.
- The Murphy M3 hardware includes an external RTC that is not visible in the current schematic/reference material and not obvious from strings.

## What We Do Not Know Yet

- Whether clock time survives full battery disconnect.
- Whether alarms can wake the device from deep sleep.
- Whether there is an external RTC IC on the Murphy PCB.
- If an RTC exists, its part number, I2C address, interrupt pin, and backup-power source.
- How `Clock Ref Interval` maps to NTP refresh or UI refresh behavior.
- Whether time zone selection is stored in NVS, LittleFS, or app settings.

## Recommended Hardware Checks

- Inspect the PCB for RTC part markings near the battery/power-management area and any low-speed crystal.
- Search visually for a 32.768 kHz crystal or tiny tuning-fork crystal package.
- Run an I2C scan on known/likely I2C buses after identifying touch/display/codec pins. Look for common RTC addresses such as `0x51`, `0x68`, and related variants.
- Probe for a square-wave or interrupt line from a suspected RTC to the ESP32-S3.

## Recommended Behavioral Tests

- Disable WiFi, set the time manually, power-cycle the device on battery, and record whether time persists.
- Disconnect USB and main battery long enough to fully remove power, reconnect, and see whether wall-clock time survives.
- Set an alarm, let the device sleep, and measure whether it wakes without WiFi.
- Capture DNS/NTP traffic during time sync and confirm whether `time2.cloud.tencent.com` or `Btime2.cloud.tencent.com` is contacted as written in firmware strings.
- Compare drift over several hours without network access.

## Porting Impact

For `community-sdk` and `crosspoint-reader-main`, assume no external RTC until the PCB proves otherwise. The initial port should use WiFi/NTP plus ESP32-S3 internal timekeeping. Keep the time provider behind a small board abstraction so an external RTC driver can be added later without rewriting application clock logic.

Avoid assigning unknown I2C pins permanently until touch, audio codec, and possible RTC devices are mapped.

