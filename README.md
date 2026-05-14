# IFAMDS — Intelligent Forest Advisory & Multi-Structure Decision System

**Course:** CL2001 — Data Structures  
**Submitted By:**
- Maaz Sher — 24I-0083  
- Muhammad Hamid — 24F-0030  

**Date:** May 9, 2026

---

## Project Overview

IFAMDS is a C++ simulation system that models an intelligent forest management environment. It integrates eight major data structures — arrays, linked lists, queues, stacks, trees, graphs, and hash tables — across eight processing layers to handle real-time forest sensor data, fire detection, resource allocation, anomaly escalation, and emergency response across nine forest zones.

The project ships **two fully working implementations**:

| Version | Entry Point | UI |
|---------|-------------|----|
| **Console** | `main_console.cpp` | Terminal menu, plain text |
| **SFML GUI** | `main.cpp` | Graphical window with keyboard navigation |

Both versions share the same underlying data structure layer files.

---

## File Structure

```
DSA_Updated/
├── main.cpp               ← SFML GUI entry point
├── main_console.cpp       ← Console (terminal) entry point
├── arraylayer.cpp/.h      ← Array layer  (A1–A4)
├── linklist_layer.cpp/.h  ← Linked list layer (L1–L10)
├── queue_layer.cpp/.h     ← Queue scheduler (Q1–Q4)
├── stack_layer.cpp/.h     ← Stack / execution state
├── tree_layer.cpp/.h      ← Decision tree layer (T1–T12)
├── graph_layer.cpp/.h     ← Graph routing (G1–G2, BFS/DFS)
├── hash_layer.cpp/.h      ← Hash tables (H1–H3)
├── monitoring_layer.cpp/.h← System monitoring layer
├── validation_helpers.h   ← Shared validation utilities
├── Makefile               ← Build system (see below)
├── README.md              ← This file
├── SFML_IMPROVEMENTS.md   ← GUI change log
├── QUICK_REFERENCE.md     ← Keyboard controls cheatsheet
└── CL2001_Project_2026.pdf← Original assignment brief
```

---

## Building & Running

### Prerequisites

- **g++** with C++17 support (`g++ --version`)
- **SFML** libraries (only for the GUI version)

```bash
# Ubuntu/Debian
sudo apt install libsfml-dev

# Fedora
sudo dnf install SFML-devel
```

### Console Version (no SFML required)

```bash
# Build
make console

# Run
./ifamds_console
```

### SFML GUI Version

```bash
# Build
make sfml

# Run
./sfml_menu
```

### Build Both at Once

```bash
make        # builds sfml_menu and ifamds_console
make clean  # remove all build artefacts
```

---

## Console Version

The console version runs entirely in the terminal with a numbered menu system.

**Menu highlights (10 top-level sections):**

1. Input Environmental Data — add sensor readings, compare against baseline, filter noise  
2. View Forest Grid Status — 1D time-series, 2D zone matrix, per-zone conditions  
3. Event Memory System — linked-list event storage, forward/backward traversal, circular monitoring  
4. Fire Detection & Control — threshold checks, emergency alerts, priority response, BFS fire spread  
5. Task Scheduling — FIFO queue management, surveillance, emergency priority, pause/resume  
6. Decision System — risk score computation, zone/regional/global decision trees  
7. Spatial Routing — load adjacency list/matrix, BFS/DFS traversal, safe-path computation  
8. Hash-Based Fast Access — insert/retrieve/update zone data, collision handling, cache view  
9. System Monitoring — load metrics, execution latency, bottleneck detection, health view  
10. Scenario Simulation — five pre-built scenarios plus full system run  

Navigate by typing the option number and pressing **Enter**.

---

## SFML GUI Version

The GUI version renders all menus in a graphical window using SFML.

### Keyboard Controls

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

### Navigation Flow

```
Start → Main Menu → Sub-Menu ─┬─ [Add Sensor Reading] → Input Mode → Output View
                               └─ [Other options]      → Output View
                                                              ↓
                                                         ESC to go back
```

---

## Data Structures at a Glance

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

## Five Scenarios

| # | Scenario | Key Data Structures |
|---|----------|---------------------|
| 1 | Cascading Fire & Resource Conflict | Arrays, LL chains, Queues, Graph BFS, Stack |
| 2 | Sensor Failure & System Reconstruction | Arrays, Doubly LL, Hash cache, State sync |
| 3 | Multi-Factor Anomaly Escalation | Anomaly stream, Trees, Circular LL |
| 4 | System Overload & Load Redistribution | All queues, Stack snapshots, Hash |
| 5 | Global Multi-Zone Emergency | All modules in coordinated crisis mode |

Run any scenario from **Menu → 10. Scenario Simulation**.

---

## Anomaly Thresholds

| Sensor | Threshold | Condition |
|--------|-----------|-----------|
| Temperature | > 45 °C | Fire risk |
| Smoke level | > 70 | Possible fire |
| Humidity | < 20 % | Dry / high-risk |

Risk score: `0.4 × (Temp/100) + 0.3 × (Smoke/100) + 0.3 × ((100 − Humidity)/100)`  
→ ≥ 0.6 triggers emergency response.

---

*IFAMDS — CL2001 Data Structures Project, 2026*
