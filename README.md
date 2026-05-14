# 🌲 IFAMDS — Intelligent Forest Advisory & Multi-Structure Decision System

> A C++ simulation system that models an intelligent forest management environment using **8 major data structures** across 8 processing layers — handling real-time sensor data, fire detection, resource allocation, anomaly escalation, and emergency response across 9 forest zones.

**Course:** CL2001 — Data Structures | **Date:** May 2026

---

## 📌 Project Versions

This project ships **two fully working implementations:**

| Version | Entry Point | UI |
|---------|-------------|-----|
| **Console** | `main_console.cpp` | Terminal menu, plain text |
| **SFML GUI** | `main.cpp` | Graphical window with keyboard navigation |

Both versions share the same underlying data structure layer files.

---

## 📁 File Structure

```
IFAMDS/
├── main.cpp                   ← SFML GUI entry point
├── main_console.cpp           ← Console (terminal) entry point
├── sfml_handler_complete.cpp  ← SFML UI handler
├── arraylayer.cpp/.h          ← Array layer (A1–A4)
├── linklist_layer.cpp/.h      ← Linked list layer (L1–L10)
├── queue_layer.cpp/.h         ← Queue scheduler (Q1–Q4)
├── stack_layer.cpp/.h         ← Stack / execution state
├── tree_layer.cpp/.h          ← Decision tree layer (T1–T12)
├── graph_layer.cpp/.h         ← Graph routing (G1–G2, BFS/DFS)
├── hash_layer.cpp/.h          ← Hash tables (H1–H3)
├── monitoring_layer.cpp/.h    ← System monitoring layer
├── validation_helpers.h       ← Shared validation utilities
├── Makefile                   ← Build system
└── README.md                  ← This file
```

---

## ⚙️ Prerequisites

### Windows

- **g++** with C++17 support — install via [MinGW-w64](https://www.mingw-w64.org/)
- **SFML 2.6** — download from [sfml-dev.org](https://www.sfml-dev.org/download.php) (only for GUI version)

Check if g++ is installed:
```bash
g++ --version
```

### Ubuntu / Linux

```bash
# Install g++
sudo apt install g++ build-essential

# Install SFML (only for GUI version)
sudo apt install libsfml-dev
```

---

## 🚀 Building & Running

### 🖥️ Console Version (No SFML Required)

#### Windows
```bash
# Build
g++ -std=c++17 -o ifamds_console main_console.cpp arraylayer.cpp linklist_layer.cpp queue_layer.cpp stack_layer.cpp tree_layer.cpp graph_layer.cpp hash_layer.cpp monitoring_layer.cpp

# Run
.\ifamds_console.exe
```

#### Ubuntu / Linux
```bash
# Build
make console

# Run
./ifamds_console
```

---

### 🎮 SFML GUI Version

#### Windows
```bash
# Build (adjust SFML path to where you extracted it)
g++ -std=c++17 -o sfml_menu main.cpp sfml_handler_complete.cpp arraylayer.cpp linklist_layer.cpp queue_layer.cpp stack_layer.cpp tree_layer.cpp graph_layer.cpp hash_layer.cpp monitoring_layer.cpp -IC:/SFML/include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system

# Run
.\sfml_menu.exe
```

> **Note:** Make sure SFML `.dll` files are in the same folder as the executable or in your system PATH.

#### Ubuntu / Linux
```bash
# Build
make sfml

# Run
./sfml_menu
```

---

### 🔧 Build Both at Once (Linux/Ubuntu)

```bash
make        # builds sfml_menu and ifamds_console
make clean  # remove all build artefacts
```

---

## 🗂️ Data Structures at a Glance

| Layer | Instances | Time Complexity | Role |
|-------|-----------|-----------------|------|
| Arrays | A1–A4 | O(1) access | Sensor storage, 1D/2D grid |
| Singly Linked Lists | L1–L3 | O(n) search | Raw / verified / anomaly event streams |
| Doubly Linked Lists | L4–L6 | O(n) search | Forward/backward correction chains |
| Circular LL | L7–L10 | O(n) per cycle | Continuous monitoring loops |
| Queues | Q1–Q4 | O(1) enqueue | Task scheduling, FIFO priority |
| Stack | — | O(1) push/pop | Execution state & rollback |
| Trees | T1–T12 | O(log n) / O(h) | Risk scoring, decision intelligence |
| Graphs | G1–G2 | O(V+E) BFS/DFS | Fire spread, evacuation routing |
| Hash Tables | H1–H3 | O(1) avg | Fast zone data retrieval |

---

## 🎮 Keyboard Controls (SFML GUI)

| Context | Key | Action |
|---------|-----|--------|
| Menu navigation | ↑ / ↓ | Move selection |
| Menu navigation | Enter | Confirm / select |
| Menu navigation | Esc / Backspace | Go back |
| Sensor input | 0–9 | Type numeric value |
| Sensor input | Backspace | Delete last digit |
| Sensor input | Tab / Enter | Move to next field |
| Sensor input | Esc | Cancel input |
| Output view | Esc / Backspace | Return to sub-menu |

---

## 📋 Console Menu Highlights

1. **Input Environmental Data** — add sensor readings, compare against baseline
2. **View Forest Grid Status** — 1D time-series, 2D zone matrix
3. **Event Memory System** — linked-list event storage, forward/backward traversal
4. **Fire Detection & Control** — threshold checks, emergency alerts, BFS fire spread
5. **Task Scheduling** — FIFO queue management, surveillance, emergency priority
6. **Decision System** — risk score computation, zone/regional/global decision trees
7. **Spatial Routing** — BFS/DFS traversal, safe-path computation
8. **Hash-Based Fast Access** — insert/retrieve/update zone data
9. **System Monitoring** — load metrics, execution latency, bottleneck detection
10. **Scenario Simulation** — five pre-built scenarios plus full system run

---

## 🔥 Five Scenarios

| # | Scenario | Key Data Structures |
|---|----------|---------------------|
| 1 | Cascading Fire & Resource Conflict | Arrays, LL chains, Queues, Graph BFS, Stack |
| 2 | Sensor Failure & System Reconstruction | Arrays, Doubly LL, Hash cache, State sync |
| 3 | Multi-Factor Anomaly Escalation | Anomaly stream, Trees, Circular LL |
| 4 | System Overload & Load Redistribution | All queues, Stack snapshots, Hash |
| 5 | Global Multi-Zone Emergency | All modules in coordinated crisis mode |

Run any scenario from **Menu → 10. Scenario Simulation**

---

## ⚠️ Anomaly Thresholds

| Sensor | Threshold | Condition |
|--------|-----------|-----------|
| Temperature | > 45 °C | Fire risk |
| Smoke level | > 70 | Possible fire |
| Humidity | < 20 % | Dry / high-risk |

**Risk Score Formula:**
```
score = 0.4 × (Temp/100) + 0.3 × (Smoke/100) + 0.3 × ((100 − Humidity)/100)
```
→ Score ≥ 0.6 triggers emergency response

---

*IFAMDS — CL2001 Data Structures Project, 2026*
