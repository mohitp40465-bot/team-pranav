#include "Customer.h"
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

Customer::Customer() : customerId(""), customerName(""), phoneNumber("") {}

Customer::Customer(const std::string& id, const std::string& name, const std::string& phone)
    : customerId(id), customerName(name), phoneNumber(phone) {}

Customer::~Customer() {}

std::string Customer::getId() const { return customerId; }
std::string Customer::getName() const { return customerName; }
std::string Customer::getPhone() const { return phoneNumber; }

void Customer::setName(const std::string& name) { customerName = name; }
void Customer::setPhone(const std::string& phone) { phoneNumber = phone; }

void Customer::displayCustomer() const {
    std::cout << std::left 
              << std::setw(12) << customerId
              << std::setw(25) << customerName
              << std::setw(16) << phoneNumber
              << "\n";
}

std::string Customer::serialize() const {
    // Format: CustomerID|CustomerName|PhoneNumber
    return customerId + "|" + customerName + "|" + phoneNumber;
}

Customer Customer::deserialize(const std::string& line) {
    if (line.empty()) return Customer();
    std::vector<std::string> tokens = split(line, '|');
    if (tokens.size() < 3) return Customer();
    return Customer(tokens[0], tokens[1], tokens[2]);
}
