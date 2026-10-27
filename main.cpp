
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

// Base class
class Person {
protected:
    int id;
    string name;

public:
    Person(int id = 0, string name = "")
        : id(id), name(name) {}

    virtual void display() const {
        cout << "ID: " << id << '\n';
        cout << "Name: " << name << '\n';
    }

    virtual ~Person() = default;
};

// Derived class
class Employee : public Person {
private:
    string department;
    double salary;

public:
    Employee(int id = 0, string name = "",
             string department = "", double salary = 0.0)
        : Person(id, name),
          department(department), salary(salary) {}

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getDepartment() const {
        return department;
    }

    double getSalary() const {
        return salary;
    }

    void updateDetails(string newName,
                       string newDepartment,
                       double newSalary) {
        name = newName;
        department = newDepartment;
        salary = newSalary;
    }

    double annualSalary() const {
        return salary * 12;
    }

    void display() const override {
        cout << "\n----------------------------------------\n";
        cout << "Employee ID    : " << id << '\n';
        cout << "Name           : " << name << '\n';
        cout << "Department     : " << department << '\n';
        cout << fixed << setprecision(2);
        cout << "Monthly Salary : Rs. " << salary << '\n';
        cout << "Annual Salary  : Rs. "
             << annualSalary() << '\n';
        cout << "----------------------------------------\n";
    }
};

// Main management system class
class EmployeeManagementSystem {
private:
    vector<Employee> employees;
    const string filename = "employees.txt";

    string readText(const string& message) {
        string value;

        while (true) {
            cout << message;
            getline(cin, value);

            if (value.find_first_not_of(" \t") != string::npos)
                return value;

            cout << "Input cannot be empty. Try again.\n";
        }
    }

    int readInt(const string& message) const {
        string line;
        int value;

        while (true) {
            cout << message;
            getline(cin, line);

            stringstream ss(line);
            char extra;

            if ((ss >> value) && !(ss >> extra))
                return value;

            cout << "Invalid input! Enter a whole number.\n";
        }
    }

    double readSalary(const string& message) {
        string line;
        double value;

        while (true) {
            cout << message;
            getline(cin, line);

            stringstream ss(line);
            char extra;

            if ((ss >> value) && !(ss >> extra)
                && value >= 0) {
                return value;
            }

            cout << "Enter a valid non-negative salary.\n";
        }
    }

    int findIndex(int searchId) const {
        for (int i = 0; i < static_cast<int>(employees.size()); i++) {
            if (employees[i].getId() == searchId)
                return i;
        }

        return -1;
    }

    void saveToFile() const {
        ofstream file(filename);

        if (!file) {
            cout << "Error: Could not save employee records!\n";
            return;
        }

        for (const Employee& e : employees) {
            file << e.getId() << ' '
                 << quoted(e.getName()) << ' '
                 << quoted(e.getDepartment()) << ' '
                 << setprecision(17) << e.getSalary() << '\n';
        }

        if (!file)
            cout << "Error while writing employee records!\n";
    }

    void loadFromFile() {
        ifstream file(filename);

        if (!file)
            return;

        int id;
        string name, department;
        double salary;

        while (file >> id
                    >> quoted(name)
                    >> quoted(department)
                    >> salary) {
            if (id > 0 && salary >= 0
                && findIndex(id) == -1) {
                employees.emplace_back(
                    id, name, department, salary
                );
            }
        }
    }

public:
    EmployeeManagementSystem() {
        loadFromFile();
    }

    void addEmployee() {
        cout << "\n========== ADD EMPLOYEE ==========\n";

        int id = readInt("Enter employee ID: ");

        if (id <= 0) {
            cout << "Employee ID must be positive!\n";
            return;
        }

        if (findIndex(id) != -1) {
            cout << "This employee ID already exists!\n";
            return;
        }

        string name = readText("Enter employee name: ");
        string department = readText("Enter department: ");
        double salary = readSalary("Enter monthly salary: ");

        employees.emplace_back(id, name, department, salary);
        saveToFile();

        cout << "Employee added successfully!\n";
    }

    void displayAll() const {
        cout << "\n========== ALL EMPLOYEES ==========\n";

        if (employees.empty()) {
            cout << "No employee records found.\n";
            return;
        }

        // Base-class pointer demonstrates runtime polymorphism.
        for (const Employee& e : employees) {
            const Person* person = &e;
            person->display();
        }

        cout << "\nTotal employees: " << employees.size() << '\n';
    }

    void searchEmployee() const {
        cout << "\n========== SEARCH EMPLOYEE ==========\n";

        int id = readInt("Enter employee ID: ");
        int index = findIndex(id);

        if (index == -1) {
            cout << "Employee not found!\n";
            return;
        }

        employees[index].display();
    }

    void updateEmployee() {
        cout << "\n========== UPDATE EMPLOYEE ==========\n";

        int id = readInt("Enter employee ID to update: ");
        int index = findIndex(id);

        if (index == -1) {
            cout << "Employee not found!\n";
            return;
        }

        string name = readText("Enter new name: ");
        string department = readText("Enter new department: ");
        double salary = readSalary("Enter new monthly salary: ");

        employees[index].updateDetails(name, department, salary);
        saveToFile();

        cout << "Employee details updated successfully!\n";
    }

    void deleteEmployee() {
        cout << "\n========== DELETE EMPLOYEE ==========\n";

        int id = readInt("Enter employee ID to delete: ");
        int index = findIndex(id);

        if (index == -1) {
            cout << "Employee not found!\n";
            return;
        }

        string confirm = readText(
            "Are you sure you want to delete this employee? (yes/no): "
        );

        if (confirm == "yes" || confirm == "YES"
            || confirm == "Yes") {
            employees.erase(employees.begin() + index);
            saveToFile();
            cout << "Employee deleted successfully!\n";
        } else {
            cout << "Deletion cancelled.\n";
        }
    }

    void run() {
        int choice;

        do {
            cout << "\n";
            cout << "========================================\n";
            cout << "      EMPLOYEE MANAGEMENT SYSTEM\n";
            cout << "========================================\n";
            cout << "1. Add Employee\n";
            cout << "2. Display All Employees\n";
            cout << "3. Search Employee\n";
            cout << "4. Update Employee\n";
            cout << "5. Delete Employee\n";
            cout << "6. Exit\n";
            cout << "========================================\n";

            choice = readInt("Enter your choice: ");

            switch (choice) {
                case 1:
                    addEmployee();
                    break;

                case 2:
                    displayAll();
                    break;

                case 3:
                    searchEmployee();
                    break;

                case 4:
                    updateEmployee();
                    break;

                case 5:
                    deleteEmployee();
                    break;

                case 6:
                    cout << "Thank you for using Employee Management System!\n";
                    break;

                default:
                    cout << "Invalid choice! Please select 1-6.\n";
            }

        } while (choice != 6);
    }
};

int main() {
    EmployeeManagementSystem system;
    system.run();

    return 0;
}
