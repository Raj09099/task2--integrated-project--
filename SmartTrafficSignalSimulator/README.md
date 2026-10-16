# Smart Traffic Signal Simulator

**Integrated Project for Academic Review**  
*Combining Data Structures (DS), Object-Oriented Programming (OOPS), Computer Organization & Architecture (COA), and Computer Graphics (CG).*

---

## 1. Subject Breakdown & Role

| Subject | Language / Framework | Key Concept Demonstrated | Role in Simulator |
| :--- | :--- | :--- | :--- |
| **DS** | C++ | Singly Linked List, CRUD operations, Sorting, Queue management | Manages the waiting vehicle queue at traffic signals. |
| **OOPS** | C++ | Inheritance, Encapsulation, Polymorphism, Virtual Functions, Destructors | Models vehicle types (`Car`, `Bus`, `Ambulance`) with priorities. |
| **COA** | NASM Assembly (x86_64) | Processor-level registers (`RAX`, `RBX`, `RCX`, `RDX`), `IMUL`, `CMP`, `JGE` | Calculates total traffic, highest traffic road, and signal duration. |
| **CG** | C++ + OpenGL / freeGLUT | 2D Transformations, Clipping, Viewports, 3D Rotating Cube, Animations | Visualizes the 4-way intersection, dynamic signal state, HUD & vehicles. |

---

## 2. Directory Structure

```text
SmartTrafficSignalSimulator/
│
├── DS/
│   ├── ds.h
│   └── ds.cpp
│
├── OOPS/
│   ├── oops.h
│   └── oops.cpp
│
├── COA/
│   └── coa.asm
│
├── CG/
│   ├── cg.h
│   └── cg.cpp
│
├── assets/
│   └── textures/
│
└── README.md
```

---

## 3. Build & Execution Instructions

### A. Independent Subject Builds

1. **DS Standalone CLI Test**:
   ```bash
   g++ -DDS_STANDALONE DS/ds.cpp -o DS/ds.exe
   ./DS/ds.exe
   ```

2. **OOPS Standalone CLI Test**:
   ```bash
   g++ -DOOPS_STANDALONE OOPS/oops.cpp -o OOPS/oops.exe
   ./OOPS/oops.exe
   ```

3. **COA Standalone Assembly Test**:
   ```bash
   nasm -f win64 COA/coa.asm -o COA/coa.obj
   gcc COA/coa.obj -o COA/coa.exe
   ./COA/coa.exe
   ```

### B. Integrated CG Application Build

Assemble the COA module and link all subjects into the final OpenGL simulator:

```bash
# Step 1: Assemble COA Assembly into a win64 COFF object file
nasm -f win64 COA/coa.asm -o COA/coa.obj

# Step 2: Compile & Link Integrated C++ OpenGL Application with COA Object
g++ CG/cg.cpp DS/ds.cpp OOPS/oops.cpp COA/coa.obj -o CG/cg.exe -lfreeglut -lopengl32 -lglu32

# Step 3: Run the Integrated OpenGL Application
./CG/cg.exe
```

---

## 4. User Controls in OpenGL Window

### Traffic Simulator (Left Section Controls)
* `Space` / Auto Timer: Continuous traffic signal cycle & vehicle movement.
* `T`: Toggle current vehicle type (`Car` -> `Bus` -> `Ambulance`).
* `A` / `D`: Translate interactive vehicle position left/right.
* `R`: Rotate interactive vehicle angle.
* `+` / `-`: Scale interactive vehicle size up/down.
* `V`: Toggle left/right viewport layout split size (Preserved!).
* `X`: Toggle left-side scissor clipping region on/off (clips traffic simulator content).

### 3D Car Demonstration (Right Section Controls)
* `Arrow Keys (Left/Right/Up/Down)`: Translate 3D Car along X and Y axes.
* `U` / `O`: Translate 3D Car along Z axis (depth forward/backward).
* `Y` / `H` / `G`: Rotate 3D Car around Y (Yaw), X (Pitch), and Z (Roll) axes.
* `[` / `]`: Scale 3D Car down/up.
* `C`: Toggle continuous 3D Car auto-spin rotation.
* `ESC`: Exit application cleanly.
