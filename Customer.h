#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
#include <string>
#include <iomanip>

/**
 * ============================================================================
 * CLASS: Customer
 * Demonstrates: Encapsulation (private members, public getters/setters),
 *               Constructors, Validation
 * ============================================================================
 */
class Customer {
private:
    std::string customerId;
    std::string customerName;
    std::string phoneNumber;

public:
    // Constructors
    Customer();
    Customer(const std::string& id, const std::string& name, const std::string& phone);
    ~Customer();

    // Getters
    std::string getId() const;
    std::string getName() const;
    std::string getPhone() const;

    // Setters
    void setName(const std::string& name);
    void setPhone(const std::string& phone);

    // Display
    void displayCustomer() const;

    // File I/O Serialization
    std::string serialize() const;
    static Customer deserialize(const std::string& line);
};

#endif // CUSTOMER_H
