#!/usr/bin/env python3
# Auto-reconnecting serial monitor. Creates a timestamped log file per run.
# Usage:
#   python3 monitor.py            → logs to /tmp/probe_<timestamp>.txt
#   python3 monitor.py sd_mmc_v4  → logs to /tmp/probe_sd_mmc_v4_<timestamp>.txt
# Exit with Ctrl-C.
import serial, time, sys, os
from datetime import datetime

PORT = '/dev/cu.usbmodem2101'
BAUD = 115200

label = sys.argv[1] if len(sys.argv) > 1 else "run"
ts    = datetime.now().strftime("%Y%m%d_%H%M")
LOGFILE = f"/tmp/probe_{label}_{ts}.txt"

print(f"Monitoring {PORT} at {BAUD} baud")
print(f"Logging to {LOGFILE}")
print("Press Ctrl-C to exit\n")

with open(LOGFILE, 'w') as log:
    while True:
        try:
            with serial.Serial(PORT, BAUD, timeout=1) as s:
                sys.stdout.write("[connected]\n")
                sys.stdout.flush()
                while True:
                    line = s.readline()
                    if line:
                        text = line.decode('utf-8', errors='replace')
                        sys.stdout.write(text)
                        sys.stdout.flush()
                        log.write(text)
                        log.flush()
        except serial.SerialException:
            sys.stdout.write("[disconnected, waiting...]\n")
            sys.stdout.flush()
            time.sleep(0.5)
        except KeyboardInterrupt:
            print(f"\nExiting. Log saved: {LOGFILE}")
            break
