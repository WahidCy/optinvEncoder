# STM32 IR Encoder Speed Reader

This STM32 library allows you to read **rotational speed** (in RPM and Hz) from an **infrared encoder** using a timer in **Input Capture mode**. It’s lightweight, supports timeout detection, and is suitable for motors, rotating shafts, and encoder disks.

---

## 📌 Features

- Read speed in **RPM** and **frequency (Hz)**
- Uses **Timer Input Capture**
- Handles **timer overflow**
- Detects **encoder stop** via **timeout**
- Optional **anti-noise filtering**

---

## 🧰 Requirements

- STM32 microcontroller (F1, F4, or others with TIM input capture)
- STM32CubeMX or STM32CubeIDE
- HAL driver (not LL)
- IR encoder with digital pulse output (slot-based)

---

## 🔧 STM32CubeMX Configuration

1. **Enable a Timer** (e.g. TIM2)
    - Channel: **Input Capture - Direct TI**
    - Polarity: **Rising Edge**
    - Prescaler: `83` → for 1 MHz timer tick (if clock is 84 MHz)
    - Period (ARR): `65535`

2. **Enable NVIC interrupt** for TIMx (TIM2 global interrupt)

3. **GPIO**
    - Set encoder input pin to **Alternate Function**
    - Enable **Pull-down** resistor (optional, depending on encoder)

4. **Generate code**

---

## 📁 Project Structure

```
STM32_IR_Encoder_SpeedReader/
│
├── Core/
│   └── Src/
│       └── main.c               ← Usage example
│
├── lib/
│   └── ir_encoder/
│       ├── ir_encoder.h
│       └── ir_encoder.c
│
├── README.md                    ← You are here
```

---

## 🚀 How to Use

### 1. Add the Library

Copy the contents of `lib/ir_encoder/` into your project. Make sure the include path is set correctly.

### 2. Include and Initialize

```c
#include "ir_encoder.h"

IR_Encoder myEncoder;

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_TIM2_Init();  // Ensure this is set in CubeMX

    // Initialize encoder with:
    // &htim, TIM_CHANNEL_x, pulses per revolution, timeout in ms
    IR_Encoder_Init(&myEncoder, &htim2, TIM_CHANNEL_1, 20, 300);

    while (1) {
        IR_Encoder_CheckTimeout(&myEncoder);  // Set RPM to 0 if no pulses

        float rpm = IR_Encoder_GetRPM(&myEncoder);
        float freq = IR_Encoder_GetFrequency(&myEncoder);

        // Use as needed (e.g., send via UART)
        HAL_Delay(100);
    }
}
```

### 3. Implement Interrupt Callback

```c
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
        IR_Encoder_Update(&myEncoder);
    }
}
```

---

## 🧪 Output Example

```text
RPM: 150.23
RPM: 142.88
RPM: 0.00     ← when encoder stops
```

---

## 📘 API Reference

| Function | Description |
|----------|-------------|
| `IR_Encoder_Init(...)` | Initializes encoder system with timer config |
| `IR_Encoder_Update(...)` | Updates RPM on each input capture |
| `IR_Encoder_CheckTimeout(...)` | Checks if encoder has stopped moving |
| `IR_Encoder_GetRPM(...)` | Returns the calculated RPM |
| `IR_Encoder_GetFrequency(...)` | Returns the signal frequency in Hz |

---

## 🔁 Use Cases

- Motor speed control
- Wheel/shaft RPM measurement
- DIY tachometers
- Encoder disk readers

---

## 📜 License

This project is released under the **MIT License**. Free to use and modify.

---

## 🙌 Credits

Based on STM32 HAL and CubeMX, inspired by *Mastering STM32* by **Carmine Noviello** and ST's official documentation.

Feel free to contribute, star the repo, or fork for your own project!
