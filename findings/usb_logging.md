# USB Logging And Console Notes

## Confirmed Runtime Evidence

The stock OEM firmware emits application logs over USB by default.

Observed over USB from a running device:

| Message | Matching firmware string |
| --- | --- |
| `#ClickLock` | `analysis/app0.strings.txt` offset `0x2b1f` |
| `>>>Count:4 Time:3ms` | Format string `>>>Count:%d Time:%dms` at offset `0x1464` |

This confirms the USB path is not limited to bootloader output or mass-storage/card-transfer behavior. At least some application-level `printf`/log output reaches the host over USB on the stock image.

Current practical constraint: these logs are not reliably accessible on the available setup. Treat OEM USB logs as opportunistic evidence, not as a required input for display/front-light/audio bring-up.

## Firmware Evidence

Relevant strings in `analysis/extracted/app0.bin`:

| Area | Strings |
| --- | --- |
| Arduino USB | `USB.cpp`, `ESPUSB`, `ARDUINO_USB_EVENTS`, `arduino_usb_events` |
| TinyUSB | `TinyUSB MSC`, `TinyUSB Device`, `tinyusb_driver_install`, `tinyusb_enable_interface` |
| USB CDC/JTAG | `HW USB CDC failed to init interrupts`, `usb_switch_to_cdc_jtag`, `vfs_usb_serial_jtag.c`, `usb_serial_jtag_return_char` |
| Console/VFS | `/dev/console`, `/dev/console/`, `Failed to register vfs console` |
| UART fallback | `HardwareSerial.cpp`, `uart_write`, `uart_write_bytes`, `/dev/uart` |
| ESP logging | `/IDF/components/log/log.c`, `get_cached_log_level`, `esp_log_level_set` |

## Interpretation

The stock firmware likely initializes a USB console or USB CDC/JTAG logging path for at least part of application runtime. This makes USB serial capture a useful reverse-engineering tool before any binary patching.

The logs seen so far look like application debug prints, not only ESP-IDF `E/W/I` logs. That is useful because touch, key, timing, and UI paths may already emit high-level state transitions.

## Verbosity / Debug Flag Search

A runtime verbosity flag is possible, but not proven.

Current evidence:

- No readable `verbose`, `debug`, `trace`, `log`, `console`, `uart`, or `usb` key was found in `analysis/extracted/nvs.bin`.
- Readable NVS strings currently look like normal WiFi/AP/eeprom data, including `eeprom`, `misc`, `sta.ssid`, `sta.pswd`, `ap.ssid`, and `ap.passwd`.
- The app links ESP-IDF log-level code (`esp_log_level_set`, `get_cached_log_level`), but those strings are generic framework symbols and do not prove an OEM setting toggles verbosity.
- The observed app debug string `>>>Count:%d Time:%dms` has code references in Ghidra export `analysis/usb_logging_string_refs.md`.
- The observed `#ClickLock` string currently has no clean direct xref, likely because it is reached through a table or raw string path.

Most likely options:

| Mechanism | Current read |
| --- | --- |
| Compile-time debug prints | Plausible; some app debug strings are always emitted. |
| Hidden UI/key combo enabling more logs | Plausible, not proven. |
| NVS verbosity key | Not found in readable NVS strings. |
| ESP-IDF global log level changed at runtime | Possible, but no OEM control path recovered yet. |
| Web UI debug endpoint | No obvious debug endpoint found in the embedded web UI string pass. |

Things to try on hardware:

1. Capture logs during boot, normal UI use, and factory reset screen entry.
2. Try long-hold combinations while connected to USB: top-right/back, power/menu, and available two-button combinations during boot.
3. Enter hotspot/web UI mode and check whether any undocumented endpoint responds, especially simple names like `/Debug`, `/Log`, `/Read_Log`, `/Set_Log`, `/Config`, or `/Factory`.
4. Trigger errors intentionally but safely, such as missing SD card, bad WiFi credentials, or failed weather sync, and see whether ESP-IDF `E/W/I` logs appear.
5. Compare logs before and after factory reset to see whether persisted settings affect output.

## Capture Workflow If Available

On macOS:

```zsh
before=$(ls /dev/cu.* | sort)
# plug in or reset the device
after=$(ls /dev/cu.* | sort)
comm -13 <(printf '%s\n' "$before") <(printf '%s\n' "$after")
```

Then monitor the new device:

```zsh
screen /dev/cu.usbmodemXXXX 115200
```

If output is garbled or absent, try common ESP serial rates:

```zsh
screen /dev/cu.usbmodemXXXX 460800
screen /dev/cu.usbmodemXXXX 921600
```

Useful capture command:

```zsh
python3 -m serial.tools.miniterm /dev/cu.usbmodemXXXX 115200 --raw | tee analysis/oem_usb_runtime.log
```

## Reverse-Engineering Use

Exercise one feature at a time while capturing logs:

- Key presses and long presses.
- Lock/unlock.
- Touch events and touch-area reset.
- Front-light brightness changes.
- Audio playback start/stop.
- Clock/alarm/weather screens.
- SD card insertion/removal.
- USB card-transfer mode.

For front-light work, capture the exact log lines while changing from level 0 through level 10. If the OEM logs print setting indices or timing counters near brightness changes, they may give us the settings-table index or state variable needed to find the handler in Ghidra.

## No-Log Fallback

If OEM USB logs are unavailable, use these paths instead:

1. Static firmware mining with headless Ghidra outputs under `analysis/oem_*`.
2. Live electrical observation while stock firmware runs: meter/scope/logic analyzer on candidate GPIOs and panel/front-light rails.
3. Controlled custom firmware probes that print their own serial output and sweep only one subsystem at a time.
4. Visual behavior as the signal: display refresh noise, front-light brightness changes, touch interrupt transitions, and headphone output.

For display and front-light recovery, lack of USB logs is annoying but not blocking. Electrical state changes during stock firmware are more valuable than log text because they directly reveal which ESP32-S3 pins are connected.

## Binary Patch Impact

Because stock USB logs are only partially/irregularly accessible, patching the image to force more logs is still not the first priority. It is high-risk compared with writing targeted probe firmware or measuring pins under the stock firmware.

Better next steps if logs are available:

1. Capture a complete USB log while using each UI path.
2. Search observed log strings in `analysis/app0.strings.txt`.
3. Use Ghidra xrefs from those strings to locate the surrounding handlers.
4. Only patch logging if a specific path is silent, the hook is clearly identified, and the patch can be tested without risking boot loops.

If patching becomes necessary, the safer target is adding or redirecting specific application debug prints, not changing global USB initialization.
