# ESP-IDF Dynamic Frequency Scaling

A minimal ESP32-S3 app that blinks an LED using **Dynamic Frequency Scaling (DFS)** and **Light Sleep** to reduce power consumption.

## Features
- CPU scales between **40 MHz** (idle) and **80 MHz** (active)
- Automatic **Light Sleep** when all tasks are blocked
- Simple GPIO blink with UART logging

## Requirements
- ESP-IDF v5.x or v6.x
- ESP32-S3 (or compatible ESP32 variant)

## Configuration
Run `idf.py menuconfig` and enable:

| Option | Path |
|---|---|
| Power Management | `Component config → Power Management` |
| Tickless Idle | `Component config → FreeRTOS → Kernel` |

## Build & Flash
```bash
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## How It Works
```mermaid
flowchart TD
    A[app_main starts] --> B[esp_pm_configure<br/>40–80 MHz + Light Sleep]
    B --> C[gpio_config LED pin]
    C --> D[Toggle LED]
    D --> E[vTaskDelay 1000ms]
    E --> F{All tasks blocked?}
    F -- Yes --> G[PM: Lower CPU to 40 MHz<br/>or enter Light Sleep]
    F -- No --> H[Stay at max 80 MHz]
    G --> I[Timer wakes CPU]
    H --> I
    I --> D
```

## Power States
| State | Frequency | Notes |
|---|---|---|
| Active | 80 MHz | During GPIO toggle and logging |
| Idle | 40 MHz | Between delays |
| Light Sleep | — | Entered automatically by PM |
```
