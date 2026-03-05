#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Structure to store appliance information
struct Appliance
{
    string name;
    double power;
    double hours;
};

// Function to register appliance
void registerAppliance(vector<Appliance>& appliances)
{
    Appliance a;

    cout << "Enter appliance name: ";
    cin >> a.name;

    cout << "Enter power rating (W): ";
    cin >> a.power;

    cout << "Enter hours used per day: ";
    cin >> a.hours;

    appliances.push_back(a);

    cout << "Appliance added successfully.\n";
}

// Function to display appliances
void viewAppliances(const vector<Appliance>& appliances)
{
    if (appliances.empty())
    {
        cout << "No appliances registered.\n";
        return;
    }

    cout << "\nRegistered Appliances\n";

    for (int i = 0; i < appliances.size(); i++)
    {
        cout << i + 1 << ". "
             << appliances[i].name << " | "
             << appliances[i].power << "W | "
             << appliances[i].hours << " hrs\n";
    }
}

int main()
{
    vector<Appliance> appliances;
    int choice;

    do
    {
        cout << "\nElectrical Appliance Load Monitoring\n";
        cout << "1. Register Appliance\n";
        cout << "2. View Appliances\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                registerAppliance(appliances);
                break;

            case 2:
                viewAppliances(appliances);
                break;

            case 0:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid option\n";
        }

    } while(choice != 0);

    return 0;
}