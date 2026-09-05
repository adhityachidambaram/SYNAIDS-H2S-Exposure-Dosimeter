# SYN-AIDS — H₂S Exposure Dosimeter Prototype

**Smart India Hackathon 2026 prototype**

SYN-AIDS is a wearable gas-exposure safety concept combining real-time gas monitoring with a camera-based sensing module for future colorimetric dosimetry.

## Prototype contents

### 1. Gas detection firmware
`firmware/gas_detection/Gas_Detection_Code.ino`

Current prototype firmware demonstrates:
- MQ-2 analog gas/smoke sensing
- SAFE / WARNING / DANGER thresholds
- 16×2 I²C LCD status display
- Green / orange / red status LEDs
- Buzzer alerts
- SIM800L SMS notification

> **Important:** The current firmware uses an MQ-2 sensor, which is a broad gas/smoke sensor and is **not a selective H₂S sensor**. It should not be presented as a calibrated H₂S concentration meter. For the final SYN-AIDS device, use an H₂S-specific sensing element and validate it with controlled calibration.

### 2. XIAO ESP32-S3 camera module
`firmware/xiao_esp32s3_camera/`

The supplied camera code is based on the ESP32 camera web-server implementation and includes:
- Camera capture
- JPEG/BMP capture endpoints
- MJPEG streaming
- Camera controls
- XIAO ESP32-S3 pin configuration
- Camera web interface assets

The board configuration explicitly selects `CAMERA_MODEL_XIAO_ESP32S3`.

## Hardware used in the current prototype

- Arduino-compatible controller for gas detection
- MQ-2 gas/smoke sensor
- 16×2 I²C LCD
- 3 status LEDs
- Buzzer
- SIM800L GSM module
- XIAO ESP32-S3 CAM for camera functionality

## Intended SYN-AIDS architecture

```text
H₂S atmosphere
      │
      ├── H₂S-specific sensor ──> Real-time alert
      │
      └── Lead-acetate strip
                    │
                    ↓
             XIAO ESP32-S3 CAM
                    │
                    ↓
             Colour analysis / AI
                    │
                    ↓
             Exposure estimation
```

## Getting started

### Gas detection firmware
1. Open `firmware/gas_detection/Gas_Detection_Code.ino` in Arduino IDE.
2. Install the libraries required by the sketch:
   - LiquidCrystal_I2C
   - SoftwareSerial
3. Replace `YOUR_PHONE_NUMBER` with the intended test number.
4. Connect the hardware according to the pin definitions in the sketch.
5. Upload and monitor the serial output at 9600 baud.

### Camera firmware
1. Open the camera project files in an ESP32 Arduino project.
2. Ensure the ESP32-S3 board configuration is selected.
3. Keep `CAMERA_MODEL_XIAO_ESP32S3` enabled in `board_config.h`.
4. Upload to the XIAO ESP32-S3 CAM.
5. Use the camera web interface to verify streaming/capture.

## Roadmap

- Replace MQ-2 with a validated H₂S-specific sensor.
- Build a controlled optical chamber for the lead-acetate strip.
- Capture a calibration dataset at known exposure conditions.
- Extract colour features from the strip ROI.
- Train and validate the exposure-estimation model.
- Integrate OLED, battery and wearable enclosure.
- Validate the complete system against a reference instrument before any real-world safety use.

## Safety

This repository contains a prototype for development and demonstration. It is **not a certified personal H₂S safety monitor** and should not be used as the sole protection against toxic-gas exposure.

## Project

**SYN-AIDS — AI-enabled H₂S Exposure Dosimeter**

Detect • Measure • Protect
