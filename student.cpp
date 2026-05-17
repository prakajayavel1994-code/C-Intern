#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

class Student {
private:
    int id;
    char name[50];
    int age;
    char course[50];

public:
    void input() {
        cout << "Enter ID: ";
        cin >> id;
        cin.ignore();

        cout << "Enter Name: ";
        cin.getline(name, 50);

        cout << "Enter Age: ";
        cin >> age;
        cin.ignore();

        cout << "Enter Course: ";
        cin.getline(course, 50);
    }

    void display() const {
        cout << left << setw(10) << id
             << setw(20) << name
             << setw(10) << age
             << setw(20) << course << endl;
    }

    int getId() const {
        return id;
    }
};

// Function to add student
void addStudent() {
    Student s;
    ofstream file("students.dat", ios::binary | ios::app);

    s.input();
    file.write(reinterpret_cast<char*>(&s), sizeof(s));

    file.close();
    cout << "Student added successfully!\n";
}

// Function to display all students
void displayStudents() {
    Student s;
    ifstream file("students.dat", ios::binary);

    if (!file) {
        cout << "No records found.\n";
        return;
    }

    cout << left << setw(10) << "ID"
         << setw(20) << "Name"
         << setw(10) << "Age"
         << setw(20) << "Course" << endl;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        s.display();
    }

    file.close();
}

// Function to search student
void searchStudent(int id) {
    Student s;
    ifstream file("students.dat", ios::binary);
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.getId() == id) {
            cout << "Record Found:\n";
            s.display();
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Student not found.\n";

    file.close();
}

// Function to update student
void updateStudent(int id) {
    fstream file("students.dat", ios::binary | ios::in | ios::out);
    Student s;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.getId() == id) {
            cout << "Enter new details:\n";
            s.input();

            file.seekp(-static_cast<int>(sizeof(s)), ios::cur);
            file.write(reinterpret_cast<char*>(&s), sizeof(s));

            cout << "Record updated successfully!\n";
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Student not found.\n";

    file.close();
}

// Function to delete student
void deleteStudent(int id) {
    ifstream inFile("students.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);

    Student s;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.getId() != id) {
            outFile.write(reinterpret_cast<char*>(&s), sizeof(s));
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
        cout << "Record deleted successfully!\n";
    else
        cout << "Student not found.\n";
}

// Menu
void menu() {
    int choice, id;

    do {
        cout << "\n===== Student Management System =====\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                cout << "Enter ID to search: ";
                cin >> id;
                searchStudent(id);
                break;
            case 4:
                cout << "Enter ID to update: ";
                cin >> id;
                updateStudent(id);
                break;
            case 5:
                cout << "Enter ID to delete: ";
                cin >> id;
                deleteStudent(id);
                break;
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 6);
}

int main() {
    menu();
    return 0;
}