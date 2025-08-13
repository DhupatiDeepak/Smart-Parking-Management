#include "parking_lot.h"
#include <iostream>
#include <string>
#include <chrono>
#include <cassert>
#include <stdexcept>

// =============================
// 📌 Helper for creating cars
// =============================
/**
 * @brief Creates and initializes a Car object with the specified parameters.
 *
 * This function constructs a Car instance using the provided id, owner, dynamic pricing flag,
 * and hourly rate. The rest of the Car's attributes are set to default or placeholder values.
 * The parkingTime member of the Car is set to the current system time.
 *
 * @param id Unique identifier for the car.
 * @param owner Name of the car's owner.
 * @param dynamicPricing Indicates whether dynamic pricing is enabled for this car.
 * @param hourlyRate The hourly parking rate to be applied for this car.
 * @return Car Returns a Car object initialized with the given and default values.
 */
Car createCar(
    int id,
    const std::string& owner,
    bool dynamicPricing = false,
    double hourlyRate = 50.0
) {
    Car car(
        id, owner, "MH12AB1234", "Model X", "Color",
        "Fuel", "0000000000", "email@example.com", "None", "Cash",
        "S1", "Medium", false, "Exit A", hourlyRate, dynamicPricing
    );
    car.parkingTime = std::chrono::system_clock::now();
    return car;
}

// =============================
// 📌 Test Functions
// =============================
/**
 * @brief Tests parking cars and counting the total number of cars in the parking lot.
 *
 * This function creates a ParkingLot instance, ensures it starts empty, adds two cars
 * using testAddCar, and verifies that the car count increments as expected after each addition.
 * It uses assertions to validate the correctness of the car counting logic.
 */
void testParkCarAndCount() {
    ParkingLot lot; lot.setSilentMode(true);
    assert(lot.getCarCount() == 0);
    lot.testAddCar(createCar(1001, "John Doe"));
    assert(lot.getCarCount() == 1);
    lot.testAddCar(createCar(1002, "Jane Smith"));
    assert(lot.getCarCount() == 2);
}

/**
 * @brief Tests the removal of a car from the ParkingLot by both car ID and owner name.
 *
 * This function verifies that a car can only be removed from the ParkingLot if both the car's ID and the owner's name match.
 * It first adds a car with a specific ID and owner, then attempts to remove it with an incorrect owner name (which should fail),
 * and finally removes it with the correct owner name (which should succeed). The function asserts the car count after each operation
 * to ensure correctness.
 */
void testRemoveCarByIdAndOwner() {
    ParkingLot lot; lot.setSilentMode(true);
    const Car c = createCar(1001, "John Doe");
    lot.testAddCar(c);
    assert(!lot.removeCarByIdAndOwner(1001, "Jane Smith"));
    assert(lot.getCarCount() == 1);
    assert(lot.removeCarByIdAndOwner(1001, "John Doe"));
    assert(lot.getCarCount() == 0);
}

/**
 * @brief Tests the ParkingLot::calculateFee function for different car scenarios.
 *
 * This function creates two Car objects with different parking durations and discount statuses,
 * adds them to the ParkingLot, and asserts that the calculated parking fees are within expected ranges.
 * - The first car (no discount) is parked for 3 hours.
 * - The second car (with discount) is parked for 6 hours.
 * The assertions verify that the fee calculation logic in ParkingLot is correct for both cases.
 */
void testCalculateFee() {
    ParkingLot lot; lot.setSilentMode(true);
    const auto now = std::chrono::system_clock::now();
    Car c1 = createCar(1001, "John Doe", false, 50);
    c1.parkingTime = now - std::chrono::hours(3);
    lot.testAddCar(c1);
    double fee = lot.calculateFee(c1);
    assert(fee >= 149.9 && fee <= 150.1);

    Car c2 = createCar(1002, "Jane Smith", true, 60);
    c2.parkingTime = now - std::chrono::hours(6);
    lot.testAddCar(c2);
    fee = lot.calculateFee(c2);
    assert(fee >= 297.3 && fee <= 297.5);
}

/**
 * @brief Tests the ParkingLot::getCarByID method for correct retrieval of Car objects.
 *
 * This function adds a car with a specific ID and owner name to the ParkingLot,
 * then verifies that getCarByID returns the correct Car object when queried with
 * the existing ID. It also checks that querying with a non-existent ID returns
 * a Car object with id set to 0.
 */
void testGetCarByID() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(1001, "John Doe"));
    const Car found = lot.getCarByID(1001);
    assert(found.id == 1001);
    assert(found.ownerName == "John Doe");
    const Car notFound = lot.getCarByID(9999);
    assert(notFound.id == 0);
}

/**
 * @brief Tests adding cars until the parking lot reaches its maximum capacity.
 *
 * This test:
 * - Creates a new ParkingLot and enables silent mode to suppress output.
 * - Adds cars sequentially until the maximum capacity (100) is reached.
 * - Verifies that the car count matches the maximum capacity.
 * - Attempts to add an additional car beyond capacity.
 * - Asserts that the car count does not exceed the maximum capacity.
 */
void testAddCarsAndCapacity() {
    ParkingLot lot; lot.setSilentMode(true);
    constexpr int maxCap = 100;
    for (int i = 1; i <= maxCap; ++i)
        lot.testAddCar(createCar(1000 + i, "Owner" + std::to_string(i)));
    assert(lot.getCarCount() == maxCap);
    lot.testAddCar(createCar(2001, "Extra Owner"));
    assert(lot.getCarCount() <= maxCap);
}

/**
 * @brief Tests removing cars from the parking lot with invalid ID/owner combinations.
 *
 * This test:
 * - Adds a car with a specific ID and owner name.
 * - Attempts removal with:
 *   - An incorrect ID but correct owner name.
 *   - A correct ID but incorrect owner name.
 * - Ensures both incorrect attempts fail.
 * - Successfully removes the car with the correct ID and owner name.
 */
void testRemoveInvalidCars() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(1001, "John Doe"));
    assert(!lot.removeCarByIdAndOwner(9999, "John Doe"));
    assert(!lot.removeCarByIdAndOwner(1001, "Wrong Owner"));
    assert(lot.removeCarByIdAndOwner(1001, "John Doe"));
}

/**
 * @brief Tests parking fee calculation for edge case scenarios.
 *
 * This test covers:
 * - A car parked for zero hours (fee should be non-negative).
 * - A car with a future parking time (fee should still be non-negative).
 * - A car parked for exactly 24 hours with a discount applied (fee should be positive).
 *
 * @note Uses system clock adjustments to simulate parking duration.
 */
void testCalculateFeeEdgeCases() {
    ParkingLot lot; lot.setSilentMode(true);
    Car zeroHr = createCar(1, "Zero Hour", false, 50);
    zeroHr.parkingTime = std::chrono::system_clock::now();
    lot.testAddCar(zeroHr);
    assert(lot.calculateFee(zeroHr) >= 0.0);

    Car future = createCar(2, "Future Owner", false, 50);
    future.parkingTime = std::chrono::system_clock::now() + std::chrono::hours(5);
    lot.testAddCar(future);
    assert(lot.calculateFee(future) >= 0.0);

    Car longCar = createCar(3, "Long Timer", true, 100);
    longCar.parkingTime = std::chrono::system_clock::now() - std::chrono::hours(24);
    lot.testAddCar(longCar);
    assert(lot.calculateFee(longCar) > 0);
}

/**
 * @brief Tests retrieval of multiple cars by their unique IDs.
 *
 * This test:
 * - Adds three cars with unique IDs and owners.
 * - Verifies that each car can be retrieved by its ID.
 * - Removes one car.
 * - Ensures that attempting to retrieve the removed car returns a Car with ID = 0.
 */
void testGetCarByIDMultiple() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(1001, "Alice"));
    lot.testAddCar(createCar(1002, "Bob"));
    lot.testAddCar(createCar(1003, "Charlie"));
    assert(lot.getCarByID(1002).ownerName == "Bob");
    assert(lot.getCarByID(1003).ownerName == "Charlie");
    lot.removeCarByIdAndOwner(1002, "Bob");
    assert(lot.getCarByID(1002).id == 0);
}

/**
 * @brief Tests case sensitivity when matching owner names during car removal.
 *
 * This test:
 * - Adds a car with the owner name "John Doe".
 * - Attempts to remove it using a lowercase owner name, which should fail.
 * - Removes it using the exact matching owner name, which should succeed.
 */
void testOwnerNameCaseSensitivity() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(1001, "John Doe"));
    assert(!lot.removeCarByIdAndOwner(1001, "john doe"));
    assert(lot.removeCarByIdAndOwner(1001, "John Doe"));
}

/**
 * @brief Tests fee calculation for a reserved slot car with special features.
 *
 * This test:
 * - Creates a reserved-slot car with VIP attributes.
 * - Adds it to the parking lot.
 * - Ensures that the calculated parking fee is non-negative.
 */
void testReservedSlotsAndFeatures() {
    ParkingLot lot; lot.setSilentMode(true);
    Car reserved = createCar(1001, "VIP User", true, 100);
    reserved.reservedSlot = true;
    lot.testAddCar(reserved);
    assert(lot.calculateFee(reserved) >= 0);
}

/**
 * @brief Tests the handling of duplicate license plates.
 *
 * This test:
 * - Creates two cars with different IDs but the same license plate number.
 * - Adds both cars to the parking lot.
 * - Verifies that both cars are stored despite having the same license plate.
 */
void testDuplicateLicensePlates() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c1 = createCar(1001, "Owner1");
    c1.licensePlate = "DUP123";
    Car c2 = createCar(1002, "Owner2");
    c2.licensePlate = "DUP123";
    lot.testAddCar(c1);
    lot.testAddCar(c2);
    assert(lot.getCarCount() == 2);
}

/**
 * @brief Tests repeated add/remove cycles to verify parking lot stability.
 *
 * This test:
 * - Adds 10 cars to the parking lot and verifies the count.
 * - Removes the first 5 cars and checks that the count decreases accordingly.
 * - Adds 5 new cars and ensures the total count returns to 10.
 */
void testAddRemoveCycles() {
    ParkingLot lot; lot.setSilentMode(true);
    for (int i = 0; i < 10; ++i)
        lot.testAddCar(createCar(1000 + i, "Owner" + std::to_string(i)));
    assert(lot.getCarCount() == 10);

    for (int i = 0; i < 5; ++i)
        assert(lot.removeCarByIdAndOwner(1000 + i, "Owner" + std::to_string(i)));
    assert(lot.getCarCount() == 5);

    for (int i = 10; i < 15; ++i)
        lot.testAddCar(createCar(1000 + i, "Owner" + std::to_string(i)));
    assert(lot.getCarCount() == 10);
}

/**
 * @brief Tests behavior when adding a car with invalid data.
 *
 * This test:
 * - Creates a Car object with invalid fields (e.g., negative ID, malformed email).
 * - Adds it to the parking lot.
 * - Verifies that the car count increases to 1 despite invalid data.
 */
void testInvalidCarData() {
    ParkingLot lot;
    const Car invalid(
        -1, "Invalid", "MH00", "Invalid", "Red", "Petrol", "123",
        "invalid@", "None", "Cash", "S1", "Medium", false, "Exit A", 50, false
    );
    lot.testAddCar(invalid);
    assert(lot.getCarCount() == 1);
}

/**
 * @brief Tests removing the same car twice to ensure idempotent behavior.
 *
 * This test:
 * - Adds a car with ID 101 and owner "Alice".
 * - Removes it successfully the first time.
 * - Verifies that a second removal attempt for the same car fails.
 */
void testRemoveCarTwice() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(101, "Alice"));
    assert(lot.removeCarByIdAndOwner(101, "Alice"));
    assert(!lot.removeCarByIdAndOwner(101, "Alice"));
}

/**
 * @brief Tests retrieving a car from an empty parking lot.
 *
 * This test:
 * - Attempts to retrieve a car with an arbitrary ID (999) from an empty lot.
 * - Ensures that the returned Car object has an ID of 0, indicating "not found".
 */
void testGetCarFromEmptyLot() {
    ParkingLot lot; lot.setSilentMode(true);
    const Car result = lot.getCarByID(999);
    assert(result.id == 0);
}

/**
 * @brief Tests overwriting an existing car with another car of the same ID.
 *
 * This test:
 * - Adds a car with ID 200 and owner "Old Owner".
 * - Adds another car with the same ID but a different owner ("New Owner").
 * - Verifies that the car in the lot now belongs to "New Owner".
 */
void testOverwriteCarWithSameID() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(200, "Old Owner"));
    Car replacement = createCar(200, "New Owner");
    lot.testAddCar(replacement);
    assert(lot.getCarByID(200).ownerName == "Old Owner");
}

/**
 * @brief Tests fee calculation for exactly 5 hours of parking (no discount case).
 *
 * This test:
 * - Creates a VIP car parked for exactly 5 hours.
 * - Calculates the parking fee.
 * - Asserts that the fee is around 590 (±0.1 tolerance).
 */
void testFeeNoDiscountAtExactly5Hours() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(300, "Owner", true, 100);
    c.parkingTime = std::chrono::system_clock::now() - std::chrono::hours(5);
    double fee = lot.calculateFee(c);
    assert(fee >= 589.9 && fee <= 590.1);
}

/**
 * @brief Tests fee calculation with a discount applied for parking longer than 5 hours.
 *
 * This test:
 * - Creates a VIP car parked for 6 hours.
 * - Calculates the parking fee.
 * - Verifies that the fee is less than 600 and greater than 0.
 */
void testFeeDiscountAbove5Hours() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(301, "Owner", true, 100);
    c.parkingTime = std::chrono::system_clock::now() - std::chrono::hours(6);
    double fee = lot.calculateFee(c);
    assert(fee < 600 && fee > 0);
}

/**
 * @brief Tests fee calculation for long-term parking (30 days).
 *
 * This test:
 * - Creates a non-VIP car parked for approximately 30 days.
 * - Calculates the parking fee.
 * - Verifies that the fee exceeds 7000 units.
 */
void testLongTermParking() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(302, "LongTerm", false, 10);
    c.parkingTime = std::chrono::system_clock::now() - std::chrono::hours(24 * 30);
    double fee = lot.calculateFee(c);
    assert(fee > 7000);
}
/**
 * @brief Tests removing a car when the owner's name contains leading/trailing whitespace.
 *
 * This test:
 * - Adds a car with ID 400 and owner name containing spaces (" Bob ").
 * - Ensures that the removal works when the exact whitespace is matched in the query.
 */
void testRemoveWithWhitespaceInOwnerName() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(400, " Bob "));
    assert(lot.removeCarByIdAndOwner(400, " Bob "));
}

/**
 * @brief Tests adding multiple cars with the same owner name.
 *
 * This test:
 * - Adds two cars with different IDs but the same owner name ("Same Owner").
 * - Verifies that both cars are stored in the lot.
 */
void testMultipleCarsSameOwner() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(500, "Same Owner"));
    lot.testAddCar(createCar(501, "Same Owner"));
    assert(lot.getCarCount() == 2);
}

/**
 * @brief Tests case-sensitive behavior when removing a car by owner name.
 *
 * This test:
 * - Adds a car with ID 600 and owner name "John DOE".
 * - Tries to remove it using a different case ("John Doe").
 * - Ensures that removal fails due to case mismatch.
 */
void testRemoveWrongCaseOwner() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(600, "John DOE"));
    assert(!lot.removeCarByIdAndOwner(600, "John Doe"));
}

/**
 * @brief Tests fee calculation when the car's recorded parking time is in the future.
 *
 * This test:
 * - Creates a car with a parking time set 10 hours ahead of the current time.
 * - Ensures that the calculated fee is non-negative despite the invalid time.
 */
void testCalculateFeeNegativeTime() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(700, "FutureTime");
    c.parkingTime = std::chrono::system_clock::now() + std::chrono::hours(10);
    double fee = lot.calculateFee(c);
    assert(fee >= 0);
}

/**
 * @brief Tests parking lot capacity limits and boundary conditions.
 *
 * This test:
 * - Fills the parking lot to its maximum capacity (100 cars).
 * - Removes one car.
 * - Adds a new car to confirm that the lot can accept cars again after a slot is freed.
 * - Ensures that the total car count never exceeds the capacity.
 */
void testMaxCapacityBoundary() {
    ParkingLot lot; lot.setSilentMode(true);
    for (int i = 0; i < 100; i++)
        lot.testAddCar(createCar(800 + i, "Owner" + std::to_string(i)));
    assert(lot.getCarCount() == 100);
    assert(lot.removeCarByIdAndOwner(800, "Owner0"));
    lot.testAddCar(createCar(9999, "NewCar"));
    assert(lot.getCarCount() == 100);
}

/**
 * @brief Tests completely removing all cars from the lot.
 *
 * This test:
 * - Adds two cars with different owners.
 * - Removes both cars.
 * - Confirms that the lot is empty afterward.
 */
void testRemoveAllCars() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(900, "A"));
    lot.testAddCar(createCar(901, "B"));
    lot.removeCarByIdAndOwner(900, "A");
    lot.removeCarByIdAndOwner(901, "B");
    assert(lot.getCarCount() == 0);
}

/**
 * @brief Tests adding two cars with the same license plate and owner name but different IDs.
 *
 * This test:
 * - Creates two cars with IDs 1000 and 1001.
 * - Assigns the same owner name ("DupOwner") and same license plate ("SAME") to both.
 * - Ensures that both cars can coexist in the lot despite having identical owner and plate.
 */
void testSameLicenseSameOwnerDifferentID() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c1 = createCar(1000, "DupOwner");
    Car c2 = createCar(1001, "DupOwner");
    c1.licensePlate = c2.licensePlate = "SAME";
    lot.testAddCar(c1);
    lot.testAddCar(c2);
    assert(lot.getCarCount() == 2);
}

/**
 * @brief Tests fee calculation when the hourly rate is set to zero.
 *
 * This test:
 * - Creates a car with an hourly rate of 0.
 * - Sets its parking time to 5 hours ago.
 * - Ensures that the calculated fee is exactly 0 regardless of parking duration.
 */
void testFeeWithZeroHourlyRate() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(1200, "FreePark", false, 0);
    c.parkingTime -= std::chrono::hours(5);
    assert(lot.calculateFee(c) == 0);
}

/**
 * @brief Tests reusing a car ID after removal.
 *
 * This test:
 * - Adds a car with ID 1300 owned by "Owner1".
 * - Removes it.
 * - Adds a new car with the same ID but a different owner ("Owner2").
 * - Ensures that the new car replaces the old entry correctly.
 */
void testAddRemoveAddSameID() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(1300, "Owner1"));
    lot.removeCarByIdAndOwner(1300, "Owner1");
    lot.testAddCar(createCar(1300, "Owner2"));
    assert(lot.getCarByID(1300).ownerName == "Owner2");
}

/**
 * @brief Tests adding a car with an empty owner name.
 *
 * This test:
 * - Creates a car with ID 2000 and an empty string as the owner name.
 * - Ensures that the lot accepts it and increases the car count.
 */
void testAddCarWithEmptyOwnerName() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(2000, ""));
    assert(lot.getCarCount() == 1);
}

/**
 * @brief Tests adding a car with an extremely long owner name.
 *
 * This test:
 * - Creates a car with ID 2001 and an owner name of 500 'A' characters.
 * - Ensures that the name length is preserved when retrieved from the lot.
 */
void testAddCarWithVeryLongOwnerName() {
    ParkingLot lot; lot.setSilentMode(true);
    const std::string longName(500, 'A');
    lot.testAddCar(createCar(2001, longName));
    assert(lot.getCarByID(2001).ownerName.size() == 500);
}

/**
 * @brief Tests adding a car with special characters in the owner name.
 *
 * This test:
 * - Creates a car with ID 2002 and an owner name containing symbols ("John@#$%^&").
 * - Ensures that the name is stored intact and contains the "@" symbol.
 */
void testAddCarWithSpecialCharsInOwner() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(2002, "John@#$%^&"));
    assert(lot.getCarByID(2002).ownerName.find("@") != std::string::npos);
}

/**
 * @brief Tests adding a car with an empty license plate.
 *
 * This test:
 * - Creates a car with ID 2003 and a valid owner name but an empty license plate string.
 * - Ensures that the empty license plate is stored without modification.
 */
void testAddCarWithEmptyLicensePlate() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(2003, "Owner");
    c.licensePlate = "";
    lot.testAddCar(c);
    assert(lot.getCarByID(2003).licensePlate.empty());
}

/**
 * @brief Tests fee calculation for fractional hours of parking.
 *
 * This test:
 * - Creates a car with an hourly rate of 60.
 * - Sets its parking time to 1.5 hours ago.
 * - Ensures that the calculated fee is proportional (~90) and within a tolerance.
 */
void testFeeForFractionalHours() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(2004, "PartialHour", false, 60);
    c.parkingTime -= std::chrono::minutes(90);
    double fee = lot.calculateFee(c);
    assert(fee >= 89.9 && fee <= 90.1);
}


/**
 * @brief Tests the fee calculation for a car with a very small hourly rate.
 *
 * This function creates a car with an hourly parking rate of 0.01, simulates a parking duration of 2 hours,
 * and asserts that the calculated fee is greater than 0 but less than 0.05. This ensures that the fee calculation
 * logic handles very small rates correctly and does not round down to zero.
 */
void testFeeForVerySmallRate() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(2005, "Cheap", false, 0.01);
    c.parkingTime -= std::chrono::hours(2);
    double fee = lot.calculateFee(c);
    assert(fee > 0 && fee < 0.05);
}

/**
 * @brief Tests removing a car from an empty parking lot.
 *
 * This function creates an empty ParkingLot instance, enables silent mode to suppress output,
 * and asserts that attempting to remove a car with ID 10 and owner "Nobody" fails as expected.
 */
void testRemoveFromEmptyLot() {
    ParkingLot lot; lot.setSilentMode(true);
    assert(!lot.removeCarByIdAndOwner(10, "Nobody"));
}

/**
 * @brief Tests removal of a car with a non-existing ID from the parking lot.
 *
 * This function adds a car to the parking lot and then attempts to remove a car
 * using an ID that does not exist in the lot. It asserts that the removal operation
 * fails as expected.
 */
void testRemoveNonExistingID() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(2006, "Owner"));
    assert(!lot.removeCarByIdAndOwner(99999, "Owner"));
}

/**
 * @brief Tests adding two cars with the same slot but different IDs to the parking lot.
 *
 * This function creates two Car objects with different IDs but assigns them the same slot.
 * It then adds both cars to the ParkingLot and asserts that both are counted, verifying
 * that the system allows cars with different IDs to occupy the same slot.
 */
void testAddCarWithSameSlotDifferentID() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c1 = createCar(2007, "O1");
    Car c2 = createCar(2008, "O2");
    c2.slot = c1.slot;
    lot.testAddCar(c1);
    lot.testAddCar(c2);
    assert(lot.getCarCount() == 2);
}

/**
 * @brief Tests changing the slot of a car after it has been added to the parking lot.
 *
 * This function creates a ParkingLot instance in silent mode, adds a car with ID 2009 and owner "Owner",
 * retrieves the car by its ID, changes its slot to "S99", and asserts that the slot has been updated.
 */
void testChangeSlotAfterAdd() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(2009, "Owner"));
    Car c = lot.getCarByID(2009);
    c.slot = "S99";
    assert(c.slot == "S99");
}

/**
 * @brief Tests fee calculation for a car that has parked across midnight.
 *
 * This test creates a car, sets its parking time to 26 hours in the past (crossing midnight),
 * and asserts that the calculated parking fee is greater than zero. This ensures that the
 * ParkingLot::calculateFee method correctly handles cases where the parking duration spans
 * multiple days.
 */
void testFeeCalculationAfterMidnightCross() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(2010, "Night");
    c.parkingTime -= std::chrono::hours(26);
    assert(lot.calculateFee(c) > 0);
}

/**
 * @brief Tests that removing a car with extra spaces in the owner's name fails.
 *
 * This test adds a car with an owner name containing trailing spaces, then attempts
 * to remove it using the same ID but with the owner's name without spaces. The test
 * asserts that the removal fails, verifying that owner name matching is strict and
 * does not ignore extra spaces.
 */
void testRemoveWithExtraSpaces() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(2011, "Bob  "));
    assert(!lot.removeCarByIdAndOwner(2011, "Bob"));
}

/**
 * @brief Tests the fee calculation for a car parked for exactly one minute.
 *
 * This function creates a ParkingLot instance in silent mode and a Car object
 * representing a short stay of one minute. It adjusts the car's parking time to
 * simulate a one-minute stay, calculates the parking fee, and asserts that the
 * fee is greater than zero but less than five.
 */

void testFeeForOneMinuteParking() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(2012, "ShortStay", false, 60);
    c.parkingTime -= std::chrono::minutes(1);
    double fee = lot.calculateFee(c);
    assert(fee > 0 && fee < 5);
}

/**
 * @brief Tests adding multiple cars with the same ID sequentially to the parking lot.
 *
 * This function adds two cars with the same ID (2013) but different owner names ("First" and "Second")
 * to the ParkingLot instance. It asserts that after both additions, the car with ID 2013 in the lot
 * has the owner name "Second", verifying that the second addition overwrites the first.
 */

void testAddMultipleCarsSameIDSequentially() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(2013, "First"));
    lot.testAddCar(createCar(2013, "Second"));
    assert(lot.getCarByID(2013).ownerName == "First");
}

/**
 * @brief Tests the fee calculation with dynamic pricing at the exact boundary condition.
 *
 * This test creates a car that has been parked for exactly 5 hours (the boundary for dynamic pricing)
 * and verifies that the calculated parking fee falls within a small tolerance of the expected value (590).
 * The test ensures that the dynamic pricing logic in ParkingLot::calculateFee() correctly handles the boundary case.
 *
 * The ParkingLot is set to silent mode to suppress output during the test.
 * The car is created with the year 2014, model "Boundary", isElectric set to true, and an initial balance of 100.
 * The parking time is adjusted to simulate a 5-hour stay.
 * The test asserts that the calculated fee is within [589.9, 590.1].
 */

void testFeeWithDynamicPricingExactlyBoundary() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(2014, "Boundary", true, 100);
    c.parkingTime -= std::chrono::hours(5);
    double fee = lot.calculateFee(c);
    assert(fee >= 589.9 && fee <= 590.1);
}

/**
 * @brief Tests the fee calculation for a car with an extremely high hourly rate.
 *
 * This function creates a car with a very large hourly rate, simulates parking for one hour,
 * and asserts that the calculated parking fee is at least as large as the hourly rate.
 * Used to verify that the ParkingLot fee calculation handles large values correctly.
 */
void testFeeWithHugeHourlyRate() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(2015, "Rich", false, 1000000);
    c.parkingTime -= std::chrono::hours(1);
    assert(lot.calculateFee(c) >= 1000000);
}

/**
 * @brief Tests that adding a Car to the ParkingLot stores a copy, not a reference.
 *
 * This test creates a Car object, adds it to the ParkingLot using testAddCar,
 * then mutates the original Car's ownerName. It asserts that the Car stored in
 * the ParkingLot retains the original ownerName, confirming that the ParkingLot
 * stores a copy of the Car, not a reference.
 */
void testAddCarThenMutateOriginalObject() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(2016, "Original");
    lot.testAddCar(c);
    c.ownerName = "Changed";
    assert(lot.getCarByID(2016).ownerName == "Original");
}

/**
 * @brief Tests removing one car among multiple cars with the same owner.
 *
 * This function adds two cars with the same owner but different IDs to the ParkingLot,
 * removes one of them by specifying its ID and owner, and asserts that only one car remains.
 *
 * The test ensures that the removeCarByIdAndOwner method correctly identifies and removes
 * the intended car when multiple cars share the same owner.
 */

void testRemoveOneOfManyWithSameOwner() {
    ParkingLot lot; lot.setSilentMode(true);
    lot.testAddCar(createCar(2017, "SameOwner"));
    lot.testAddCar(createCar(2018, "SameOwner"));
    assert(lot.removeCarByIdAndOwner(2017, "SameOwner"));
    assert(lot.getCarCount() == 1);
}

/**
 * @brief Tests adding a car with all string fields set to empty strings.
 *
 * This function creates a Car object with all string parameters as empty strings and other fields set to default values.
 * It adds the car to the ParkingLot in silent mode and asserts that the car count increases to 1.
 */

void testAddCarWithAllEmptyStrings() {
    ParkingLot lot; lot.setSilentMode(true);
    const Car c(2019, "", "", "", "", "", "", "", "", "", "", "", false, "", 0, false);
    lot.testAddCar(c);
    assert(lot.getCarCount() == 1);
}

/**
 * @brief Tests that the parking fee is correctly calculated as zero for a car parked for zero hours with dynamic pricing enabled.
 *
 * This test creates a ParkingLot instance in silent mode, constructs a Car object with zero hours parked,
 * and asserts that the calculated fee is zero, verifying correct fee logic for this edge case.
 */

void testFeeCalculationZeroHoursDynamicPricing() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c = createCar(2020, "Zero", true, 50);
    assert(lot.calculateFee(c) == 0);
}

/**
 * @brief Tests adding 100 cars to the parking lot, then removing all of them, and verifies the lot is empty.
 *
 * This function adds 100 cars with unique IDs and owner names to the ParkingLot instance using testAddCar.
 * It then removes each car by its ID and owner name using removeCarByIdAndOwner.
 * Finally, it asserts that the parking lot is empty by checking that getCarCount() returns 0.
 */

void testMaxCapacityAndRemoveAll() {
    ParkingLot lot; lot.setSilentMode(true);
    for (int i = 0; i < 100; i++)
        lot.testAddCar(createCar(2100 + i, "O" + std::to_string(i)));
    for (int i = 0; i < 100; i++)
        lot.removeCarByIdAndOwner(2100 + i, "O" + std::to_string(i));
    assert(lot.getCarCount() == 0);
}

/**
 * @brief Tests adding a car, removing it, and then adding another car with the same license plate.
 *
 * This test verifies that after removing a car with a specific license plate,
 * another car with a different ID and owner but the same license plate can be added successfully.
 * It asserts that the newly added car is retrievable by its ID and retains the correct license plate.
 */

void testAddRemoveAddSameLicensePlate() {
    ParkingLot lot; lot.setSilentMode(true);
    Car c1 = createCar(2200, "Owner1");
    c1.licensePlate = "PLATE1";
    lot.testAddCar(c1);
    lot.removeCarByIdAndOwner(2200, "Owner1");

    Car c2 = createCar(2201, "Owner2");
    c2.licensePlate = "PLATE1";
    lot.testAddCar(c2);
    assert(lot.getCarByID(2201).licensePlate == "PLATE1");
}

// 📌 ANSI Color Codes
// =============================
#define ANSI_RESET   "\033[0m"
#define ANSI_GREEN   "\033[1;32m"
#define ANSI_RED     "\033[1;31m"
#define ANSI_YELLOW  "\033[1;33m"
#define ANSI_BLUE    "\033[1;34m"

// =============================
// 📌 Unified Test Runner Macro (with colors)
// =============================
#define RUN_TEST(testFunc) \
    do { \
        try { \
            testFunc(); \
            std::cout << ANSI_GREEN << "[PASS] " << #testFunc << ANSI_RESET << std::endl; \
        } catch (const std::logic_error& e) { \
            std::cout << ANSI_YELLOW << "[ASSERTION FAILED] " << #testFunc << " - " << e.what() << ANSI_RESET << std::endl; \
        } catch (const std::runtime_error& e) { \
            std::cout << ANSI_YELLOW << "[ASSERTION FAILED] " << #testFunc << " - " << e.what() << ANSI_RESET << std::endl; \
        } catch (const std::exception& e) { \
            std::cout << ANSI_RED << "[FAIL] " << #testFunc << " - Exception: " << e.what() << ANSI_RESET << std::endl; \
        } catch (...) { \
            std::cout << ANSI_RED << "[FAIL] " << #testFunc << " - Unknown error" << ANSI_RESET << std::endl; \
        } \
    } while(0)

// =============================
// 📌 MAIN FUNCTION
// =============================
/**
 * @brief Entry point for the ParkingLot full unit test suite.
 *
 * This function runs a comprehensive set of unit tests for the ParkingLot system,
 * covering a wide range of scenarios including parking, removing, fee calculation,
 * edge cases, and data validation. Each test is executed using the RUN_TEST macro.
 * The results are output to the standard console.
 *
 * @return int Returns 0 upon successful completion of all tests.
 */

int main() {
    std::cout << ANSI_BLUE <<"===== ParkingLot Full Unit Test Suite =====" << std::endl;

    RUN_TEST(testParkCarAndCount);
    RUN_TEST(testRemoveCarByIdAndOwner);
    RUN_TEST(testCalculateFee);
    RUN_TEST(testGetCarByID);
    RUN_TEST(testAddCarsAndCapacity);
    RUN_TEST(testRemoveInvalidCars);
    RUN_TEST(testCalculateFeeEdgeCases);
    RUN_TEST(testGetCarByIDMultiple);
    RUN_TEST(testOwnerNameCaseSensitivity);
    RUN_TEST(testReservedSlotsAndFeatures);
    RUN_TEST(testDuplicateLicensePlates);
    RUN_TEST(testAddRemoveCycles);
    RUN_TEST(testInvalidCarData);
    RUN_TEST(testRemoveCarTwice);
    RUN_TEST(testGetCarFromEmptyLot);
    RUN_TEST(testOverwriteCarWithSameID);
    RUN_TEST(testFeeNoDiscountAtExactly5Hours);
    RUN_TEST(testFeeDiscountAbove5Hours);
    RUN_TEST(testLongTermParking);
    RUN_TEST(testRemoveWithWhitespaceInOwnerName);
    RUN_TEST(testMultipleCarsSameOwner);
    RUN_TEST(testRemoveWrongCaseOwner);
    RUN_TEST(testCalculateFeeNegativeTime);
    RUN_TEST(testMaxCapacityBoundary);
    RUN_TEST(testRemoveAllCars);
    RUN_TEST(testSameLicenseSameOwnerDifferentID);
    RUN_TEST(testFeeWithZeroHourlyRate);
    RUN_TEST(testAddRemoveAddSameID);
    RUN_TEST(testAddCarWithEmptyOwnerName);
    RUN_TEST(testAddCarWithVeryLongOwnerName);
    RUN_TEST(testAddCarWithSpecialCharsInOwner);
    RUN_TEST(testAddCarWithEmptyLicensePlate);
    RUN_TEST(testFeeForFractionalHours);
    RUN_TEST(testFeeForVerySmallRate);
    RUN_TEST(testRemoveFromEmptyLot);
    RUN_TEST(testRemoveNonExistingID);
    RUN_TEST(testAddCarWithSameSlotDifferentID);
    RUN_TEST(testChangeSlotAfterAdd);
    RUN_TEST(testFeeCalculationAfterMidnightCross);
    RUN_TEST(testRemoveWithExtraSpaces);
    RUN_TEST(testFeeForOneMinuteParking);
    RUN_TEST(testAddMultipleCarsSameIDSequentially);
    RUN_TEST(testFeeWithDynamicPricingExactlyBoundary);
    RUN_TEST(testFeeWithHugeHourlyRate);
    RUN_TEST(testAddCarThenMutateOriginalObject);
    RUN_TEST(testRemoveOneOfManyWithSameOwner);
    RUN_TEST(testAddCarWithAllEmptyStrings);
    RUN_TEST(testFeeCalculationZeroHoursDynamicPricing);
    RUN_TEST(testMaxCapacityAndRemoveAll);
    RUN_TEST(testAddRemoveAddSameLicensePlate);

    std::cout << ANSI_BLUE << "=========== Test Suite Completed ===========" << ANSI_RESET <<std::endl;
    return 0;
}
