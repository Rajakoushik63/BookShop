#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Book {
public:
    string title;
    string author;
    string publisher;
    float price;
    int stock;

    Book(string t, string a, string p, float pr, int s) : title(t), author(a), publisher(p), price(pr), stock(s) {}

    void display() const {  // Marking the function as const
        cout << "Title: " << title <<endl;
        cout<< "Author: " << author << endl;
        cout<<"Publisher: " << publisher<<endl;
        cout<< "Price: " << price <<endl;
        cout<< " Stock: " << stock << endl;
    }

    string serialize() const {  // Marking the function as const
        return title + ";" + author + ";" + publisher + ";" + to_string(price) + ";" + to_string(stock) + "\n";
    }

    static Book deserialize(const string& line) {
        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        size_t pos3 = line.find(';', pos2 + 1);
        size_t pos4 = line.find(';', pos3 + 1);

        string t = line.substr(0, pos1);
        string a = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string p = line.substr(pos2 + 1, pos3 - pos2 - 1);
        float pr = stof(line.substr(pos3 + 1, pos4 - pos3 - 1));
        int s = stoi(line.substr(pos4 + 1));

        return Book(t, a, p, pr, s);
    }
};

class Bookshop {
private:
    vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
        saveToFile();
    }

    void displayBooks() const {
        for (const auto& book : books) {
            book.display();
            cout<<"------------------------"<<endl;
        }
    }

    void searchBook(const string& title) const {
        for (const auto& book : books) {
            if (book.title == title) {
                book.display();
                return;
            }
        }
        cout << "Book not found." <<endl<<endl;
    }

    void updateBookDetails(const string& title) {
        for (auto& book : books) {
            if (book.title == title) {
                cout << "Enter new title (leave blank to keep current): ";
                string newTitle;
                getline(cin, newTitle);
                if (!newTitle.empty()) {
                    book.title = newTitle;
                }

                cout << "Enter new author (leave blank to keep current): ";
                string newAuthor;
                getline(cin, newAuthor);
                if (!newAuthor.empty()) {
                    book.author = newAuthor;
                }

                cout << "Enter new publisher (leave blank to keep current): ";
                string newPublisher;
                getline(cin, newPublisher);
                if (!newPublisher.empty()) {
                    book.publisher = newPublisher;
                }

                cout << "Enter new price (enter -1 to keep current): ";
                float newPrice;
                cin >> newPrice;
                if (newPrice != -1) {
                    book.price = newPrice;
                }

                cout << "Enter new stock (enter -1 to keep current): ";
                int newStock;
                cin >> newStock;
                if (newStock != -1) {
                    book.stock = newStock;
                }

                saveToFile();
                cout << "Book details updated successfully." << endl<<endl;
                return;
            }
        }
        cout << "Book not found." << endl<<endl;
    }

    void deleteBook(const string& title) {
        auto it = remove_if(books.begin(), books.end(), [&](const Book& book) {
            return book.title == title;
        });

        if (it != books.end()) {
            books.erase(it, books.end());
            saveToFile();
            cout << "Book deleted successfully." << endl;
        } else {
            cout << "Book not found." << endl<<endl;
        }
    }

    void sortBooksByTitle() {
        sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
            return a.title < b.title;
        });
        cout << "Books sorted by title." << endl<<endl;
    }

    void generateSalesReport() const {

        float totalRevenue = 0;
        for (const auto& book : books) {
            totalRevenue += (book.price * (100 - book.stock));  // Assuming initial stock was 100 for all books
        }
        cout << "Total Revenue: " << totalRevenue << endl<<endl;
    }

    void saveToFile() const {
        ofstream file("books.txt");
        for (const auto& book : books) {
            file << book.serialize();
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file("books.txt");
        string line;
        while (getline(file, line)) {
            books.push_back(Book::deserialize(line));
        }
        file.close();
    }
};

int main() {
    Bookshop shop;
    shop.loadFromFile();

    int choice;
    do {
        cout << "1. Add Book" << endl;
        cout << "2. Display Books" << endl;
        cout << "3. Search Book" << endl;
        cout << "4. Update Book Details" << endl;
        cout << "5. Delete Book" << endl;
        cout << "6. Sort Books by Title" << endl;
        cout << "7. Generate Sales Report" << endl;
        cout << "8. Exit" << endl<<endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string title, author, publisher;
            float price;
            int stock;

            cout << "Enter title: ";
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter publisher: ";
            getline(cin, publisher);
            cout << "Enter price: ";
            cin >> price;
            cout << "Enter stock: ";
            cin >> stock;

            Book book(title, author, publisher, price, stock);
            shop.addBook(book);
        } else if (choice == 2) {
            shop.displayBooks();
        } else if (choice == 3) {
            string title;
            cout << "Enter title to search: ";
            getline(cin, title);
            shop.searchBook(title);
        } else if (choice == 4) {
            string title;
            cout << "Enter title of the book to update: ";
            getline(cin, title);
            shop.updateBookDetails(title);
        } else if (choice == 5) {
            string title;
            cout << "Enter title of the book to delete: ";
            getline(cin, title);
            shop.deleteBook(title);
        } else if (choice == 6) {
            shop.sortBooksByTitle();
        } else if (choice == 7) {
            shop.generateSalesReport();
        }
    } while (choice != 8);

    return 0;
}
