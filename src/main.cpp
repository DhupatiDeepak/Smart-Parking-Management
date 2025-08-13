#include "parking_lot.h"
#include <iostream>
#include <fstream>
#include <ctime>

// ANSI Colors
#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define BOLD    "\033[1m"

// Global session log
std::ofstream logFile;

// Startup banner without delays
/**
 * @brief Displays the startup banner for the Deepak Parking System application.
 *
 * Prints a stylized welcome message to the standard output using colored and bold text formatting.
 * Utilizes the CYAN, BOLD, and RESET formatting constants to enhance the visual appearance.
 * Intended to be called at the start of the application to greet the user.
 */
void startupBanner();
void startupBanner() {
    std::cout << CYAN << BOLD
              << "\n"
              << "******************************************************\n"
              << "*                                                    *\n"
              << "*          Welcome to Deepak Parking System            *\n"
              << "*                                                    *\n"
              << "******************************************************\n\n"
              << RESET;
}

// Open log file
/**
 * @brief Opens the session log file and writes a session start header.
 *
 * This function opens the "session_log.txt" file in append mode and writes a header
 * indicating the start of a new session, including the current date and time.
 * The header is formatted for easy identification of session boundaries in the log file.
 */
void openLogFiles() {
    logFile.open("session_log.txt", std::ios::app);
    const std::time_t now = std::time(nullptr);
    logFile << "\n===== New Session Started: " << std::ctime(&now) << "=====\n";
}

// Close log file
/**
 * @brief Closes the log file after writing a session end marker.
 *
 * Appends "===== Session Ended =====" to the log file to indicate the end of the current session,
 * then closes the file stream to ensure all data is flushed and resources are released.
 */
void closeLogFiles() {
    logFile << "===== Session Ended =====\n";
    logFile.close();
}
/**
 * @brief The entry point for the Deva Parking System application.
 *
 * Initializes the logging system, displays the startup banner, and manages the main menu loop
 * for parking, removing, and displaying cars in the parking lot. Handles user input validation
 * and logs all major actions and menu selections.
 *
 * @return int Returns 0 upon successful program termination.
 */
int main() {
    openLogFiles();
    ParkingLot lot;
    int choice;

    // Show startup banner instantly
    startupBanner();
    logFile << "🚗 Welcome to Deva Parking System — Your car is safe with us!\n";

    while (true) {
        std::cout << GREEN << "\n========= MAIN MENU =========\n" << RESET
                  << YELLOW << "1." << RESET << " Park Car\n"
                  << YELLOW << "2." << RESET << " Remove Car\n"
                  << YELLOW << "3." << RESET << " Display Parked Cars\n"
                  << YELLOW << "4." << RESET << " Exit\n"
                  << GREEN << "=============================\n" << RESET
                  << BOLD << "Enter choice: " << RESET;

        logFile << "\n=== Deva Parking Menu ===\n"
                << "1. Park Car\n2. Remove Car\n3. Display Parked Cars\n4. Exit\nEnter choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << RED << "Invalid input! Please enter a number.\n" << RESET;
            logFile << "Invalid input! Please enter a number.\n";
            continue;
        }
        std::cin.ignore();

        switch (choice) {
            case 1:
                lot.parkCar();
                break;
            case 2:
                lot.removeCar();
                break;
            case 3:
                lot.displayCars();
                break;
            case 4:
                closeLogFiles();
                return 0;
            default:
                std::cout << RED << "Invalid choice! Try again.\n" << RESET;
                logFile << "Invalid choice! Try again.\n";
        }
    }
}
