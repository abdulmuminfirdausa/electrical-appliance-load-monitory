#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// Structure to store appliance data
struct Appliance {
    string name;
    double power;
    double hours;
};

// Function to calculate energy
double calculateEnergy(double power, double hours) {
    return (power * hours) / 1000;
}

int main() {

    vector<Appliance> appliances;

    int choice;

    while (true) {

        cout << "\nElectrical Load Monitoring System\n";
        cout << "1. Register Appliance\n";
        cout << "2. View Appliances\n";
        cout << "3. Energy Summary\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {

            Appliance a;

            cout << "Enter appliance name: ";
            cin >> a.name;

            cout << "Enter power (Watts): ";
            cin >> a.power;

            cout << "Enter hours used per day: ";
            cin >> a.hours;

            appliances.push_back(a);

        }

        else if (choice == 2) {

            for (auto a : appliances) {
                cout << a.name << "  "
                     << a.power << "W  "
                     << a.hours << " hours\n";
            }

        }

        else if (choice == 3) {

            double totalEnergy = 0;

            cout << "\nEnergy Summary\n";

            for (auto a : appliances) {

                double energy = calculateEnergy(a.power, a.hours);

                cout << a.name << " -> "
                     << fixed << setprecision(2)
                     << energy << " kWh/day\n";

                totalEnergy += energy;
            }

            cout << "Total Energy: "
                 << totalEnergy
                 << " kWh/day\n";
        }

        else if (choice == 0) {
            break;
        }

    }

    return 0;
}