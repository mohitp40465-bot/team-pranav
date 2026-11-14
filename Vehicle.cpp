#include "Vehicle.h"
#include <vector>

// Helper to split string by delimiter
static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// ==========================================
// Vehicle Base Class Implementation
// ==========================================
Vehicle::Vehicle() 
    : vehicleId(""), vehicleName(""), vehicleType("Vehicle"), brand(""), rentPerDay(0.0), isAvailable(true) {}

Vehicle::Vehicle(const std::string& id, const std::string& name, const std::string& type,
                 const std::string& brandName, double rent, bool available)
    : vehicleId(id), vehicleName(name), vehicleType(type), brand(brandName), rentPerDay(rent), isAvailable(available) {}

Vehicle::~Vehicle() {
    // Base virtual destructor
}

std::string Vehicle::getId() const { return vehicleId; }
std::string Vehicle::getName() const { return vehicleName; }
std::string Vehicle::getType() const { return vehicleType; }
std::string Vehicle::getBrand() const { return brand; }
double Vehicle::getRentPerDay() const { return rentPerDay; }
bool Vehicle::getAvailability() const { return isAvailable; }

void Vehicle::setName(const std::string& name) { vehicleName = name; }
void Vehicle::setBrand(const std::string& brandName) { brand = brandName; }
void Vehicle::setRentPerDay(double rent) { rentPerDay = rent; }
void Vehicle::setAvailability(bool available) { isAvailable = available; }

double Vehicle::calculateRent(int days) const {
    if (days <= 0) return 0.0;
    return rentPerDay * days;
}

Vehicle* Vehicle::deserialize(const std::string& line) {
    if (line.empty()) return nullptr;
    std::vector<std::string> tokens = split(line, '|');
    // Format: Type|Id|Name|Brand|Rent|Available|Extra1|Extra2
    if (tokens.size() < 8) return nullptr;

    std::string type = tokens[0];
    std::string id = tokens[1];
    std::string name = tokens[2];
    std::string brand = tokens[3];
    double rent = std::stod(tokens[4]);
    bool available = (tokens[5] == "1" || tokens[5] == "true");

    if (type == "Car") {
        int seats = std::stoi(tokens[6]);
        std::string fuel = tokens[7];
        return new Car(id, name, brand, rent, seats, fuel, available);
    } else if (type == "Bike") {
        int cc = std::stoi(tokens[6]);
        std::string bikeType = tokens[7];
        return new Bike(id, name, brand, rent, cc, bikeType, available);
    } else if (type == "Scooter") {
        double storage = std::stod(tokens[6]);
        bool electric = (tokens[7] == "1" || tokens[7] == "true");
        return new Scooter(id, name, brand, rent, storage, electric, available);
    }
    return nullptr;
}

// ==========================================
// Car Class Implementation
// ==========================================
Car::Car() : Vehicle("", "", "Car", "", 0.0, true), seatingCapacity(5), fuelType("Petrol") {}

Car::Car(const std::string& id, const std::string& name, const std::string& brandName,
         double rent, int seats, const std::string& fuel, bool available)
    : Vehicle(id, name, "Car", brandName, rent, available), seatingCapacity(seats), fuelType(fuel) {}

Car::~Car() {}

int Car::getSeatingCapacity() const { return seatingCapacity; }
std::string Car::getFuelType() const { return fuelType; }
void Car::setSeatingCapacity(int seats) { seatingCapacity = seats; }
void Car::setFuelType(const std::string& fuel) { fuelType = fuel; }

void Car::displayDetails() const {
    std::cout << std::left 
              << std::setw(8)  << vehicleId
              << std::setw(18) << vehicleName
              << std::setw(10) << vehicleType
              << std::setw(14) << brand
              << std::setw(12) << rentPerDay
              << std::setw(14) << (isAvailable ? "Available" : "Rented")
              << "Seats: " << seatingCapacity << ", Fuel: " << fuelType
              << "\n";
}

double Car::calculateRent(int days) const {
    double base = Vehicle::calculateRent(days);
    // 7 or more days gets a 5% weekly discount
    if (days >= 7) {
        base *= 0.95;
    }
    return base;
}

std::string Car::serialize() const {
    // Format: Car|ID|Name|Brand|Rent|Available|Seats|Fuel
    std::ostringstream oss;
    oss << "Car|" << vehicleId << "|" << vehicleName << "|" << brand << "|"
        << rentPerDay << "|" << (isAvailable ? "1" : "0") << "|"
        << seatingCapacity << "|" << fuelType;
    return oss.str();
}

// ==========================================
// Bike Class Implementation
// ==========================================
Bike::Bike() : Vehicle("", "", "Bike", "", 0.0, true), engineCapacity(150), bikeType("Commuter") {}

Bike::Bike(const std::string& id, const std::string& name, const std::string& brandName,
           double rent, int cc, const std::string& category, bool available)
    : Vehicle(id, name, "Bike", brandName, rent, available), engineCapacity(cc), bikeType(category) {}

Bike::~Bike() {}

int Bike::getEngineCapacity() const { return engineCapacity; }
std::string Bike::getBikeType() const { return bikeType; }
void Bike::setEngineCapacity(int cc) { engineCapacity = cc; }
void Bike::setBikeType(const std::string& category) { bikeType = category; }

void Bike::displayDetails() const {
    std::cout << std::left 
              << std::setw(8)  << vehicleId
              << std::setw(18) << vehicleName
              << std::setw(10) << vehicleType
              << std::setw(14) << brand
              << std::setw(12) << rentPerDay
              << std::setw(14) << (isAvailable ? "Available" : "Rented")
              << "Engine: " << engineCapacity << "cc, Type: " << bikeType
              << "\n";
}

double Bike::calculateRent(int days) const {
    double base = Vehicle::calculateRent(days);
    // Sports bikes include a small helmet & safety gear charge per booking
    if (bikeType == "Sport" || bikeType == "Cruiser") {
        base += 50.0; // flat gear charge
    }
    return base;
}

std::string Bike::serialize() const {
    // Format: Bike|ID|Name|Brand|Rent|Available|CC|Type
    std::ostringstream oss;
    oss << "Bike|" << vehicleId << "|" << vehicleName << "|" << brand << "|"
        << rentPerDay << "|" << (isAvailable ? "1" : "0") << "|"
        << engineCapacity << "|" << bikeType;
    return oss.str();
}

// ==========================================
// Scooter Class Implementation
// ==========================================
Scooter::Scooter() : Vehicle("", "", "Scooter", "", 0.0, true), storageCapacity(18.0), isElectric(false) {}

Scooter::Scooter(const std::string& id, const std::string& name, const std::string& brandName,
                 double rent, double storage, bool electric, bool available)
    : Vehicle(id, name, "Scooter", brandName, rent, available), storageCapacity(storage), isElectric(electric) {}

Scooter::~Scooter() {}

double Scooter::getStorageCapacity() const { return storageCapacity; }
bool Scooter::getIsElectric() const { return isElectric; }
void Scooter::setStorageCapacity(double storage) { storageCapacity = storage; }
void Scooter::setIsElectric(bool electric) { isElectric = electric; }

void Scooter::displayDetails() const {
    std::cout << std::left 
              << std::setw(8)  << vehicleId
              << std::setw(18) << vehicleName
              << std::setw(10) << vehicleType
              << std::setw(14) << brand
              << std::setw(12) << rentPerDay
              << std::setw(14) << (isAvailable ? "Available" : "Rented")
              << "Boot: " << storageCapacity << "L, " << (isElectric ? "Electric (EV)" : "Petrol")
              << "\n";
}

double Scooter::calculateRent(int days) const {
    double base = Vehicle::calculateRent(days);
    // Electric scooter has an eco-friendly green subsidy discount of 5%
    if (isElectric) {
        base *= 0.95;
    }
    return base;
}

std::string Scooter::serialize() const {
    // Format: Scooter|ID|Name|Brand|Rent|Available|BootStorage|IsElectric
    std::ostringstream oss;
    oss << "Scooter|" << vehicleId << "|" << vehicleName << "|" << brand << "|"
        << rentPerDay << "|" << (isAvailable ? "1" : "0") << "|"
        << storageCapacity << "|" << (isElectric ? "1" : "0");
    return oss.str();
}
