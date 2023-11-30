/* Eli Berg, Project 1, CS121-03, 10/30/23 ----- Updated as of 11/30/23 For Project #2*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <map>


using namespace std;


struct FurnitureItem {
    string name;
    string color;
    double price;
};

struct ProductStatistics {
    int quantitySold;
    double totalRevenue;
    double totalTax;
};

const double TAX_RATE = 0.08;  // 8% tax rate


// Function to load furniture data from a text file
bool loadFurnitureData(const string& filename, FurnitureItem items[], int& itemCount) {
    ifstream file(filename);


    if (!file) {
        cerr << "Error: Could not open the furniture data file." << endl;
        return false;
    }


    itemCount = 0;
    while (itemCount < 100 && file >> items[itemCount].name >> items[itemCount].color >> items[itemCount].price) {
        itemCount++;
    }


    file.close();
    return true;
}


// Function to calculate the total price with taxes
double calculateTotalPrice(double itemPrice) {
    return itemPrice + itemPrice * TAX_RATE;
}


// Function to save the transaction to a text file
void saveTransaction(const string& filename, const FurnitureItem& item, double totalPrice) {
    ofstream file(filename, ios::app);


    if (file) {
        time_t now = time(0);
        tm* localTime = localtime(&now);


        file << "Date: " << localTime->tm_year + 1900 << "-" << localTime->tm_mon + 1 << "-"
             << localTime->tm_mday << " " << localTime->tm_hour << ":" << localTime->tm_min << ":"
             << localTime->tm_sec << endl;
        file << "Item: " << item.name << endl;
        file << "Color: " << item.color << endl;
        file << "Price: " << item.price << endl;
        file << "Total Price with Taxes: " << totalPrice << endl;
        file << string(30, '-') << endl;
        file.close();
    } else {
        cerr << "Error: Could not save the transaction." << endl;
    }
}

// Function to calculate and store product statistics
void updateProductStatistics(map<string, ProductStatistics>& productStats, const FurnitureItem& item, double totalPrice) {
    auto it = productStats.find(item.name);
    if (it != productStats.end()) {
        it->second.quantitySold++;
        it->second.totalRevenue += item.price;
        it->second.totalTax += totalPrice - item.price;
    } else {
        ProductStatistics stats = {1, item.price, totalPrice - item.price};
        productStats[item.name] = stats;
    }
}
int main() {
    const string furnitureDataFile = "furniture_data.txt";
    const string transactionHistoryFile = "transaction_history.txt";
    map<string, ProductStatistics> productStats;

    FurnitureItem furnitureItems[100];
    int itemCount = 0;


    if (!loadFurnitureData(furnitureDataFile, furnitureItems, itemCount)) {
        cout << "No furniture items found. Please check the data file." << endl;
        return 1;
    }


    while (true) {
        cout << "Available Furniture Items:\n";
        cout << setw(20) << "Item" << setw(15) << "Color" << setw(10) << "Price" << endl;
        cout << string(40, '-') << endl;


        for (int i = 0; i < itemCount; i++) {
            cout << i+1 << setw(20) << furnitureItems[i].name << setw(15) << furnitureItems[i].color << setw(10) 
            << fixed << setprecision(2) << furnitureItems[i].price << endl;
        }
        cout << string(36, '-') << endl;


        cout << "Select an item to purchase (Enter the item number) or enter 'q' to quit: ";
        string userInput;
        getline(cin, userInput);


        if (userInput == "q") {
            break;
        } 

        int itemNumber = stoi(userInput) - 1;
        
        if (itemNumber >= 0 && itemNumber < itemCount) {
            const FurnitureItem& selectedItem = furnitureItems[itemNumber];
            double totalPrice = calculateTotalPrice(selectedItem.price);
            cout << "You have selected: " << selectedItem.name << " " << selectedItem.color << endl;
            cout << "Total Price with Taxes: " << fixed << setprecision(2) << totalPrice << endl;


            // Save the transaction
            saveTransaction(transactionHistoryFile, selectedItem, totalPrice);

            //update product report
            updateProductStatistics(productStats, selectedItem, totalPrice);


            cout << "Transaction saved!" << endl;
        } else {
            cout << "Invalid item number. Please try again." << endl;
        }
        
        ofstream reportFile("product_report.txt");
    if (reportFile) {
        reportFile << setw(20) << "Product" << setw(15) << "Quantity Sold" << setw(15) << "Total Revenue" << setw(15) << "Total Tax" << endl;
        reportFile << string(65, '-') << endl;

        for (const auto& entry : productStats) {
            reportFile << setw(20) << entry.first << setw(15) << entry.second.quantitySold << setw(15) << fixed << setprecision(2)
                       << entry.second.totalRevenue << setw(15) << entry.second.totalTax << endl;
        }

        cout << "Product report written to product_report.txt." << endl;
    } else {
        cerr << "Error: Could not write the product report." << endl;
    }
    }


    cout << "Thank you for using the point of sale program." << endl;
    return 0;
}

