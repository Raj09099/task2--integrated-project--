#ifndef DS_H
#define DS_H

#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Data Structure Node representing a Vehicle in the Waiting List
struct Node
{
    int vehicleId;
    string vehicleType;
    string registrationNo;
    int waitingTime;

    Node* next;
};

// Global head pointer for the DS module
extern Node* head;

// Helper: safely read an integer from console
int readInt(const string& prompt);

// DS Linked List Function Declarations
void insertVehicle();
void insertVehicleDirect(int id, const string& type, const string& regNo, int waitTime);
void displayVehicles();
void searchVehicle();
void updateVehicle();
void deleteVehicle();
void sortVehicles();
void freeAllVehicles();
int getVehicleCount();

#endif // DS_H
