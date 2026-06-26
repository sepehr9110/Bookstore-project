#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

class Book {
private:
    string title;
    string author;
    string genre;
    string country;
    bool availableForLoan;
    string borrowerName;
    string borrowerPhone;

public:
    Book() {
        title = "";
        author = "";
        genre = "";
        country = "";
        availableForLoan = true;
        borrowerName = "";
        borrowerPhone = "";
    }

    Book(string t, string a, string g, string c, bool available, string bName = "", string bPhone = "") {
        title = t;
        author = a;
        genre = g;
        country = c;
        availableForLoan = available;
        borrowerName = bName;
        borrowerPhone = bPhone;
    }

    string getTitle() const {
        return title;
    }

    bool isAvailable() const {
        return availableForLoan;
    }

    void setAvailable(bool status) {
        availableForLoan = status;
    }

    void setBorrowerInfo(string name, string phone) {
        borrowerName = name;
        borrowerPhone = phone;
    }

    void clearBorrowerInfo() {
        borrowerName = "";
        borrowerPhone = "";
    }

    string toFileString() const {
        return title + "|" + author + "|" + genre + "|" + country + "|" +
               (availableForLoan ? "1" : "0") + "|" + borrowerName + "|" + borrowerPhone;
    }

    static Book fromFileString(string line) {
        stringstream ss(line);

        string title;
        string author;
        string genre;
        string country;
        string availableText;
        string borrowerName;
        string borrowerPhone;

        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, genre, '|');
        getline(ss, country, '|');
        getline(ss, availableText, '|');
        getline(ss, borrowerName, '|');
        getline(ss, borrowerPhone, '|');

        bool available = (availableText == "1");

        return Book(title, author, genre, country, available, borrowerName, borrowerPhone);
    }

    friend void showBookInfo(const Book& book);
};

void showBookInfo(const Book& book) {
    cout << "Title: " << book.title << endl;
    cout << "Author: " << book.author << endl;
    cout << "Genre: " << book.genre << endl;
    cout << "Country: " << book.country << endl;

    if (book.availableForLoan) {
        cout << "Loan status: Available" << endl;
    } else {
        cout << "Loan status: Not available" << endl;
        cout << "Borrower name: " << book.borrowerName << endl;
        cout << "Borrower phone: " << book.borrowerPhone << endl;
    }

    cout << "-----------------------------" << endl;
}

class Person {
protected:
    string name;

public:
    Person() {
        name = "";
    }

    Person(string n) {
        name = n;
    }

    virtual void showRole() const {
        cout << "This is a person." << endl;
    }

    virtual ~Person() {
    }
};

class Member : public Person {
private:
    int memberId;

public:
    Member() : Person() {
        memberId = 0;
    }

    Member(string n, int id) : Person(n) {
        memberId = id;
    }

    void showRole() const override {
        cout << "Member name: " << name << endl;
        cout << "Member ID: " << memberId << endl;
    }
};

void showPersonRole(const Person& person) {
    person.showRole();
}

class LibrarySystem {
private:
    vector<Book> books;
    string fileName;
    Member currentMember;

public:
    LibrarySystem() {
        fileName = "books.txt";

        string userName;

        cout << "Enter your name: ";
        getline(cin, userName);

        if (userName == "") {
            userName = "Sepehr";
        }

        currentMember = Member(userName, 1001);

        cout << endl;
        cout << "Welcome to Sepehr's Book Store'" << endl;
        showPersonRole(currentMember);
        cout << endl;

        loadBooksFromFile();
    }

    ~LibrarySystem() {
        saveBooksToFile();
    }

    void addBook() {
        string title;
        string author;
        string genre;
        string country;
        int availableChoice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter book title: ";
        getline(cin, title);

        cout << "Enter author name: ";
        getline(cin, author);

        cout << "Enter genre: ";
        getline(cin, genre);

        cout << "Enter country: ";
        getline(cin, country);

        cout << "Is book available for loan? (1 = Yes, 0 = No): ";
        cin >> availableChoice;

        Book newBook(title, author, genre, country, availableChoice == 1);
        books.push_back(newBook);

        cout << "Book added successfully." << endl;
    }

    void showAllBooks() const {
        if (books.empty()) {
            cout << "No books found." << endl;
            return;
        }

        for (int i = 0; i < books.size(); i++) {
            cout << "Book number: " << i + 1 << endl;
            showBookInfo(books[i]);
        }
    }

    void searchBook() const {
        if (books.empty()) {
            cout << "No books found." << endl;
            return;
        }

        string title;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter book title to search: ";
        getline(cin, title);

        for (int i = 0; i < books.size(); i++) {
            if (books[i].getTitle() == title) {
                cout << "Book found:" << endl;
                showBookInfo(books[i]);
                return;
            }
        }

        cout << "Book not found." << endl;
    }

    void loanBook() {
        if (books.empty()) {
            cout << "No books found." << endl;
            return;
        }

        string title;
        string borrowerName;
        string borrowerPhone;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter book title for loan: ";
        getline(cin, title);

        for (int i = 0; i < books.size(); i++) {
            if (books[i].getTitle() == title) {
                if (books[i].isAvailable()) {
                    cout << "Enter borrower name: ";
                    getline(cin, borrowerName);

                    cout << "Enter borrower phone: ";
                    getline(cin, borrowerPhone);

                    books[i].setAvailable(false);
                    books[i].setBorrowerInfo(borrowerName, borrowerPhone);

                    cout << "Book loaned successfully." << endl;
                } else {
                    cout << "This book is not available for loan." << endl;
                }

                return;
            }
        }

        cout << "Book not found." << endl;
    }

    void returnBook() {
        if (books.empty()) {
            cout << "No books found." << endl;
            return;
        }

        string title;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter returned book title: ";
        getline(cin, title);

        for (int i = 0; i < books.size(); i++) {
            if (books[i].getTitle() == title) {
                books[i].setAvailable(true);
                books[i].clearBorrowerInfo();

                cout << "Book returned successfully." << endl;
                return;
            }
        }

        cout << "Book not found." << endl;
    }

    void saveBooksToFile() const {
        ofstream file(fileName.c_str());

        for (int i = 0; i < books.size(); i++) {
            file << books[i].toFileString() << endl;
        }

        file.close();
    }

    void loadBooksFromFile() {
        ifstream file(fileName.c_str());

        if (!file) {
            return;
        }

        string line;

        while (getline(file, line)) {
            if (line != "") {
                Book book = Book::fromFileString(line);
                books.push_back(book);
            }
        }

        file.close();
    }
};

int main() {
    LibrarySystem system;
    int choice;

    do {
        cout << endl;
        cout << "===== Library Menu =====" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Show All Books" << endl;
        cout << "3. Search Book" << endl;
        cout << "4. Loan Book" << endl;
        cout << "5. Return Book" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            system.addBook();
            break;

        case 2:
            system.showAllBooks();
            break;

        case 3:
            system.searchBook();
            break;

        case 4:
            system.loanBook();
            break;

        case 5:
            system.returnBook();
            break;

        case 0:
            cout << "Program finished." << endl;
            break;

        default:
            cout << "Invalid choice." << endl;
        }

    } while (choice != 0);

    return 0;
}
