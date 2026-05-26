# Button Input And Combo Notes

## Actual Murphy Unit

The Murphy unit being analyzed does not have a rotary button/encoder. Earlier notes used the public CrowPanel 3.7-inch reference as a starting point; that reference exposes rotary-style previous/next/confirm inputs, but it does not match this Murphy unit's physical controls.

Confirmed physical controls on this unit:

| Control | GPIO | Behavior |
| --- | ---: | --- |
| Back / exit | GPIO1 | Digital active-low candidate from CrowPanel reference; verify on Murphy |
| Menu / home / lock key candidate | GPIO2 | Digital active-low candidate; sleep wake source in vendor reference |

The OEM firmware and/or board may still use direction/OK-style logical actions internally, but those actions must be reached through the actual buttons, touch targets, or hidden mappings rather than a physical rotary control.

## Public CrowPanel Reference

| Control | GPIO | Behavior |
| --- | ---: | --- |
| Back / exit | GPIO1 | Digital active-low |
| Menu / home / lock key candidate | GPIO2 | Digital active-low; sleep wake source in vendor reference |
| Rotary previous/up | GPIO6 | Digital active-low |
| Rotary next/down | GPIO4 | Digital active-low |
| Rotary confirm / OK | GPIO5 | Digital active-low |

Treat GPIO4/GPIO5/GPIO6 as reference-board candidates only until they are verified on the Murphy PCB or with USB logs.

## OEM Firmware Evidence

Recovered strings from `analysis/extracted/app0.bin` show these input features:

| String | Inference |
| --- | --- |
| `<Reset key map>` | OEM supports a configurable key map and a reset-to-default path. |
| `Invalid keyID: %u` | Key actions are represented by numeric IDs. |
| `Swapped key%d and key%d` | Key mappings can be swapped, likely through the `Custom keys` UI. |
| `Custom keys` | User-facing key customization menu item. |
| `Short press toggle, long press modify` | Some settings use short-vs-long press semantics. |
| `Hold up/down keys to adjust faster` | Repeated/held direction keys accelerate numeric adjustments. |
| `Lock screen long press` | Lock-screen behavior has a configurable long-press action. |
| Right-side button long press | Confirmed on hardware: both the top-right button and the lower button below it open front-light controls. |
| `Press lock key to stop tomato clock` | Lock key exits/stops the tomato timer mode. |
| `Use direction keys to adjust tomato time` | Direction keys adjust timer duration. |
| `Time sync failed, press lock key to return` | Lock key is used as a return/back action in at least one modal state. |
| `#LockClick`, `#ClickLock` | Runtime USB logs seen around lock/click actions. |
| `ClickDir %d`, `#ClickDIR: %d->%d` | Runtime/debug logging for direction changes. |
| `waitLockKeyRelease:%d/%dms` | Firmware explicitly waits for lock-key release and records elapsed time. |

Ghidra exports:

- `analysis/button_combo_string_refs.md`
- `analysis/button_combo_pointer_refs.md`
- `analysis/button_combo_disassembly.md`
- `analysis/button_combo_objdump_42000020.md`
- `analysis/button_combo_objdump_keymap.md`

## Combo Search Result

No explicit named multi-button debug/verbose combo has been recovered yet.

What is confirmed:

- Long-press handling exists.
- Direction-key hold acceleration exists.
- Lock-key release timing exists.
- Custom key mapping exists.
- Key swap/reset logic exists.
- Application input logs are emitted over USB for at least some input paths.

What is not yet proven:

- A hidden verbose/debug/logging combo.
- A boot-time key chord.
- A factory/test-mode chord in the OEM app.
- The exact firmware handler for the right-side-button long-press front-light shortcut.

## Practical Hardware Tests

Capture USB logs while testing:

1. Short press each key from the home screen.
2. Long press each key for 1, 2, 3, 5, and 10 seconds.
3. Hold `Menu/Home` during power-on.
4. Hold `Back/Exit` during power-on.
5. Hold pairs during power-on:
   - `Menu/Home` + `Back/Exit`
6. Try the same pair after boot, while already in the UI.
7. Enter `Custom keys`, change a mapping, then capture the logs and compare NVS before/after.
8. Enter the lock screen and test the `Lock screen long press` setting.
9. Press and hold each right-side button to enter front-light controls, then capture all USB output while changing brightness and returning.

Useful log strings to watch for:

- `#LockClick`
- `#ClickLock`
- `ClickDir`
- `#ClickDIR`
- `waitLockKeyRelease`
- `Invalid keyID`
- `Swapped key`

## Current Read For Verbose Logs

The best current read is that stock firmware has unconditional app-level input debug prints, not a discovered verbose flag. A hidden combo remains possible, but the binary string/xref pass did not surface a clear one.
