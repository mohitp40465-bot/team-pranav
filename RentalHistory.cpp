#include "RentalHistory.h"
#include <fstream>
#include <sstream>
#include <vector>

static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

RentalHistory::RentalHistory() : head(nullptr), tail(nullptr), recordCount(0) {}

RentalHistory::~RentalHistory() {
    clear();
}

void RentalHistory::clear() {
    HistoryNode* curr = head;
    while (curr != nullptr) {
        HistoryNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    recordCount = 0;
}

void RentalHistory::insertRecord(const std::string& bId, const std::string& cId, const std::string& cName,
                                 const std::string& vId, const std::string& vName, int days,
                                 double rent, const std::string& status) {
    HistoryNode* newNode = new HistoryNode(bId, cId, cName, vId, vName, days, rent, status);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    recordCount++;
}

void RentalHistory::displayHistory() const {
    if (head == nullptr) {
        std::cout << "\n========================================================================================\n";
        std::cout << "                             NO RENTAL HISTORY FOUND                                    \n";
        std::cout << "========================================================================================\n";
        return;
    }

    std::cout << "\n==================================================================================================\n";
    std::cout << "                             COMPLETE RENTAL HISTORY (LINKED LIST)                                \n";
    std::cout << "==================================================================================================\n";
    std::cout << std::left 
              << std::setw(10) << "Book ID"
              << std::setw(10) << "Cust ID"
              << std::setw(18) << "Customer Name"
              << std::setw(10) << "Veh ID"
              << std::setw(22) << "Vehicle Name"
              << std::setw(8)  << "Days"
              << std::setw(12) << "Total Rent"
              << std::setw(12) << "Status"
              << "\n";
    std::cout << "--------------------------------------------------------------------------------------------------\n";

    HistoryNode* curr = head;
    while (curr != nullptr) {
        std::cout << std::left 
                  << std::setw(10) << curr->bookingId
                  << std::setw(10) << curr->customerId
                  << std::setw(18) << curr->customerName
                  << std::setw(10) << curr->vehicleId
                  << std::setw(22) << curr->vehicleName
                  << std::setw(8)  << curr->rentalDays
                  << std::setw(12) << std::fixed << std::setprecision(2) << curr->totalRent
                  << std::setw(12) << curr->bookingStatus
                  << "\n";
        curr = curr->next;
    }
    std::cout << "--------------------------------------------------------------------------------------------------\n";
    std::cout << "Total records recorded: " << recordCount << "\n";
}

HistoryNode* RentalHistory::searchByBookingId(const std::string& bId) const {
    HistoryNode* curr = head;
    while (curr != nullptr) {
        if (curr->bookingId == bId) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

void RentalHistory::searchByCustomerId(const std::string& cId) const {
    bool found = false;
    HistoryNode* curr = head;
    std::cout << "\n--- Rental Records for Customer ID: " << cId << " ---\n";
    while (curr != nullptr) {
        if (curr->customerId == cId) {
            if (!found) {
                std::cout << std::left 
                          << std::setw(10) << "Book ID"
                          << std::setw(10) << "Veh ID"
                          << std::setw(18) << "Vehicle Name"
                          << std::setw(8)  << "Days"
                          << std::setw(12) << "Total Rent"
                          << std::setw(12) << "Status"
                          << "\n";
                std::cout << "----------------------------------------------------------------------\n";
            }
            std::cout << std::left 
                      << std::setw(10) << curr->bookingId
                      << std::setw(10) << curr->vehicleId
                      << std::setw(18) << curr->vehicleName
                      << std::setw(8)  << curr->rentalDays
                      << std::setw(12) << std::fixed << std::setprecision(2) << curr->totalRent
                      << std::setw(12) << curr->bookingStatus
                      << "\n";
            found = true;
        }
        curr = curr->next;
    }
    if (!found) {
        std::cout << "[INFO] No rental records found for Customer ID: " << cId << "\n";
    }
}

void RentalHistory::searchByVehicleId(const std::string& vId) const {
    bool found = false;
    HistoryNode* curr = head;
    std::cout << "\n--- Rental Records for Vehicle ID: " << vId << " ---\n";
    while (curr != nullptr) {
        if (curr->vehicleId == vId) {
            if (!found) {
                std::cout << std::left 
                          << std::setw(10) << "Book ID"
                          << std::setw(10) << "Cust ID"
                          << std::setw(18) << "Customer Name"
                          << std::setw(8)  << "Days"
                          << std::setw(12) << "Total Rent"
                          << std::setw(12) << "Status"
                          << "\n";
                std::cout << "----------------------------------------------------------------------\n";
            }
            std::cout << std::left 
                      << std::setw(10) << curr->bookingId
                      << std::setw(10) << curr->customerId
                      << std::setw(18) << curr->customerName
                      << std::setw(8)  << curr->rentalDays
                      << std::setw(12) << std::fixed << std::setprecision(2) << curr->totalRent
                      << std::setw(12) << curr->bookingStatus
                      << "\n";
            found = true;
        }
        curr = curr->next;
    }
    if (!found) {
        std::cout << "[INFO] No rental records found for Vehicle ID: " << vId << "\n";
    }
}

bool RentalHistory::updateStatus(const std::string& bId, const std::string& status, double finalRent) {
    HistoryNode* node = searchByBookingId(bId);
    if (node != nullptr) {
        node->bookingStatus = status;
        if (finalRent >= 0) {
            node->totalRent = finalRent;
        }
        return true;
    }
    return false;
}

double RentalHistory::calculateTotalRevenue() const {
    double revenue = 0.0;
    HistoryNode* curr = head;
    while (curr != nullptr) {
        if (curr->bookingStatus == "Completed") {
            revenue += curr->totalRent;
        }
        curr = curr->next;
    }
    return revenue;
}

int RentalHistory::countCompletedBookings() const {
    int count = 0;
    HistoryNode* curr = head;
    while (curr != nullptr) {
        if (curr->bookingStatus == "Completed") {
            count++;
        }
        curr = curr->next;
    }
    return count;
}

int RentalHistory::getRecordCount() const {
    return recordCount;
}

void RentalHistory::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return;

    HistoryNode* curr = head;
    while (curr != nullptr) {
        outFile << curr->bookingId << "|"
                << curr->customerId << "|"
                << curr->customerName << "|"
                << curr->vehicleId << "|"
                << curr->vehicleName << "|"
                << curr->rentalDays << "|"
                << curr->totalRent << "|"
                << curr->bookingStatus << "\n";
        curr = curr->next;
    }
    outFile.close();
}

void RentalHistory::loadFromFile(const std::string& filename) {
    clear();
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return;

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        std::vector<std::string> tokens = split(line, '|');
        if (tokens.size() >= 8) {
            insertRecord(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4],
                         std::stoi(tokens[5]), std::stod(tokens[6]), tokens[7]);
        }
    }
    inFile.close();
}
