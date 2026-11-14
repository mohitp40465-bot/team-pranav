#ifndef RENTALHISTORY_H
#define RENTALHISTORY_H

#include <iostream>
#include <string>
#include <iomanip>

/**
 * Singly Linked List Node representing a single Rental History record.
 */
struct HistoryNode {
    std::string bookingId;
    std::string customerId;
    std::string customerName;
    std::string vehicleId;
    std::string vehicleName;
    int rentalDays;
    double totalRent;
    std::string bookingStatus; // "Active", "Completed", etc.

    HistoryNode* next;

    HistoryNode(const std::string& bId, const std::string& cId, const std::string& cName,
                const std::string& vId, const std::string& vName, int days,
                double rent, const std::string& status)
        : bookingId(bId), customerId(cId), customerName(cName), vehicleId(vId),
          vehicleName(vName), rentalDays(days), totalRent(rent), bookingStatus(status),
          next(nullptr) {}
};

/**
 * ============================================================================
 * CLASS: RentalHistory (Manual Singly Linked List Implementation)
 * Demonstrates: DSA - Singly Linked List, dynamic node allocation,
 *               sequential traversal, search algorithms, proper memory deallocation.
 * ============================================================================
 */
class RentalHistory {
private:
    HistoryNode* head;
    HistoryNode* tail;
    int recordCount;

public:
    RentalHistory();
    ~RentalHistory();

    // Insertion
    void insertRecord(const std::string& bId, const std::string& cId, const std::string& cName,
                      const std::string& vId, const std::string& vName, int days,
                      double rent, const std::string& status);

    // Display
    void displayHistory() const;

    // Search Functions (DSA: Linked List Traversal & Search)
    HistoryNode* searchByBookingId(const std::string& bId) const;
    void searchByCustomerId(const std::string& cId) const;
    void searchByVehicleId(const std::string& vId) const;

    // Update status and final rent upon return
    bool updateStatus(const std::string& bId, const std::string& status, double finalRent);

    // Analytics / Statistics
    double calculateTotalRevenue() const;
    int countCompletedBookings() const;
    int getRecordCount() const;

    // File Handling
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    // Memory Cleanup
    void clear();
};

#endif // RENTALHISTORY_H
