# Frontlight

The Murphy M4 has two independently controlled LED channels:

| Channel | GPIO | Drive |
|---|---:|---|
| Cool white | 47 | active-high PWM |
| Warm white | 48 | active-high PWM |

The port drives both channels at 25 kHz with 10-bit PWM. Brightness uses a
perceptual curve so its lowest non-zero setting maps to a hardware duty of one,
which provides a particularly dim night-reading level. The user interface offers
16 brightness steps; warmth remains adjustable from 0 to 100.

## Controls

- Swipe downward from the top edge to open the frontlight panel.
- Drag either vertical bar to set brightness or warmth.
- Tap the power icon to toggle the light.
- On the panel, the top side button increases the selected value by one step.
- The middle side button decreases the selected value by one step.
- A short press of the bottom button switches between brightness and warmth.
- Press the top and middle side buttons together anywhere to toggle the
  frontlight globally. The chord is consumed through release so it does not also
  trigger a page turn or menu movement.

The enabled state, brightness, and warmth are persisted in CrossPoint settings.
Entering sleep turns the LEDs off without discarding those settings; they are
restored after the next normal startup.

The panel icons are derived from the ISC-licensed Lucide icon set already vendored
by FreeInk SDK (`sun`, `sun-snow`, and `power`).
