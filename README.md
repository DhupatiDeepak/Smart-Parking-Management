Here’s your **complete README.md** in a single copy-paste friendly block:

```markdown
# 🚗 Parking Management System

A **comprehensive**, **feature-rich**, and **modular** C++ console application for managing a parking lot — built with robust data handling, dynamic pricing, billing, CSV persistence, and an extensive automated unit testing suite.  

This system is designed to simulate **real-world parking management** operations while ensuring maintainability, scalability, and ease of testing.

---

## ✨ Features

- **Interactive Car Parking**  
  ➤ Park vehicles with full owner and vehicle details.  
  ➤ Support for **reserved slots** and **multiple slot sizes**.

- **Dynamic Pricing & Discounts**  
  ➤ Adjustable hourly rates per vehicle.  
  ➤ 30% discount applied for stays exceeding 5 hours.  
  ➤ GST calculation at 18% for dynamic pricing.

- **Detailed Billing System**  
  ➤ Auto-generated bills with parking duration, gross, discount, GST, and total.  
  ➤ Bills saved to `bill_history.txt` for permanent record.

- **Data Persistence**  
  ➤ Vehicle entry data stored in `cars_data.csv`.  
  ➤ Session logs maintained in `session_log.txt`.

- **Unit Testing Framework**  
  ➤ Over **50 automated test cases** covering:
    - Parking & removal
    - Fee calculation
    - Edge cases (0 hours, future times, long durations, fractional hours, etc.)
    - Data validation and integrity
    - Capacity management
    - Special conditions (reserved slots, duplicates, long names, special characters, etc.)

- **User-Friendly Console Interface**  
  ➤ ANSI colors for better readability.  
  ➤ Clear menu-based navigation.

---

## 🛠️ Technical Details

- **Language:** C++11  
- **Build System:** CMake (v3.10+)  
- **Testing:** Custom unit test suite using `assert` macros with color-coded results  
- **Data Storage:** CSV & text files for persistence  
- **UI:** Console-based with ANSI color support

---

## 📁 Project Structure
```

parking\_management/
├── src/
│   ├── car.h                  # Car class definition
│   ├── car.cpp                # Car class implementation
│   ├── parking\_lot.h          # ParkingLot class definition
│   ├── parking\_lot.cpp        # ParkingLot class implementation
│   ├── parking\_lot\_test.cpp   # Comprehensive unit tests
│   ├── main.cpp               # Application entry point
│   ├── bill\_history.txt       # Generated bills history
│   ├── cars\_data.csv          # Saved car data
│   └── session\_log.txt        # Runtime logs
├── CMakeLists.txt             # Build configuration
└── README.md                  # Documentation

````

---

## 🚀 Building and Running

### **Prerequisites**
- CMake ≥ 3.10
- C++11 compatible compiler (GCC, Clang, MSVC)
- Make (on Linux/macOS)

### **Build Steps**
1. Clone the repository:
```bash
git clone https://github.com/yourusername/parking_management.git
cd parking_management
````

2. Create a build directory and compile:

```bash
mkdir build && cd build
cmake ..
make
```

3. Run the main application:

```bash
./bin/parking-system
```

4. Run the test suite:

```bash
./bin/parking-test
```

---

## 💡 Usage

On running `parking-system`, you will see:

```
========= MAIN MENU =========
1. Park Car
2. Remove Car
3. Display Parked Cars
4. Exit
=============================
Enter choice:
```

**Options:**

* **Park Car** → Enter car and owner details.
* **Remove Car** → Calculate bill and remove a car by ID and owner name.
* **Display Parked Cars** → List all currently parked vehicles.
* **Exit** → Quit application.

---

## 🧪 Testing

Run `parking-test` to execute all automated tests.
Tests cover:

* Normal operations
* Edge cases
* Invalid data
* Pricing logic
* Capacity boundaries

Example output:

```
[PASS] testParkCarAndCount
[PASS] testRemoveCarByIdAndOwner
[PASS] testCalculateFee
...
```

---

## 📝 Logging

* `session_log.txt` → Runtime events
* `cars_data.csv` → All active vehicle records
* `bill_history.txt` → Full bill history

---

## 🔒 Limitations

* Single parking lot (no multi-lot support)
* Max capacity: **100 vehicles**
* No graphical UI (console-based only)

---

## 📜 License

This project is licensed under the **MIT License** – see the LICENSE file for details.

```

I also noticed your **test suite is huge and very detailed**, so if you want, I can add a **"Testing Highlights" section** in the README showing some key test scenarios.  
Do you want me to add that?
```
