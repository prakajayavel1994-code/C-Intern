#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

class Book {
private:
    int bookID;
    char title[50];
    char author[50];
    bool issued;

public:
    void addBook() {
        cout << "Enter Book ID: ";
        cin >> bookID;
        cin.ignore();

        cout << "Enter Title: ";
        cin.getline(title, 50);

        cout << "Enter Author: ";
        cin.getline(author, 50);

        issued = false;
    }

    void displayBook() const {
        cout << left << setw(10) << bookID
             << setw(25) << title
             << setw(20) << author
             << setw(10) << (issued ? "Issued" : "Available") << endl;
    }

    int getID() const { return bookID; }
    const char* getTitle() const { return title; }
    const char* getAuthor() const { return author; }

    bool isIssued() const { return issued; }
    void issueBook() { issued = true; }
    void returnBook() { issued = false; }
};

// Add Book
void addBook() {
    Book b;
    ofstream file("library.dat", ios::binary | ios::app);

    b.addBook();
    file.write(reinterpret_cast<char*>(&b), sizeof(b));

    file.close();
    cout << "Book added successfully!\n";
}

// Display all books
void displayBooks() {
    Book b;
    ifstream file("library.dat", ios::binary);

    if (!file) {
        cout << "No records found.\n";
        return;
    }

    cout << left << setw(10) << "ID"
         << setw(25) << "Title"
         << setw(20) << "Author"
         << setw(10) << "Status" << endl;

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        b.displayBook();
    }

    file.close();
}

// Search by Title
void searchByTitle() {
    Book b;
    char search[50];
    bool found = false;

    cout << "Enter Title to search: ";
    cin.ignore();
    cin.getline(search, 50);

    ifstream file("library.dat", ios::binary);

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (strstr(b.getTitle(), search)) {
            b.displayBook();
            found = true;
        }
    }

    if (!found)
        cout << "No book found.\n";

    file.close();
}

// Search by Author
void searchByAuthor() {
    Book b;
    char search[50];
    bool found = false;

    cout << "Enter Author to search: ";
    cin.ignore();
    cin.getline(search, 50);

    ifstream file("library.dat", ios::binary);

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (strstr(b.getAuthor(), search)) {
            b.displayBook();
            found = true;
        }
    }

    if (!found)
        cout << "No book found.\n";

    file.close();
}

// Issue Book
void issueBook(int id) {
    fstream file("library.dat", ios::binary | ios::in | ios::out);
    Book b;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.getID() == id) {
            if (!b.isIssued()) {
                b.issueBook();
                file.seekp(-static_cast<int>(sizeof(b)), ios::cur);
                file.write(reinterpret_cast<char*>(&b), sizeof(b));
                cout << "Book issued successfully!\n";
            } else {
                cout << "Book already issued.\n";
            }
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Book not found.\n";

    file.close();
}

// Return Book
void returnBook(int id) {
    fstream file("library.dat", ios::binary | ios::in | ios::out);
    Book b;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.getID() == id) {
            if (b.isIssued()) {
                b.returnBook();
                file.seekp(-static_cast<int>(sizeof(b)), ios::cur);
                file.write(reinterpret_cast<char*>(&b), sizeof(b));
                cout << "Book returned successfully!\n";
            } else {
                cout << "Book was not issued.\n";
            }
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Book not found.\n";

    file.close();
}

// Menu
void menu() {
    int choice, id;

    do {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Search by Title\n";
        cout << "4. Search by Author\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                searchByTitle();
                break;
            case 4:
                searchByAuthor();
                break;
            case 5:
                cout << "Enter Book ID: ";
                cin >> id;
                issueBook(id);
                break;
            case 6:
                cout << "Enter Book ID: ";
                cin >> id;
                returnBook(id);
                break;
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 7);
}

int main() {
    menu();
    return 0;
}