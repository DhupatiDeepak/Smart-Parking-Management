#include "parking_lot.h"
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <ctime>
#include <sstream>
#include <sys/stat.h>

// ANSI Colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

/**
 * @brief Logs a message to the standard output if not in silent mode.
 * 
 * This templated function prints the provided message to std::cout
 * only if the 'silent' parameter is false. It can be used to control
 * logging output in the ParkingLot system.
 * 
 * @tparam T The type of the message to be logged. Must be streamable to std::cout.
 * @param silent If true, suppresses the output; if false, outputs the message.
 * @param msg The message to be logged.
 */
template<typename T>
void ParkingLot_logOut(const bool silent, const T& msg) {
    if (!silent) {
        std::cout << msg;
    }
}
/**
 * @class ParkingLot
 * @brief Manages parking lot operations including billing and record keeping.
 */
ParkingLot::ParkingLot() : nextCarID(1001) {}



/**
 * @brief Saves the details of a car to a CSV file.
 *
 * This function appends the provided car's information to a CSV file named "cars_data.csv".
 * If the file does not exist, it writes the header row before appending the car data.
 * The current entry time is also recorded for each car.
 *
 * @param car The Car object containing all relevant details to be saved.
 */
void ParkingLot::saveCarToCSV(const Car& car) const {
    constexpr const char* filename = "cars_data.csv";
    bool write_header = false;
    struct stat buffer;
    if (stat(filename, &buffer) != 0) write_header = true;

    std::ofstream csv(filename, std::ios::app);
    if (write_header) {
        csv << "CarID,OwnerName,LicensePlate,Model,Color,FuelType,Phone,Email,Membership,PaymentMethod,Slot,Size,Rate,DynamicPricing,EntryTime\n";
    }

    std::time_t now = std::time(nullptr);
    std::string timeStr = std::ctime(&now);
    if (!timeStr.empty()) timeStr.pop_back();

    csv << car.id << ',' << car.ownerName << ',' << car.licensePlate << ','
        << car.model << ',' << car.color << ',' << car.fuelType << ','
        << car.phone << ',' << car.email << ',' << car.membership << ','
        << car.paymentMethod << ',' << car.slot << ',' << car.slotSize << ','
        << car.hourlyRate << ',' << (car.dynamicPricing ? "Yes" : "No") << ','
        << timeStr << '\n';
}

/**
 * @brief Saves the provided bill information to a text file.
 *
 * Appends the given bill string to the "bill_history.txt" file, ensuring
 * that each bill is written on a new line. This function is used to
 * maintain a persistent record of all generated bills.
 *
 * @param bill The bill information to be saved as a string.
 */

void ParkingLot::saveBillToText(const std::string& bill) const {
    std::ofstream file("bill_history.txt", std::ios::app);
    file << bill << '\n';
}

/**
 * @brief Handles the process of parking a car in the parking lot.
 *
 * Prompts the user for car and owner details, slot information, and parking preferences.
 * Supports reserved slots, dynamic pricing, and custom parking durations.
 * If the parking lot has available capacity, creates a Car object with the provided details,
 * optionally adjusts the parking time based on user input, and adds the car to the lot.
 * Saves the car information to CSV if not in silent mode.
 * Provides user feedback on successful parking.
 *
 * @note Uses console input/output for interaction. Updates internal car list and persists data.
 */

void ParkingLot::parkCar() {
    std::string ownerName, licensePlate, model, color, fuelType, phone, email, membership, paymentMethod;
    std::string slot, slotSize, exitGate;
    bool reservedSlot = false;
    double hourlyRate = 50.0;
    char reservedChoice, dynamicChoice;
    bool dynamicPricing = false;
    double parkingHours = 0.0;

    ParkingLot_logOut(silentMode, std::string(BOLD) + CYAN + "\n--- Car Parking Entry ---\n" + RESET);

    ParkingLot_logOut(silentMode, YELLOW "Owner Name: " RESET); 
    std::getline(std::cin, ownerName);

    ParkingLot_logOut(silentMode, YELLOW "License Plate: " RESET); 
    std::getline(std::cin, licensePlate);

    ParkingLot_logOut(silentMode, YELLOW "Car Model: " RESET); 
    std::getline(std::cin, model);

    ParkingLot_logOut(silentMode, YELLOW "Color: " RESET); 
    std::getline(std::cin, color);

    ParkingLot_logOut(silentMode, YELLOW "Fuel Type: " RESET); 
    std::getline(std::cin, fuelType);

    // PHONE VALIDATION - Only digits allowed
    while (true) {
        ParkingLot_logOut(silentMode, YELLOW "Phone: " RESET);
        std::getline(std::cin, phone);
        bool valid = !phone.empty() && std::all_of(phone.begin(), phone.end(), ::isdigit);
        if (valid) break;
        std::cout << RED << "Invalid input! Please enter a number.\n" << RESET;
    }

    // EMAIL VALIDATION - Must contain '@'
    while (true) {
        ParkingLot_logOut(silentMode, YELLOW "Email: " RESET);
        std::getline(std::cin, email);
        if (email.find('@') != std::string::npos) break;
        std::cout << RED << "Invalid email! Please include '@'.\n" << RESET;
    }

    ParkingLot_logOut(silentMode, YELLOW "Membership: " RESET); 
    std::getline(std::cin, membership);

    ParkingLot_logOut(silentMode, YELLOW "Payment Method: " RESET); 
    std::getline(std::cin, paymentMethod);

    ParkingLot_logOut(silentMode, YELLOW "Slot: " RESET); 
    std::getline(std::cin, slot);

    ParkingLot_logOut(silentMode, YELLOW "Slot Size: " RESET); 
    std::getline(std::cin, slotSize);

    // RESERVED SLOT VALIDATION - Only y/n allowed
    while (true) {
        ParkingLot_logOut(silentMode, YELLOW "Reserved Slot? (y/n): " RESET);
        std::cin >> reservedChoice;
        if (reservedChoice == 'y' || reservedChoice == 'Y' || reservedChoice == 'n' || reservedChoice == 'N') {
            reservedSlot = (reservedChoice == 'y' || reservedChoice == 'Y');
            break;
        }
        std::cout << RED << "Invalid choice! Enter only y or n.\n" << RESET;
    }
    std::cin.ignore();

    ParkingLot_logOut(silentMode, YELLOW "Exit Gate: " RESET); 
    std::getline(std::cin, exitGate);

    ParkingLot_logOut(silentMode, YELLOW "Hourly Rate: " RESET); 
    std::cin >> hourlyRate;

    ParkingLot_logOut(silentMode, YELLOW "Dynamic Pricing? (y/n): " RESET); 
    std::cin >> dynamicChoice;
    dynamicPricing = (dynamicChoice == 'y' || dynamicChoice == 'Y');

    ParkingLot_logOut(silentMode, YELLOW "Parking Duration (hours, enter 0 for current time): " RESET); 
    std::cin >> parkingHours;
    std::cin.ignore();

    Car car(nextCarID++, ownerName, licensePlate, model, color, fuelType,
            phone, email, membership, paymentMethod, slot, slotSize,
            reservedSlot, exitGate, hourlyRate, dynamicPricing);

    if (parkingHours > 0.0) {
        auto now = std::chrono::system_clock::now();
        car.parkingTime = now - std::chrono::duration_cast<std::chrono::system_clock::duration>(
                                  std::chrono::duration<double>(parkingHours * 3600.0));
    }

    if (cars.size() < MAX_CAPACITY) {
        cars.push_back(std::move(car));
        if (!silentMode) {
            saveCarToCSV(cars.back());
        }
        ParkingLot_logOut(silentMode, GREEN "✅ Car parked successfully! and Ticket is Generated\n" RESET);
    }
}


/**
 * @brief Removes a car from the parking lot based on user input.
 *
 * Prompts the user to enter a car ID and owner name, then attempts to remove the car
 * matching both criteria from the parking lot. If the car is not found or the owner name
 * does not match, an error message is displayed.
 */
void ParkingLot::removeCar() {
    int carID;
    std::string ownerName;
    ParkingLot_logOut(silentMode, CYAN "\n--- Car Removal ---\n" RESET);
    ParkingLot_logOut(silentMode, "Enter Car ID: "); std::cin >> carID;
    std::cin.ignore();
    ParkingLot_logOut(silentMode, "Enter Owner Name: "); std::getline(std::cin, ownerName);

    if (!removeCarByIdAndOwner(carID, ownerName))
        ParkingLot_logOut(silentMode, RED "❌ Car not found or owner mismatch.\n" RESET);
}

/**
 * @brief Displays a formatted list of all parked cars in the parking lot.
 *
 * If no cars are currently parked, outputs a message indicating this.
 * Otherwise, prints a table header followed by a row for each car,
 * showing details such as ID, owner, license plate, model, color, fuel type,
 * slot, slot size, hourly rate, and whether dynamic pricing is enabled.
 *
 * @note Output is sent via ParkingLot_logOut, respecting the silentMode setting.
 */
void ParkingLot::displayCars() const {
    if (cars.empty()) {
        ParkingLot_logOut(silentMode, YELLOW "No cars parked.\n" RESET);
        return;
    }
    ParkingLot_logOut(silentMode, CYAN "ID\tOwner\tPlate\tModel\tColor\tFuel\tSlot\tSize\tRate\tDyn?\n" RESET);
    for (const auto& car : cars) {
        std::ostringstream row;
        row << car.id << '\t' << car.ownerName << '\t' << car.licensePlate << '\t'
            << car.model << '\t' << car.color << '\t' << car.fuelType << '\t'
            << car.slot << '\t' << car.slotSize << '\t'
            << car.hourlyRate << '\t' << (car.dynamicPricing ? "Yes" : "No") << '\n';
        ParkingLot_logOut(silentMode, row.str());
    }
}

/**
 * @brief Removes a car from the parking lot by its ID and owner name, and generates a detailed bill.
 *
 * Searches for a car in the parking lot matching the specified ID and owner name.
 * If found, calculates the total hours parked, applies dynamic pricing and discounts if applicable,
 * computes GST, and generates a formatted bill. The bill is logged and saved unless silent mode is enabled.
 * Finally, removes the car from the lot.
 *
 * @param id The unique identifier of the car to be removed.
 * @param owner The name of the car's owner to match for removal.
 * @return true if the car was found, billed, and removed; false if no matching car was found.
 */
bool ParkingLot::removeCarByIdAndOwner(const int id, const std::string& owner) {
    auto it = std::find_if(cars.begin(), cars.end(),
        [&](const Car& c) { return c.id == id && c.ownerName == owner; });

    if (it == cars.end()) return false;

    using namespace std::chrono;
    const double hours = std::max(0.0, duration_cast<std::chrono::minutes>(
        system_clock::now() - it->parkingTime).count() / 60.0);

    double gross = hours * it->hourlyRate;
    double discount = 0.0, gst = 0.0;

    if (it->dynamicPricing) {
        if (hours > 5.0) {
            discount = gross * 0.30;
            gross -= discount;
        }
        gst = gross * 0.18;
    }
    const double total = gross + gst;

    std::ostringstream bill;
    bill << "\n========= 🧾 PARKING BILL 🧾 =========\n"
         << "Car ID            : " << it->id << "\n"
         << "Owner Name        : " << it->ownerName << "\n"
         << "License Plate     : " << it->licensePlate << "\n"
         << "Hours Parked      : " << std::fixed << std::setprecision(2) << hours << "\n"
         << "Rate per Hour (₹) : " << it->hourlyRate << "\n"
         << "Gross (₹)         : " << (hours * it->hourlyRate) << "\n";
    if (discount > 0) bill << "Discount (30%)    : -" << discount << "\n";
    bill << "GST @ 18% (₹)     : " << gst << "\n"
         << "TOTAL (₹)         : " << total << "\n"
         << "======================================\n";

    if (!silentMode) {
        ParkingLot_logOut(silentMode, bill.str());
        saveBillToText(bill.str());
    }

    cars.erase(it);
    return true;
}

/**
 * @brief Retrieves a car from the parking lot by its unique ID.
 * 
 * Searches the internal list of cars for a car with the specified ID.
 * If a matching car is found, it is returned; otherwise, a default-constructed Car object is returned.
 * 
 * @param id The unique identifier of the car to retrieve.
 * @return Car The car with the specified ID if found; otherwise, a default Car object.
 */
Car ParkingLot::getCarByID(const int id) const {
    auto it = std::find_if(cars.begin(), cars.end(),
        [id](const Car& car) { return car.id == id; });
    return (it != cars.end()) ? *it : Car();
}

size_t ParkingLot::getCarCount() const {
    return cars.size();
}

/**
 * @brief Calculates the total parking fee for a given car based on parking duration, hourly rate, dynamic pricing, and GST.
 * 
 * This method computes the parking fee by determining the number of hours the car has been parked,
 * applying the hourly rate, and then adjusting for dynamic pricing discounts and GST if applicable.
 * If dynamic pricing is enabled and the parking duration exceeds 5 hours, a 30% discount is applied before GST.
 * GST is calculated at 18% on the subtotal after any discount.
 * 
 * @param car Reference to a Car object containing parking time, hourly rate, and dynamic pricing flag.
 * @return The total fee to be charged for the car's parking session, including any discounts and GST.
 */
double ParkingLot::calculateFee(const Car& car) const {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto duration = now - car.parkingTime;
    double hours = std::max(0.0, duration_cast<std::chrono::minutes>(duration).count() / 60.0);

    double subtotal = hours * car.hourlyRate;
    double discount = 0.0, gst = 0.0;
    if (car.dynamicPricing) {
        if (hours > 5.0) {
            discount = subtotal * 0.30;
            subtotal -= discount;
        }
        gst = subtotal * 0.18;
    }
    return subtotal + gst;
}

/**
 * @brief Attempts to add a car to the parking lot if there is available capacity.
 * 
 * If the current number of cars is less than the maximum allowed capacity,
 * the provided car is added to the parking lot's collection.
 * 
 * @param car The Car object to be added to the parking lot.
 */
void ParkingLot::testAddCar(const Car& car) {
    if (cars.size() < MAX_CAPACITY) {
        cars.push_back(car);
    }
}
