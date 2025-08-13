#include "car.h"

Car::Car() 
  : id(0), hourlyRate(50.0), 
    dynamicPricing(false), 
    reservedSlot(false),
    parkingTime(std::chrono::system_clock::now()) {}

/**
 * @brief Constructs a Car object with detailed information about the car and its parking status.
 * 
 * Initializes all member variables with the provided parameters and sets the parking time to the current system time.
 * 
 * @param id Unique identifier for the car.
 * @param ownerName Name of the car's owner.
 * @param licensePlate License plate number of the car.
 * @param model Model name or number of the car.
 * @param color Color of the car.
 * @param fuelType Type of fuel the car uses (e.g., petrol, diesel, electric).
 * @param phone Contact phone number of the owner.
 * @param email Contact email address of the owner.
 * @param membership Membership type or status associated with the car or owner.
 * @param paymentMethod Preferred payment method for parking fees.
 * @param slot Assigned parking slot identifier.
 * @param slotSize Size category of the parking slot (e.g., small, medium, large).
 * @param reservedSlot Indicates if the parking slot is reserved for this car.
 * @param exitGate Designated exit gate for the car.
 * @param hourlyRate Hourly parking rate applicable to this car.
 * @param dynamicPricing Indicates if dynamic pricing is enabled for this car.
 */
Car::Car(int id, 
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
    bool dynamicPricing)

  : id(id),
    ownerName(ownerName), 
    licensePlate(licensePlate), 
    model(model),
    color(color), 
    fuelType(fuelType), 
    phone(phone), 
    email(email),
    membership(membership), 
    paymentMethod(paymentMethod), 
    slot(slot),
    slotSize(slotSize), 
    reservedSlot(reservedSlot), 
    exitGate(exitGate),
    hourlyRate(hourlyRate),
     dynamicPricing(dynamicPricing),
    parkingTime(std::chrono::system_clock::now()) {}
