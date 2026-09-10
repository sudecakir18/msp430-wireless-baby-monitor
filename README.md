
# 🎙️ MSP430 & Wi-Fi Based Wireless Baby Monitor System

An embedded wireless audio monitoring and alert system designed to detect ambient baby crying/sound thresholds and wirelessly trigger remote acoustic alarms via Wi-Fi[cite: 2].

---

## 📌 Project Overview
The system consists of two dedicated units: a **Transmitter Unit** and a **Receiver Unit**[cite: 2]. When ambient sound exceeds a defined amplitude threshold, the transmitter unit processes the analog signal and broadcasts an alert command over Wi-Fi via HTTP/UART to the receiver unit, which activates an acoustic buzzer and LED alert[cite: 2].

---

## 🛠️ Hardware & Components

| Component | Description / Function |
| :--- | :--- |
| **TI MSP430G2553** | Ultra-low power MCU running ADC sampling and UART control[cite: 2] |
| **MAX4466** | Electret microphone with adjustable gain for ambient sound capture[cite: 2] |
| **ESP8266 / ESP-01** | Wi-Fi module handling HTTP-based wireless communication[cite: 2] |
| **AMS1117-3.3** | Linear voltage regulator providing stable 3.3V power rails[cite: 2] |
| **Active Buzzer & LED** | Acoustic and visual alert indicators on the receiver side[cite: 2] |
| **Custom PCB** | Single-layer custom designed PCB (Altium Designer)[cite: 2] |

---
---

## ⚙️ Firmware Architecture & Logic

### 1. Transmitter Unit (`transmitter.c`)
- **Clock Configuration:** Calibrated internal DCO at 1 MHz (`CALBC1_1MHZ`)[cite: 2].
- **ADC Configuration:** Configured on Channel A5 (P1.5), `SREF_0` (Vcc/GND reference), 64-cycle sample-and-hold time (`ADC10SHT_3`)[cite: 2].
- **Threshold Detection:** Continuous sampling via `read_adc()`. If ADC value $> 750$, it asserts the LED (`P1.6`) and transmits ASCII `'1'` over UART at 9600 baud[cite: 2].
- **Timeout Mechanism:** A decrementing software timer keeps the alert active for ~2 seconds before sending `'0'` (idle state)[cite: 2].

### 2. Receiver Unit (`receiver.c`)
- Continuously polls the UART RX buffer (`UCA0RXBUF`)[cite: 2].
- When `'1'` is received $\rightarrow$ Drives `P1.0` HIGH to sound the Active Buzzer[cite: 2].
- When `'0'` is received $\rightarrow$ Drives `P1.0` LOW to silence the buzzer[cite: 2].

---

## 📂 Repository Contents
├── transmitter.c      # MSP430 firmware for sound sampling & UART Tx
├── receiver.c         # MSP430 firmware for alert polling & buzzer control
├── Babymonitor.pdf    # Complete academic engineering report & Altium schematics
└── README.md          # Project technical documentation
