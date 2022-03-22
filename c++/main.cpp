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

employee *addEmployee () {
    employee *newArray = new employee[countOfEmployees];
    for (int i = 0; i < countOfEmployees - 1; i++)
        newArray[i] = Employees[i];

    swap(newArray, Employees);
    delete [] newArray;
    return Employees;
}

void readDatabase () {
    ifstream openFile(fileName);
    char line[500];
    Employees = new employee[countOfEmployees];
    cout << endl << "Чтение БД в оперативную память" << endl;
    while (!openFile.eof()) {
        openFile.getline(line, 500);
        int count = 0;
        countOfEmployees++;
        Employees = addEmployee();
        string temp;
        for (int i = 0; i < 500 || line[i] != '\0'; i++) {
            if (line[i] == '|' || line[i] == ';') {
                switch (count++) {
                    case 0:
                        Employees[countOfEmployees - 1].name = temp;
                        break;
                    case 1:
                        Employees[countOfEmployees - 1].surname = temp;
                        break;
                    case 2:
                        Employees[countOfEmployees - 1].middleName = temp;
                        break;
                    case 3:
                        Employees[countOfEmployees - 1].id = stoi(id);
                        break;
                    case 4:
                        Employees[countOfEmployees - 1].year = stoi(temp);
                        break;
                    case 5:
                        Employees[countOfEmployees - 1].month = stoi(temp);
                        break;
                    case 6:
                        Employees[countOfEmployees - 1].countOfHours = stoi(temp);
                        break;
                    case 7:
                        Employees[countOfEmployees - 1].rate = stoi(temp);
                }

                temp.clear();
                continue;
            }

            temp += line[i];
        }
    }

    openFile.close();
    cout << endl << "БД успешно открыта в памяти компьютера" << endl;
}

void openDatabase () {
    enterDatabaseName();
    readDatabase();
}

int main() {
    openDatabase();

    while (true) {
        int menu;
        cout << endl << "Меню работы с БД \"" << fileName << "\"" << endl;
        cout 
        cout << "1 - Режим редактирования" << endl;
        cout << "2 - Режим обработки данных" << endl;
        cout << "0 - Завершение работы" << endl;
        cout << "Ваш выбор: ";
        cin >> menu;
        switch (menu) {
            case 1:
                editMode();
                break;
            case 2:
        }
    }
}
