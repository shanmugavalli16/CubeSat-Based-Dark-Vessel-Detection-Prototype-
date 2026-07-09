#  Arduino Satellite Node for CubeSat-Based Dark Vessel Detection

## Overview

This repository contains the Arduino implementation of the **Satellite Node** used in the **CubeSat-Based Dark Vessel Detection Prototype**.

The Satellite Node acts as an onboard controller between the Vessel Node and the Ground Station. It continuously receives heartbeat packets from the vessel, determines the vessel status, reads orientation data from an MPU6050 sensor, generates a telemetry packet, and wirelessly transmits the packet to the Ground Station using the nRF24L01 transceiver.

---

# Project Objectives

- Receive heartbeat packets from the Vessel Node.
- Detect missing heartbeat packets (Dark Vessel Detection).
- Read satellite orientation using the MPU6050 sensor.
- Calculate Pitch, Roll, and Yaw angles.
- Generate a telemetry packet containing vessel status and orientation.
- Transmit telemetry wirelessly to the Ground Station.

---

# System Architecture

```
                Heartbeat
          (VESSEL_OK Packet)

        +-------------------+
        |   Vessel Node     |
        |   Arduino UNO     |
        |   nRF24L01 (TX)   |
        +---------+---------+
                  |
                  |
                  ▼
        +---------------------------+
        |     Satellite Node        |
        |       Arduino UNO         |
        |                           |
        | nRF24L01 (RX & TX)        |
        | MPU6050                   |
        |                           |
        | • Receive Heartbeat       |
        | • Detect Vessel Status    |
        | • Read MPU6050            |
        | • Calculate Orientation   |
        | • Generate Telemetry      |
        +-------------+-------------+
                      |
                      |
                      ▼
        +-------------------------+
        |    Ground Station       |
        |     Arduino UNO         |
        |     nRF24L01 (RX)       |
        +-------------------------+
```

---

# Hardware Used

| Component | Quantity |
|------------|---------:|
| Arduino UNO | 1 |
| MPU6050 | 1 |
| nRF24L01+ | 1 |
| AMS1117 3.3V Regulator | 1 |
| 10µF Capacitor | 1 |
| Breadboard | 1 |
| Jumper Wires | As Required |

---

# Features

- Heartbeat Reception
- Dark Vessel Detection
- MPU6050 Sensor Interface
- Orientation Estimation
- Wireless Telemetry Transmission
- Real-Time Serial Monitoring

---

# Working Principle

## Step 1 — Receive Heartbeat

The nRF24L01 is configured in **Receiver Mode**.

The satellite listens for heartbeat packets sent by the Vessel Node.

Example packet:

```
VESSEL_OK
```

If the heartbeat is received within the timeout period:

```
Vessel Status = OK
```

Otherwise:

```
Vessel Status = LOST
```

which indicates a potential **Dark Vessel**.

---

## Step 2 — Read MPU6050

The satellite communicates with the MPU6050 using the I²C protocol.

The following sensor values are collected:

- Accelerometer X
- Accelerometer Y
- Accelerometer Z
- Gyroscope X
- Gyroscope Y
- Gyroscope Z

---

## Step 3 — Calculate Orientation

Using the sensor readings,

- Pitch
- Roll
- Yaw

are calculated.

These values represent the orientation of the satellite.

---

## Step 4 — Generate Telemetry Packet

The satellite combines

- Vessel Status
- Pitch
- Roll
- Yaw

into one telemetry packet.

Example:

```
P:-12 R:3 Y:15 V:OK
```

or

```
P:-11 R:2 Y:18 V:LOST
```

---

## Step 5 — Transmit to Ground Station

The nRF24L01 switches to **Transmit Mode** and sends the telemetry packet to the Ground Station.

---

# Telemetry Packet Format

```
P:<Pitch> R:<Roll> Y:<Yaw> V:<Status>
```

Example:

```
P:-12 R:3 Y:15 V:OK
```

Field Description

| Field | Description |
|--------|-------------|
| P | Pitch Angle |
| R | Roll Angle |
| Y | Yaw Angle |
| V | Vessel Status |

---

# Libraries Used

Install the following Arduino libraries before uploading the code.

- nRF24L01

---

# Serial Monitor Output

Example:

```
SATELLITE READY

Received: VESSEL_OK

Pitch : -12
Roll  : 3
Yaw   : 15

Sending...

P:-12 R:3 Y:15 V:OK
```

If no heartbeat is received:

```
No Heartbeat Detected

Vessel Status : LOST

P:-10 R:2 Y:17 V:LOST
```

---

# Communication Flow

```
Vessel
   │
   │ Heartbeat
   ▼
Satellite
   │
   ├── Detect Vessel
   ├── Read MPU6050
   ├── Calculate Pitch
   ├── Calculate Roll
   ├── Calculate Yaw
   │
   ▼
Telemetry Packet
   │
   ▼
Ground Station
```

---

# Applications

- CubeSat Communication
- Maritime Surveillance
- Dark Vessel Detection
- Wireless Telemetry
- Remote Monitoring
- Satellite Communication Prototypes

---

# Future Improvements

- GPS Integration
- LoRa Communication
- AES Encrypted Telemetry
- SD Card Data Logging
- OLED Display
- Cloud Dashboard
- Real-Time Mapping
- Long-Range Communication

