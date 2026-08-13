# M4 hardware probes

These small PlatformIO targets record the hardware-discovery process. They are
not required to build or install CrossPoint. Several are destructive to the
ESP32 application partition and must only be used after making a verified full
flash backup.

The generic `i2c_scan` result on GPIO10/GPIO4 was an investigative dead end for
touch. The working factory-derived touch path is GPIO13/GPIO12 at address `0x2E`;
see [`../findings/touch.md`](../findings/touch.md).

Touch-controller firmware images and update experiments are intentionally not
part of this repository. Do not modify the controller firmware while developing
or installing the CrossPoint port.
