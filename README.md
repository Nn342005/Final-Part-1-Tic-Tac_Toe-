# Tic-Tac-Toe on ESP32 with MQTT (GCP)

This project is a terminal-based Tic-Tac-Toe game that supports:

* **1 Player Mode**: Human vs. Bash Bot (runs on GCP)
* **2 Player Mode**: Human vs. Human (local)
* **Auto Mode**: C Bot vs. Bash Bot (both run on GCP)

The game is built in **C** and designed to run on an **ESP32**, with game state displayed via a **Text User Interface (TUI)** on the laptop. Communication between components is done using **MQTT** via a Mosquitto broker on a GCP VM.

## Files Included

* `main.c`: Entry point, manages menu and game flow.
* `game.c`, `game.h`: Game logic (board handling, win/draw detection, etc).
* `botX.c`: C bot that automatically plays as Player X (Option 3).
* `playerO.sh`: Bash bot that simulates Player O (Option 1 and 3).
* `ticgame.exe`: Compiled game executable (Windows).

## Requirements

* ESP32 (wired and flashed)
* Mosquitto MQTT Broker running on a GCP VM
* MQTT tools: `mosquitto_pub`, `mosquitto_sub`
* Bash, GCC, `make`, Windows CMD or terminal

## How to Run

### Option 1: 1 Player Mode (Player vs Bash Bot)

```bash
./ticgame.exe
# Select Option 1
```

Then manually run this on the GCP VM:

```bash
./playerO.sh
```

### Option 2: 2 Player Mode (Local)

```bash
./ticgame.exe
# Select Option 2
```

Play locally using numbers 1-9 as grid input.

### Option 3: Auto Mode (C Bot vs Bash Bot)

```bash
./ticgame.exe
# Select Option 3
```

Both bots run automatically on the GCP VM and play to completion.

---

## Notes

* The game publishes the board state to MQTT topic `tictactoe/board`
* Bots subscribe and send moves via `tictactoe/moveX` and `tictactoe/moveO`

---

##  Submission Checklist

* [x] All code files included
* [x] Bash script bot provided
* [x] C bot provided
* [x] Game runs in all three modes

---

## Author

**nguyennam342005**

ESP32 + GCP MQTT Tic-Tac-Toe Final Project
