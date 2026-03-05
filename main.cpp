#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

struct Appliance {
    string name;
    double powerW;
    double hoursPerDay;
};

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static double energyKwh(double watts, double hours) {
    return (watts * hours) / 1000.0;
}

static string lowerCopy(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

static int menu() {
    cout << "\nElectrical Load Monitoring System\n";
    cout << "1. Register Appliance\n";
    cout << "2. View Appliances\n";
    cout << "3. Search Appliance\n";
    cout << "4. Energy Summary\n";
    cout << "0. Exit\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
        clearInput();
        return -1;
    }
    return choice;
}

static Appliance registerAppliance() {
    Appliance a{};

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // name
    while (true) {
        cout << "Enter appliance name: ";
        getline(cin, a.name);
        if (!a.name.empty()) break;
        cout << "Name cannot be empty.\n";
    }

    // power
    while (true) {
        cout << "Enter power (Watts): ";
        cin >> a.powerW;
        if (!cin.fail() && a.powerW > 0) break;
        cout << "Power must be a number greater than 0.\n";
        clearInput();
    }

    // hours
    while (true) {
        cout << "Enter hours used per day (0 - 24): ";
        cin >> a.hoursPerDay;
        if (!cin.fail() && a.hoursPerDay >= 0 && a.hoursPerDay <= 24) break;
        cout << "Hours must be between 0 and 24.\n";
        clearInput();
    }

    cout << "Appliance registered.\n";
    return a;
}

static void viewAppliances(const vector<Appliance>& appliances) {
    if (appliances.empty()) {
        cout << "No appliances registered.\n";
        return;
    }

    cout << "\n================= APPLIANCES =================\n";
    cout << left
         << setw(5)  << "No."
         << setw(20) << "Name"
         << setw(12) << "Watts"
         << setw(12) << "Hours"
         << "\n----------------------------------------------\n";

    for (size_t i = 0; i < appliances.size(); i++) {
        const auto& a = appliances[i];
        cout << left
             << setw(5)  << (i + 1)
             << setw(20) << a.name
             << setw(12) << fixed << setprecision(2) << a.powerW
             << setw(12) << fixed << setprecision(2) << a.hoursPerDay
             << "\n";
    }
}

static void energySummary(const vector<Appliance>& appliances) {
    if (appliances.empty()) {
        cout << "No appliances registered.\n";
        return;
    }

    double total = 0.0;

    cout << "\n============= ENERGY SUMMARY (kWh/day) =============\n";
    cout << left << setw(20) << "Name" << setw(12) << "kWh/day" << "\n";
    cout << "----------------------------------------------------\n";

    for (const auto& a : appliances) {
        double kwh = energyKwh(a.powerW, a.hoursPerDay);
        total += kwh;
        cout << left << setw(20) << a.name
             << setw(12) << fixed << setprecision(3) << kwh << "\n";
    }

    cout << "----------------------------------------------------\n";
    cout << "TOTAL: " << fixed << setprecision(3) << total << " kWh/day\n";
}

static void searchAppliance(const vector<Appliance>& appliances) {
    if (appliances.empty()) {
        cout << "No appliances registered.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string query;
    cout << "Search name: ";
    getline(cin, query);

    if (query.empty()) {
        cout << "Search term cannot be empty.\n";
        return;
    }

    string q = lowerCopy(query);
    bool found = false;

    for (const auto& a : appliances) {
        if (lowerCopy(a.name).find(q) != string::npos) {
            if (!found) {
                cout << "\nResults:\n";
                cout << left
                     << setw(20) << "Name"
                     << setw(12) << "Watts"
                     << setw(12) << "Hours"
                     << setw(12) << "kWh/day"
                     << "\n----------------------------------------------\n";
            }
            found = true;

            cout << left
                 << setw(20) << a.name
                 << setw(12) << fixed << setprecision(2) << a.powerW
                 << setw(12) << fixed << setprecision(2) << a.hoursPerDay
                 << setw(12) << fixed << setprecision(3) << energyKwh(a.powerW, a.hoursPerDay)
                 << "\n";
        }
    }

    if (!found) {
        cout << "No appliance matched: " << query << "\n";
    }
}

int main() {
    vector<Appliance> appliances;

    while (true) {
        int choice = menu();

        switch (choice) {
            case 1:
                appliances.push_back(registerAppliance());
                break;
            case 2:
                viewAppliances(appliances);
                break;
            case 3:
                searchAppliance(appliances);
                break;
            case 4:
                energySummary(appliances);
                break;
            case 0:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid option.\n";
        }
    }
}