#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class TradingAccount {
public:
    int accountId;
    string name;
    string accountType; // e.g., "Savings", "Checking"
    float balance;

    void displayAccountDetails() const {
        cout << "Account ID: " << accountId << "\nName: " << name << "\nAccount Type: " << accountType << "\nBalance: $" << fixed << setprecision(2) << balance << endl;
    }

    string toString() const {
        stringstream ss;
        ss << accountId << '\n' << name << '\n' << accountType << '\n' << balance;
        return ss.str();
    }

    static TradingAccount fromString(const string& str) {
        TradingAccount account;
        stringstream ss(str);
        ss >> account.accountId;
        ss.ignore(); // Ignore newline after accountId
        getline(ss, account.name);
        getline(ss, account.accountType);
        ss >> account.balance;
        return account;
    }
};

class TradingManagementSystem {
private:
    vector<TradingAccount> accounts;
    const string filename = "tradecpp.txt";

public:
    void addAccount() {
        TradingAccount newAccount;
        cout << "Enter Account ID: ";
        cin >> newAccount.accountId;
        cin.ignore();  // Clear newline from buffer
        cout << "Enter Name: ";
        getline(cin, newAccount.name);
        cout << "Enter Account Type: ";
        getline(cin, newAccount.accountType);
        cout << "Enter Balance: ";
        cin >> newAccount.balance;

        accounts.push_back(newAccount);
        saveDataToFile();
        cout << "Account added successfully!" << endl;
    }

    void displayAllAccounts() const {
        if (accounts.empty()) {
            cout << "No accounts available in the system." << endl;
            return;
        }

        cout << "Account List:\n";
        for (const TradingAccount& account : accounts) {
            account.displayAccountDetails();
            cout << "------------------------\n";
        }
    }

    void modifyAccount(int accountId) {
        for (TradingAccount& account : accounts) {
            if (account.accountId == accountId) {
                cout << "Modify Account ID " << accountId << ":\n";
                cout << "Enter New Name: ";
                cin.ignore();  // Clear newline from buffer
                getline(cin, account.name);
                cout << "Enter New Account Type: ";
                getline(cin, account.accountType);
                cout << "Enter New Balance: ";
                cin >> account.balance;

                saveDataToFile();
                cout << "Account modified successfully!" << endl;
                return;
            }
        }

        cout << "Account with ID " << accountId << " not found." << endl;
    }

    void deleteAccount(int accountId) {
        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            if (it->accountId == accountId) {
                accounts.erase(it);
                saveDataToFile();
                cout << "Account deleted successfully!" << endl;
                return;
            }
        }

        cout << "Account with ID " << accountId << " not found." << endl;
    }

    void loadDataFromFile() {
        ifstream inFile(filename);
        if (!inFile) {
            cout << "No existing data file found. Starting with an empty system." << endl;
            return;
        }

        string line;
        stringstream ss;
        string accountData;

        while (getline(inFile, line)) {
            if (line.empty()) continue; // Skip empty lines
            ss << line << '\n';
            if (getline(inFile, line)) {
                ss << line << '\n';
                if (getline(inFile, line)) {
                    ss << line << '\n';
                    if (getline(inFile, line)) {
                        ss << line << '\n';
                        accountData = ss.str();
                        accounts.push_back(TradingAccount::fromString(accountData));
                        ss.str(""); // Clear stringstream
                        ss.clear(); // Clear any error flags
                    }
                }
            }
        }

        inFile.close();
    }

    void saveDataToFile() const {
        ofstream outFile(filename);
        for (const TradingAccount& account : accounts) {
            outFile << account.toString() << '\n' << "----" << '\n'; // Separator
        }
        outFile.close();
    }

    ~TradingManagementSystem() {
        saveDataToFile();
    }

    // Add more functionalities like search, transaction, etc., based on your requirements
};

int main() {
    TradingManagementSystem tradingSystem;
    tradingSystem.loadDataFromFile();

    char choice;
    do {
        cout << "Menu:\n1. Add Account\n2. Display All Accounts\n3. Modify Account\n4. Delete Account\n5. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case '1':
            tradingSystem.addAccount();
            break;
        case '2':
            tradingSystem.displayAllAccounts();
            break;
        case '3': {
            int accountId;
            cout << "Enter Account ID to modify: ";
            cin >> accountId;
            tradingSystem.modifyAccount(accountId);
            break;
        }
        case '4': {
            int accountId;
            cout << "Enter Account ID to delete: ";
            cin >> accountId;
            tradingSystem.deleteAccount(accountId);
            break;
        }
        case '5':
            cout << "Exiting the Trading Management System. Thank you!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '5');

    return 0;
}
