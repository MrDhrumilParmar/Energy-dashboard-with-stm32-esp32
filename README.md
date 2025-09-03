# STM32 + ESP32 Energy Monitoring Dashboard ⚡

An **RTOS-based dual-MCU project** showcasing **real-time data acquisition, signal processing, and IoT visualization**.  
Designed as a portfolio-grade project to demonstrate **embedded firmware, multitasking (RTOS), and system integration** skills.

---

## ⚡ Why This Project?
This project is more than just a data logger. It demonstrates:
- **RTOS Task Management** (on STM32)  
- **DMA-driven ADC Sampling** for real-time performance  
- **Signal Processing** (Vrms, Irms, Power, Power Factor, Temperature)  
- **Inter-MCU Communication** (UART between STM32 ↔ ESP32)  
- **IoT/Web Integration** (ESP32 with WebSocket + Chart.js Dashboard)  
- **Clean System Design** (separation of acquisition, processing, and visualization)  

---

## ⚡ System Architecture
      ┌─────────────────────┐
      │   STM32F446RE       │
      │  (Nucleo Board)     │
      │                     │
      │  • ADC + DMA        │
      │  • RTOS Tasks       │
      │  • Signal Processing│
      │      Vrms, Irms     │
      │      Power, Temp    │
      └─────────┬───────────┘
                │ UART
                ▼
      ┌─────────────────────┐
      │      ESP32          │
      │   (WiFi + Web)      │
      │                     │
      │  • UART Rx Parser   │
      │  • WebSocket Server │
      │  • HTML/JS Dashboard│
      └─────────┬───────────┘
                │ WiFi
                ▼
      ┌─────────────────────┐
      │   Web Browser       │
      │  (PC / Mobile)      │
      │  • Live Charts      │
      │  • Stats View       │
      └─────────────────────┘

---

## ⚡ Tech Stack
- **MCU 1 (STM32F446RE, Nucleo)**  
  - FreeRTOS for task scheduling  
  - ADC with DMA for fast sampling  
  - Processing tasks for Vrms, Irms, Power, Temp  
  - UART Tx for inter-MCU comms  

- **MCU 2 (ESP32)**  
  - UART Rx + packet parsing  
  - Async WebSocket Server  
  - Data pushed to Web Dashboard  

- **Frontend**  
  - Chart.js for real-time graphs  
  - HTML/CSS/JS for clean UI  

---

## ⚡ Skills Demonstrated
- RTOS design (task creation, synchronization, DMA-driven I/O)  
- Signal processing on MCU  
- Reliable inter-MCU communication (UART framing, error handling)  
- IoT integration with ESP32 and WebSocket server  
- Frontend visualization with Chart.js  

---

## ⚡ Demo
- Left Side: Real-time sine waveforms (Voltage, Current)  
- Right Side: Numeric stats (Power, Vrms, Irms, Temp)  
- Updates live via WebSockets  

