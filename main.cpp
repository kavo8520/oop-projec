#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class MenuItem {
protected:
    string name;
    double price;
    bool available;

public:
    MenuItem(const string& name, double price, bool available)
            : name(name), price(price), available(available) {}

    virtual ~MenuItem() {}

    string getName() const { return name; }
    double getPrice() const { return price; }
    bool isAvailable() const { return available; }

    void setAvailability(bool availability) { available = availability; }

    virtual void display() const {
        cout << name << " - $" << price
             << (available ? " (Available)" : " (Not Available)") << "\n";
    }
};

class FoodItem : public MenuItem {
public:
    FoodItem(const string& name, double price, bool available)
            : MenuItem(name, price, available) {}

    void display() const override {
        cout << "Food: " << getName() << " - $" << getPrice()
             << (isAvailable() ? " (Available)" : " (Not Available)") << "\n";
    }
};

class DrinkItem : public MenuItem {
public:
    DrinkItem(const string& name, double price, bool available)
            : MenuItem(name, price, available) {}

    void display() const override {
        cout << "Drink: " << getName() << " - $" << getPrice()
             << (isAvailable() ? " (Available)" : " (Not Available)") << "\n";
    }
};

class Menu {
private:
    vector<MenuItem*> items;
    static int totalItems;

    string toLower(const string& str) const {
        string lowerStr = str;
        transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

public:
    Menu() {}

    ~Menu() {
        for (auto item : items) {
            delete item;
        }
    }

    template<typename T>
    void addItem(const string& name, double price, bool available) {
        items.push_back(new T(name, price, available));
        totalItems++;
    }

    void displayMenu() const {
        cout << "Menu:\n";
        for (const auto& item : items) {
            item->display();
        }
    }

    bool isItemAvailable(const string& itemName) const {
        string lowerItemName = toLower(itemName);
        for (const auto& menuItem : items) {
            string menuItemName = menuItem->getName();
            string lowerMenuItemName = toLower(menuItemName);
            if (lowerMenuItemName == lowerItemName) {
                return menuItem->isAvailable();
            }
        }
        return false;
    }

    double getItemPrice(const string& itemName) const {
        string lowerItemName = toLower(itemName);
        for (const auto& menuItem : items) {
            string menuItemName = menuItem->getName();
            string lowerMenuItemName = toLower(menuItemName);
            if (lowerMenuItemName == lowerItemName) {
                return menuItem->getPrice();
            }
        }
        return 0.0; // Return 0 if item not found
    }

    static int getTotalItems() {
        return totalItems;
    }
};

int Menu::totalItems = 0;

class Order {
private:
    vector<MenuItem*> orderedItems;

public:
    Order() {}

    ~Order() {
        for (auto item : orderedItems) {
            delete item;
        }
    }

    Order& operator+=(MenuItem* item) {
        addItem(item);
        return *this;
    }

    void addItem(MenuItem* item) {
        orderedItems.push_back(item);
    }

    void displayOrder() const {
        cout << "Your Order:\n";
        double total = 0.0;
        for (const auto& item : orderedItems) {
            cout << item->getName() << " - $" << item->getPrice() << "\n";
            total += item->getPrice();
        }
        cout << "Total: $" << total << "\n";
    }
};

int main() {
    Menu menu;
    menu.addItem<FoodItem>("Pizza", 8.99, true);
    menu.addItem<FoodItem>("Burger", 5.99, true);
    menu.addItem<FoodItem>("Pasta", 7.99, false);
    menu.addItem<FoodItem>("Fries", 2.99, true);
    menu.addItem<DrinkItem>("Soda", 1.99, true);
    menu.addItem<DrinkItem>("Coffee", 2.49, true);

    menu.displayMenu();

    Order order;

    string choice;
    while (true) {

        cout << "What would you like to order? (type 'done' to finish, 'exit' to leave): ";
        cin >> choice;

        if (choice == "done") {
            break;
        } else if (choice == "exit") {
            cout << "Exiting the program.\n";
            return 0;
        }

        if (menu.isItemAvailable(choice)) {
            double itemPrice = menu.getItemPrice(choice);
            if (itemPrice != 0.0) {
                if (choice == "pizza" || choice == "burger" || choice == "pasta" || choice == "fries") {
                    order += new FoodItem(choice, itemPrice, true);
                } else if (choice == "soda" || choice == "coffee") {
                    order += new DrinkItem(choice, itemPrice, true);
                }
                cout << choice << " added to your order.\n";
            } else {
                cout << choice << " is not available.\n";
            }
        } else {
            cout << "Invalid choice. Please select an item from the menu.\n";
        }
    }

    order.displayOrder();

    return 0;
}
