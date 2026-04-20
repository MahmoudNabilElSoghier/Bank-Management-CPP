
### لـ Bank Management System:

# Bank Management System – C++

A comprehensive bank management console application built with C++ (structured programming). It handles client accounts, financial transactions, and user permissions.

## ✨ Features

### 👥 Client Management
- List all clients
- Add new client
- Delete client
- Update client info
- Find client by account number

### 💰 Transactions
- Deposit
- Withdraw
- Transfer between accounts
- View total bank balances

### 👤 User Management & Access Control
- Login system
- Granular permissions (view, add, delete, update, find, transactions, manage users)
- Admin user with full access

## 🔗 Integration with ATM Client

This system manages the central database files:
- `clientData.txt` – client accounts
- `userData.txt` – employee credentials

The [ATM Client System](https://github.com/MahmoudNabilElSoghier/ATM-Client-CPP) uses the same `clientData.txt` to provide customer-facing ATM operations.

## 📁 Project Structure
```text
📦 Bank-Management-CPP
┣ 📜 BankSystem.cpp
┣ 📜 clientData.txt
┣ 📜 userData.txt
┗ 📜 README.md
```
## 🚀 How to Run
```bash
g++ -std=c++11 BankSystem.cpp -o bank
./bank
```
## 🔐 Default Login
- Username: `admin`
- Password: `1234`

## 🛠️ Technologies & Concepts
- C++17
- File I/O (`fstream`)
- Vectors & Structs
- Permissions system (flags)
- Full CRUD operations

## 👤 Author
#### Mahmoud Nabil El-Soghier
[GitHub](https://github.com/MahmoudNabilElSoghier)