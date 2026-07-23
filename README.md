# STM32 Sensor-to-Dashboard Pipeline

A full-stack embedded systems project: reading a live temperature sensor on an STM32 Nucleo board, streaming it over UART, and pushing it through a Python backend into a live web dashboard.

Built as a portfolio project to demonstrate the complete path from hardware register writes to a browser-rendered chart — firmware, serial communication, data storage, REST API, and frontend visualization.

![Dashboard Screenshot](docs/dashboard-screenshot.png)

## Overview

| Layer | Tech |
|---|---|
| Firmware | STM32F439ZI (Nucleo-F439ZI), bare-metal CMSIS register access |
| Serial link | USART3 @ 115200 baud |
| Data ingestion | Python 3.12, PySerial |
| Storage | SQLite |
| API | FastAPI + Uvicorn |
| Dashboard | HTML/CSS/JS, Chart.js |

Data flow:

```
STM32 ADC (internal temp sensor)
        │  raw 12-bit reading
        ▼
   USART3 @ 115200 baud   →   TEMP:<value>\r\n
        │
        ▼
  Python serial logger (get data.py)
        │  parses + timestamps
        ▼
   SQLite (tempdata.db → Temperature table)
        │
        ▼
  FastAPI (api.py → /readings endpoint)
        │  JSON
        ▼
  Dashboard (Chart.js, 5-min bucketed line chart)
```

## Project Structure

```
my_temp_project/
├── firmware/              # STM32CubeIDE project (CMSIS bare-metal)
│   ├── Src/
│   └── Inc/
├── pipeline/
│   ├── get data.py        # Serial reader → SQLite logger
│   ├── api.py              # FastAPI app (/readings)
│   └── server.py           # Static file server for the dashboard
├── dashboard/
│   ├── index.html
│   ├── style.css
│   └── script.js
└── tempdata.db             # SQLite database (generated at runtime)
```

## Hardware

- **Board:** ST Nucleo-F439ZI (STM32F439ZI)
- **Sensor:** Internal chip temperature sensor (ADC1, channel 18)
- **Connection:** USB (ST-LINK) for flashing + virtual COM port for UART

## Getting Started

### 1. Firmware

Open `firmware/` in STM32CubeIDE, build, and flash to the Nucleo board over ST-LINK. Confirm readings are streaming by opening a serial terminal (PuTTY or STM32CubeIDE's built-in terminal) on the board's COM port at **115200 baud**.

### 2. Python pipeline

```bash
cd pipeline
pip install pyserial fastapi uvicorn

# Start logging readings into SQLite (update COM port as needed)
py "get data.py"

# In a separate terminal, serve the API
py api.py
```

The API will be available at `http://localhost:8000/readings`.

### 3. Dashboard

```bash
cd pipeline
py server.py
```

Open `http://localhost:8001` in a browser to view the live dashboard.

> **Note:** the API (port 8000) and dashboard (port 8001) run on different ports, so CORS is enabled on the FastAPI app to allow the browser fetch to succeed.

## Key Engineering Notes

A few non-obvious things learned along the way:

- **UART baud rate:** the BRR register formula is `fCK / (16 × baud)` — forgetting the divide-by-16 gives a value 16× too large and silent output.
- **ADC enable:** `ADON` must be explicitly set; under HAL, `Init()` configures the ADC but doesn't start conversions without a separate start call.
- **Float printing:** `printf`/`sprintf` float support is disabled by default under `--specs=nano.specs` — fixed by adding `-u _printf_float` to the linker flags.
- **HAL + bare-metal don't mix well:** partially initializing a peripheral with HAL and then writing registers directly can cause conflicts (e.g. HAL's `MX_USART3_UART_Init()` corrupting a manually-set BRR value). Milestone 8 moves fully off HAL for this reason.
- **CORS:** different ports count as different origins in the browser, so cross-port `fetch()` calls between the dashboard and API need CORS middleware even when both are on `localhost`.

## Roadmap

- Finish ADC1 bare-metal conversion (calibration constants, data alignment/signedness)
- Add a live polling loop to the dashboard for real-time updates without manual refresh
- Possible enhancements: multi-sensor support, historical data export, alert thresholds

## License

MIT (or update as preferred).
