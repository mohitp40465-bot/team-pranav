#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

/**
 * ============================================================================
 * CLASS: Vehicle (Abstract Base Class)
 * Demonstrates: Abstraction, Encapsulation, Polymorphism, Constructors/Destructors
 * ============================================================================
 */
class Vehicle {
protected:
    std::string vehicleId;
    std::string vehicleName;
    std::string vehicleType;    // "Car", "Bike", "Scooter"
    std::string brand;
    double rentPerDay;
    bool isAvailable;

public:
    // Constructors
    Vehicle();
    Vehicle(const std::string& id, const std::string& name, const std::string& type,
            const std::string& brandName, double rent, bool available = true);

    // Virtual Destructor (essential for polymorphic base classes)
    virtual ~Vehicle();

    // Getters (Encapsulation)
    std::string getId() const;
    std::string getName() const;
    std::string getType() const;
    std::string getBrand() const;
    double getRentPerDay() const;
    bool getAvailability() const;

    // Setters (Encapsulation)
    void setName(const std::string& name);
    void setBrand(const std::string& brandName);
    void setRentPerDay(double rent);
    void setAvailability(bool available);

    // Pure Virtual Function -> Makes Vehicle an Abstract Class (Abstraction)
    virtual void displayDetails() const = 0;

    // Virtual Function -> Polymorphic Rent Calculation
    virtual double calculateRent(int days) const;

    // File Serialization
    virtual std::string serialize() const = 0;

    // Static helper to reconstruct polymorphic objects from file strings
    static Vehicle* deserialize(const std::string& line);
};

/**
 * ============================================================================
 * CLASS: Car (Derived from Vehicle)
 * Demonstrates: Inheritance, Function Overriding (Runtime Polymorphism)
 * ============================================================================
 */
class Car : public Vehicle {
private:
    int seatingCapacity;
    std::string fuelType; // "Petrol", "Diesel", "Electric", "CNG"

public:
    // Constructors
    Car();
    Car(const std::string& id, const std::string& name, const std::string& brandName,
        double rent, int seats, const std::string& fuel, bool available = true);

    virtual ~Car();

    // Getters and Setters
    int getSeatingCapacity() const;
    std::string getFuelType() const;
    void setSeatingCapacity(int seats);
    void setFuelType(const std::string& fuel);

    // Overridden methods
    void displayDetails() const override;
    double calculateRent(int days) const override;
    std::string serialize() const override;
};

/**
 * ============================================================================
 * CLASS: Bike (Derived from Vehicle)
 * Demonstrates: Inheritance, Function Overriding (Runtime Polymorphism)
 * ============================================================================
 */
class Bike : public Vehicle {
private:
    int engineCapacity; // in cc, e.g. 150, 350
    std::string bikeType; // "Sport", "Cruiser", "Commuter", "Adventure"

public:
    // Constructors
    Bike();
    Bike(const std::string& id, const std::string& name, const std::string& brandName,
         double rent, int cc, const std::string& category, bool available = true);

    virtual ~Bike();

    // Getters and Setters
    int getEngineCapacity() const;
    std::string getBikeType() const;
    void setEngineCapacity(int cc);
    void setBikeType(const std::string& category);

    // Overridden methods
    void displayDetails() const override;
    double calculateRent(int days) const override;
    std::string serialize() const override;
};

/**
 * ============================================================================
 * CLASS: Scooter (Derived from Vehicle)
 * Demonstrates: Inheritance, Function Overriding (Runtime Polymorphism)
 * ============================================================================
 */
class Scooter : public Vehicle {
private:
    double storageCapacity; // in liters, e.g. 21.0
    bool isElectric;        // true = EV, false = Petrol

public:
    // Constructors
    Scooter();
    Scooter(const std::string& id, const std::string& name, const std::string& brandName,
            double rent, double storage, bool electric, bool available = true);

    virtual ~Scooter();

    // Getters and Setters
    double getStorageCapacity() const;
    bool getIsElectric() const;
    void setStorageCapacity(double storage);
    void setIsElectric(bool electric);

    // Overridden methods
    void displayDetails() const override;
    double calculateRent(int days) const override;
    std::string serialize() const override;
};

#endif // VEHICLE_H
