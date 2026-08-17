# IoT GPS Tracking & Anti-Theft Security System

An embedded C++ firmware for Arduino Mega 2560 implementing a real-time GPS tracking and anti-theft system designed for IoT applications. The system integrates GPS/GSM cellular communication, RFID authentication, motion detection, and an OLED user interface into a single device.

---

## Overview

This project provides a complete hardware security solution combining:

- **Real-time GPS tracking** via a SIM908 GSM/GPS/GPRS cellular module
- **RFID-based authentication** for secure locking and unlocking
- **Vibration and motion detection** with graduated alarm escalation
- **6-axis IMU (accelerometer + gyroscope)** for orientation and movement analysis
- **OLED display** for live status visualization
- **Capacitive touch interface** for user interaction

---

## Hardware Platform

| Component | Details |
|-----------|---------|
| **Microcontroller** | Arduino Mega 2560 (ATmega2560 @ 16 MHz) |
| **GPS/GSM Module** | SIM908 Shield (DFRobot v3.0) — GSM/GPS/GPRS |
| **RFID Reader** | MFRC522 (SPI, ISO 14443 Type-A) |
| **IMU** | MPU-6050 (I2C, 6-DoF accelerometer + gyroscope) |
| **Display** | SSD1306 OLED 128x64 (I2C) |
| **Vibration Sensor** | Piezo sensor (analog, pin A6) |
| **Touch Input** | Capacitive touch sensor (analog, pin A3) |

### Pin Mapping

| Component | Pins | Protocol |
|-----------|------|----------|
| SIM908 GPS | D3, D4, D5 | UART @ 9600 baud |
| MFRC522 RFID | D49 (RST), D50 (MISO), D51 (MOSI), D52 (SCK), D53 (SS) | SPI |
| MPU-6050 IMU | D20 (SDA), D21 (SCL), addr 0x68 | I2C |
| SSD1306 Display | D20 (SDA), D21 (SCL) — shared I2C bus | I2C |
| Piezo Sensor | A6 | Analog input |
| Touch Button | A3 | Analog input |

---

## Software Architecture

### Build System

Built with **PlatformIO** targeting the `atmelavr` toolchain (`megaatmega2560` board) using the Arduino framework. Compiled with `-std=gnu++11 -Os` (size-optimized).

### Module Breakdown

#### `gps_sensor.h` — GPS/Cellular Communication

Manages all interaction with the SIM908 module via AT commands (Hayes modem protocol) over UART.

- `setup_gps_sensor()` — Initialize GPS, power on, wait for 2D/3D fix
- `read_gps()` — Request current GPS coordinates via AT commands
- `latitude()` / `longitude()` — Parse and return decimal-degree coordinates (Google Maps compatible)
- `decimalgps()` — Convert raw NMEA format (DDMM.MMMM) to decimal degrees
- `Datatransfer()` — Convert character buffers to floating-point values
- `check_gps_time()` — Periodic GPS polling (default interval: 120 seconds)
- `off_gps_sensor()` — Power down GPS to conserve energy

**Key AT Commands used:**
```
AT+CGPSPWR=1        // Power GPS on
AT+CGPSPWR=0        // Power GPS off
AT+CGPSRST=0        // Cold start reset
AT+CGPSSTATUS?      // Check fix status (2D Fix / 3D Fix)
AT+CGPSINF=32       // Request RMC sentence with coordinates
AT+HTTPINIT         // Initialize HTTP service
AT+HTTPTERM         // Terminate HTTP service
```

#### `http.h` — AT Command Handler & HTTP

- `sendATcommand()` — Core parser: sends AT commands, waits for expected response with timeout handling
- `http_init()` — Initialize HTTP service (stub)
- `http_term()` — Terminate HTTP service (stub)
- `http_post()` — HTTP POST data upload (stub, pending implementation)

#### `rfid.h` — RFID Authentication

Implements a master-key enrollment and validation system using the MFRC522 reader.

- `setup_rfid()` — First-boot master key enrollment (blocks until a card is presented)
- `save_key()` — Store 4-byte NUID of the master RFID card
- `validate_key()` — Compare presented card against stored NUID
- `check_RFID(status)` — Main polling loop (active only when device is locked)
- `lock()` / `unlock()` — State transition handlers
- `blocked()` — Lockout mode after maximum failed attempts (configurable, default: 4)

#### `imu.h` — Motion & Orientation Tracking

Interfaces with the MPU-6050 via I2C to compute pitch and roll using a complementary filter.

- `setup_imu()` — Initialize I2C, calibrate gyroscope (2000-sample average)
- `read_mpu_6050_data()` — Fetch raw 14-byte sensor data (accel XYZ, temp, gyro XYZ)
- `run_imu()` — Main loop: fuses gyroscope and accelerometer data (90% gyro / 10% accel) at 250 Hz

**IMU Configuration:**
- Accelerometer full scale: ±8g
- Gyroscope full scale: 500°/s
- Sample rate: 250 Hz (4000 µs per loop)

#### `display.h` — OLED User Interface

Drives the SSD1306 128x64 display using the U8g2 library.

- `display_developer()` — Debug overlay: shows lock state, RFID status, GPS signal, pitch/roll/yaw, sensor states
- `display_vibrating(level)` — Renders alarm state (0=none, 1=vibration, 2=parking alert, 3=theft)
- `display_lock()` / `display_unlock()` — Status-specific screens
- `display_blocked()` — Lockout screen
- `display_locked_attempt()` — Remaining unlock attempts counter

#### `vibration_sensor.h` — Alarm System

Monitors a piezo vibration sensor (analog threshold: 500/1023) with a 5-level state machine.

| State | Meaning |
|-------|---------|
| 0 | No vibration |
| 1 | Initial vibration detected |
| 2 | Parking alarm (5+ seconds continuous) |
| 3 | Theft alarm (20+ seconds continuous) |
| 4-5 | Alarm restarting / re-enabling |

Auto-resets after a 10-second timeout from the theft alarm state.

#### `touch_sensor.h` — Touch Button Interface

Capacitive touch sensor on A3 with multi-event detection.

| Event | Trigger | Action |
|-------|---------|--------|
| Click | Quick press/release | `ClickEvent()` — lock device if unlocked |
| Short Hold | 3 seconds | `ShortHoldEvent()` |
| Long Hold | 7 seconds | `LongHoldEvent()` |
| Stuck | 10+ seconds | `StuckEvent()` — requires release |

#### `serial.h` — Debug Output

Serial logging at 38400 baud covering all subsystems. Controlled globally by `developer_mode` flag.

---

## System States

The device operates across multiple modes controlled by the `general_status` variable:

| Mode | Description |
|------|-------------|
| Locked Riding | GPS logging every 120s, IMU monitoring, vibration armed |
| Locked Parking | Enhanced vibration sensitivity, theft detection active |
| Unlocked Active | User in possession, GPS active, reduced alarm sensitivity |
| Unlocked Idle | Device at rest, minimal power consumption |

---

## Libraries

| Library | Version | Purpose |
|---------|---------|---------|
| MFRC522 | v1.4.4 | RFID card reading (SPI) |
| U8g2 | — | Monochrome OLED graphics |
| Wire | Arduino built-in | I2C communication |
| SPI | Arduino built-in | SPI bus for RFID |

---

## Feature Status

| Feature | Status |
|---------|--------|
| GPS tracking (SIM908, AT commands) | Implemented |
| RFID lock/unlock with master key | Implemented |
| Vibration sensor alarm (3 levels) | Implemented |
| IMU pitch/roll with complementary filter | Implemented |
| OLED display UI | Implemented |
| Capacitive touch interface | Implemented |
| Serial debug output | Implemented |
| HTTP POST data upload | Stub — pending |
| Timer-based interrupts | Disabled |
| Bluetooth authentication | Planned |
| Fingerprint authentication | Planned |

---

## Author

**Federico Favia** — April 2019
