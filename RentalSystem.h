#ifndef RENTALSYSTEM_H
#define RENTALSYSTEM_H

#include "Vehicle.h"
#include "Customer.h"
#include "Booking.h"
#include "WaitingQueue.h"
#include "RentalHistory.h"

#include <vector>
#include <string>

// Maximum capacity for the static/pointer array of vehicles to demonstrate DSA Array concept
const int MAX_VEHICLES = 100;

/**
 * ============================================================================
 * CLASS: RentalSystem
 * Demonstrates: Controller / Architecture pattern, Composition & Association,
 *               System integration, DSA algorithms (Linear Search, Array Operations)
 * ============================================================================
 */
class RentalSystem {
private:
    // DSA Component A: Array for Vehicle Storage
    Vehicle* vehicles[MAX_VEHICLES];
    int vehicleCount;

    // Customer Storage
    std::vector<Customer> customers;

    // Active Bookings Storage
    std::vector<Booking> activeBookings;

    // DSA Component B: Queue for Waiting List (Composition)
    WaitingQueue waitingQueue;

    // DSA Component C: Singly Linked List for Rental History (Composition)
    RentalHistory rentalHistory;

    // Auto-increment IDs
    int nextBookingNum;
    int nextCustomerNum;

    // File paths
    const std::string VEHICLE_FILE = "data/vehicles.txt";
    const std::string CUSTOMER_FILE = "data/customers.txt";
    const std::string BOOKING_FILE = "data/bookings.txt";
    const std::string HISTORY_FILE = "data/history.txt";
    const std::string QUEUE_FILE = "data/waiting_queue.txt";

    // Helper Functions
    int findVehicleIndexLinearSearch(const std::string& id) const;
    int findCustomerIndex(const std::string& id) const;
    int findActiveBookingIndex(const std::string& bookingId) const;
    void ensureDataDirectoryExists();
    void seedInitialData();

public:
    RentalSystem();
    ~RentalSystem();

    // ==========================================
    // 1. Vehicle Management
    // ==========================================
    bool addVehicle(Vehicle* v);
    void addNewVehicleInteractive();
    void displayAllVehicles() const;
    void displayVehiclesByStatus(bool availableOnly) const;
    void filterVehiclesByType(const std::string& type) const;
    void searchVehicleInteractive() const;
    void updateVehicleInteractive();
    void deleteVehicleInteractive();

    // ==========================================
    // 2. Customer Management
    // ==========================================
    bool registerCustomer(const std::string& name, const std::string& phone);
    void registerCustomerInteractive();
    void displayAllCustomers() const;
    void searchCustomerInteractive() const;

    // ==========================================
    // 3. Booking Management
    // ==========================================
    void bookVehicleInteractive();
    bool processBooking(const std::string& cId, const std::string& vId, int days);

    // ==========================================
    // 4. Return Management
    // ==========================================
    void returnVehicleInteractive();

    // ==========================================
    // 5. Waiting Queue Management
    // ==========================================
    void displayWaitingQueue() const;
    void checkAndServeWaitingCustomer(const std::string& vehicleId);

    // ==========================================
    // 6. Rental History Management
    // ==========================================
    void displayRentalHistory() const;
    void searchRentalHistoryInteractive() const;

    // ==========================================
    // 7. Dashboard / Statistics
    // ==========================================
    void displayDashboard() const;

    // ==========================================
    // 8. File Handling (Persistence)
    // ==========================================
    void loadData();
    void saveData() const;

    // Clean memory
    void clearVehicles();
};

#endif // RENTALSYSTEM_H
