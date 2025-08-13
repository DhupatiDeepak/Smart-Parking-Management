#pragma once
#include <string>
#include <chrono>

/**
 * @class Car
 * @brief Represents a car parked in the parking management system, encapsulating owner, vehicle, and parking details.
 *
 * The Car class stores comprehensive information about a parked vehicle, including owner details, vehicle attributes,
 * parking slot assignment, membership and payment information, and parking time. It supports both static and dynamic
 * pricing models and tracks whether the slot is reserved.
 */
class Car {
public:
    /**
     * @brief Unique identifier for the car within the system.
     */
    int id = 0;

    /**
     * @brief Name of the car's owner.
     */
    std::string ownerName;

    /**
     * @brief License plate number of the car.
     */
    std::string licensePlate;

    /**
     * @brief Model name or number of the car.
     */
    std::string model;

    /**
     * @brief Color of the car.
     */
    std::string color;

    /**
     * @brief Type of fuel used by the car (e.g., Petrol, Diesel, Electric).
     */
    std::string fuelType;

    /**
     * @brief Contact phone number of the car's owner.
     */
    std::string phone;

    /**
     * @brief Email address of the car's owner.
     */
    std::string email;

    /**
     * @brief Membership type associated with the car or owner (e.g., Regular, VIP).
     */
    std::string membership;

    /**
     * @brief Preferred or selected payment method for parking fees.
     */
    std::string paymentMethod;

    /**
     * @brief Identifier of the parking slot assigned to the car.
     */
    std::string slot;

    /**
     * @brief Size category of the assigned parking slot (e.g., Small, Medium, Large).
     */
    std::string slotSize;

    /**
     * @brief Name or identifier of the exit gate assigned for the car's departure.
     */
    std::string exitGate;

    /**
     * @brief Indicates whether the assigned slot is reserved for this car.
     */
    bool reservedSlot = false;

    /**
     * @brief Hourly parking rate applicable to this car.
     */
    double hourlyRate = 50.0;

    /**
     * @brief Indicates if dynamic pricing is enabled for this car's parking session.
     */
    bool dynamicPricing = false;

    /**
     * @brief Timestamp marking when the car was parked.
     */
    std::chrono::system_clock::time_point parkingTime;

    /**
     * @brief Default constructor. Initializes a Car object with default values.
     */
    Car();

    /**
     * @brief Parameterized constructor. Initializes a Car object with the provided details.
     * @param id Unique identifier for the car.
     * @param ownerName Name of the car's owner.
     * @param licensePlate License plate number of the car.
     * @param model Model name or number of the car.
     * @param color Color of the car.
     * @param fuelType Type of fuel used by the car.
     * @param phone Contact phone number of the owner.
     * @param email Email address of the owner.
     * @param membership Membership type associated with the car or owner.
     * @param paymentMethod Preferred or selected payment method.
     * @param slot Identifier of the assigned parking slot.
     * @param slotSize Size category of the assigned slot.
     * @param reservedSlot Indicates if the slot is reserved for this car.
     * @param exitGate Name or identifier of the assigned exit gate.
     * @param hourlyRate Hourly parking rate for this car.
     * @param dynamicPricing Indicates if dynamic pricing is enabled.
     */
    Car(int id,
        const std::string& ownerName,
        const std::string& licensePlate,
        const std::string& model,
        const std::string& color,
        const std::string& fuelType,
        const std::string& phone,
        const std::string& email,
        const std::string& membership,
        const std::string& paymentMethod,
        const std::string& slot,
        const std::string& slotSize,
        bool reservedSlot,
        const std::string& exitGate,
        double hourlyRate,
        bool dynamicPricing);

    // Add const member functions if you plan to add getters or other methods
};
