Pediatric Clinic Management System

A console-based hospital management system developed in C for managing pediatric clinic operations efficiently through multiple integrated portals. The project focuses on digitalizing clinic workflows such as staff management, appointment handling, billing, and patient record management using core C programming concepts and file handling.

Features
Staff Management Portal
- Secure admin login system
- View all staff records
- Add new staff
- Search staff by name
- Edit staff information
- Delete staff records
- File-based data storage using CSV/TXT

Staff Portal
- Staff login authentication
- View personal profile information

Patient Portal
- Patient registration and login
- View patient profile
- View available doctors
- Book appointments
- Search appointments
- Reschedule appointments
- Cancel appointments
- View prescriptions
- View bed information
- View billing details

Billing & Payment Portal
- Generate patient bills
- View itemized billing information
- Process payments (cash/card)
- View payment history

Technologies Used
- Language: C
- Concepts:
  - Structures
  - Arrays
  - Functions
  - File Handling
  - String Manipulation
  - Pointers
  - Menu-driven programming

Libraries Used
- `stdio.h`
- `stdlib.h`
- `string.h`
- `stdbool.h`

File Handling
The project uses CSV and TXT files as lightweight databases for persistent storage.

Example files:
- `records.csv`
- `records.txt`
- `patientfile.txt`
- `appointments_file.txt`
- `bills_file.txt`
- `staff_login.txt`

Learning Outcomes
This project helped strengthen understanding of:
- Modular programming in C
- File I/O operations
- Input validation and buffer handling
- Struct-based data organization
- CRUD operations
- Real-world problem solving through programming

Limitations
- Hardcoded admin credentials
- Linear search used for database operations
- Case-sensitive searching
- No encryption for passwords
- Limited input validation
- Console-based interface only

Future Improvements
- Password encryption and stronger authentication
- Binary search with sorted records
- Graphical User Interface (GUI)
- Database integration (MySQL/SQLite)
- Better appointment validation
- Improved error handling and security

Team Members
| Team Member | Contribution |
| :--- | :--- |
| **Sadman Sakib** | Developed the Staff Management Portal including view, add, search, edit, and delete staff features using CSV/TXT file handling. Implemented helper functions for reading CSV data into arrays and automatic ID generation. Also contributed heavily to preparing and structuring the project report. |
| **Mahir Daiyan Hisham** | Developed the Staff Portal including staff login authentication and profile viewing functionality using file handling. |
| **Shormila Sawom** | Developed the complete Patient Portal including registration, login, appointment booking, searching, rescheduling, cancellation, prescription viewing, bed information, and billing details. Implemented doctor availability checking and appointment management systems. |
| **Zahra Mahbuba Rahman** | Developed the Billing and Payment Portal including bill generation, payment processing, payment history tracking, and billing record management. Also contributed to project planning and report writing. |

Project Information
Developed as a final project for:
Programming Language I Lab (CSE115.8)
Department of Electrical and Computer Engineering  
North South University

Submitted to:
- Anas Md. Ishfaqul
- Muktadir Osmani

Submission Date: May 11, 2026

License
This project was developed for educational purposes.
