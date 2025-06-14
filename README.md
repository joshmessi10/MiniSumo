# 🤖 Minisumo Robot - Multi-Mode Strategy

This project implements the embedded control of an **autonomous Minisumo robot**, designed to compete in sumo matches. The robot is capable of searching, attacking, and evading its opponent using proximity and infrared sensors, with selectable strategies via physical switches.

---

## 📋 Key Features

- 🧠 **Finite State Machine (FSM)** to manage modes: search, attack, and evade.
- 🔄 **Four selectable strategies** using switches (`SW1` and `SW2`).
- 📉 **Automatic floor sensor calibration** at startup.
- 👁️ **Infrared proximity and line sensors** for opponent and ring edge detection.
- ⚡ **PWM speed control** for precise maneuvers.
- 🛡️ **Smart evasion** to prevent falls.

---

## 🧰 Required Hardware: [MiniBlack - Minisumo Robot at JA-Bots](https://ja-bots.com/producto/minisumo-miniblack/)

- MiniBlack control board (1 unit): 5A per channel driver, ideal for efficient motor handling.
- JS40F sensors (5 units)
- White line sensors (2 units)
- FingerTech Gearmotor – Silver Spark 22.2:1 (2 units)
- AVR programmer (1 unit)
- Startup module (1 unit)

---

## 🎮 Strategy Modes

| SW1 | SW2 | Mode | Description                                  |
|-----|-----|------|----------------------------------------------|
| 0   | 0   | A    | Linear search + spin when detected           |
| 0   | 1   | B    | Search + reverse and turn                    |
| 1   | 0   | C    | Curved search (differential motors)          |
| 1   | 1   | D    | Same as C + smart evasion                    |

---

## 🔍 Robot States

### 1. `SEARCHING`
- The robot spins or moves forward until an opponent is detected using Sharp sensors.

### 2. `ATTACK`
- Moves directly toward the opponent.
  - If both front sensors are triggered: move forward.
  - If one sensor is triggered: turn toward the opponent.

### 3. `EVADE`
- If the edge of the dohyo is detected:
  - Quickly move backward.
  - Turn away from the edge.
  - Execute a random maneuver if necessary.

---

## ⚙️ Floor Calibration

- Upon startup, for 3 seconds:
  - Reads floor sensors to obtain max and min values.
  - Automatically calculates an average threshold.
- This allows adaptation to different rings without reprogramming.

---

## 🧪 Main Functions

- `robot_buscandoX()`: Different strategies based on mode (A-D).
- `robot_atacar()`: Direct attack to the opponent.
- `robot_evadir()`: Fall prevention with escape maneuvers.
- `lecturaSensores()`: Updates IR sensors and switch states.
- `calibrarSensores()`: Automatic line sensor calibration.

---

## 📦 Code Structure



```
main.c
├── setup() // Port, timer, and PWM initialization
├── loop()
│ ├── calibrarSensores()
│ ├── lecturaSensores()
│ └── switch (state)
│ ├── SEARCHING: robot_buscandoX()
│ ├── ATTACK: robot_atacar()
│ └── EVADE: robot_evadir()

```

## 📸 Images

Participating in the Robotic People League SOFA 2023

<img src="(https://github.com/user-attachments/assets/f92229dd-8bef-4827-8bb9-6a8f84f9b029" width="200"/>

https://github.com/user-attachments/assets/cb2c7059-16a6-4471-b555-46d8da943084



---

## 👥 Credits

Josh Sebastián López Murcia  
