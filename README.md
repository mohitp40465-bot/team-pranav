# team-pranav
The Smart Vehicle Rental Management System is a C++-based project that simplifies vehicle booking and management. It allows customers to view vehicles, book and return cars, calculate rental charges, and join waiting lists. Admins can manage booking history, monitor revenue, and generate receipts. It uses OOP, queues, vectors, and file handling.
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream>
#include <queue>

using namespace std;

// ==========================================
// 1. DATA STRUCTURES (C-Style Structs)
// ==========================================

struct Customer {
    string name;
    string mobile;
    string dl;
    string address;
};

struct Vehicle {
    string model;
    double rentPerDay;
    bool isAvailable;
    queue<Customer> waitlist; // FEATURE 3: Waitlist using Queue Data Structure

    Vehicle(string m, double r) : model(m), rentPerDay(r), isAvailable(true) {}
};

struct Booking {
    int bookingID;
    Customer customer;
    string vehicleModel;
    int durationDays;
    double totalPayment;
    bool isActive; // Tracks if the car is currently with the customer

    Booking(int id, Customer c, string vModel, int days, double payment)
        : bookingID(id), customer(c), vehicleModel(vModel), durationDays(days), totalPayment(payment), isActive(true) {}
};

// ==========================================
// 2. SYSTEM MANAGEMENT CLASS (OOP)
// ==========================================

class RentalSystem {
private:
    vector<Vehicle> vehicles;
    vector<Booking> bookingHistory;
    int nextBookingID;
    double totalRevenue;

    // Helper to clear input buffer and prevent infinite loops
    void clearBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    RentalSystem() {
        nextBookingID = 101;
        totalRevenue = 0.0;
        // Pre-populating inventory
        vehicles.push_back(Vehicle("Hyundai Creta", 2000.0));
        vehicles.push_back(Vehicle("Maruti Swift", 1200.0));
        vehicles.push_back(Vehicle("Honda City", 1800.0));
        vehicles.push_back(Vehicle("Mahindra Thar", 3000.0));
    }

    // --- CUSTOMER FEATURES ---

    void showVehicles() {
        cout << "\n================ CURRENT INVENTORY ================\n";
        for (size_t i = 0; i < vehicles.size(); i++) {
            cout << "[" << i + 1 << "] " << left << setw(15) << vehicles[i].model 
                 << " | Rent: Rs. " << setw(7) << fixed << setprecision(2) << vehicles[i].rentPerDay << "/day | Status: ";
            if (vehicles[i].isAvailable) {
                cout << "AVAILABLE\n";
            } else {
                cout << "BOOKED (Waitlist: " << vehicles[i].waitlist.size() << ")\n";
            }
        }
        cout << "===================================================\n";
    }

    void bookVehicle() {
        int choice, days;
        Customer c;

        showVehicles();
        cout << "\nSelect Vehicle Number to Book: ";
        while (!(cin >> choice) || choice < 1 || choice > vehicles.size()) {
            cout << "Invalid selection. Try again: ";
            clearBuffer();
        }
        clearBuffer();

        int vIndex = choice - 1;

        cout << "\n--- Enter Customer Details ---\n";
        cout << "Full Name: "; getline(cin, c.name);
        cout << "Mobile No: "; getline(cin, c.mobile);
        cout << "Driving License (DL): "; getline(cin, c.dl);
        cout << "Address: "; getline(cin, c.address);

        // FEATURE 3: Queue Waitlist Logic
        if (!vehicles[vIndex].isAvailable) {
            char joinQueue;
            cout << "\n[!] This vehicle is currently booked.\n";
            cout << "Would you like to join the waitlist? (Y/N): ";
            cin >> joinQueue;
            if (toupper(joinQueue) == 'Y') {
                vehicles[vIndex].waitlist.push(c);
                cout << "Added to waitlist! You are position " << vehicles[vIndex].waitlist.size() << " in line.\n";
            }
            return;
        }

        cout << "Duration (in days): "; 
        while (!(cin >> days) || days <= 0) {
            cout << "Enter a valid number of days: ";
            clearBuffer();
        }

        // Processing Booking
        double payment = days * vehicles[vIndex].rentPerDay;
        vehicles[vIndex].isAvailable = false; 
        
        Booking newBooking(nextBookingID++, c, vehicles[vIndex].model, days, payment);
        bookingHistory.push_back(newBooking);
        totalRevenue += payment;

        cout << "\n>>> BOOKING SUCCESSFUL! <<<\n";
        cout << "Your Booking ID is: " << newBooking.bookingID << "\n";
        
        generateBill(newBooking); // Call feature 5
    }

    // FEATURE 1: Vehicle Return Mechanism
    void returnVehicle() {
        int bID;
        cout << "\nEnter your Booking ID to return the vehicle: ";
        cin >> bID;

        bool found = false;
        for (auto& b : bookingHistory) {
            if (b.bookingID == bID && b.isActive) {
                b.isActive = false; // Mark returned
                found = true;
                cout << "\nVehicle successfully returned! Hope you had a safe trip.\n";

                // Find the vehicle in inventory to make it available
                for (auto& v : vehicles) {
                    if (v.model == b.vehicleModel) {
                        v.isAvailable = true;

                        // Check if someone is waiting for this car
                        if (!v.waitlist.empty()) {
                            Customer nextCust = v.waitlist.front();
                            v.waitlist.pop();
                            cout << "\n[SYSTEM ALERT] Vehicle auto-assigned to waitlisted customer: " << nextCust.name << "!\n";
                            v.isAvailable = false;
                            
                            // Auto-create 1-day default booking for waitlist (can be adjusted later)
                            Booking waitlistBooking(nextBookingID++, nextCust, v.model, 1, v.rentPerDay);
                            bookingHistory.push_back(waitlistBooking);
                            totalRevenue += v.rentPerDay;
                            generateBill(waitlistBooking);
                        }
                        break;
                    }
                }
                break;
            }
        }
        if (!found) cout << "[Error] Invalid Booking ID or vehicle already returned.\n";
    }

    // FEATURE 5: Generate Printable Bill (File Handling)
    void generateBill(Booking b) {
        string filename = "Receipt_" + to_string(b.bookingID) + ".txt";
        ofstream bill(filename); // Creates a text file

        if (bill.is_open()) {
            bill << "====================================\n";
            bill << "      SMART VEHICLE RENTALS         \n";
            bill << "====================================\n";
            bill << "Booking ID : " << b.bookingID << "\n";
            bill << "Customer   : " << b.customer.name << "\n";
            bill << "Mobile     : " << b.customer.mobile << "\n";
            bill << "Vehicle    : " << b.vehicleModel << "\n";
            bill << "Duration   : " << b.durationDays << " Days\n";
            bill << "------------------------------------\n";
            bill << "Base Rent  : Rs. " << fixed << setprecision(2) << b.totalPayment * 0.82 << "\n";
            bill << "Taxes (18%): Rs. " << b.totalPayment * 0.18 << "\n";
            bill << "------------------------------------\n";
            bill << "TOTAL PAID : Rs. " << b.totalPayment << "\n";
            bill << "====================================\n";
            bill << "     Drive Safe. Come Again!        \n";
            bill.close();
            cout << "[Success] A printable receipt has been saved as '" << filename << "' in your folder.\n";
        }
    }

    // --- ADMIN FEATURES ---

    void viewHistory() {
        cout << "\n=============== ALL BOOKING HISTORY ===============\n";
        if (bookingHistory.empty()) {
            cout << "No bookings in the system.\n";
            return;
        }
        for (const auto& b : bookingHistory) {
            cout << "ID: " << b.bookingID << " | Cust: " << setw(10) << b.customer.name 
                 << " | Car: " << setw(13) << b.vehicleModel 
                 << " | Rs." << b.totalPayment 
                 << " | Status: " << (b.isActive ? "ON RENT" : "RETURNED") << "\n";
        }
        cout << "===================================================\n";
    }

    void viewRevenue() {
        cout << "\n[FINANCIAL REPORT]\n";
        cout << "Total Revenue Generated: Rs. " << fixed << setprecision(2) << totalRevenue << "\n\n";
    }

    // FEATURE 2: Save Database on Exit (File Handling)
    void saveDatabase() {
        ofstream db("RentalDatabase.csv");
        if (db.is_open()) {
            db << "BookingID,CustomerName,Vehicle,Duration,Payment,Status\n";
            for (const auto& b : bookingHistory) {
                db << b.bookingID << "," << b.customer.name << "," << b.vehicleModel 
                   << "," << b.durationDays << "," << b.totalPayment << "," 
                   << (b.isActive ? "Active" : "Returned") << "\n";
            }
            db.close();
            cout << "Database successfully backed up to 'RentalDatabase.csv'.\n";
        }
    }
};

// ==========================================
// 3. MAIN MENU & ROLES (FEATURE 4)
// ==========================================

int main() {
    RentalSystem system;
    int mainRole;

    do {
        cout << "\n**************************************************\n";
        cout << "      SMART VEHICLE RENTAL MANAGEMENT SYSTEM      \n";
        cout << "**************************************************\n";
        cout << "Select Login Role:\n";
        cout << "1. Customer Portal\n";
        cout << "2. Admin Portal\n";
        cout << "3. Exit System\n";
        cout << "Enter choice: ";
        
        if (!(cin >> mainRole)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            mainRole = 0;
        }

        if (mainRole == 1) {
            // --- CUSTOMER MENU ---
            int custOpt;
            do {
                cout << "\n--- CUSTOMER PORTAL ---\n";
                cout << "1. View Available Cars\n2. Book a Car\n3. Return a Car\n4. Main Menu\nSelect: ";
                cin >> custOpt;
                switch (custOpt) {
                    case 1: system.showVehicles(); break;
                    case 2: system.bookVehicle(); break;
                    case 3: system.returnVehicle(); break;
                    case 4: break;
                    default: cout << "Invalid option.\n";
                }
            } while (custOpt != 4);
            
        } 
        else if (mainRole == 2) {
            // --- ADMIN MENU ---
            string password;
            cout << "\nEnter Admin Password: ";
            cin >> password;
            
            if (password == "admin123") {
                int adminOpt;
                do {
                    cout << "\n--- ADMIN PORTAL ---\n";
                    cout << "1. View Complete Inventory\n2. View Booking History\n3. View Financial Report\n4. Main Menu\nSelect: ";
                    cin >> adminOpt;
                    switch (adminOpt) {
                        case 1: system.showVehicles(); break;
                        case 2: system.viewHistory(); break;
                        case 3: system.viewRevenue(); break;
                        case 4: break;
                        default: cout << "Invalid option.\n";
                    }
                } while (adminOpt != 4);
            } else {
                cout << "[Access Denied] Incorrect password.\n";
            }
        } 
        else if (mainRole == 3) {
            cout << "\nInitiating system shutdown...\n";
            system.saveDatabase(); // Save data before closing
            cout << "Goodbye!\n";
        } 
        else {
            cout << "Invalid selection.\n";
        }
    } while (mainRole != 3);

    return 0;
}
