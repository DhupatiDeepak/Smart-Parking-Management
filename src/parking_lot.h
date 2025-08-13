#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "car.h"
#include <chrono>

/**
 * @class ParkingLot
 * @brief Manages a collection of parked cars, their addition, removal, and billing in a parking lot system.
 *
 * The ParkingLot class encapsulates the logic for handling car parking operations, including adding and removing cars,
 * displaying parked cars, saving car and billing information, and calculating parking fees. It maintains an internal
 * list of cars, enforces a maximum capacity, and supports a silent mode for suppressing output.
 */
class ParkingLot {
private:
    /**
     * @brief Stores the list of currently parked cars.
     */
    std::vector<Car> cars;

    /**
     * @brief Tracks the next unique car ID to assign when a new car is parked.
     */
    int nextCarID;

    /**
     * @brief The maximum number of cars that can be parked in the lot.
     */
    static constexpr int MAX_CAPACITY = 100;

    /**
     * @brief If true, suppresses output and notifications for silent operation.
     */
    bool silentMode = false;

public:
    /**
     * @brief Constructs a new ParkingLot object, initializing internal state.
     */
    ParkingLot();

    /**
     * @brief Enables or disables silent mode for the parking lot.
     * @param mode Set to true to suppress output, false to enable normal operation.
     */
    void setSilentMode(bool mode) { silentMode = mode; }

    /**
     * @brief Parks a new car in the lot, assigning it a unique ID and storing its information.
     *
     * If the lot is at maximum capacity, the operation is aborted.
     * Prompts the user for car details unless in silent mode.
     */
    void parkCar();

    /**
     * @brief Removes a car from the lot based on user input.
     *
     * Prompts the user for car identification and removes the corresponding car if found.
     * Generates and saves a bill for the removed car.
     */
    void removeCar();

    /**
     * @brief Displays information about all currently parked cars.
     *
     * Outputs car details to the console unless silent mode is enabled.
     */
    void displayCars() const;

    /**
     * @brief Saves the details of a car to a CSV file for record-keeping.
     * @param car The car whose information is to be saved.
     */
    void saveCarToCSV(const Car& car) const;

    /**
     * @brief Saves a billing statement to a text file.
     * @param bill The billing information to be saved.
     */
    void saveBillToText(const std::string& bill) const;

    /**
     * @brief Removes a car from the lot by its ID and owner's name.
     * @param id The unique identifier of the car to remove.
     * @param owner The name of the car's owner for verification.
     * @return True if the car was found and removed; false otherwise.
     */
    bool removeCarByIdAndOwner(int id, const std::string& owner);

    /**
     * @brief Retrieves a car object by its unique ID.
     * @param id The unique identifier of the car to retrieve.
     * @return The Car object with the specified ID. Throws if not found.
     */
    Car getCarByID(int id) const;

    /**
     * @brief Gets the current number of cars parked in the lot.
     * @return The number of cars currently parked.
     */
    size_t getCarCount() const;

    /**
     * @brief Calculates the parking fee for a given car based on its parking duration and other criteria.
     * @param car The car for which to calculate the fee.
     * @return The calculated parking fee.
     */
    double calculateFee(const Car& car) const;

    /**
     * @brief Adds a car to the lot for testing purposes, bypassing user input.
     * @param car The car object to add to the lot.
     */
    void testAddCar(const Car& car);
};
