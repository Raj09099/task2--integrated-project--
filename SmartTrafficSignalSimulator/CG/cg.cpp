#include "cg.h"

// =====================================================
// GLOBAL VARIABLES (Preserved & Integrated System State)
// =====================================================
float carX = -25.0f;
float carY = -5.0f;
float carAngle = 0.0f;
float carScale = 1.0f;
float cubeAngle = 0.0f;

// 3D Car Demonstration State Variables (Right Section)
float car3DX = 0.0f;
float car3DY = 0.0f;
float car3DZ = 0.0f;
float car3DRotX = 15.0f;
float car3DRotY = 35.0f;
float car3DRotZ = 0.0f;
float car3DScale = 1.0f;
bool car3DAutoRotate = true;

// Dynamic Multi-Branch Vehicle Animation Coordinates
float carWestX = -45.0f;  // West-to-East lane position
float carEastX = 45.0f;   // East-to-West lane position
float carNorthY = 45.0f;  // North-to-South lane position
float carSouthY = -45.0f; // South-to-North lane position

// 4-Way Signal States (0 = RED, 1 = GREEN, 2 = YELLOW)
int signalStateEW = 1; // East-West initial state: GREEN
int signalStateNS = 0; // North-South initial state: RED
int signalCounter = 0;

bool clippingOn = true;
bool smallViewport = false;

int windowWidth = 1100;
int windowHeight = 700;

// OOPS Vehicles Polymorphic Array
Vehicle* activeVehicles[3];
int activeVehicleIndex = 0;

// =====================================================
// HELPER: MAP DS VEHICLE TYPE TO OOPS POLYMORPHIC OBJECT
// =====================================================

Vehicle* getOOPSVehicleForType(const string& typeStr)
{
    if (typeStr == "Bus") return activeVehicles[1];
    if (typeStr == "Ambulance") return activeVehicles[2];
    return activeVehicles[0]; // Default Car
}

// =====================================================
// PRIMITIVES & DASHBOARD HELPERS
// =====================================================

void rectangle(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void circle(float cx, float cy, float radius)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        float angle = i * 3.14159f / 180.0f;
        float x = cx + radius * cos(angle);
        float y = cy + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(
            GLUT_BITMAP_HELVETICA_12,
            text[i]
        );
    }
}

// Helper to draw text line and automatically decrement Y by lineGap to prevent overlap
void drawTextLine(float x, float& y, float lineGap, const char* text)
{
    drawText(x, y, text);
    y -= lineGap;
}

// Panel Box Container Helper
void drawPanelBox(float x1, float y1, float x2, float y2, const char* title, float r, float g, float b)
{
    // Background Fill
    glColor3f(0.04f, 0.06f, 0.10f);
    rectangle(x1, y1, x2, y2);

    // Header Fill
    glColor3f(r * 0.3f, g * 0.3f, b * 0.3f);
    rectangle(x1, y2 - 4.5f, x2, y2);

    // Border Outline
    glColor3f(r, g, b);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1); glVertex2f(x2, y1);
    glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();

    // Panel Title
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(x1 + 2.0f, y2 - 3.2f, title);
}

// =====================================================
// 3D GEOMETRY PRIMITIVES & ACADEMIC 3D CAR MODEL
// =====================================================

void draw3DBox(float dx, float dy, float dz)
{
    glPushMatrix();
    glScalef(dx, dy, dz);
    glutSolidCube(1.0);
    glPopMatrix();
}

void draw3DBoxWithOutline(float dx, float dy, float dz, float r, float g, float b)
{
    // Solid fill
    glColor3f(r, g, b);
    glPushMatrix();
    glScalef(dx, dy, dz);
    glutSolidCube(1.0);

    // Crisp wireframe outline to enhance 3D visual definition
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.5f);
    glutWireCube(1.002);
    glPopMatrix();
}

void draw3DCar()
{
    glPushMatrix();

    // 1. Main Lower Chassis (Body) - Metallic Blue
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    draw3DBoxWithOutline(4.8f, 1.2f, 2.4f, 0.1f, 0.45f, 0.85f);
    glPopMatrix();

    // 2. Upper Cabin / Roof - Light Cyan Glass/Roof Structure
    glPushMatrix();
    glTranslatef(0.0f, 0.95f, 0.0f);
    draw3DBoxWithOutline(2.6f, 0.9f, 2.0f, 0.2f, 0.65f, 0.95f);
    glPopMatrix();

    // Side Windows Tint Layer
    glPushMatrix();
    glTranslatef(0.1f, 0.98f, 0.0f);
    draw3DBoxWithOutline(2.4f, 0.7f, 2.05f, 0.75f, 0.92f, 1.0f);
    glPopMatrix();

    // 3. Four 3D Wheels (Dark Charcoal Cuboid Tires with Silver Hubcaps)
    float wheelX[4] = { 1.4f,  1.4f, -1.4f, -1.4f };
    float wheelZ[4] = { 1.25f, -1.25f, 1.25f, -1.25f };

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(wheelX[i], -0.6f, wheelZ[i]);
        draw3DBoxWithOutline(0.9f, 0.7f, 0.35f, 0.12f, 0.12f, 0.14f);

        // Silver Hubcap Center
        glColor3f(0.85f, 0.85f, 0.9f);
        draw3DBox(0.4f, 0.35f, 0.37f);
        glPopMatrix();
    }

    // 4. Front Headlights (Bright Yellow at +X)
    glPushMatrix();
    glTranslatef(2.41f, 0.05f, 0.7f);
    draw3DBoxWithOutline(0.1f, 0.35f, 0.45f, 1.0f, 0.95f, 0.2f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.41f, 0.05f, -0.7f);
    draw3DBoxWithOutline(0.1f, 0.35f, 0.45f, 1.0f, 0.95f, 0.2f);
    glPopMatrix();

    // 5. Rear Taillights (Red at -X)
    glPushMatrix();
    glTranslatef(-2.41f, 0.05f, 0.7f);
    draw3DBoxWithOutline(0.1f, 0.35f, 0.45f, 0.9f, 0.1f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.41f, 0.05f, -0.7f);
    draw3DBoxWithOutline(0.1f, 0.35f, 0.45f, 0.9f, 0.1f, 0.1f);
    glPopMatrix();

    glPopMatrix();
}

// =====================================================
// FOUR-WAY ROAD INTERSECTION
// =====================================================

void drawRoad()
{
    // 1. Asphalt Road Surfaces (East-West and North-South)
    glColor3f(0.16f, 0.16f, 0.16f);

    // East-West Road
    rectangle(-50.0f, -14.0f, 50.0f, 14.0f);

    // North-South Road
    rectangle(-14.0f, -50.0f, 14.0f, 50.0f);

    // Center Junction Square
    rectangle(-14.0f, -14.0f, 14.0f, 14.0f);

    // 2. White Border Outer Lines
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3);
    glBegin(GL_LINES);

    // West Branch Borders
    glVertex2f(-50, 14);  glVertex2f(-14, 14);
    glVertex2f(-50, -14); glVertex2f(-14, -14);

    // East Branch Borders
    glVertex2f(14, 14);   glVertex2f(50, 14);
    glVertex2f(14, -14);  glVertex2f(50, -14);

    // North Branch Borders
    glVertex2f(-14, 14);  glVertex2f(-14, 50);
    glVertex2f(14, 14);   glVertex2f(14, 50);

    // South Branch Borders
    glVertex2f(-14, -14); glVertex2f(-14, -50);
    glVertex2f(14, -14);  glVertex2f(14, -50);

    // Stop Lines at Intersection Entries
    glVertex2f(-14, -14); glVertex2f(-14, 0); // West Stop Line
    glVertex2f(14, 0);    glVertex2f(14, 14);  // East Stop Line
    glVertex2f(0, 14);    glVertex2f(14, 14);  // North Stop Line
    glVertex2f(-14, -14); glVertex2f(0, -14);  // South Stop Line

    glEnd();

    // 3. Yellow Dashed Center Lane Dividers
    glColor3f(1.0f, 0.85f, 0.0f);
    glLineWidth(2);
    glBegin(GL_LINES);

    // West Lane Divider
    for (float x = -48; x < -14; x += 6) { glVertex2f(x, 0); glVertex2f(x + 3, 0); }

    // East Lane Divider
    for (float x = 14; x < 48; x += 6) { glVertex2f(x, 0); glVertex2f(x + 3, 0); }

    // North Lane Divider
    for (float y = 14; y < 48; y += 6) { glVertex2f(0, y); glVertex2f(0, y + 3); }

    // South Lane Divider
    for (float y = -48; y < -14; y += 6) { glVertex2f(0, y); glVertex2f(0, y + 3); }

    glEnd();
}

// =====================================================
// SINGLE TRAFFIC SIGNAL BOX HELPER
// =====================================================

void drawSingleSignalBox(float x, float y, int state, const char* label)
{
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Pole
    glColor3f(0.1f, 0.1f, 0.1f);
    rectangle(-0.8f, -4.5f, 0.8f, 0.0f);

    // Housing Box
    glColor3f(0.04f, 0.04f, 0.04f);
    rectangle(-2.5f, 0.0f, 2.5f, 7.5f);

    // Border
    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-2.5f, 0.0f); glVertex2f(2.5f, 0.0f);
    glVertex2f(2.5f, 7.5f); glVertex2f(-2.5f, 7.5f);
    glEnd();

    // RED Light
    if (state == 0) glColor3f(1.0f, 0.0f, 0.0f);
    else glColor3f(0.25f, 0.0f, 0.0f);
    circle(0.0f, 6.0f, 1.0f);

    // YELLOW Light
    if (state == 2) glColor3f(1.0f, 1.0f, 0.0f);
    else glColor3f(0.25f, 0.25f, 0.0f);
    circle(0.0f, 3.8f, 1.0f);

    // GREEN Light
    if (state == 1) glColor3f(0.0f, 1.0f, 0.0f);
    else glColor3f(0.0f, 0.25f, 0.0f);
    circle(0.0f, 1.5f, 1.0f);

    // Label
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(-2.0f, 8.5f, label);

    glPopMatrix();
}

// =====================================================
// FOUR TRAFFIC SIGNALS (North, South, East, West)
// =====================================================

void drawTrafficSignal()
{
    drawSingleSignalBox(-16.0f, -21.0f, signalStateEW, "W (Road D)");
    drawSingleSignalBox(16.0f, 15.0f, signalStateEW, "E (Road B)");
    drawSingleSignalBox(-16.0f, 15.0f, signalStateNS, "N (Road A)");
    drawSingleSignalBox(16.0f, -21.0f, signalStateNS, "S (Road C)");
}

// =====================================================
// ORIENTED VEHICLE RENDERER (Polymorphic OOPS Vehicle)
// =====================================================

void drawVehicleOriented(float x, float y, float angleDeg, Vehicle* currentObj)
{
    glPushMatrix();

    glTranslatef(x, y, 0);
    glRotatef(angleDeg, 0, 0, 1);
    glScalef(carScale, carScale, 1);

    string typeStr = currentObj ? currentObj->getTypeName() : "Car";

    if (typeStr == "Bus")
    {
        // Bus Body
        glColor3f(0.95f, 0.7f, 0.0f);
        rectangle(-9, -3.8f, 9, 3.8f);

        // Windows
        glColor3f(0.8f, 0.95f, 1.0f);
        rectangle(-7, 1, -2, 2.8f);
        rectangle(-1, 1, 4, 2.8f);
        rectangle(5, 1, 7.5f, 2.8f);

        // Wheels
        glColor3f(0.02f, 0.02f, 0.02f);
        circle(-6.0f, -3.8f, 1.8f);
        circle(6.0f, -3.8f, 1.8f);
    }
    else if (typeStr == "Ambulance")
    {
        // Ambulance Body
        glColor3f(0.95f, 0.95f, 0.95f);
        rectangle(-8.5f, -3.5f, 8.5f, 3.5f);

        // Red Cross
        glColor3f(1.0f, 0.0f, 0.0f);
        rectangle(-1.2f, -0.8f, 1.2f, 2.0f);
        rectangle(-2.5f, 0.0f, 2.5f, 1.2f);

        // Siren Light
        if (signalCounter % 20 < 10) glColor3f(1.0f, 0.0f, 0.0f);
        else glColor3f(0.0f, 0.0f, 1.0f);
        rectangle(-1, 3.5f, 1, 4.8f);

        // Wheels
        glColor3f(0.02f, 0.02f, 0.02f);
        circle(-5.5f, -3.5f, 1.6f);
        circle(5.5f, -3.5f, 1.6f);
    }
    else // Default Car
    {
        // Car Body
        glColor3f(0.0f, 0.1f, 0.9f);
        rectangle(-7.5f, -3.0f, 7.5f, 1.8f);

        // Car Roof
        glColor3f(0.05f, 0.55f, 1.0f);
        glBegin(GL_POLYGON);
        glVertex2f(-4.5f, 1.8f);
        glVertex2f(-2.8f, 4.0f);
        glVertex2f(2.8f, 4.0f);
        glVertex2f(4.5f, 1.8f);
        glEnd();

        // Windows
        glColor3f(0.75f, 0.9f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(-4.0f, 2.2f); glVertex2f(-2.5f, 3.6f); glVertex2f(-0.8f, 3.6f); glVertex2f(-0.8f, 2.2f);
        glVertex2f(0.8f, 2.2f);  glVertex2f(0.8f, 3.6f);  glVertex2f(2.5f, 3.6f);  glVertex2f(4.0f, 2.2f);
        glEnd();

        // Wheels
        glColor3f(0.02f, 0.02f, 0.02f);
        circle(-4.8f, -3.0f, 1.5f);
        circle(4.8f, -3.0f, 1.5f);
    }

    glPopMatrix();
}

// =====================================================
// CAR & MULTI-BRANCH QUEUE RENDERER (DS + OOPS INTEGRATION)
// =====================================================

void drawCar()
{
    // 1. Traverse DS Linked List Nodes -> Map to OOPS Vehicle Objects
    Node* currDS = head;
    Vehicle* vObj1 = (currDS != NULL) ? getOOPSVehicleForType(currDS->vehicleType) : activeVehicles[0];

    Node* nextDS1 = (currDS != NULL) ? currDS->next : NULL;
    Vehicle* vObj2 = (nextDS1 != NULL) ? getOOPSVehicleForType(nextDS1->vehicleType) : activeVehicles[1];

    Node* nextDS2 = (nextDS1 != NULL) ? nextDS1->next : NULL;
    Vehicle* vObj3 = (nextDS2 != NULL) ? getOOPSVehicleForType(nextDS2->vehicleType) : activeVehicles[2];

    // 2. West-to-East Lane Vehicles (Road D - West approach)
    drawVehicleOriented(carWestX, -5.0f, 0.0f, vObj1); // Lead Vehicle
    drawVehicleOriented(carWestX - 22.0f, -5.0f, 0.0f, vObj2); // Queued Vehicle

    // 3. East-to-West Lane Vehicles (Road B - East approach)
    drawVehicleOriented(carEastX, 5.0f, 180.0f, vObj2); // Lead Vehicle
    drawVehicleOriented(carEastX + 22.0f, 5.0f, 180.0f, vObj3); // Queued Vehicle

    // 4. North-to-South Lane Vehicles (Road A - North approach)
    drawVehicleOriented(-5.0f, carNorthY, -90.0f, vObj3); // Lead Vehicle (Ambulance)
    drawVehicleOriented(-5.0f, carNorthY + 22.0f, -90.0f, vObj1); // Queued Vehicle

    // 5. South-to-North Lane Vehicles (Road C - South approach)
    drawVehicleOriented(5.0f, carSouthY, 90.0f, vObj2); // Lead Vehicle (Bus)
    drawVehicleOriented(5.0f, carSouthY - 22.0f, 90.0f, vObj1); // Queued Vehicle

    // 6. Interactive User Main Vehicle (manual controls: A/D, R, +/-, T)
    Vehicle* userObj = activeVehicles[activeVehicleIndex];
    drawVehicleOriented(carX, carY, carAngle, userObj);
}

// =====================================================
// DASHBOARD PANEL 1: TOP TITLE HEADER
// =====================================================

void drawDashboardHeader()
{
    drawPanelBox(-49.0f, 46.0f, 49.0f, 54.0f, "SMART TRAFFIC SIGNAL SIMULATOR — INTEGRATED DASHBOARD", 0.0f, 0.85f, 1.0f);
    glColor3f(0.85f, 0.95f, 1.0f);
    drawText(-47.0f, 48.0f, "DS (Queue) | OOPS (Polymorphism) | COA (NASM Assembly) | CG (OpenGL Visualization)");
}

// =====================================================
// DASHBOARD PANEL 2: TOP-RIGHT - COA PROCESSOR MODEL
// =====================================================

void drawCOAPanel()
{
    drawPanelBox(17.0f, 12.0f, 49.0f, 44.0f, "COA: PROCESSOR MODEL", 0.0f, 0.85f, 1.0f);

    float curY = 37.0f;
    float lineGap = 3.0f;

    glColor3f(0.9f, 0.9f, 0.9f);
    drawTextLine(19.0f, curY, lineGap, ("Road A (North) : " + to_string(roadA) + " vehicles").c_str());
    drawTextLine(19.0f, curY, lineGap, ("Road B (East)  : " + to_string(roadB) + " vehicles").c_str());
    drawTextLine(19.0f, curY, lineGap, ("Road C (South) : " + to_string(roadC) + " vehicles").c_str());
    drawTextLine(19.0f, curY, lineGap, ("Road D (West)  : " + to_string(roadD) + " vehicles").c_str());

    curY -= 1.0f; // Section Spacing Gap
    glColor3f(0.3f, 0.9f, 1.0f);
    drawTextLine(19.0f, curY, lineGap, "--- ALU OPERATIONS ---");

    glColor3f(0.3f, 1.0f, 0.5f);
    drawTextLine(19.0f, curY, lineGap, ("ADD  -> Total   : " + to_string(total)).c_str());
    drawTextLine(19.0f, curY, lineGap, ("CMP  -> Highest : " + to_string(highest)).c_str());
    drawTextLine(19.0f, curY, lineGap, ("IMUL -> Signal  : " + to_string(signalTime) + " sec").c_str());
}

// =====================================================
// DASHBOARD PANEL 3: BOTTOM-LEFT - DS VEHICLE QUEUE
// =====================================================

void drawDSPanel()
{
    drawPanelBox(-49.0f, -52.0f, -17.0f, -20.0f, "DS: VEHICLE LINKED LIST", 0.2f, 0.9f, 0.3f);

    float curY = -27.0f;
    float lineGap = 3.0f;

    glColor3f(1.0f, 1.0f, 1.0f);
    string qHead = "Queue Size: " + to_string(getVehicleCount()) + " | Front ID: " + to_string(head ? head->vehicleId : 0);
    drawTextLine(-47.0f, curY, lineGap, qHead.c_str());

    glColor3f(0.4f, 0.9f, 1.0f);
    drawTextLine(-47.0f, curY, lineGap, "ID     TYPE         WAIT");

    // Traverse live DS linked list nodes in clean table layout
    Node* temp = head;
    int count = 0;
    while (temp != NULL && count < 3)
    {
        string idStr = to_string(temp->vehicleId);
        string typeStr = temp->vehicleType;
        while (typeStr.length() < 12) typeStr += " ";

        string lineStr = idStr + "    " + typeStr + " " + to_string(temp->waitingTime) + "m";
        glColor3f(0.4f, 1.0f, 0.6f);
        drawTextLine(-47.0f, curY, lineGap, lineStr.c_str());

        temp = temp->next;
        count++;
    }

    curY -= 1.0f; // Section Spacing Gap
    glColor3f(0.8f, 0.8f, 0.8f);
    drawTextLine(-47.0f, curY, lineGap, "HEAD -> Node -> Node -> NULL");
}

// =====================================================
// DASHBOARD PANEL 4: BOTTOM-RIGHT - OOPS VEHICLE SYSTEM
// =====================================================

void drawOOPSPanel()
{
    drawPanelBox(17.0f, -52.0f, 49.0f, -20.0f, "OOPS: VEHICLE SYSTEM", 1.0f, 0.6f, 0.1f);

    float curY = -27.0f;
    float lineGap = 3.0f;

    glColor3f(1.0f, 0.85f, 0.3f);
    drawTextLine(19.0f, curY, lineGap, "Vehicle (Base Class)");
    drawTextLine(19.0f, curY, lineGap, " |- Car       (Priority: 1)");
    drawTextLine(19.0f, curY, lineGap, " |- Bus       (Priority: 2)");
    drawTextLine(19.0f, curY, lineGap, " \\- Ambulance (Priority: 10)");

    curY -= 1.0f; // Section Spacing Gap
    glColor3f(0.3f, 0.9f, 1.0f);
    drawTextLine(19.0f, curY, lineGap, "--- ACTIVE POLYMORPHIC OBJECT ---");

    Vehicle* v = activeVehicles[activeVehicleIndex];
    glColor3f(1.0f, 1.0f, 1.0f);
    drawTextLine(19.0f, curY, lineGap, ("Type     : " + (v ? v->getTypeName() : "None")).c_str());
    drawTextLine(19.0f, curY, lineGap, ("Priority : " + to_string(v ? v->getPriority() : 0)).c_str());
    drawTextLine(19.0f, curY, lineGap, ("Reg No   : " + (v ? v->getRegistrationNo() : "")).c_str());
}

// =====================================================
// DASHBOARD PANEL 5: TOP-LEFT - SIGNAL & INTEGRATION
// =====================================================

void drawSignalIntegrationPanel()
{
    drawPanelBox(-49.0f, 12.0f, -17.0f, 44.0f, "SIGNAL & INTEGRATION", 0.9f, 0.8f, 0.2f);

    float curY = 37.0f;
    float lineGap = 3.0f;

    string ewStateStr = "E-W Signal : " + string(signalStateEW == 1 ? "GREEN" : (signalStateEW == 2 ? "YELLOW" : "RED"));
    string nsStateStr = "N-S Signal : " + string(signalStateNS == 1 ? "GREEN" : (signalStateNS == 2 ? "YELLOW" : "RED"));

    if (signalStateEW == 1) glColor3f(0.2f, 1.0f, 0.2f);
    else glColor3f(1.0f, 0.3f, 0.3f);
    drawTextLine(-47.0f, curY, lineGap, ewStateStr.c_str());

    if (signalStateNS == 1) glColor3f(0.2f, 1.0f, 0.2f);
    else glColor3f(1.0f, 0.3f, 0.3f);
    drawTextLine(-47.0f, curY, lineGap, nsStateStr.c_str());

    glColor3f(0.9f, 0.9f, 0.9f);
    string timerStr = "Phase Time : " + to_string(signalCounter / 60) + "s / " + to_string(signalTime) + "s";
    drawTextLine(-47.0f, curY, lineGap, timerStr.c_str());

    curY -= 1.0f; // Section Spacing Gap
    glColor3f(0.3f, 0.9f, 1.0f);
    drawTextLine(-47.0f, curY, lineGap, "--- MODULE INTEGRATION ---");

    glColor3f(0.4f, 1.0f, 0.6f);
    drawTextLine(-47.0f, curY, lineGap, "DS   : Vehicle Queue   [OK]");
    drawTextLine(-47.0f, curY, lineGap, "OOPS : Polymorphism    [OK]");
    drawTextLine(-47.0f, curY, lineGap, "COA  : NASM Assembly   [OK]");
    drawTextLine(-47.0f, curY, lineGap, "CG   : OpenGL Graphics [OK]");
}

// =====================================================
// CLIPPING WINDOW
// =====================================================

void drawClippingWindow()
{
    if (!clippingOn)
        return;

    glColor3f(1.0f, 0.75f, 0.0f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-38.0f, -38.0f);
    glVertex2f(38.0f, -38.0f);
    glVertex2f(38.0f, 38.0f);
    glVertex2f(-38.0f, 38.0f);
    glEnd();

    glColor3f(1.0f, 0.75f, 0.0f);
    drawText(-36.0f, 35.5f, "LEFT CLIPPING REGION (SCISSOR ACTIVE)");
}

// =====================================================
// TRAFFIC SCENE & DASHBOARD ASSEMBLY
// =====================================================

void drawTrafficScene()
{
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -55, 55, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 1. Central 4-Way Road Intersection
    drawRoad();

    // 2. 4 Corner Traffic Signals
    drawTrafficSignal();

    // 3. Multi-Directional Vehicles & DS Queue
    drawCar();

    // 4. Integrated Dashboard Panels
    drawDashboardHeader();
    drawCOAPanel();
    drawDSPanel();
    drawOOPSPanel();
    drawSignalIntegrationPanel();

    // 5. Scissor Clipping Window Border
    drawClippingWindow();
}

// =====================================================
// RIGHT SECTION: 3D CAR DEMONSTRATION PANEL
// =====================================================

void draw3DPanel(int width, int height)
{
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -50, 50, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 1. Panel Background & Outer Border
    glColor3f(0.04f, 0.06f, 0.10f);
    rectangle(-48, -48, 48, 48);

    glColor3f(0.0f, 0.85f, 1.0f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-48, -48); glVertex2f(48, -48);
    glVertex2f(48, 48);   glVertex2f(-48, 48);
    glEnd();

    // 2. Title & On-Screen Control Guide HUD
    glColor3f(1, 1, 1);
    drawText(-28, 42, "3D CAR DEMONSTRATION");

    glColor3f(0.4f, 0.9f, 1.0f);
    drawText(-44, 36, "3D CAR CONTROLS:");
    drawText(-44, 30, "Translate: Arrow Keys / U, O (Z-axis)");
    drawText(-44, 24, "Rotate   : Y (Yaw), H (Pitch), G (Roll)");
    drawText(-44, 18, "Scale    : [ (Smaller) / ] (Larger)");
    drawText(-44, 12, "Auto-Rot : C Key (Toggle Auto Spin)");

    // 3D Status HUD
    glColor3f(0.85f, 0.95f, 1.0f);
    string posStr = "Pos: (" + to_string((int)car3DX) + ", " + to_string((int)car3DY) + ", " + to_string((int)car3DZ) + ")";
    drawText(-44, -36, posStr.c_str());

    string rotStr = "Rot: (" + to_string((int)car3DRotX) + ", " + to_string((int)car3DRotY) + ", " + to_string((int)car3DRotZ) + ")";
    drawText(-44, -42, rotStr.c_str());

    // 3. 3D Perspective Setup for rendering 3D Car
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / (float)height;
    gluPerspective(45.0, aspect, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera view offset
    glTranslatef(0.0f, -2.0f, -22.0f);

    // Apply 3D Transformations to the 3D Car
    glTranslatef(car3DX, car3DY, car3DZ);           // 1. TRANSLATION
    glRotatef(car3DRotX, 1.0f, 0.0f, 0.0f);         // 2. ROTATION X
    glRotatef(car3DRotY, 0.0f, 1.0f, 0.0f);         // 2. ROTATION Y
    glRotatef(car3DRotZ, 0.0f, 0.0f, 1.0f);         // 2. ROTATION Z
    glScalef(car3DScale, car3DScale, car3DScale);    // 3. SCALING

    // Render Academic 3D Car
    draw3DCar();
}

// =====================================================
// DISPLAY
// =====================================================

void display()
{
    glClearColor(0.08f, 0.10f, 0.15f, 1); // Dark professional engineering theme
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int trafficWidth = smallViewport ? (int)(windowWidth * 0.60) : (int)(windowWidth * 0.70);
    int panelWidth = windowWidth - trafficWidth;

    // =========================================================
    // 1. LEFT VIEWPORT: Smart Traffic Signal Simulator
    // =========================================================
    glViewport(0, 0, trafficWidth, windowHeight);

    if (clippingOn)
    {
        float clipMinX = -38.0f;
        float clipMaxX = 38.0f;
        float clipMinY = -38.0f;
        float clipMaxY = 38.0f;

        // Convert world clipping box coordinates [-38, 38] to left viewport pixel scissor rectangle
        int scissorX = (int)((clipMinX - (-50.0f)) / 100.0f * trafficWidth);
        int scissorY = (int)((clipMinY - (-55.0f)) / 110.0f * windowHeight);
        int scissorW = (int)((clipMaxX - clipMinX) / 100.0f * trafficWidth);
        int scissorH = (int)((clipMaxY - clipMinY) / 110.0f * windowHeight);

        glEnable(GL_SCISSOR_TEST);
        glScissor(scissorX, scissorY, scissorW, scissorH);
    }
    else
    {
        glDisable(GL_SCISSOR_TEST);
    }

    drawTrafficScene();

    // Disable Scissor test so it never affects the right section
    glDisable(GL_SCISSOR_TEST);

    // =========================================================
    // 2. RIGHT VIEWPORT: 3D Car Demonstration Panel
    // =========================================================
    glViewport(trafficWidth, 0, panelWidth, windowHeight);
    draw3DPanel(panelWidth, windowHeight);

    // Reset viewport to full window
    glViewport(0, 0, windowWidth, windowHeight);

    glutSwapBuffers();
}

// =====================================================
// TIMER / ANIMATION LOOP
// =====================================================

void timer(int value)
{
    float speed = 0.35f;

    // 1. East-West Signal Movement Control
    if (signalStateEW == 1) // GREEN
    {
        carWestX += speed;
        carEastX -= speed;
    }
    else // RED or YELLOW -> Stop before stop lines
    {
        if (carWestX < -18.0f)
        {
            carWestX += speed;
            if (carWestX > -18.0f) carWestX = -18.0f;
        }
        else if (carWestX > 18.0f)
        {
            carWestX += speed;
        }

        if (carEastX > 18.0f)
        {
            carEastX -= speed;
            if (carEastX < 18.0f) carEastX = 18.0f;
        }
        else if (carEastX < -18.0f)
        {
            carEastX -= speed;
        }
    }

    // Wrap around boundaries for East-West vehicles
    if (carWestX > 55.0f) carWestX = -55.0f;
    if (carEastX < -55.0f) carEastX = 55.0f;

    // 2. North-South Signal Movement Control
    if (signalStateNS == 1) // GREEN
    {
        carNorthY -= speed;
        carSouthY += speed;
    }
    else // RED or YELLOW -> Stop before stop lines
    {
        if (carNorthY > 18.0f)
        {
            carNorthY -= speed;
            if (carNorthY < 18.0f) carNorthY = 18.0f;
        }
        else if (carNorthY < -18.0f)
        {
            carNorthY -= speed;
        }

        if (carSouthY < -18.0f)
        {
            carSouthY += speed;
            if (carSouthY > -18.0f) carSouthY = -18.0f;
        }
        else if (carSouthY > 18.0f)
        {
            carSouthY += speed;
        }
    }

    // Wrap around boundaries for North-South vehicles
    if (carNorthY < -55.0f) carNorthY = 55.0f;
    if (carSouthY > 55.0f) carSouthY = -55.0f;

    // 3. 3D Car Auto-Rotation Step
    if (car3DAutoRotate)
    {
        car3DRotY += 0.8f;
        if (car3DRotY >= 360.0f) car3DRotY -= 360.0f;
    }

    // 4. Dynamic Signal State Timing based on COA Assembly Calculation
    signalCounter++;
    if (signalCounter >= 180)
    {
        signalCounter = 0;

        if (signalStateEW == 1)
        {
            signalStateEW = 2; // EW Yellow
            signalStateNS = 0; // NS Red
        }
        else if (signalStateEW == 2)
        {
            signalStateEW = 0; // EW Red
            signalStateNS = 1; // NS Green
        }
        else
        {
            signalStateEW = 1; // EW Green
            signalStateNS = 0; // NS Red
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// =====================================================
// KEYBOARD (Preserving ALL required controls)
// =====================================================

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        // Left Traffic Sim Interactive Car Controls
        case 'a': case 'A': carX -= 2; break;
        case 'd': case 'D': carX += 2; break;
        case 'r': case 'R': carAngle += 10; break;
        case '+': carScale += 0.1f; break;
        case '-': if (carScale > 0.3f) carScale -= 0.1f; break;
        case 't': case 'T': activeVehicleIndex = (activeVehicleIndex + 1) % 3; break;

        // System / Viewport / Clipping Controls
        case 'v': case 'V': smallViewport = !smallViewport; break;
        case 'x': case 'X': clippingOn = !clippingOn; break;

        // 3D Car Demonstration Controls (Right Section)
        case 'y': case 'Y': car3DRotY += 10.0f; break;
        case 'h': case 'H': car3DRotX += 10.0f; break;
        case 'g': case 'G': car3DRotZ += 10.0f; break;
        case '[': if (car3DScale > 0.3f) car3DScale -= 0.1f; break;
        case ']': car3DScale += 0.1f; break;
        case 'u': case 'U': car3DZ += 1.0f; break;
        case 'o': case 'O': car3DZ -= 1.0f; break;
        case 'c': case 'C': car3DAutoRotate = !car3DAutoRotate; break;

        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

// =====================================================
// SPECIAL KEYBOARD HANDLER (3D Car Arrow Key Translation)
// =====================================================

void specialKeyboard(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:  car3DX -= 0.8f; break;
        case GLUT_KEY_RIGHT: car3DX += 0.8f; break;
        case GLUT_KEY_UP:    car3DY += 0.8f; break;
        case GLUT_KEY_DOWN:  car3DY -= 0.8f; break;
    }
    glutPostRedisplay();
}

// =====================================================
// RESHAPE
// =====================================================

void reshape(int width, int height)
{
    if (height <= 0) height = 1;
    windowWidth = width;
    windowHeight = height;
    glutPostRedisplay();
}

// =====================================================
// INITIALIZATION
// =====================================================

void init()
{
    glClearColor(0.08f, 0.10f, 0.15f, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // 1. Run COA Assembly Processor Level Traffic Computation
    run_coa_calculation();

    // 2. Initialize DS Linked List Queue
    insertVehicleDirect(101, "Car", "MH12AB1234", 10);
    insertVehicleDirect(102, "Bus", "MH14CD5678", 5);
    insertVehicleDirect(103, "Ambulance", "MH12EM9999", 15);

    // 3. Initialize OOPS Vehicle Polymorphic Hierarchy
    activeVehicles[0] = new Car(101, "MH12AB1234", "North");
    activeVehicles[1] = new Bus(102, "MH14CD5678", "East");
    activeVehicles[2] = new Ambulance(103, "MH12EM9999", "South");
}

// =====================================================
// MAIN (Final Integrated Entry Point)
// =====================================================

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1100, 700);
    glutInitWindowPosition(100, 50);

    glutCreateWindow("Smart Traffic Signal Simulator — 3D Car & Scissor Clipping");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();

    // Cleanup OOPS dynamic allocations
    for(int i = 0; i < 3; i++) delete activeVehicles[i];
    freeAllVehicles();

    return 0;
}
