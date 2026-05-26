# Online Research Notes

Research date: 2026-05-25

## HamGeek M3 Product Listing

Source:

`https://www.hgeek.com/products/hamgeek-m3-portable-e-book-reader-3-7-inch-touch-screen-e-paper-10-level-adjustable-backlight-clock-e-ink-display`

The HamGeek listing appears to describe the same Murphy/HamGeek M3-class product.

Useful claims from the listing:

- 3.7-inch touch-screen e-paper reader.
- 10-level adjustable backlight/front light.
- Image mode uses `416 * 240` resolution.
- TXT and EPUB reading.
- JPG/BMP image viewing.
- MP3/WAV/AAC/FLAC music playback.
- Wired headphones.
- WiFi wireless upload, USB transfer, and card-reader copying.
- OTA online and offline updates.
- Seller claims a built-in independent clock chip for offline time/calendar use.

Impact:

- The listing reinforces our firmware evidence for touch, audio playback, image viewing, WiFi upload, OTA, weather, alarm, and EPUB support.
- The "independent clock chip" claim makes an external RTC more plausible than the firmware-string pass alone suggested. It is still not proven at IC/address level.
- The "backlight" wording is likely a front-light assembly over the e-paper panel.

## Elecrow CrowPanel 3.7-Inch Reference

Source:

`https://www.elecrow.com/wiki/CrowPanel_ESP32_E-paper_3.7-inch_HMI_Display.html`

Useful facts:

- ESP32-S3-WROOM-1-N8R8.
- 8 MB PSRAM.
- UC8253 display driver.
- 240(H) x 416(V) resolution.
- SPI e-paper interface.
- SD card on separate SPI pins: MOSI GPIO40, MISO GPIO13, CLK GPIO39, CS GPIO10.
- Buttons: MENU GPIO2, EXIT GPIO1, rotary down GPIO4, rotary up GPIO6, rotary confirm GPIO5.
- GPIO header exposes GPIO3, GPIO9, GPIO15, GPIO17, GPIO19, GPIO21, GPIO8, GPIO14, GPIO16, GPIO18, GPIO20, GPIO38.
- Official resources link to the same GitHub/schematic material already cloned under `analysis/vendor/`.

Impact:

- Confirms our existing board profile assumptions for the non-touch CrowPanel reference.
- Does not document touch pins or touch controller for the 3.7-inch CrowPanel reference.

## Good Display 3.7-Inch Touch/Front-Light Panels

Touch-only source:

`https://www.good-display.com/product/474.html`

Touch/front-light source:

`https://www.good-display.com/product/656.html`

Useful facts:

- `GDEY037T03-T02`: 3.7-inch 416x240 UC8253 e-paper panel with capacitive touch.
- `GDEY037T03-FT21`: 3.7-inch 416x240 UC8253 e-paper panel with touch screen and front light.
- Good Display lists the touch controller as `FT6336U`.
- Touch interface is I2C.
- Touch connector is 6-pin FPC.
- Touch voltage range is 2.8 V to 3.6 V.
- Touch operating current listed around 4.32 mA; standby current around 55 uA.
- Touch-only page lists the touch pin order as: GND, VDD, RST, INT, SDA, SCL.
- Front-light variant lists 9 front-light LEDs and a 2.8 V to 3.3 V front-light operating voltage.
- Front-light variant lists maximum front-light current as 60 mA.
- Good Display publishes ESP32 sample code for the touch/front-light panel.

Impact:

- This is the strongest external hardware lead so far. It does not prove the Murphy panel is Good Display `GDEY037T03-FT21`, but it matches the resolution, UC8253 controller, touch, and front-light/backlight claims.
- `FT6336U` should move to the top of the touch-controller probe list.
- Candidate touch driver should support FocalTech/FT6x36 register layout first, while retaining runtime probing for other controllers.
- The front-light current rating means the ESP32-S3 should be treated as controlling a driver/transistor input, not powering the LED rail directly.

Live Murphy testing now confirms the board-level front-light control pin is `GPIO48`, active high, with 25 kHz PWM working. This supersedes the public CrowPanel GPIO42 front-light lead and the public CrowPanel GPIO48 display-BUSY lead for this Murphy unit.

## Current Best External Hypotheses

| Area | Best online lead | Confidence |
| --- | --- | --- |
| Touch controller | FocalTech `FT6336U`, I2C | Medium as a panel-family lead; not proven on Murphy PCB. `SDA=13/SCL=12 @ 0x38` returns static invalid data. |
| Touch panel wiring | 6-pin touch FPC: GND, VDD, RST, INT, SDA, SCL | Medium for the panel family; Murphy board GPIO mapping unknown. |
| Front light | 3.7-inch touch/front-light e-paper assembly, likely 9 LEDs, controlled by Murphy `GPIO48` | High for board control pin; LED current path still not physically measured. |
| External RTC | Seller claims independent clock chip | Low/medium; firmware still needs I2C/device proof. |

## Follow-Up Searches / Downloads

- Retrieve Good Display `GDEY037T03-FT21 ESP32 Sample Code` and inspect its touch driver and pin defaults.
- Retrieve Good Display `IC Driver FT6336U Datasheet`.
- Try to locate board photos or teardown images showing the Murphy M3 PCB and panel/FPC markings.
- Search AliExpress/1688/Taobao listings for the same `100504`, HamGeek M3, Murphy M3/M4, or Corogoo reader SKU.
