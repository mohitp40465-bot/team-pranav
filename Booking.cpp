#include "Booking.h"
#include <vector>
#include <sstream>

static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

Booking::Booking()
    : bookingId(""), customerId(""), customerName(""), vehicleId(""), vehicleName(""),
      rentalDays(0), totalRent(0.0), bookingStatus("Active") {}

Booking::Booking(const std::string& bId, const std::string& cId, const std::string& cName,
                 const std::string& vId, const std::string& vName, int days,
                 double rent, const std::string& status)
    : bookingId(bId), customerId(cId), customerName(cName), vehicleId(vId), vehicleName(vName),
      rentalDays(days), totalRent(rent), bookingStatus(status) {}

Booking::~Booking() {}

std::string Booking::getBookingId() const { return bookingId; }
std::string Booking::getCustomerId() const { return customerId; }
std::string Booking::getCustomerName() const { return customerName; }
std::string Booking::getVehicleId() const { return vehicleId; }
std::string Booking::getVehicleName() const { return vehicleName; }
int Booking::getRentalDays() const { return rentalDays; }
double Booking::getTotalRent() const { return totalRent; }
std::string Booking::getBookingStatus() const { return bookingStatus; }

void Booking::setBookingStatus(const std::string& status) { bookingStatus = status; }
void Booking::setTotalRent(double rent) { totalRent = rent; }

void Booking::displayBooking() const {
    std::cout << std::left 
              << std::setw(10) << bookingId
              << std::setw(12) << customerId
              << std::setw(18) << customerName
              << std::setw(10) << vehicleId
              << std::setw(22) << vehicleName
              << std::setw(8)  << rentalDays
              << std::setw(12) << std::fixed << std::setprecision(2) << totalRent
              << std::setw(12) << bookingStatus
              << "\n";
}

std::string Booking::serialize() const {
    // Format: BookingID|CustomerID|CustomerName|VehicleID|VehicleName|RentalDays|TotalRent|Status
    std::ostringstream oss;
    oss << bookingId << "|" << customerId << "|" << customerName << "|"
        << vehicleId << "|" << vehicleName << "|" << rentalDays << "|"
        << totalRent << "|" << bookingStatus;
    return oss.str();
}

Booking Booking::deserialize(const std::string& line) {
    if (line.empty()) return Booking();
    std::vector<std::string> tokens = split(line, '|');
    if (tokens.size() < 8) return Booking();

    std::string bId = tokens[0];
    std::string cId = tokens[1];
    std::string cName = tokens[2];
    std::string vId = tokens[3];
    std::string vName = tokens[4];
    int days = std::stoi(tokens[5]);
    double rent = std::stod(tokens[6]);
    std::string status = tokens[7];

    return Booking(bId, cId, cName, vId, vName, days, rent, status);
}
