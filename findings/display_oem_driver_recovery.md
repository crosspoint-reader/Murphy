# OEM Display Driver Recovery

Date: 2026-05-26

This note records the deeper Ghidra pass that recovered the Murphy M3 display bus and the OEM UC8253 init/update flow from the stock firmware.

## Confirmed GPIO Dataflow

The important clue was not a string or a comment. It was the low-level byte writer:

```c
FUN_42038648(byte value) {
  repeat 8 times:
    digitalWrite(4, 0);
    digitalWrite(3, value & 0x80);
    digitalWrite(4, 1);
    value <<= 1;
  digitalWrite(4, 0);
}
```

That means:

| Signal | GPIO | Evidence |
| --- | ---: | --- |
| EPD MOSI / SDA | `3` | Byte writer shifts data bits on GPIO3. |
| EPD SCK / CLK | `4` | Byte writer clocks each bit with GPIO4 low/high. |
| EPD CS | `5` | Command wrapper pulls GPIO5 low around each byte. |
| EPD DC | `6` | Command wrapper drives GPIO6 low for command, high afterward. |
| EPD RST | `7` | Constructor/callsite stores reset pin as object field at offset `0x14`; reset helper pulses it. |
| EPD BUSY | `8` | Constructor/callsite stores busy pin as object field at offset `0x16`; waits loop while `digitalRead(BUSY) == 0`. |

The BUSY loop proves ready-high polarity:

```c
while (digitalRead(*(uint16_t *)(object + 0x16)) == 0) {
  delay(10);
}
```

## Constructor Chain

The display object constructor wrapper is `FUN_42038810`.

It wraps `FUN_42038280` and hard-codes the panel geometry:

```c
FUN_42038280(object, cs, dc, rst, busy, 0, ..., 0xf0, 0x1a0, ...);
```

`0xf0` x `0x1a0` is 240 x 416, matching the Good Display 3.7 inch panel.

The caller around `0x42022618` supplies the GPIO tuple that resolves to:

```text
CS=5 DC=6 RST=7 BUSY=8
```

The serial byte writer separately proves:

```text
MOSI=3 SCK=4
```

## OEM Init Sequence

Main init is `FUN_420389ec(object, mode)`.

For the default mode, the OEM sequence is:

```text
reset
wait BUSY ready-high

0x01: 03 10 3F 3B 0D
0x06: D7 D7 1F
0x04
wait power-on
0x00: FF
0x30: 09
0x61: F0 01 A0
0x82: 0F
0x50: 97
```

When `mode != 0`, the fifth byte of `0x01` and the `0x82` value change to `00`, but the working probe used the default `mode == 0` sequence above.

## Frame Write And Refresh

The OEM full-screen path is `FUN_42038cac`.

It does:

```text
FUN_42038b60(object)       ; init and LUT/table load
write image to 0x10        ; old/previous frame plane
write image to 0x13        ; new/current frame plane
FUN_4203885c(...)          ; commit/update wrapper
```

The refresh trigger path is:

```text
FUN_42038f74:
  if default mode:
    command 0x12
  else:
    command 0x17, data 0xA5
  wait "_Update_Fast"
```

Power handling:

```text
FUN_420389cc: command 0x04, wait "_PowerOn", mark powered
FUN_42038f24: command 0x02, wait "_PowerOff", mark unpowered
```

## LUT / Table Load Evidence

`FUN_42038b60` loads five table commands before frame writes:

```text
0x20: 42 bytes
0x21: 42 bytes
0x22: 42 bytes
0x23: 42 bytes
0x24: 42 bytes
```

There is also an alternate mode that uses mixed 56-byte and 42-byte tables and `0x17 0xA5` refresh.

The default and alternate table bytes have now been recovered from `analysis/segments/app0_seg0_3c190020.bin`; see [Display LUT And Refresh Path](display_lut_refresh.md). The current SDK port loads the default `0x20..0x24` LUT set before Murphy full-frame writes. The alternate table set is documented but not active yet because it belongs with the alternate init/refresh mode.

## Why We Missed It Initially

The first searches were biased toward strings, public board maps, and obvious GPIO constants. That found UI features and false public-reference leads, but not the display path.

The OEM driver has almost no helpful display strings near the pin setup. The winning path was:

1. Find the UC8253 command cluster (`0x01`, `0x06`, `0x61`, `0x82`, `0x50`, `0x10`, `0x13`, `0x12`).
2. Follow calls down to the command/data byte writers.
3. Observe direct `digitalWrite()` calls on GPIO3/GPIO4/GPIO5/GPIO6.
4. Follow the constructor fields back to CS/DC/RST/BUSY.
5. Confirm with the standalone GPIO3-8 probe on hardware.

So yes, the OEM binary had the answer. It just was not exposed by comments or strings; it required function-level dataflow from the low-level writer back to the constructor callsite.

## Related Artifacts

- `analysis/display_driver_core_decompile_20260526.md`
- `analysis/display_function_refs_20260526.md`
- `analysis/display_high_level_refs_20260526.md`
- `findings/display_gpio_recovery.md`
- `findings/display_bringup.md`
