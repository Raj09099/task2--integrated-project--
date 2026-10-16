#include "ds.h"

Node* head = NULL;

// Helper: safely read an integer, re-prompting on bad input
int readInt(const string& prompt)
{
    int value;

    while (true)
    {
        cout << prompt;
        cin >> value;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
        }
        else
        {
            return value;
        }
    }
}

// Function to insert a vehicle programmatically
void insertVehicleDirect(int id, const string& type, const string& regNo, int waitTime)
{
    Node* newNode = new Node;
    newNode->vehicleId = id;
    newNode->vehicleType = type;
    newNode->registrationNo = regNo;
    newNode->waitingTime = waitTime;
    newNode->next = NULL;

    if (head == NULL)
    {
        head = newNode;
    }
    else
    {
        Node* temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Function to insert a vehicle via CLI interactive input
void insertVehicle()
{
    Node* newNode = new Node;

    newNode->vehicleId = readInt("\nEnter Vehicle ID: ");

    cout << "Enter Vehicle Type: ";
    cin >> ws;
    getline(cin, newNode->vehicleType);

    cout << "Enter Registration Number: ";
    cin >> newNode->registrationNo;

    newNode->waitingTime = readInt("Enter Waiting Time: ");

    newNode->next = NULL;

    // If list is empty
    if (head == NULL)
    {
        head = newNode;
    }
    else
    {
        Node* temp = head;

        // Move to the last node
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        // Connect last node to new node
        temp->next = newNode;
    }

    cout << "\nVehicle inserted successfully!\n";
}

// Function to display all vehicles
void displayVehicles()
{
    if (head == NULL)
    {
        cout << "\nNo vehicles available.\n";
        return;
    }

    Node* temp = head;

    cout << "\n----------------------------------------------------------\n";
    cout << "ID\tType\t\tRegistration\tWaiting Time\n";
    cout << "----------------------------------------------------------\n";

    while (temp != NULL)
    {
        cout << temp->vehicleId << "\t"
             << temp->vehicleType << "\t\t"
             << temp->registrationNo << "\t"
             << temp->waitingTime << " min"
             << endl;

        temp = temp->next;
    }

    cout << "----------------------------------------------------------\n";
}

// Function to search for a vehicle
void searchVehicle()
{
    int id = readInt("\nEnter Vehicle ID to search: ");

    Node* temp = head;

    while (temp != NULL)
    {
        if (temp->vehicleId == id)
        {
            cout << "\nVehicle Found!\n";
            cout << "Vehicle ID: " << temp->vehicleId << endl;
            cout << "Vehicle Type: " << temp->vehicleType << endl;
            cout << "Registration Number: "
                 << temp->registrationNo << endl;
            cout << "Waiting Time: "
                 << temp->waitingTime << " minutes"
                 << endl;

            return;
        }

        temp = temp->next;
    }

    cout << "\nVehicle not found!\n";
}

// Function to update a vehicle
void updateVehicle()
{
    int id = readInt("\nEnter Vehicle ID to update: ");

    Node* temp = head;

    while (temp != NULL)
    {
        if (temp->vehicleId == id)
        {
            cout << "Enter New Vehicle Type: ";
            cin >> ws;
            getline(cin, temp->vehicleType);

            cout << "Enter New Registration Number: ";
            cin >> temp->registrationNo;

            temp->waitingTime = readInt("Enter New Waiting Time: ");

            cout << "\nVehicle updated successfully!\n";

            return;
        }

        temp = temp->next;
    }

    cout << "\nVehicle not found!\n";
}

// Function to delete a vehicle
void deleteVehicle()
{
    int id = readInt("\nEnter Vehicle ID to delete: ");

    Node* temp = head;
    Node* previous = NULL;

    while (temp != NULL)
    {
        if (temp->vehicleId == id)
        {
            // If deleting the first node
            if (previous == NULL)
            {
                head = temp->next;
            }
            else
            {
                // Connect previous node to next node
                previous->next = temp->next;
            }

            delete temp;

            cout << "\nVehicle deleted successfully!\n";

            return;
        }

        previous = temp;
        temp = temp->next;
    }

    cout << "\nVehicle not found!\n";
}

// Function to sort vehicles according to waiting time
void sortVehicles()
{
    if (head == NULL || head->next == NULL)
    {
        cout << "\nNot enough vehicles to sort.\n";
        return;
    }

    Node* i;
    Node* j;

    for (i = head; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (i->waitingTime > j->waitingTime)
            {
                swap(i->vehicleId, j->vehicleId);
                swap(i->vehicleType, j->vehicleType);
                swap(i->registrationNo, j->registrationNo);
                swap(i->waitingTime, j->waitingTime);
            }
        }
    }

    cout << "\nVehicles sorted according to waiting time.\n";
}

// Function to free all allocated nodes before exiting
void freeAllVehicles()
{
    Node* temp = head;

    while (temp != NULL)
    {
        Node* next = temp->next;
        delete temp;
        temp = next;
    }

    head = NULL;
}

// Helper to return current number of vehicles in queue
int getVehicleCount()
{
    int count = 0;
    Node* temp = head;
    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

#ifdef DS_STANDALONE
int main()
{
    // Creating first vehicle
    Node* v1 = new Node;

    v1->vehicleId = 101;
    v1->vehicleType = "Car";
    v1->registrationNo = "MH12AB1234";
    v1->waitingTime = 10;
    v1->next = NULL;

    // Creating second vehicle
    Node* v2 = new Node;

    v2->vehicleId = 102;
    v2->vehicleType = "Bike";
    v2->registrationNo = "MH14CD5678";
    v2->waitingTime = 5;
    v2->next = NULL;

    // Creating third vehicle
    Node* v3 = new Node;

    v3->vehicleId = 103;
    v3->vehicleType = "Bus";
    v3->registrationNo = "MH12EF4567";
    v3->waitingTime = 15;
    v3->next = NULL;

    // Connecting the nodes
    head = v1;
    v1->next = v2;
    v2->next = v3;

    int choice;

    do
    {
        cout << "\n========================================";
        cout << "\n   SMART TRAFFIC SIGNAL SIMULATOR";
        cout << "\n       VEHICLE WAITING LIST";
        cout << "\n========================================";

        cout << "\n1. Insert Vehicle";
        cout << "\n2. Display Vehicles";
        cout << "\n3. Search Vehicle";
        cout << "\n4. Update Vehicle";
        cout << "\n5. Sort Vehicles";
        cout << "\n6. Delete Vehicle";
        cout << "\n7. Exit";

        cout << "\n\n";
        choice = readInt("Enter your choice: ");

        switch (choice)
        {
            case 1:
                insertVehicle();
                break;

            case 2:
                displayVehicles();
                break;

            case 3:
                searchVehicle();
                break;

            case 4:
                updateVehicle();
                break;

            case 5:
                sortVehicles();
                break;

            case 6:
                deleteVehicle();
                break;

            case 7:
                cout << "\nExiting program...\n";
                break;

            default:
                cout << "\nInvalid choice!\n";
        }

    } while (choice != 7);

    freeAllVehicles();

    return 0;
}
#endif
