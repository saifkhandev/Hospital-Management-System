# 🏥 Hospital Management System

A comprehensive **Hospital Management System** developed in **C language** that simulates the daily operations of a hospital through a menu-driven console application. The project focuses on efficient management of patients, doctors, appointments, hospital beds, medicines, and billing while using file handling to permanently store data.

This project was developed to strengthen concepts of **C programming**, including structures, arrays, functions, file handling, input validation, loops, conditional statements, and modular programming.

---

## ✨ Features

### 👨‍⚕️ Manager Module
- Secure manager login
- Patient management
- Doctor management
- Appointment management
- Bed management
- Medicine inventory management
- Billing system

### 🧑 Patient Module
- Patient registration
- Patient login
- View personal profile
- Schedule appointments
- View appointments
- Cancel appointments
- View bills
- Pay bills
- Check bed availability
- View doctor directory

---

## 📋 Patient Management

- Add new patients
- View all patients
- Search patient by ID
- Delete patient
- Store patient details permanently using file handling

Information stored includes:

- Name
- Age
- Gender
- Phone Number
- Email
- Address
- Medical History
- Blood Group
- Registration Date
- Password

---

## 👨‍⚕️ Doctor Management

- Add doctors
- View doctor list
- Search doctors by specialization
- Delete doctors

Doctor information includes:

- Doctor ID
- Name
- Specialization
- Experience
- Consultation Fee
- Contact Information

---

## 📅 Appointment Management

- Schedule appointments
- Update appointment status
- View appointments
- Cancel appointments

Appointment information includes:

- Appointment ID
- Patient ID
- Doctor ID
- Date
- Time
- Reason
- Status

---

## 🛏️ Bed Management

- Add hospital beds
- View bed availability
- Admit patients
- Discharge patients
- Track occupied and available beds

Supported bed types:

- Standard
- Deluxe
- ICU

---

## 💊 Medicine Inventory

- Add medicines
- View medicine list
- Search medicines
- Edit medicine details
- Delete medicines
- Low stock detection

Medicine information includes:

- Medicine Name
- Price
- Quantity
- Expiry Date
- Manufacturer

---

## 💳 Billing System

- Generate hospital bills
- View bills
- Process bill payments
- Calculate outstanding balance
- Track payment status

Billing includes:

- Consultation Charges
- Medicine Charges
- Bed Charges
- Total Amount
- Paid Amount
- Outstanding Amount

---

## ✅ Input Validation

The system validates user input including:

- Alphabet-only names
- Numeric values
- Phone numbers
- Gmail email addresses
- Dates
- Time format
- Blood groups
- Doctor specializations
- Bed types

This reduces invalid data entry and improves reliability.

---

## 💾 Data Storage

The project uses **file handling** to permanently save records.

Data is stored in separate text files:

- patients.txt
- doctors.txt
- appointments.txt
- beds.txt
- medicines.txt
- bills.txt

Records are automatically loaded when the application starts and saved whenever changes are made.

---

## 🛠 Technologies Used

- C Programming Language
- Structures
- Arrays
- Functions
- File Handling
- Input Validation
- Console-Based User Interface

---

## 📂 Project Structure

```
hospital_management_system.c
patients.txt
doctors.txt
appointments.txt
beds.txt
medicines.txt
bills.txt
```

---

## 🚀 How to Run

### Using GCC

Compile the project:

```bash
gcc hospital_management_system.c -o hospital
```

Run the executable:

```bash
./hospital
```

Windows:

```bash
hospital.exe
```

---

## 📖 Learning Outcomes

This project helped in understanding:

- Structured Programming
- File Handling in C
- Data Validation
- Menu-Driven Applications
- Modular Programming
- Working with Structures
- Managing Large Console Projects

---

## Future Improvements

Possible future enhancements include:

- Database integration (MySQL)
- Graphical User Interface (GUI)
- Password encryption
- Role-based authentication
- Report generation
- Search filters
- Backup and restore system
- Online appointment scheduling

---

## Author

**Muhammad Saifullah Khan**

If you found this project useful, consider giving it a ⭐ on GitHub.
