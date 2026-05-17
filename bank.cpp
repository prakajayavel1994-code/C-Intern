#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

class Account {
private:
    int accNo;
    char name[50];
    double balance;

public:
    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accNo;
        cin.ignore();

        cout << "Enter Account Holder Name: ";
        cin.getline(name, 50);

        cout << "Enter Initial Balance: ";
        cin >> balance;
    }

    void showAccount() const {
        cout << "\nAccount No: " << accNo;
        cout << "\nName: " << name;
        cout << "\nBalance: " << balance << endl;
    }

    int getAccNo() const {
        return accNo;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
    }

    void withdraw(double amount) {
        if (amount <= balance)
            balance -= amount;
        else
            cout << "Insufficient Balance!\n";
    }
};

// Create Account
void createAccount() {
    Account acc;
    ofstream file("accounts.dat", ios::binary | ios::app);

    acc.createAccount();
    file.write(reinterpret_cast<char*>(&acc), sizeof(acc));

    file.close();
    cout << "Account created successfully!\n";
}

// Display Account
void displayAccount(int accNo) {
    Account acc;
    ifstream file("accounts.dat", ios::binary);
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.getAccNo() == accNo) {
            acc.showAccount();
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Account not found.\n";

    file.close();
}

// Deposit
void depositMoney(int accNo) {
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    Account acc;
    bool found = false;
    double amount;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.getAccNo() == accNo) {
            cout << "Enter amount to deposit: ";
            cin >> amount;

            acc.deposit(amount);

            file.seekp(-static_cast<int>(sizeof(acc)), ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(acc));

            cout << "Amount deposited successfully!\n";
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Account not found.\n";

    file.close();
}

// Withdraw
void withdrawMoney(int accNo) {
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    Account acc;
    bool found = false;
    double amount;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.getAccNo() == accNo) {
            cout << "Enter amount to withdraw: ";
            cin >> amount;

            acc.withdraw(amount);

            file.seekp(-static_cast<int>(sizeof(acc)), ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(acc));

            found = true;
            break;
        }
    }

    if (!found)
        cout << "Account not found.\n";

    file.close();
}

// Menu
void menu() {
    int choice, accNo;

    do {
        cout << "\n===== Bank Management System =====\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Balance Inquiry\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                cout << "Enter Account Number: ";
                cin >> accNo;
                depositMoney(accNo);
                break;
            case 3:
                cout << "Enter Account Number: ";
                cin >> accNo;
                withdrawMoney(accNo);
                break;
            case 4:
                cout << "Enter Account Number: ";
                cin >> accNo;
                displayAccount(accNo);
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 5);
}

int main() {
    menu();
    return 0;
}