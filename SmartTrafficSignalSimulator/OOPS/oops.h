#ifndef OOPS_H
#define OOPS_H

#include <iostream>
#include <string>
using namespace std;

// Base class
class Vehicle
{
protected:
    int vehicleId;
    string registrationNo;
    string direction;

public:
    Vehicle(int id, string reg, string dir)
    {
        vehicleId = id;
        registrationNo = reg;
        direction = dir;
    }

    virtual void displayVehicle()
    {
        cout << "Vehicle ID: " << vehicleId << endl;
        cout << "Registration: " << registrationNo << endl;
        cout << "Direction: " << direction << endl;
    }

    virtual int getPriority()
    {
        return 1;
    }

    virtual string getTypeName() const
    {
        return "Generic Vehicle";
    }

    int getVehicleId() const { return vehicleId; }
    string getRegistrationNo() const { return registrationNo; }
    string getDirection() const { return direction; }

    virtual ~Vehicle()
    {
    }
};

// Derived class 1 - Car
class Car : public Vehicle
{
public:
    Car(int id, string reg, string dir)
        : Vehicle(id, reg, dir)
    {
    }

    void displayVehicle() override
    {
        cout << "\nType: Car" << endl;
        cout << "Vehicle ID: " << vehicleId << endl;
        cout << "Registration: " << registrationNo << endl;
        cout << "Direction: " << direction << endl;
    }

    int getPriority() override
    {
        return 1;
    }

    string getTypeName() const override
    {
        return "Car";
    }
};

// Derived class 2 - Bus
class Bus : public Vehicle
{
public:
    Bus(int id, string reg, string dir)
        : Vehicle(id, reg, dir)
    {
    }

    void displayVehicle() override
    {
        cout << "\nType: Bus" << endl;
        cout << "Vehicle ID: " << vehicleId << endl;
        cout << "Registration: " << registrationNo << endl;
        cout << "Direction: " << direction << endl;
    }

    int getPriority() override
    {
        return 2;
    }

    string getTypeName() const override
    {
        return "Bus";
    }
};

// Derived class 3 - Ambulance
class Ambulance : public Vehicle
{
public:
    Ambulance(int id, string reg, string dir)
        : Vehicle(id, reg, dir)
    {
    }

    void displayVehicle() override
    {
        cout << "\nType: Ambulance" << endl;
        cout << "Vehicle ID: " << vehicleId << endl;
        cout << "Registration: " << registrationNo << endl;
        cout << "Direction: " << direction << endl;
    }

    int getPriority() override
    {
        return 10;
    }

    string getTypeName() const override
    {
        return "Ambulance";
    }
};

#endif // OOPS_H
