"""
fake_sensor.py

Stands in for the STM32 board until it's available. Generates fake
temperature readings on a timer, formatted the same way the real
board will eventually send them over UART (one number per line).

Run it directly to watch readings print to the console:
    python fake_sensor.py

Once the STM32 is reading the real internal temperature sensor and
sending values over UART, milestone 4 (the pyserial logger script)
will read from the serial port instead of this script's stdout -
the rest of the pipeline (database, API, dashboard) won't need to
change at all.
"""

import random
import time
from datetime import datetime

# Roughly mimics room temperature with small natural drift,
# rather than pure random noise every reading.
BASE_TEMP = 22.0
DRIFT_RANGE = 0.3       # how much the temp can wander per reading
NOISE_RANGE = 0.15      # small extra jitter for realism
READ_INTERVAL_SECONDS = 1.0


def generate_readings():
    current_temp = BASE_TEMP

    while True:
        # Let the "true" temperature wander slowly over time
        current_temp += random.uniform(-DRIFT_RANGE, DRIFT_RANGE)
        current_temp = max(15.0, min(35.0, current_temp))  # keep it sane

        # Add a bit of sensor-like noise on top
        reading = current_temp + random.uniform(-NOISE_RANGE, NOISE_RANGE)

        yield round(reading, 2)
        time.sleep(READ_INTERVAL_SECONDS)


if __name__ == "__main__":
    print("Fake sensor running. Press Ctrl+C to stop.\n")
    try:
        for value in generate_readings():
            timestamp = datetime.now().strftime("%H:%M:%S")
            print(f"{timestamp} TEMP:{value}", flush=True)
    except KeyboardInterrupt:
        print("\nStopped.")