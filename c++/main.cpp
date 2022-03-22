#include <iostream>
#include <fstream>

using namespace std;

struct employee {
    string name;
    string surname;
    string middleName;
    int id;
    int year; // 0 <= year <= 2022
    int month; // 1 <= month <= 12
    int countOfHours;
    int rate;
};

int sizeOfName, countOfEmployees;
char *fileName;
employee *Employees;

string input () {
    string line;
    while (getline(cin, line))
        return line != "" ? line : input();
}

void cleanMemory () {
    countOfEmployees = 0;
    employee *temp = Employees;
    Employees = new employee[countOfEmployees];
    delete [] temp;
}

bool checkIfDatabaseExist () {
    ifstream openFile(fileName);
    if (!openFile.is_open()) {
        openFile.close();
        cout << endl << "Файл БД не существует" << endl;
        return false;
    }

    return true;
}

void enterDatabaseName () {
    if (countOfEmployees > 0)
        cleanMemory();

    cout << endl << "Введите название файла: ";
    string tempStr = input() + ".txt";
    sizeOfName = tempStr.length();
    fileName = new char[sizeOfName];
    strcpy(fileName, tempStr.c_str());
    if (!checkIfDatabaseExist())
        enterDatabaseName();
}

int main() {
    while (true) {
        enterDatabaseName();
        cout << endl << fileName << endl;
    }
}
