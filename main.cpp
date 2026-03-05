#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

struct Appliance {
    string name;
    double powerW;
    double hoursPerDay;
};

const string APPLIANCES_FILE = "appliances.txt";
const string BILLING_FILE    = "billing_summary.txt";

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

// ---------- FILE HANDLING ----------
static void saveAppliances(const vector<Appliance>& appliances) {
    ofstream out(APPLIANCES_FILE);
    if (!out) {
        cout << "Error: Could not save appliances to file.\n";
        return;
    }

    for (const auto& a : appliances) {
        out << a.name << "|" << a.powerW << "|" << a.hoursPerDay << "\n";
    }
}

static vector<Appliance> loadAppliances() {
    vector<Appliance> appliances;
    ifstream in(APPLIANCES_FILE);
    if (!in) return appliances; // file might not exist yet

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string name, pStr, hStr;

        if (!getline(ss, name, '|')) continue;
        if (!getline(ss, pStr, '|')) continue;
        if (!getline(ss, hStr, '|')) continue;

        try {
            Appliance a{};
            a.name = name;
            a.powerW = stod(pStr);
            a.hoursPerDay = stod(hStr);

            if (a.name.empty()) continue;
            if (a.powerW <= 0) continue;
            if (a.hoursPerDay < 0 || a.hoursPerDay > 24) continue;

            appliances.push_back(a);
        } catch (...) {
            continue;
        }
    }
    return appliances;
}

// ---------- MENU ----------
static int menu() {
    cout << "\nElectrical Load Monitoring System\n";
    cout << "1. Register Appliance\n";
    cout << "2. View Appliances\n";
    cout << "3. Search Appliance\n";
    cout << "4. Energy Summary\n";
    cout << "5. Billing Summary (save to file)\n";
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

// ---------- FEATURES ----------
static Appliance registerAppliance() {
    Appliance a{};

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (true) {
        cout << "Enter appliance name: ";
        getline(cin, a.name);
        if (!a.name.empty()) break;
        cout << "Name cannot be empty.\n";
    }

    while (true) {
        cout << "Enter power (Watts): ";
        cin >> a.powerW;
        if (!cin.fail() && a.powerW > 0) break;
        cout << "Power must be a number > 0.\n";
        clearInput();
    }

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

    if (!found) cout << "No appliance matched: " << query << "\n";
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

static void billingSummaryToFile(const vector<Appliance>& appliances) {
    if (appliances.empty()) {
        cout << "No appliances registered.\n";
        return;
    }

    double tariff;
    while (true) {
        cout << "Enter tariff per kWh: ";
        cin >> tariff;
        if (!cin.fail() && tariff > 0) break;
        cout << "Tariff must be a positive number.\n";
        clearInput();
    }

    ofstream out(BILLING_FILE);
    if (!out) {
        cout << "Error: Could not write billing file.\n";
        return;
    }

    // Print + Save report
    cout << "\n================ BILLING REPORT ================\n";
    out  << "================ BILLING REPORT ================\n";

    cout << left << setw(5) << "No." << setw(20) << "Name"
         << setw(12) << "kWh/day" << setw(12) << "Cost/day" << "\n";
    out  << left << setw(5) << "No." << setw(20) << "Name"
         << setw(12) << "kWh/day" << setw(12) << "Cost/day" << "\n";

    cout << "------------------------------------------------\n";
    out  << "------------------------------------------------\n";

    double totalEnergyDay = 0.0;
    double totalCostDay = 0.0;

    for (size_t i = 0; i < appliances.size(); i++) {
        const auto& a = appliances[i];

        double kwhDay = energyKwh(a.powerW, a.hoursPerDay);
        double costDay = kwhDay * tariff;

        totalEnergyDay += kwhDay;
        totalCostDay += costDay;

        cout << left << setw(5) << (i + 1)
             << setw(20) << a.name
             << setw(12) << fixed << setprecision(3) << kwhDay
             << setw(12) << fixed << setprecision(2) << costDay << "\n";

        out << left << setw(5) << (i + 1)
            << setw(20) << a.name
            << setw(12) << fixed << setprecision(3) << kwhDay
            << setw(12) << fixed << setprecision(2) << costDay << "\n";
    }

    double monthlyEnergy = totalEnergyDay * 30;
    double monthlyCost = totalCostDay * 30;

    cout << "------------------------------------------------\n";
    out  << "------------------------------------------------\n";

    cout << "Total Energy (per day): " << fixed << setprecision(3) << totalEnergyDay << " kWh/day\n";
    cout << "Total Cost (per day):   " << fixed << setprecision(2) << totalCostDay << "\n";
    cout << "Monthly Energy (30d):   " << fixed << setprecision(3) << monthlyEnergy << " kWh\n";
    cout << "Monthly Cost (30d):     " << fixed << setprecision(2) << monthlyCost << "\n";

    out  << "Total Energy (per day): " << fixed << setprecision(3) << totalEnergyDay << " kWh/day\n";
    out  << "Total Cost (per day):   " << fixed << setprecision(2) << totalCostDay << "\n";
    out  << "Monthly Energy (30d):   " << fixed << setprecision(3) << monthlyEnergy << " kWh\n";
    out  << "Monthly Cost (30d):     " << fixed << setprecision(2) << monthlyCost << "\n";

    out.close();
    cout << "Billing report saved to " << BILLING_FILE << "\n";
}

int main() {
    // Week 4: load appliances at startup
    vector<Appliance> appliances = loadAppliances();

    while (true) {
        int choice = menu();

        switch (choice) {
            case 1: {
                appliances.push_back(registerAppliance());
                saveAppliances(appliances);
                break;
            }
            case 2:
                viewAppliances(appliances);
                break;
            case 3:
                searchAppliance(appliances);
                break;
            case 4:
                energySummary(appliances);
                break;
            case 5:
                billingSummaryToFile(appliances);
                break;
            case 0:
                saveAppliances(appliances);
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid option.\n";
        }
    }
}