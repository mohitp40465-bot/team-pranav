#include "RentalSystem.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/types.h>
#define MKDIR(dir) mkdir(dir, 0777)
#endif

// ==========================================
// Console Input Utilities
// ==========================================
static void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// ==========================================
// RentalSystem Implementation
// ==========================================
RentalSystem::RentalSystem() : vehicleCount(0), nextBookingNum(1001), nextCustomerNum(101) {
    for (int i = 0; i < MAX_VEHICLES; ++i) {
        vehicles[i] = nullptr;
    }
    ensureDataDirectoryExists();
    loadData();

    // If no vehicles or customers were loaded, seed initial realistic demo data
    if (vehicleCount == 0 && customers.empty()) {
        seedInitialData();
    }
}

RentalSystem::~RentalSystem() {
    clearVehicles();
}

void RentalSystem::clearVehicles() {
    for (int i = 0; i < vehicleCount; ++i) {
        if (vehicles[i] != nullptr) {
            delete vehicles[i];
            vehicles[i] = nullptr;
        }
    }
    vehicleCount = 0;
}

void RentalSystem::ensureDataDirectoryExists() {
    struct stat info;
    if (stat("data", &info) != 0) {
        MKDIR("data");
    }
}

// ==========================================
// DSA: Array Linear Search Implementation
// ==========================================
int RentalSystem::findVehicleIndexLinearSearch(const std::string& id) const {
    // Demonstrating standard Linear Search algorithm (O(N) time complexity)
    for (int i = 0; i < vehicleCount; ++i) {
        if (vehicles[i] != nullptr && vehicles[i]->getId() == id) {
            return i; // Element found at index i
        }
    }
    return -1; // Element not found
}

int RentalSystem::findCustomerIndex(const std::string& id) const {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].getId() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int RentalSystem::findActiveBookingIndex(const std::string& bookingId) const {
    for (size_t i = 0; i < activeBookings.size(); ++i) {
        if (activeBookings[i].getBookingId() == bookingId) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// ==========================================
// 1. Vehicle Management
// ==========================================
bool RentalSystem::addVehicle(Vehicle* v) {
    if (vehicleCount >= MAX_VEHICLES) {
        std::cout << "[ERROR] Maximum vehicle storage capacity reached (" << MAX_VEHICLES << ")!\n";
        delete v;
        return false;
    }
    if (findVehicleIndexLinearSearch(v->getId()) != -1) {
        std::cout << "[ERROR] Vehicle with ID " << v->getId() << " already exists!\n";
        delete v;
        return false;
    }
    vehicles[vehicleCount++] = v;
    return true;
}

void RentalSystem::addNewVehicleInteractive() {
    std::cout << "\n========================================\n";
    std::cout << "           ADD NEW VEHICLE              \n";
    std::cout << "========================================\n";
    std::cout << "Select Vehicle Category:\n";
    std::cout << "1. Car\n";
    std::cout << "2. Bike\n";
    std::cout << "3. Scooter\n";
    std::cout << "Enter choice (1-3): ";
    
    int choice;
    if (!(std::cin >> choice)) {
        clearInputBuffer();
        std::cout << "[ERROR] Invalid choice input!\n";
        return;
    }
    clearInputBuffer();

    if (choice < 1 || choice > 3) {
        std::cout << "[ERROR] Invalid vehicle category selected.\n";
        return;
    }

    std::string id, name, brand;
    double rent;

    std::cout << "Enter Vehicle ID (e.g. C105, B204, S302): ";
    std::getline(std::cin, id);
    id = trim(id);
    if (id.empty()) {
        std::cout << "[ERROR] Vehicle ID cannot be empty.\n";
        return;
    }
    if (findVehicleIndexLinearSearch(id) != -1) {
        std::cout << "[ERROR] A vehicle with ID '" << id << "' already exists!\n";
        return;
    }

    std::cout << "Enter Vehicle Name/Model (e.g. Honda City): ";
    std::getline(std::cin, name);
    name = trim(name);

    std::cout << "Enter Brand (e.g. Honda, Yamaha, TVS): ";
    std::getline(std::cin, brand);
    brand = trim(brand);

    std::cout << "Enter Daily Rent Rate (Rs.): ";
    if (!(std::cin >> rent) || rent <= 0) {
        clearInputBuffer();
        std::cout << "[ERROR] Invalid rent rate. Must be a positive amount.\n";
        return;
    }
    clearInputBuffer();

    if (choice == 1) { // Car
        int seats;
        std::string fuel;
        std::cout << "Enter Seating Capacity (e.g. 4, 5, 7): ";
        if (!(std::cin >> seats) || seats <= 0) {
            clearInputBuffer();
            std::cout << "[ERROR] Invalid seating capacity.\n";
            return;
        }
        clearInputBuffer();
        std::cout << "Enter Fuel Type (Petrol/Diesel/Electric/CNG): ";
        std::getline(std::cin, fuel);
        fuel = trim(fuel);

        Car* car = new Car(id, name, brand, rent, seats, fuel, true);
        if (addVehicle(car)) {
            std::cout << "[SUCCESS] Car added successfully!\n";
        }
    } else if (choice == 2) { // Bike
        int cc;
        std::string category;
        std::cout << "Enter Engine Capacity in CC (e.g. 150, 350): ";
        if (!(std::cin >> cc) || cc <= 0) {
            clearInputBuffer();
            std::cout << "[ERROR] Invalid engine capacity.\n";
            return;
        }
        clearInputBuffer();
        std::cout << "Enter Bike Category (Sport/Cruiser/Commuter): ";
        std::getline(std::cin, category);
        category = trim(category);

        Bike* bike = new Bike(id, name, brand, rent, cc, category, true);
        if (addVehicle(bike)) {
            std::cout << "[SUCCESS] Bike added successfully!\n";
        }
    } else if (choice == 3) { // Scooter
        double storage;
        char evChar;
        std::cout << "Enter Boot Storage Capacity in Litres (e.g. 21.5): ";
        if (!(std::cin >> storage) || storage < 0) {
            clearInputBuffer();
            std::cout << "[ERROR] Invalid boot storage capacity.\n";
            return;
        }
        std::cout << "Is this an Electric Scooter (EV)? (y/n): ";
        std::cin >> evChar;
        clearInputBuffer();
        bool isEv = (evChar == 'y' || evChar == 'Y');

        Scooter* scooter = new Scooter(id, name, brand, rent, storage, isEv, true);
        if (addVehicle(scooter)) {
            std::cout << "[SUCCESS] Scooter added successfully!\n";
        }
    }
}

void RentalSystem::displayAllVehicles() const {
    if (vehicleCount == 0) {
        std::cout << "\n[INFO] No vehicles currently in the system.\n";
        return;
    }

    std::cout << "\n========================================================================================================\n";
    std::cout << "                                         ALL VEHICLES IN FLEET                                          \n";
    std::cout << "========================================================================================================\n";
    std::cout << std::left 
              << std::setw(8)  << "ID"
              << std::setw(18) << "Name"
              << std::setw(10) << "Type"
              << std::setw(14) << "Brand"
              << std::setw(12) << "Rent/Day"
              << std::setw(14) << "Status"
              << "Specifications"
              << "\n";
    std::cout << "--------------------------------------------------------------------------------------------------------\n";

    // Polymorphic function call: vehicles[i]->displayDetails()
    for (int i = 0; i < vehicleCount; ++i) {
        if (vehicles[i] != nullptr) {
            vehicles[i]->displayDetails();
        }
    }
    std::cout << "--------------------------------------------------------------------------------------------------------\n";
    std::cout << "Total vehicles in fleet: " << vehicleCount << "\n";
}

void RentalSystem::displayVehiclesByStatus(bool availableOnly) const {
    int count = 0;
    std::string header = availableOnly ? "AVAILABLE VEHICLES FOR RENT" : "CURRENTLY RENTED VEHICLES";

    std::cout << "\n========================================================================================================\n";
    std::cout << "                                  " << header << "                                      \n";
    std::cout << "========================================================================================================\n";
    std::cout << std::left 
              << std::setw(8)  << "ID"
              << std::setw(18) << "Name"
              << std::setw(10) << "Type"
              << std::setw(14) << "Brand"
              << std::setw(12) << "Rent/Day"
              << std::setw(14) << "Status"
              << "Specifications"
              << "\n";
    std::cout << "--------------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < vehicleCount; ++i) {
        if (vehicles[i] != nullptr && vehicles[i]->getAvailability() == availableOnly) {
            vehicles[i]->displayDetails();
            count++;
        }
    }
    std::cout << "--------------------------------------------------------------------------------------------------------\n";
    std::cout << "Total matching vehicles: " << count << "\n";
}

void RentalSystem::filterVehiclesByType(const std::string& type) const {
    int count = 0;
    std::cout << "\n========================================================================================================\n";
    std::cout << "                                 VEHICLES OF TYPE: " << type << "                                     \n";
    std::cout << "========================================================================================================\n";
    std::cout << std::left 
              << std::setw(8)  << "ID"
              << std::setw(18) << "Name"
              << std::setw(10) << "Type"
              << std::setw(14) << "Brand"
              << std::setw(12) << "Rent/Day"
              << std::setw(14) << "Status"
              << "Specifications"
              << "\n";
    std::cout << "--------------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < vehicleCount; ++i) {
        if (vehicles[i] != nullptr && vehicles[i]->getType() == type) {
            vehicles[i]->displayDetails();
            count++;
        }
    }
    std::cout << "--------------------------------------------------------------------------------------------------------\n";
    std::cout << "Total matching vehicles: " << count << "\n";
}

void RentalSystem::searchVehicleInteractive() const {
    std::cout << "\n--- Search Vehicle (Linear Search Algorithm) ---\n";
    std::cout << "Enter Vehicle ID to search: ";
    std::string id;
    std::getline(std::cin, id);
    id = trim(id);

    int idx = findVehicleIndexLinearSearch(id);
    if (idx != -1) {
        std::cout << "\n[FOUND] Vehicle located at array index [" << idx << "]:\n";
        std::cout << std::left 
                  << std::setw(8)  << "ID"
                  << std::setw(18) << "Name"
                  << std::setw(10) << "Type"
                  << std::setw(14) << "Brand"
                  << std::setw(12) << "Rent/Day"
                  << std::setw(14) << "Status"
                  << "Specifications\n";
        std::cout << "--------------------------------------------------------------------------------------------------------\n";
        vehicles[idx]->displayDetails();
    } else {
        std::cout << "[ERROR] Vehicle with ID '" << id << "' not found in the fleet.\n";
    }
}

void RentalSystem::updateVehicleInteractive() {
    std::cout << "\n--- Update Vehicle Information ---\n";
    std::cout << "Enter Vehicle ID to update: ";
    std::string id;
    std::getline(std::cin, id);
    id = trim(id);

    int idx = findVehicleIndexLinearSearch(id);
    if (idx == -1) {
        std::cout << "[ERROR] Vehicle with ID '" << id << "' not found.\n";
        return;
    }

    std::cout << "\nSelected Vehicle: " << vehicles[idx]->getName() << " (" << vehicles[idx]->getType() << ")\n";
    std::cout << "Current Rent/Day: Rs. " << vehicles[idx]->getRentPerDay() << "\n";

    std::cout << "Enter new Name (leave empty to keep current): ";
    std::string newName;
    std::getline(std::cin, newName);
    newName = trim(newName);
    if (!newName.empty()) {
        vehicles[idx]->setName(newName);
    }

    std::cout << "Enter new Brand (leave empty to keep current): ";
    std::string newBrand;
    std::getline(std::cin, newBrand);
    newBrand = trim(newBrand);
    if (!newBrand.empty()) {
        vehicles[idx]->setBrand(newBrand);
    }

    std::cout << "Enter new Daily Rent (enter 0 to keep current): Rs. ";
    double newRent;
    if (std::cin >> newRent && newRent > 0) {
        vehicles[idx]->setRentPerDay(newRent);
    }
    clearInputBuffer();

    std::cout << "[SUCCESS] Vehicle details updated successfully!\n";
}

void RentalSystem::deleteVehicleInteractive() {
    std::cout << "\n--- Delete Vehicle from Fleet ---\n";
    std::cout << "Enter Vehicle ID to delete: ";
    std::string id;
    std::getline(std::cin, id);
    id = trim(id);

    int idx = findVehicleIndexLinearSearch(id);
    if (idx == -1) {
        std::cout << "[ERROR] Vehicle with ID '" << id << "' not found.\n";
        return;
    }

    if (!vehicles[idx]->getAvailability()) {
        std::cout << "[ERROR] Cannot delete vehicle '" << id << "' because it is currently rented!\n";
        return;
    }

    std::cout << "Are you sure you want to delete " << vehicles[idx]->getName() << " (" << id << ")? (y/n): ";
    char confirm;
    std::cin >> confirm;
    clearInputBuffer();

    if (confirm == 'y' || confirm == 'Y') {
        // DSA Array deletion: Free dynamic memory and shift remaining elements left
        delete vehicles[idx];
        for (int i = idx; i < vehicleCount - 1; ++i) {
            vehicles[i] = vehicles[i + 1];
        }
        vehicles[vehicleCount - 1] = nullptr;
        vehicleCount--;
        std::cout << "[SUCCESS] Vehicle deleted successfully from array storage.\n";
    } else {
        std::cout << "[INFO] Deletion cancelled.\n";
    }
}

// ==========================================
// 2. Customer Management
// ==========================================
bool RentalSystem::registerCustomer(const std::string& name, const std::string& phone) {
    std::string cId = "C" + std::to_string(nextCustomerNum++);
    customers.push_back(Customer(cId, name, phone));
    std::cout << "[SUCCESS] Customer registered with Customer ID: " << cId << "\n";
    return true;
}

void RentalSystem::registerCustomerInteractive() {
    std::cout << "\n========================================\n";
    std::cout << "          REGISTER NEW CUSTOMER         \n";
    std::cout << "========================================\n";

    std::cout << "Enter Customer Full Name: ";
    std::string name;
    std::getline(std::cin, name);
    name = trim(name);
    if (name.empty()) {
        std::cout << "[ERROR] Customer name cannot be empty.\n";
        return;
    }

    std::cout << "Enter Phone Number (e.g. 9876543210): ";
    std::string phone;
    std::getline(std::cin, phone);
    phone = trim(phone);
    if (phone.empty()) {
        std::cout << "[ERROR] Phone number cannot be empty.\n";
        return;
    }

    registerCustomer(name, phone);
}

void RentalSystem::displayAllCustomers() const {
    if (customers.empty()) {
        std::cout << "\n[INFO] No registered customers found.\n";
        return;
    }

    std::cout << "\n=========================================================\n";
    std::cout << "                  REGISTERED CUSTOMERS                   \n";
    std::cout << "=========================================================\n";
    std::cout << std::left 
              << std::setw(12) << "Customer ID"
              << std::setw(25) << "Full Name"
              << std::setw(16) << "Phone Number"
              << "\n";
    std::cout << "---------------------------------------------------------\n";

    for (const auto& cust : customers) {
        cust.displayCustomer();
    }
    std::cout << "---------------------------------------------------------\n";
    std::cout << "Total registered customers: " << customers.size() << "\n";
}

void RentalSystem::searchCustomerInteractive() const {
    std::cout << "\n--- Search Customer by ID ---\n";
    std::cout << "Enter Customer ID (e.g. C101): ";
    std::string id;
    std::getline(std::cin, id);
    id = trim(id);

    int idx = findCustomerIndex(id);
    if (idx != -1) {
        std::cout << "\n[FOUND] Customer details:\n";
        std::cout << std::left 
                  << std::setw(12) << "Customer ID"
                  << std::setw(25) << "Full Name"
                  << std::setw(16) << "Phone Number"
                  << "\n";
        std::cout << "---------------------------------------------------------\n";
        customers[idx].displayCustomer();
    } else {
        std::cout << "[ERROR] Customer ID '" << id << "' not found.\n";
    }
}

// ==========================================
// 3. Booking Management
// ==========================================
bool RentalSystem::processBooking(const std::string& cId, const std::string& vId, int days) {
    int cIdx = findCustomerIndex(cId);
    if (cIdx == -1) {
        std::cout << "[ERROR] Customer ID '" << cId << "' does not exist.\n";
        return false;
    }

    int vIdx = findVehicleIndexLinearSearch(vId);
    if (vIdx == -1) {
        std::cout << "[ERROR] Vehicle ID '" << vId << "' does not exist.\n";
        return false;
    }

    if (days <= 0) {
        std::cout << "[ERROR] Invalid rental days. Must be at least 1 day.\n";
        return false;
    }

    Vehicle* v = vehicles[vIdx];
    if (!v->getAvailability()) {
        std::cout << "\n[UNAVAILABLE] Vehicle '" << v->getName() << "' (" << vId << ") is currently RENTED.\n";
        std::cout << "Would you like customer " << customers[cIdx].getName() 
                  << " to join the FIFO Waiting Queue for this vehicle? (y/n): ";
        char qChoice;
        std::cin >> qChoice;
        clearInputBuffer();
        if (qChoice == 'y' || qChoice == 'Y') {
            waitingQueue.enqueue(cId, customers[cIdx].getName(), vId);
        }
        return false;
    }

    // Calculate rent via Polymorphism
    double totalRent = v->calculateRent(days);

    std::cout << "\n----------------------------------------\n";
    std::cout << "           BOOKING SUMMARY              \n";
    std::cout << "----------------------------------------\n";
    std::cout << "Customer:     " << customers[cIdx].getName() << " (" << cId << ")\n";
    std::cout << "Vehicle:      " << v->getName() << " (" << vId << ") [" << v->getType() << "]\n";
    std::cout << "Rental Days:  " << days << "\n";
    std::cout << "Daily Rate:   Rs. " << v->getRentPerDay() << "\n";
    std::cout << "Total Rent:   Rs. " << std::fixed << std::setprecision(2) << totalRent << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Confirm this booking? (y/n): ";

    char confirm;
    std::cin >> confirm;
    clearInputBuffer();

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "[INFO] Booking cancelled by user.\n";
        return false;
    }

    // Generate unique booking ID
    std::string bId = "BK" + std::to_string(nextBookingNum++);

    // Mark vehicle as rented (Prevent double booking)
    v->setAvailability(false);

    // Create and save active booking
    Booking newBooking(bId, cId, customers[cIdx].getName(), vId, v->getName(), days, totalRent, "Active");
    activeBookings.push_back(newBooking);

    // Record in Rental History (Linked List)
    rentalHistory.insertRecord(bId, cId, customers[cIdx].getName(), vId, v->getName(), days, totalRent, "Active");

    std::cout << "\n[SUCCESS] Vehicle booked successfully!\n";
    std::cout << "Generated Booking ID: " << bId << "\n";
    return true;
}

void RentalSystem::bookVehicleInteractive() {
    std::cout << "\n========================================\n";
    std::cout << "             BOOK A VEHICLE             \n";
    std::cout << "========================================\n";

    if (customers.empty()) {
        std::cout << "[ERROR] No customers registered yet. Please register a customer first.\n";
        return;
    }

    // Step 1: Select a customer
    displayAllCustomers();
    std::cout << "Enter Customer ID for booking: ";
    std::string cId;
    std::getline(std::cin, cId);
    cId = trim(cId);

    int cIdx = findCustomerIndex(cId);
    if (cIdx == -1) {
        std::cout << "[ERROR] Invalid Customer ID.\n";
        return;
    }

    // Step 2: Display available vehicles
    displayVehiclesByStatus(true);

    // Step 3: Select a vehicle using its ID
    std::cout << "Enter Vehicle ID to book: ";
    std::string vId;
    std::getline(std::cin, vId);
    vId = trim(vId);

    // Step 4: Enter number of rental days
    std::cout << "Enter number of rental days: ";
    int days;
    if (!(std::cin >> days) || days <= 0) {
        clearInputBuffer();
        std::cout << "[ERROR] Invalid rental days entered. Rental days must be a positive integer.\n";
        return;
    }
    clearInputBuffer();

    // Steps 5-9: Check availability, calculate rent, confirm, change status, generate ID
    processBooking(cId, vId, days);
}

// ==========================================
// 4. Return Management
// ==========================================
void RentalSystem::returnVehicleInteractive() {
    std::cout << "\n========================================\n";
    std::cout << "            RETURN A VEHICLE            \n";
    std::cout << "========================================\n";

    if (activeBookings.empty()) {
        std::cout << "[INFO] There are currently no active rentals to return.\n";
        return;
    }

    std::cout << "\nCurrently Active Rentals:\n";
    std::cout << std::left 
              << std::setw(10) << "Book ID"
              << std::setw(12) << "Cust ID"
              << std::setw(18) << "Customer Name"
              << std::setw(10) << "Veh ID"
              << std::setw(22) << "Vehicle Name"
              << std::setw(8)  << "Days"
              << std::setw(12) << "Total Rent"
              << std::setw(12) << "Status"
              << "\n";
    std::cout << "--------------------------------------------------------------------------------------------------\n";
    for (const auto& b : activeBookings) {
        b.displayBooking();
    }
    std::cout << "--------------------------------------------------------------------------------------------------\n";

    std::cout << "Enter Booking ID to return (e.g. BK1001): ";
    std::string bId;
    std::getline(std::cin, bId);
    bId = trim(bId);

    int bIdx = findActiveBookingIndex(bId);
    if (bIdx == -1) {
        std::cout << "[ERROR] Active booking with ID '" << bId << "' not found.\n";
        return;
    }

    Booking booking = activeBookings[bIdx];
    std::string vId = booking.getVehicleId();
    int vIdx = findVehicleIndexLinearSearch(vId);

    std::cout << "\nBooking Verified:\n";
    std::cout << "Customer: " << booking.getCustomerName() << "\n";
    std::cout << "Vehicle:  " << booking.getVehicleName() << " (" << vId << ")\n";
    std::cout << "Original Scheduled Days: " << booking.getRentalDays() << "\n";
    std::cout << "Calculated Rent: Rs. " << booking.getTotalRent() << "\n";

    std::cout << "Enter actual days the vehicle was used (or press enter for scheduled " << booking.getRentalDays() << " days): ";
    std::string daysStr;
    std::getline(std::cin, daysStr);
    daysStr = trim(daysStr);

    int actualDays = booking.getRentalDays();
    if (!daysStr.empty()) {
        try {
            int inputDays = std::stoi(daysStr);
            if (inputDays > 0) actualDays = inputDays;
        } catch (...) {}
    }

    double finalRent = booking.getTotalRent();
    if (vIdx != -1 && actualDays != booking.getRentalDays()) {
        finalRent = vehicles[vIdx]->calculateRent(actualDays);
        if (actualDays > booking.getRentalDays()) {
            double lateFee = (actualDays - booking.getRentalDays()) * 100.0; // Rs 100 per day late penalty
            finalRent += lateFee;
            std::cout << "[NOTE] Late penalty fee of Rs. " << lateFee << " applied.\n";
        }
    }

    std::cout << "Final Total Rent Settlement: Rs. " << std::fixed << std::setprecision(2) << finalRent << "\n";
    std::cout << "Confirm vehicle return & payment settlement? (y/n): ";
    char confirm;
    std::cin >> confirm;
    clearInputBuffer();

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "[INFO] Return process cancelled.\n";
        return;
    }

    // 1. Update vehicle status to Available
    if (vIdx != -1) {
        vehicles[vIdx]->setAvailability(true);
    }

    // 2. Mark booking as completed in rental history (linked list)
    rentalHistory.updateStatus(bId, "Completed", finalRent);

    // 3. Remove from active bookings vector
    activeBookings.erase(activeBookings.begin() + bIdx);

    std::cout << "\n[SUCCESS] Vehicle returned successfully! Rental status updated to Completed.\n";

    // 4. Check Waiting Queue for eligible waiting customer!
    checkAndServeWaitingCustomer(vId);
}

// ==========================================
// 5. Waiting Queue Check & Serve
// ==========================================
void RentalSystem::displayWaitingQueue() const {
    waitingQueue.displayQueue();
}

void RentalSystem::checkAndServeWaitingCustomer(const std::string& vehicleId) {
    if (waitingQueue.hasWaitingCustomerForVehicle(vehicleId)) {
        std::cout << "\n********************************************************\n";
        std::cout << " [ALERT] Customer found in Waiting Queue for this Vehicle!\n";
        std::cout << "********************************************************\n";

        WaitingRequest nextCustomer;
        if (waitingQueue.serveCustomerForVehicle(vehicleId, nextCustomer)) {
            std::cout << "Waiting Customer: " << nextCustomer.customerName 
                      << " (ID: " << nextCustomer.customerId << ")\n";
            std::cout << "Vehicle ID: " << vehicleId << " is now AVAILABLE!\n";
            std::cout << "Would you like to book this vehicle for them now? (y/n): ";
            char ans;
            std::cin >> ans;
            clearInputBuffer();

            if (ans == 'y' || ans == 'Y') {
                std::cout << "Enter number of rental days for " << nextCustomer.customerName << ": ";
                int days;
                if (std::cin >> days && days > 0) {
                    clearInputBuffer();
                    processBooking(nextCustomer.customerId, vehicleId, days);
                } else {
                    clearInputBuffer();
                    std::cout << "[ERROR] Invalid rental days entered.\n";
                }
            } else {
                std::cout << "[INFO] Waiting customer was notified. Vehicle remains available for general rental.\n";
            }
        }
    }
}

// ==========================================
// 6. Rental History Management
// ==========================================
void RentalSystem::displayRentalHistory() const {
    rentalHistory.displayHistory();
}

void RentalSystem::searchRentalHistoryInteractive() const {
    std::cout << "\n--- Search Rental History Records ---\n";
    std::cout << "1. Search by Booking ID\n";
    std::cout << "2. Search by Customer ID\n";
    std::cout << "3. Search by Vehicle ID\n";
    std::cout << "Enter choice (1-3): ";
    int choice;
    if (!(std::cin >> choice)) {
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (choice == 1) {
        std::cout << "Enter Booking ID: ";
        std::string bId;
        std::getline(std::cin, bId);
        bId = trim(bId);
        HistoryNode* node = rentalHistory.searchByBookingId(bId);
        if (node != nullptr) {
            std::cout << "\n[FOUND] Rental Record:\n";
            std::cout << "Booking ID:    " << node->bookingId << "\n";
            std::cout << "Customer:      " << node->customerName << " (" << node->customerId << ")\n";
            std::cout << "Vehicle:       " << node->vehicleName << " (" << node->vehicleId << ")\n";
            std::cout << "Rental Days:   " << node->rentalDays << "\n";
            std::cout << "Total Rent:    Rs. " << node->totalRent << "\n";
            std::cout << "Status:        " << node->bookingStatus << "\n";
        } else {
            std::cout << "[ERROR] Booking ID '" << bId << "' not found in history.\n";
        }
    } else if (choice == 2) {
        std::cout << "Enter Customer ID: ";
        std::string cId;
        std::getline(std::cin, cId);
        cId = trim(cId);
        rentalHistory.searchByCustomerId(cId);
    } else if (choice == 3) {
        std::cout << "Enter Vehicle ID: ";
        std::string vId;
        std::getline(std::cin, vId);
        vId = trim(vId);
        rentalHistory.searchByVehicleId(vId);
    }
}

// ==========================================
// 7. Dashboard / Statistics
// ==========================================
void RentalSystem::displayDashboard() const {
    int totalVehicles = vehicleCount;
    int availableVehicles = 0;
    int rentedVehicles = 0;

    for (int i = 0; i < vehicleCount; ++i) {
        if (vehicles[i] != nullptr) {
            if (vehicles[i]->getAvailability()) availableVehicles++;
            else rentedVehicles++;
        }
    }

    int totalCustomers = static_cast<int>(customers.size());
    int activeCount = static_cast<int>(activeBookings.size());
    int completedBookings = rentalHistory.countCompletedBookings();
    double totalRevenue = rentalHistory.calculateTotalRevenue();

    std::cout << "\n=======================================================\n";
    std::cout << "           SYSTEM DASHBOARD & ANALYTICS                \n";
    std::cout << "=======================================================\n";
    std::cout << " Total Fleet Vehicles:       " << totalVehicles << "\n";
    std::cout << "   -> Available for Rent:    " << availableVehicles << "\n";
    std::cout << "   -> Currently Rented:      " << rentedVehicles << "\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << " Registered Customers:       " << totalCustomers << "\n";
    std::cout << " Active Ongoing Rentals:     " << activeCount << "\n";
    std::cout << " Completed Rentals:          " << completedBookings << "\n";
    std::cout << " Customers on Waiting List:  " << waitingQueue.getCount() << "\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << " Total Revenue Generated:    Rs. " << std::fixed << std::setprecision(2) << totalRevenue << "\n";
    std::cout << "=======================================================\n";
}

// ==========================================
// 8. File Handling Implementation
// ==========================================
void RentalSystem::loadData() {
    // 1. Load Vehicles
    std::ifstream vFile(VEHICLE_FILE);
    if (vFile.is_open()) {
        clearVehicles();
        std::string line;
        while (std::getline(vFile, line)) {
            line = trim(line);
            if (line.empty()) continue;
            Vehicle* v = Vehicle::deserialize(line);
            if (v != nullptr) {
                if (vehicleCount < MAX_VEHICLES) {
                    vehicles[vehicleCount++] = v;
                } else {
                    delete v;
                }
            }
        }
        vFile.close();
    }

    // 2. Load Customers
    std::ifstream cFile(CUSTOMER_FILE);
    if (cFile.is_open()) {
        customers.clear();
        std::string line;
        while (std::getline(cFile, line)) {
            line = trim(line);
            if (line.empty()) continue;
            Customer c = Customer::deserialize(line);
            if (!c.getId().empty()) {
                customers.push_back(c);
                try {
                    int num = std::stoi(c.getId().substr(1));
                    if (num >= nextCustomerNum) {
                        nextCustomerNum = num + 1;
                    }
                } catch (...) {}
            }
        }
        cFile.close();
    }

    // 3. Load Active Bookings
    std::ifstream bFile(BOOKING_FILE);
    if (bFile.is_open()) {
        activeBookings.clear();
        std::string line;
        while (std::getline(bFile, line)) {
            line = trim(line);
            if (line.empty()) continue;
            Booking b = Booking::deserialize(line);
            if (!b.getBookingId().empty()) {
                activeBookings.push_back(b);
                try {
                    int num = std::stoi(b.getBookingId().substr(2));
                    if (num >= nextBookingNum) {
                        nextBookingNum = num + 1;
                    }
                } catch (...) {}
            }
        }
        bFile.close();
    }

    // 4. Load Rental History (Linked list)
    rentalHistory.loadFromFile(HISTORY_FILE);

    // 5. Load Waiting Queue
    waitingQueue.loadFromFile(QUEUE_FILE);
}

void RentalSystem::saveData() const {
    // 1. Save Vehicles
    std::ofstream vFile(VEHICLE_FILE);
    if (vFile.is_open()) {
        for (int i = 0; i < vehicleCount; ++i) {
            if (vehicles[i] != nullptr) {
                vFile << vehicles[i]->serialize() << "\n";
            }
        }
        vFile.close();
    }

    // 2. Save Customers
    std::ofstream cFile(CUSTOMER_FILE);
    if (cFile.is_open()) {
        for (const auto& c : customers) {
            cFile << c.serialize() << "\n";
        }
        cFile.close();
    }

    // 3. Save Active Bookings
    std::ofstream bFile(BOOKING_FILE);
    if (bFile.is_open()) {
        for (const auto& b : activeBookings) {
            bFile << b.serialize() << "\n";
        }
        bFile.close();
    }

    // 4. Save Rental History (Linked list)
    rentalHistory.saveToFile(HISTORY_FILE);

    // 5. Save Waiting Queue
    waitingQueue.saveToFile(QUEUE_FILE);

    std::cout << "[SUCCESS] All system data (Vehicles, Customers, Bookings, History, Queue) saved to data/ files!\n";
}

void RentalSystem::seedInitialData() {
    // Add sample Vehicles
    addVehicle(new Car("C101", "Honda City", "Honda", 2200.0, 5, "Petrol", true));
    addVehicle(new Car("C102", "Hyundai Creta", "Hyundai", 2800.0, 5, "Diesel", true));
    addVehicle(new Car("C103", "Tata Nexon EV", "Tata", 2500.0, 5, "Electric", true));

    addVehicle(new Bike("B201", "Royal Enfield 350", "Royal Enfield", 1200.0, 350, "Cruiser", true));
    addVehicle(new Bike("B202", "Yamaha R15 V4", "Yamaha", 1000.0, 155, "Sport", true));
    addVehicle(new Bike("B203", "Hero Splendor+", "Hero", 500.0, 100, "Commuter", true));

    addVehicle(new Scooter("S301", "Honda Activa 6G", "Honda", 600.0, 18.0, false, true));
    addVehicle(new Scooter("S302", "Ather 450X", "Ather", 800.0, 22.0, true, true));
    addVehicle(new Scooter("S303", "TVS Jupiter", "TVS", 550.0, 21.0, false, true));

    // Add sample Customers
    customers.push_back(Customer("C101", "Aarav Sharma", "9876543210"));
    customers.push_back(Customer("C102", "Priya Verma", "9123456780"));
    customers.push_back(Customer("C103", "Rohan Mehta", "9988776655"));
    nextCustomerNum = 104;

    // Add sample Rental History records
    rentalHistory.insertRecord("BK1000", "C101", "Aarav Sharma", "C101", "Honda City", 3, 6600.0, "Completed");
    rentalHistory.insertRecord("BK1001", "C102", "Priya Verma", "B202", "Yamaha R15 V4", 2, 2050.0, "Completed");

    // Save initial seed to files
    saveData();
}
