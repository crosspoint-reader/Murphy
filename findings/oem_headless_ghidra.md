# OEM Firmware Headless Ghidra Notes

The machine does not need the Ghidra GUI for the current analysis. Homebrew Ghidra 12.1 is installed and the headless runner works:

```sh
/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless
```

The active Ghidra project is:

```text
analysis/ghidra-project/MurphyM3.gpr
```

The imported program is:

```text
app0.bin
```

## Scripts Added

- `tools/ghidra_scripts/FindCallsiteContexts.java`
  - Searches for named hardware APIs/strings and dumps direct references plus nearby instructions/decompile context.
- `tools/ghidra_scripts/FindHardwareConstants.java`
  - Broad mining helper for function/symbol names and instruction operands related to hardware keywords.

## Commands Run

Hardware API string/callsite pass:

```sh
/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless \
  /Users/jmitch/GitHub/Murphy_M3/analysis/ghidra-project MurphyM3 \
  -process app0.bin \
  -readOnly \
  -scriptPath /Users/jmitch/GitHub/Murphy_M3/tools/ghidra_scripts \
  -postScript FindCallsiteContexts.java \
  /Users/jmitch/GitHub/Murphy_M3/analysis/oem_hardware_calls.md \
  gpio_set_level gpio_config gpio_reset_pin __pinMode digitalWrite pinMode \
  spi_bus_initialize spi_bus_add_device i2c_param_config \
  ledc_channel_config ledc_timer_config i2s_set_pin i2s_driver_install i2s_write
```

Keyword mining pass:

```sh
/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless \
  /Users/jmitch/GitHub/Murphy_M3/analysis/ghidra-project MurphyM3 \
  -process app0.bin \
  -readOnly \
  -scriptPath /Users/jmitch/GitHub/Murphy_M3/tools/ghidra_scripts \
  -postScript FindHardwareConstants.java \
  /Users/jmitch/GitHub/Murphy_M3/analysis/oem_hardware_keyword_mining.md \
  gpio ledc i2c i2s spi pin touch display epd light audio busy mosi sclk dc rst
```

## Outputs

- `analysis/oem_hardware_calls.md`
  - Small, useful index of direct references to hardware API name strings.
- `analysis/oem_symbol_table_disasm.md`
  - Disassembly around suspected symbol/string-table regions.
- `analysis/oem_hardware_keyword_mining.md`
  - Large broad scan. Useful as a raw artifact, but noisy.
- `analysis/oem_hardware_constant_mining.md`
  - Very noisy partial broad scan from an intentionally broad run. Treat as scratch evidence only.

## What This Proves

The OEM firmware was built with Arduino-ESP32 / PlatformIO-style components. Embedded paths and strings include:

- `C:/Users/HZW/.platformio/packages/framework-arduinoespressif32/...`
- `.pio/libdeps/esp32-s3-devkitc-1/ESP32-audioI2S-3.0.12/src/Audio.cpp`
- `.pio/libdeps/esp32-s3-devkitc-1/ESP32-audioI2S-3.0.12/src/Audio.h`
- `touchTask`
- `displayOtaTask`
- `initPins`
- `Front Light`
- `Audiobooks`
- `ERROR: Unable to install I2S drives`

The firmware includes linked ESP-IDF / Arduino hardware APIs and driver strings for:

- GPIO: `gpio_set_level`, `gpio_config`, `gpio_reset_pin`, `__pinMode`
- SPI: `spi_bus_initialize`, `spi_bus_add_device`, Arduino SPI attach/detach helpers
- I2C: `i2c_param_config`, `i2c_driver_install`, `i2c_set_pin`, Arduino Wire errors
- LED PWM: `ledcSetup`, `ledc_channel_config`, `ledc_timer_config`
- I2S audio: `i2s_driver_install`, `i2s_set_pin`, `i2s_write`

## What This Does Not Prove Yet

The current Ghidra pass does not recover the Murphy display pin map.

The hardware API names are mostly referenced through string/symbol tables and IDF/Arduino driver code. The first useful xrefs for names such as `gpio_set_level`, `gpio_config`, `spi_bus_initialize`, `ledc_channel_config`, and `i2s_set_pin` point at table-like regions around `0x42041xxx`, not at clean OEM app callsites with literal GPIO values.

The large keyword pass mostly lands in linked framework/library code, especially Arduino GPIO and ESP32-audioI2S internals. It confirms that the capabilities are compiled in, but it does not yet tie a specific GPIO tuple to the e-paper panel, front light, touch controller, headphone output, or RTC.

## Current Interpretation

The stock binary confirms software capability and several UI/features:

- Touch support exists.
- Front-light UI exists.
- Audio playback code exists and uses the ESP32-audioI2S library.
- I2C, I2S, SPI, GPIO, and LEDC stacks are linked.

But the exact board wiring remains unproven by static Ghidra alone. Given the failed display probe matrix, the remaining likely blockers are:

- display control pins differ from public Good Display / CrowPanel / Waveshare references,
- an undiscovered display power-enable rail is required,
- or both.

The next highest-value route is still live signal capture from stock firmware during a known display refresh/front-light change. Use the OEM binary as the source of truth, and capture candidate lines rather than relying on public adapter pin maps.
