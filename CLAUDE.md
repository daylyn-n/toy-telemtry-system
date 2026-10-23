# Telemetry Project

## Goals
- Learn embedded systems deeply by building a telemetry system end to end.
- Build experience for a communications role on my school's rover team.

## About me
- Somewhat comfortable in C/C++. Previous co-op: QA at NETGEAR.
- Embedded beginner: have blinked an LED bare-metal. Know *of* FreeRTOS, ESP-IDF, ESP-NOW, but haven't used them.
- No circuits/electronics background. Explain wiring and electrical concepts when they come up.

## Hardware
- STM32 Nucleo-F446RE (sensor node)
- 2x ESP32-S3 N16R8 (radio link)
- Elegoo electronics kit (possibly includes an IMU, e.g. GY-521 / MPU-6050)

## Planned architecture
IMU → (I2C) → STM32 → (UART) → ESP32 #1 → (ESP-NOW) → ESP32 #2 → (USB / WiFi) → PC

## Approach
- STM32 firmware is bare-metal (CMSIS register headers, no HAL/CubeMX), built with `arm-none-eabi-gcc`, `Makefile`, and `f446re.ld`; flashed with `st-flash`.

## Rules for Claude
- Answer only what I ask. Don't give more information than requested; I'll keep my questions detailed.
- One step at a time. Don't jump ahead to future stages of the project.
