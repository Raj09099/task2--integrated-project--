#ifndef CG_H
#define CG_H

#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <string>

// Include Data Structures and Object-Oriented Programming headers
#include "../DS/ds.h"
#include "../OOPS/oops.h"

// Interface declarations for COA NASM Assembly linkage
extern "C" {
    void run_coa_calculation();
    extern uint64_t roadA;
    extern uint64_t roadB;
    extern uint64_t roadC;
    extern uint64_t roadD;
    extern uint64_t total;
    extern uint64_t highest;
    extern uint64_t signalTime;
}

// Global CG Animation & Viewport state variables
extern float carX;
extern float carY;
extern float carAngle;
extern float carScale;
extern float cubeAngle;

// 3D Car Demonstration State Variables (Right Section)
extern float car3DX;
extern float car3DY;
extern float car3DZ;
extern float car3DRotX;
extern float car3DRotY;
extern float car3DRotZ;
extern float car3DScale;
extern bool car3DAutoRotate;

// Dynamic Multi-Branch Vehicle Animation Coordinates
extern float carWestX;  // West-to-East lane position
extern float carEastX;  // East-to-West lane position
extern float carNorthY; // North-to-South lane position
extern float carSouthY; // South-to-North lane position

// Signal states for 4 directions (0 = RED, 1 = GREEN, 2 = YELLOW)
extern int signalStateEW; // East-West direction signal state
extern int signalStateNS; // North-South direction signal state
extern int signalCounter;

extern bool clippingOn;
extern bool smallViewport;
extern int windowWidth;
extern int windowHeight;

// Primitive and Helper Functions
void rectangle(float x1, float y1, float x2, float y2);
void circle(float cx, float cy, float radius);
void drawText(float x, float y, const char* text);
void drawTextLine(float x, float& y, float lineGap, const char* text);
void drawPanelBox(float x1, float y1, float x2, float y2, const char* title, float r, float g, float b);

// 3D Geometry Helpers
void draw3DBox(float dx, float dy, float dz);
void draw3DBoxWithOutline(float dx, float dy, float dz, float r, float g, float b);
void draw3DCar();

// CG Rendering Components
void drawRoad();
void drawSingleSignalBox(float x, float y, int state, const char* label);
void drawTrafficSignal();
void drawVehicleOriented(float x, float y, float angleDeg, Vehicle* currentObj);
void drawCar();
void drawClippingWindow();

// Integrated Dashboard Panels
void drawCOAPanel();
void drawDSPanel();
void drawOOPSPanel();
void drawSignalIntegrationPanel();
void drawDashboardHeader();

void drawTrafficScene();
void draw3DPanel(int width, int height);

void display();
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void reshape(int width, int height);
void init();

#endif // CG_H
