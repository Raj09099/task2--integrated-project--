#include "oops.h"

#ifdef OOPS_STANDALONE
int main()
{
    cout << "============================================" << endl;
    cout << "       SMART TRAFFIC SIGNAL SIMULATOR      " << endl;
    cout << "          REVIEW 2 OOPS IMPLEMENTATION     " << endl;
    cout << "============================================" << endl;

    // Base class pointers demonstrating polymorphism
    Vehicle* vehicles[3];

    vehicles[0] = new Car(
        101,
        "MH12AB1234",
        "North"
    );

    vehicles[1] = new Bus(
        102,
        "MH14CD5678",
        "East"
    );

    vehicles[2] = new Ambulance(
        103,
        "MH12EM9999",
        "South"
    );

    cout << "\nVehicle Details:\n";

    for(int i = 0; i < 3; i++)
    {
        vehicles[i]->displayVehicle();

        cout << "Priority: "
             << vehicles[i]->getPriority()
             << endl;
    }

    // Delete dynamically allocated objects
    for(int i = 0; i < 3; i++)
    {
        delete vehicles[i];
    }

    return 0;
}
#endif
