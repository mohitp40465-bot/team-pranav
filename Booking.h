#ifndef BOOKING_H
#define BOOKING_H

#include <iostream>
#include <string>
#include <iomanip>

/**
 * ============================================================================
 * CLASS: Booking
 * Demonstrates: Association between Customer and Vehicle,
 *               State Management (Active / Completed)
 * ============================================================================
 */
class Booking {
private:
    std::string bookingId;
    std::string customerId;
    std::string customerName;
    std::string vehicleId;
    std::string vehicleName;
    int rentalDays;
    double totalRent;
    std::string bookingStatus; // "Active" or "Completed"

public:
    // Constructors
    Booking();
    Booking(const std::string& bId, const std::string& cId, const std::string& cName,
            const std::string& vId, const std::string& vName, int days,
            double rent, const std::string& status = "Active");
    ~Booking();

    // Getters
    std::string getBookingId() const;
    std::string getCustomerId() const;
    std::string getCustomerName() const;
    std::string getVehicleId() const;
    std::string getVehicleName() const;
    int getRentalDays() const;
    double getTotalRent() const;
    std::string getBookingStatus() const;

    // Setters
    void setBookingStatus(const std::string& status);
    void setTotalRent(double rent);

    // Display
    void displayBooking() const;

    // File Serialization
    std::string serialize() const;
    static Booking deserialize(const std::string& line);
};

#endif // BOOKING_H
