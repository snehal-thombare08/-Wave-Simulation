# 🌊 Wave Simulation

A real-time 2D wave propagation simulator built with **C++ and SFML 3.0**, featuring interactive ripple physics with multiple visual modes.

## screenshot
https://raw.githubusercontent.com/snehal-thombare08/-Wave-Simulation/main/Screenshot%202026-06-17%20162457.png

## ✨ Features

- **Real-time wave physics** using finite difference method
- **3 Visual Modes** — Ocean Blue, Lava, Psychedelic
- **Interactive drops** — click anywhere to create ripples
- **Rain mode** — random drops across the surface
- **Storm mode** — intense multi-drop wave chaos
- **Wave interference** — watch ripples collide and interact
- Pixel-perfect rendering at 1200×800

## 🎮 Controls

| Key | Action |
|-----|--------|
| LMB (drag) | Create wave drop |
| `1` | Ocean Blue mode |
| `2` | Lava mode 🌋 |
| `3` | Psychedelic mode 🌈 |
| `R` | Toggle Rain |
| `S` | Toggle Storm |
| `Space` | Clear all waves |
| `Esc` | Quit |

## 🚀 How to Run

1. Download **WaveSimulation-v1.0-Windows.zip**
2. Extract all files to a folder
3. Run **WaveSimulation.exe**

> No installation required. All DLLs included.

## 🛠️ Build from Source

**Requirements:**
- C++17 compiler (MinGW-w64)
- SFML 3.0 (via vcpkg)
- CMake 3.16+

```bash
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -G "MinGW Makefiles"
mingw32-make
```

## 🔬 Physics

Uses the **2D wave equation** with finite differences:

```
u[t+1] = 2*u[t] - u[t-1] + c²*(∇²u[t]) * damping
```

- Wave speed `c = 0.4`
- Damping factor `0.995` (energy slowly dissipates)
- 3 simulation steps per frame for smooth propagation

## 📁 Project Structure

```
Wave Simulation/
├── src/
│   └── main.cpp
├── CMakeLists.txt
└── .gitignore
```

## 🧰 Tech Stack

- **Language:** C++17
- **Graphics:** SFML 3.0
- **Build:** CMake + MinGW
- **Physics:** Finite Difference Wave Equation

---
*Part of a C++ graphics & simulation portfolio*
