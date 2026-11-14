#include <iostream>
#include <string>
#include <limits>
#include "RentalSystem.h"

// Helper function to pause screen until user hits Enter
static void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Submenu for Vehicle Management
void handleVehicleMenu(RentalSystem& system) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "        VEHICLE MANAGEMENT MENU         \n";
        std::cout << "========================================\n";
        std::cout << "1. Add New Vehicle\n";
        std::cout << "2. Display All Vehicles\n";
        std::cout << "3. Display Available Vehicles\n";
        std::cout << "4. Display Rented Vehicles\n";
        std::cout << "5. Filter Vehicles by Type\n";
        std::cout << "6. Search Vehicle by ID (Linear Search)\n";
        std::cout << "7. Update Vehicle Details\n";
        std::cout << "8. Delete Vehicle\n";
        std::cout << "0. Return to Main Menu\n";
        std::cout << "========================================\n";
        std::cout << "Enter your choice (0-8): ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid choice input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                system.addNewVehicleInteractive();
                pauseScreen();
                break;
            case 2:
                system.displayAllVehicles();
                pauseScreen();
                break;
            case 3:
                system.displayVehiclesByStatus(true);
                pauseScreen();
                break;
            case 4:
                system.displayVehiclesByStatus(false);
                pauseScreen();
                break;
            case 5: {
                std::cout << "\nFilter by: 1. Car | 2. Bike | 3. Scooter\nEnter choice: ";
                int tChoice;
                if (std::cin >> tChoice) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (tChoice == 1) system.filterVehiclesByType("Car");
                    else if (tChoice == 2) system.filterVehiclesByType("Bike");
                    else if (tChoice == 3) system.filterVehiclesByType("Scooter");
                    else std::cout << "[ERROR] Invalid category.\n";
                } else {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                pauseScreen();
                break;
            }
            case 6:
                system.searchVehicleInteractive();
                pauseScreen();
                break;
            case 7:
                system.updateVehicleInteractive();
                pauseScreen();
                break;
            case 8:
                system.deleteVehicleInteractive();
                pauseScreen();
                break;
            case 0:
                std::cout << "Returning to Main Menu...\n";
                break;
            default:
                std::cout << "[ERROR] Invalid option! Please choose between 0 and 8.\n";
                pauseScreen();
                break;
        }
    }
}

// Submenu for Customer Management
void handleCustomerMenu(RentalSystem& system) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "        CUSTOMER MANAGEMENT MENU        \n";
        std::cout << "========================================\n";
        std::cout << "1. Register New Customer\n";
        std::cout << "2. Display All Customers\n";
        std::cout << "3. Search Customer by ID\n";
        std::cout << "0. Return to Main Menu\n";
        std::cout << "========================================\n";
        std::cout << "Enter your choice (0-3): ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid choice input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                system.registerCustomerInteractive();
                pauseScreen();
                break;
            case 2:
                system.displayAllCustomers();
                pauseScreen();
                break;
            case 3:
                system.searchCustomerInteractive();
                pauseScreen();
                break;
            case 0:
                std::cout << "Returning to Main Menu...\n";
                break;
            default:
                std::cout << "[ERROR] Invalid option! Please choose between 0 and 3.\n";
                pauseScreen();
                break;
        }
    }
}

// Submenu for Waiting Queue Management
void handleQueueMenu(RentalSystem& system) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "          WAITING QUEUE MENU            \n";
        std::cout << "========================================\n";
        std::cout << "1. Display Waiting Queue (FIFO)\n";
        std::cout << "2. Check & Serve Waiting Customer for Vehicle\n";
        std::cout << "0. Return to Main Menu\n";
        std::cout << "========================================\n";
        std::cout << "Enter your choice (0-2): ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid choice input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                system.displayWaitingQueue();
                pauseScreen();
                break;
            case 2: {
                std::cout << "Enter Vehicle ID to check waiting list: ";
                std::string vId;
                std::getline(std::cin, vId);
                system.checkAndServeWaitingCustomer(vId);
                pauseScreen();
                break;
            }
            case 0:
                std::cout << "Returning to Main Menu...\n";
                break;
            default:
                std::cout << "[ERROR] Invalid option! Please choose between 0 and 2.\n";
                pauseScreen();
                break;
        }
    }
}

// Submenu for Rental History
void handleHistoryMenu(RentalSystem& system) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "          RENTAL HISTORY MENU           \n";
        std::cout << "========================================\n";
        std::cout << "1. Display Complete Rental History (Linked List)\n";
        std::cout << "2. Search Rental Records\n";
        std::cout << "0. Return to Main Menu\n";
        std::cout << "========================================\n";
        std::cout << "Enter your choice (0-2): ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid choice input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                system.displayRentalHistory();
                pauseScreen();
                break;
            case 2:
                system.searchRentalHistoryInteractive();
                pauseScreen();
                break;
            case 0:
                std::cout << "Returning to Main Menu...\n";
                break;
            default:
                std::cout << "[ERROR] Invalid option! Please choose between 0 and 2.\n";
                pauseScreen();
                break;
        }
    }
}

// ==========================================
// Main Application Entry Point
// ==========================================
int main() {
    std::cout << "Initializing Smart Vehicle Rental Management System...\n";
    RentalSystem rentalSystem;

    int mainChoice = -1;
    while (mainChoice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "       SMART VEHICLE RENTAL SYSTEM      \n";
        std::cout << "========================================\n";
        std::cout << "1. Vehicle Management\n";
        std::cout << "2. Customer Management\n";
        std::cout << "3. Display Available Vehicles\n";
        std::cout << "4. Search Vehicle\n";
        std::cout << "5. Book a Vehicle\n";
        std::cout << "6. Return a Vehicle\n";
        std::cout << "7. Waiting Queue\n";
        std::cout << "8. Rental History\n";
        std::cout << "9. Dashboard / Statistics\n";
        std::cout << "10. Save Data\n";
        std::cout << "0. Exit\n";
        std::cout << "========================================\n";
        std::cout << "Enter your choice (0-10): ";

        if (!(std::cin >> mainChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid input. Please enter a valid number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (mainChoice) {
            case 1:
                handleVehicleMenu(rentalSystem);
                break;
            case 2:
                handleCustomerMenu(rentalSystem);
                break;
            case 3:
                rentalSystem.displayVehiclesByStatus(true);
                pauseScreen();
                break;
            case 4:
                rentalSystem.searchVehicleInteractive();
                pauseScreen();
                break;
            case 5:
                rentalSystem.bookVehicleInteractive();
                pauseScreen();
                break;
            case 6:
                rentalSystem.returnVehicleInteractive();
                pauseScreen();
                break;
            case 7:
                handleQueueMenu(rentalSystem);
                break;
            case 8:
                handleHistoryMenu(rentalSystem);
                break;
            case 9:
                rentalSystem.displayDashboard();
                pauseScreen();
                break;
            case 10:
                rentalSystem.saveData();
                pauseScreen();
                break;
            case 0:
                std::cout << "\nSaving all system data before exit...\n";
                rentalSystem.saveData();
                std::cout << "Thank you for using Smart Vehicle Rental Management System! Goodbye.\n";
                break;
            default:
                std::cout << "[ERROR] Invalid menu choice. Please select from 0 to 10.\n";
                pauseScreen();
                break;
        }
    }

    return 0;
}
