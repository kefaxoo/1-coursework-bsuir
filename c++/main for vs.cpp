#include <iostream>
#include <fstream>
#include "Wstring.h"
#include "crypto.h"
#include <string>
#include <cstring>
#include <cmath>
#include <Windows.h>

#pragma warning (disable: 4996)

using namespace std;

struct user {
	string login;
	string hashPassword;
	bool role; // 1 - admin, 0 - user
	bool access; // 1 - admin allows access to the system 0 - admin doesn't allow access to the system
};

string loginStr;
bool role;

user *Users;
int countOfUsers;

struct employee {
	string name;
	string surname;
	string middleName;
	int ID;
	int year;
	int month;
	double countOfHours;
	double rate;
};

employee *Employees;
int countOfEmployees = 0, sizeOfName = 0;
char *fileName;

user *addUser();
void cleanMemoryUsers();
void openUserFile();
void createUserFile();
void toUserFile();
void reloadUserFile();

void outputUser(int, bool); 
void outputUsers();
void showAllUsers();
int getIndexOfUser();
void addUserInDatabase();
void editUser();
void deleteUser();
void userControlPanel();

employee *addEmployee();
void cleanMemoryData();
void enterDataFileName();
void openDataFile();
void createDataFile();
void toDataFile();
void reloadDataFile();

double getSalary();
void salary();
string getTemp(employee, int);
void linearSearch();
void bubbleSort();
void dataProcessing();

void outputData();
void viewData();
int getMaxID();
void addData();
int getID();
void deleteData();
void editData();
void modifyData();

void dataAdmin();

void adminMenu();
void userMenu();

void login();

void exit();

int main() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	login();
}

user *addUser() {
	user *newArray = new user[countOfUsers];
	for (int i = 0; i < countOfUsers - 1; i++)
		newArray[i] = Users[i];

	swap(newArray, Users);
	delete[] newArray;
	return Users;
}
void cleanMemoryUsers() {
	if (countOfUsers > 0) {
		countOfUsers = 0;
		user *temp = new user[countOfUsers];
		swap(temp, Users);
		delete[] temp;
	}
}
void openUserFile() {
	ifstream openFileR("users.txt");
	if (!openFileR.is_open())
		createUserFile();

	openFileR.close();
	ifstream openFile("users.txt");

	cleanMemoryUsers();
	char line[500];
	Users = new user[countOfUsers];
	while (!openFile.eof()) {
		openFile.getline(line, 500);
		int count = 0;
		countOfUsers++;
		Users = addUser();
		string temp;
		for (int i = 0; i < 500 || line[i] != '\0'; i++) {
			if (line[i] == ' ' || line[i] == ';') {
				switch (count++) {
				case 0:
					Users[countOfUsers - 1].login = temp;
					break;
				case 1:
					Users[countOfUsers - 1].hashPassword = temp;
					break;
				case 2:
					Users[countOfUsers - 1].role = stoi(temp);
					break;
				case 3:
					Users[countOfUsers - 1].access = stoi(temp);
				}

				temp.clear();
				continue;
			}

			temp += line[i];
		}
	}

	openFile.close();
}
void createUserFile() {
	ofstream openFile("users.txt");
	system("cls");
	countOfUsers++;
	Users = new user[countOfUsers];
	cout << "Введите логин администратора: ";
	Users[0].login = input();
	cout << endl << "Введите пароль администратора: ";
	Users[0].hashPassword = crypt(input());
	toUserFile();
	openFile.close();
}
void toUserFile() {
	ofstream openFile("users.txt");
	for (int i = 0; i < countOfUsers; i++) {
		openFile << Users[i].login << " " << Users[i].hashPassword << " " << Users[i].role << " " << Users[i].access << ";";
		if (i != countOfUsers - 1)
			openFile << endl;
	}

	openFile.close();
}
void reloadUserFile() {
	toUserFile();
	cleanMemoryUsers();
	openUserFile();
}

void outputUser(int index, bool showPasswords) {
	cout << index + 1 << " | " << Users[index].login << " | ";
	if (showPasswords)
		cout << decrypt(Users[index].hashPassword);
	else
		for (int j = 0; j < Users[index].hashPassword.length(); j++)
			cout << "*";

	cout << " | ";
	if (Users[index].role)
		cout << "admin";
	else
		cout << "user";

	cout << " | ";
	if (Users[index].access)
		cout << "+";
	else
		cout << "-";

	cout << endl;
}
void outputUsers () {
	bool showPasswords;
	cout << endl << "Показать пароли? (1 - да, 0 - нет): ";
	cin >> showPasswords;
	cout << endl << "# | Login | Passwords | Уровень доступа | Подтверждённый акканут" << endl;
	for (int i = 0; i < countOfUsers; i++)
		outputUser(i, showPasswords);
}
void showAllUsers() {
	system("cls");
	cout << "Режим просмотра: ";
	outputUsers();
	cout << endl << "Нажмите Esc, чтобы выйти в меню управления учётными записями" << endl;
	while (true)
		if (_getch() == 27)
			userControlPanel();
}
int getIndexOfUser() {
	int index = -1;
	cout << "Выберите пользователя: ";
	while (!(index >= 0 && index <= countOfUsers))
		cin >> index;

	index--;
	return index;
}
void addUserInDatabase() {
	system("cls");
	countOfUsers++;
	Users = addUser();
	cout << "Добавление пользователя" << endl;
	string login;
	while (true) {
		cout << "Введите логин: ";
		login = input();
		bool find = false;
		for (int i = 0; i < countOfUsers; i++)
			if (login == Users[i].login) {
				cout << endl << "Введите уникальный логин" << endl;
				find = true;
			}

		if (!find)
			break;
	}
	Users[countOfUsers - 1].login = login;
	cout << endl << "Введите пароль: ";
	Users[countOfUsers - 1].hashPassword = crypt(input());
	cout << endl << "Выберите уровень доступа (1 - admin, 0 - user): ";
	cin >> Users[countOfUsers - 1].role;
	Users[countOfUsers - 1].access = 1;
	reloadUserFile();
	userControlPanel();
}
void editUser() {
	system("cls");
	cout << "Редактирование учётной записи" << endl;
	outputUsers();
	int index = getIndexOfUser();
	bool showPasswords;
	cout << endl << "Показывать пароли? (1 - да, 0 - нет): ";
	cin >> showPasswords;
	while (true) {
		system("cls");
		int menu;
		cout << "Выбранный пользователь: " << endl;
		cout << "# | Login | Passwords | Уровень доступа | Подтверждённый акканут" << endl;
		outputUser(index, showPasswords);
		cout << "Выберите параметр для изменения: " << endl;
		cout << "1 - Login" << endl;
		cout << "2 - Password" << endl;
		cout << "3 - Уровень доступа" << endl;
		cout << "4 - Подтвердить аккаунт" << endl;
		cout << "5 - Выбрать другого пользователя" << endl;
		cout << "0 - Выход из режима редактирования" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;
		switch (menu) {
			case 1: {
				cout << endl << "Введите новый логин: ";
				Users[index].login = input();
			}
				break;
			case 2: {
				cout << endl << "Введите новый пароль: ";
				Users[index].hashPassword = crypt(input());
			}
					break;
			case 3: {
				bool role;
				int i;
				for (i = 0; i < countOfUsers; i++)
					if (i == index && loginStr == Users[i].login && Users[i].role)
						role = true;

				if (role && loginStr == Users[index].login) {
					cout << endl << "Вам запрещено изменять права доступа на обычного пользователя" << endl;
					break;
				}

				cout << endl << "Введите уровень доступа (1 - admin, 0 - user): ";
				cin >> Users[index].role;
			}
					break;
			case 4: {
				cout << endl << "Введите статус аккаунта (1 - подтверждённый, 0 - неподтверждённый): ";
				cin >> Users[index].access;
			}
				break;
			case 5:
				editUser();
				break;
			case 0:
				userControlPanel();
		}

		if (menu > 0 && menu < 5)
			reloadUserFile();
	}
}
void deleteUser() {
	system("cls");
	cout << "Удаление пользователя" << endl;
	outputUsers();
	int index = getIndexOfUser();
	countOfUsers--;
	user *newArray = new user[countOfUsers];
	int j = 0;
	for (int i = 0; i <= countOfUsers; i++) {
		if (i == index)
			continue;

		newArray[j++] = Users[i];
	}

	swap(newArray, Users);
	delete[] newArray;
	cout << "Удаление завершено" << endl;
	reloadUserFile();
	userControlPanel();
}
void userControlPanel() {
	while (true) {
		system("cls");
		int menu;
		cout << "Меню управления учётными записями" << endl;
		cout << "1 - Просмотр всех учётных записей" << endl;
		cout << "2 - Добавление новой учётной записи" << endl;
		cout << "3 - Редактирование учётной записи" << endl;
		cout << "4 - Удаление учётной записи" << endl;
		cout << "0 - Выйти в главное меню" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;
		switch (menu) {
		case 1:
			showAllUsers();
			break;
		case 2:
			addUserInDatabase();
			break;
		case 3:
			editUser();
			break;
		case 4:
			deleteUser();
			break;
		case 0:
			adminMenu();
		}
	}
}

employee *addEmployee() {
	employee *newArray = new employee[countOfEmployees];
	for (int i = 0; i < countOfEmployees - 1; i++)
		newArray[i] = Employees[i];

	swap(newArray, Employees);
	delete [] newArray;
	return Employees;
}
void cleanMemoryData() {
	employee *temp = new employee[0];
	swap(Employees, temp);
	delete[] temp;
	countOfEmployees = 0;
}
void enterDataFileName() {
	system("cls");
	cout << "Введите название файла: ";
	string temp = input() + ".txt";
	sizeOfName = temp.length();
	fileName = new char[sizeOfName];
	strcpy(fileName, temp.c_str());
}
void openDataFile() {
	if (countOfEmployees != 0)
		cleanMemoryData();

	ifstream openFile(fileName);
	system("cls");
	if (!openFile.is_open()) {
		cout << "Файла БД не существует" << endl;
		cout << "Хотите создать? (1 - да, 0 - нет): ";
		bool menu;
		cin >> menu;
		if (menu)
			createDataFile();
		else
			return;
	}

	char line[500];
	Employees = new employee[countOfEmployees];
	while (!openFile.eof()) {
		openFile.getline(line, 500);
		int count = 0;
		countOfEmployees++;
		Employees = addEmployee();
		string temp;
		for (int i = 0; i < 500 || line[i] != '\0'; i++) {
			if (line[i] == '|' || line[i] == ';') {
				if (count > 2 && temp == "-")
					temp = "0";

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
						Employees[countOfEmployees - 1].ID = stoi(temp);
						break;
					case 4:
						Employees[countOfEmployees - 1].year = stoi(temp);
						break;
					case 5:
						Employees[countOfEmployees - 1].month = stoi(temp);
						break;
					case 6:
						Employees[countOfEmployees - 1].countOfHours = stod(temp);
						break;
					case 7:
						Employees[countOfEmployees - 1].rate = stod(temp);
				}

				temp.clear();
				continue;
			}

			temp += line[i];
		}
	}

	openFile.close();
}
void createDataFile() {
	ofstream openFile(fileName);
	system("cls");
	if (openFile.is_open())
		cout << "Файл БД успешно создан" << endl;
	else
		cout << "Файл БД не создан" << endl;

	openFile.close();
	return;
}
void toDataFile() {
	ofstream openFile(fileName);
	for (int i = 0; i < countOfEmployees; i++) {
		openFile << Employees[i].name << "|" << Employees[i].surname << "|" << Employees[i].middleName << "|" << Employees[i].ID << "|" << Employees[i].year << "|" << Employees[i].month << "|" << Employees[i].countOfHours << "|" << Employees[i].rate << ";";
		if (i != countOfEmployees - 1)
			openFile << endl;
	}

	openFile.close();
}
void reloadDataFile() {
	toDataFile();
	cleanMemoryData();
	openDataFile();
}

double getSalary(int i) {
	double salary = 0;
	if (Employees[i].countOfHours > 144) {
		salary += 2 * ((Employees[i].countOfHours - 144) * Employees[i].rate);
		salary += 144 * Employees[i].rate;
	} else
		salary += Employees[i].countOfHours * Employees[i].rate;

	salary -= salary * 0.12;
	return salary;		
}
void salary() {
	system("cls");
	while (true) {
		openDataFile();
		system("cls");
		cout << "Расчёт заработной платы" << endl;
		outputData();
		int id = getID(), i;
		for (i = 0; i < countOfEmployees; i++)
			if (id == Employees[i].ID)
				break;
		
		while (true) {
			system("cls");
			cout << "Выбранный сотрудник: ";
			cout << "Имя | Фамилия | Отчество | Табельный номер | Год | Месяц | Количество проработанных часов | Почасовой тариф" << endl;
			cout << Employees[i].name << " | " << Employees[i].surname << " | " << Employees[i].middleName << " | " << Employees[i].ID << " | " << Employees[i].year << " | " << Employees[i].month << " | " << Employees[i].countOfHours << " | " << Employees[i].rate << endl;
			cout << "Заработная плата данного работника: " << getSalary(i) << endl;
			cout << "Хотите выйти в главное меню или продолжить вычислять заработную плату (0 - Главное меню, 1 - Расчёт заработной платы): ";
			bool menu;
			cin >> menu;
			cleanMemoryData();
			if (menu)
				break;
			else
				dataProcessing();
		}
	}
}
string getTemp(employee employee, int criteria) {
	switch (criteria) {
		case 1:
			return to_string(employee.ID);
		case 2:
			return to_string(employee.year);
		case 3:
			return to_string(employee.countOfHours);
		case 4: {
			if (round(employee.rate) == employee.rate)
				return to_string(int(round(employee.rate)));
			else
				return to_string(employee.rate);
		}
	}
}
void linearSearch() {
	system("cls");
	while (true) {
		openDataFile();
		system("cls");
		cout << "Поиск данных" << endl;
		int menu;
		cout << "Выберите параметр для поиска: " << endl;
		cout << "1 - Табельный номер" << endl;
		cout << "2 - Год" << endl;
		cout << "3 - Количество часов" << endl;
		cout << "4 - Почасовой тариф" << endl;
		cout << "0 - Выход в предыдущее меню" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;
		switch (menu) {
			case 1: 
				cout << endl << "Введите табельный номер: ";
				break;
			case 2:
				cout << endl << "Введите год: ";
				break;
			case 3:
				cout << endl << "Введите количество часов: ";
				break;
			case 4:
				cout << endl << "Введите почасовой тариф: ";
				break;
			case 0: {
				toDataFile();
				cleanMemoryData();
				dataProcessing();
			}
		}

		string criteria;
		if (menu == 4) {
			criteria = to_string(stod(input()));
			if (stod(criteria) - stoi(criteria) == 0)
				criteria = to_string(stoi(criteria));
		}
		else
			criteria = input();

		bool find = false;
		for (int i = 0; i < countOfEmployees; i++) {
			cout << getTemp(Employees[i], menu) << " ";
			if (getTemp(Employees[i], menu) == criteria) {
				if (!find)
					cout << endl << "Имя | Фамилия | Отчество | Табельный номер | Год | Месяц | Количество проработанных часов | Почасовой тариф" << endl;

				find = true;
				cout << Employees[i].name << " | " << Employees[i].surname << " | " << Employees[i].middleName << " | " << Employees[i].ID << " | " << Employees[i].year << " | " << Employees[i].month << " | " << Employees[i].countOfHours << " | " << Employees[i].rate << endl;
			}
		}

		if (!find)
			cout << endl << "Совпадений не найдено" << endl;

		cout << "Нажмите любую клавишу, чтобы продолжить" << endl;
		_getch();
	}
}
void bubbleSort() {
	system("cls");
	cout << "Сортировка данных" << endl;
	cout << "1 - Табельный номер" << endl;
	cout << "2 - Год" << endl;
	cout << "3 - Количество часов" << endl;
	cout << "4 - Почасовому тарифу" << endl;
	int menu;
	while (true) {
		cout << "Ваш выбор: ";
		cin >> menu;
		if (menu < 0 || menu > 4)
			cout << endl << "Вы ввели неправаильный номер, попробуйте ещё раз" << endl;
		else
			break;
	}

	bool ascending;
	cout << endl << "Выберите тип сортировки (0 - по убыванию, 1 - по возрастанию): ";
	cin >> ascending;
	openDataFile();
	for (int i = 0; i < countOfEmployees - 1; i++) {
		for (int j = i; j < countOfEmployees; j++)
			if (ascending) {
				if (getTemp(Employees[i], menu) > getTemp(Employees[j], menu))
					swap(Employees[i], Employees[j]);
			} else {
				if (getTemp(Employees[i], menu) < getTemp(Employees[j], menu))
					swap(Employees[i], Employees[j]);
			}
	}

	toDataFile();
	cleanMemoryData();
}
void dataProcessing() {
	while (true) {
		system("cls");
		int menu;
		cout << "Режим обработки данных: " << endl;
		cout << "1 - Расчёт заработной платы" << endl;
		cout << "2 - Поиск данных" << endl;
		cout << "3 - Сортировка даннхы" << endl;
		cout << "0 - Выход в главное меню" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;

		switch (menu) {
			case 1:
				salary();
				break;
			case 2:
				linearSearch();
				break;
			case 3:
				bubbleSort();
				break;
			case 0:
				if (role)
					dataAdmin();
				else
					userMenu();
		}

	}
}

void outputData() {
	cout << "Имя | Фамилия | Отчество | Табельный номер | Год | Месяц | Количество проработанных часов | Почасовой тариф" << endl;
	for (int i = 0; i < countOfEmployees; i++)
		cout << Employees[i].name << " | " << Employees[i].surname << " | " << Employees[i].middleName << " | " << Employees[i].ID << " | " << Employees[i].year << " | " << Employees[i].month << " | " << Employees[i].countOfHours << " | " << Employees[i].rate << endl;
}
void viewData() {
	system("cls");
	openDataFile();
	system("cls");
	cout << "Содержимое БД: " << endl;
	outputData();
	cleanMemoryData();
	cout << endl << "Нажмите Esc, чтобы выйти в меню управления учётными записями" << endl;
	while (true)
		if (_getch() == 27)
			if (role)
				modifyData();
			else
				userMenu();
}
int getMaxID() {
	int maxID = INT_MIN;
	for (int i = 0; i < countOfEmployees; i++)
		maxID = maxID < Employees[i].ID ? Employees[i].ID : maxID;

	return maxID;
}
void addData() {
	system("cls");
	openDataFile();
	cout << "Добавление сотрудника в БД" << endl;
	countOfEmployees++;
	Employees = addEmployee();
	Employees[countOfEmployees - 1].ID = getMaxID() + 1;

	cout << "Введите имя: ";
	Employees[countOfEmployees - 1].name = input();
	cout << endl << "Введите фамилию: ";
	Employees[countOfEmployees - 1].surname = input();
	cout << endl << "Введите отчество: ";
	Employees[countOfEmployees - 1].middleName = input();
	Employees[countOfEmployees - 1].year = -1;
	while (!(Employees[countOfEmployees - 1].year > -1 && Employees[countOfEmployees - 1].year <= 2023)) {
		cout << endl << "Введите год: ";
		cin >> Employees[countOfEmployees - 1].year;
	}

	Employees[countOfEmployees - 1].month = 0;
	while (!(Employees[countOfEmployees - 1].month > 0 && Employees[countOfEmployees - 1].month < 13)) {
		cout << endl << "Введите месяц: ";
		cin >> Employees[countOfEmployees - 1].month;
	}

	Employees[countOfEmployees - 1].countOfHours = -1;
	while (!(Employees[countOfEmployees - 1].countOfHours > -1)) {
		cout << endl << "Введите количество проработанных часов за месяц: ";
		cin >> Employees[countOfEmployees - 1].countOfHours;
	}

	Employees[countOfEmployees - 1].rate = -1;
	while (!(Employees[countOfEmployees - 1].rate > -1)) {
		cout << endl << "Введите почасовой тариф: ";
		cin >> Employees[countOfEmployees - 1].rate;
	}

	reloadDataFile();
}
int getID() {
	int id;
	while (true) {
		cout << "Выберите табельный номер сотрудника: ";
		cin >> id;
		if (id >= 0 && id <= getMaxID())
			return id;
	}
}
void deleteData() {
	system("cls");
	openDataFile();
	cout << "Удаление сотрудника из БД" << endl;
	outputData();
	int id = getID();

	countOfEmployees--;
	employee *newArray = new employee[countOfEmployees];
	int j = 0;
	for (int i = 0; i <= countOfEmployees; i++) {
		if (Employees[i].ID == id)
			continue;

		newArray[j++] = Employees[i];
	}

	swap(newArray, Employees);
	delete [] newArray;
	reloadDataFile();
	cout << endl << "Удаление завершено" << endl;
}
void editData() {
	openDataFile();
	system("cls");
	cout << "Изменение данных сотрудника в БД: " << endl;
	outputData();
	int id = getID(), i;
	for (i = 0; i < countOfEmployees; i++)
		if (id == Employees[i].ID)
			break;

	while (true) {
		int menu;
		cout << endl << "Выбранный сотрудник: " << endl;
		cout << "Имя | Фамилия | Отчество | Табельный номер | Год | Месяц | Количество проработанных часов | Почасовой тариф" << endl;
		cout << Employees[i].name << " | " << Employees[i].surname << " | " << Employees[i].middleName << " | " << Employees[i].ID << " | " << Employees[i].year << " | " << Employees[i].month << " | " << Employees[i].countOfHours << " | " << Employees[i].rate << endl;
		cout << "Выберите данные для изменения: " << endl;
		cout << "1 - Имя" << endl;
		cout << "2 - Фамилия" << endl;
		cout << "3 - Отчество" << endl;
		cout << "4 - Табельный номер" << endl;
		cout << "5 - Год" << endl;
		cout << "6 - Месяц" << endl;
		cout << "7 - Количество проработанных часов" << endl;
		cout << "8 - Почасовой тариф" << endl;
		cout << "9 - Выбрать другого сотрудника" << endl;
		cout << "0 - Выйти из данного режима" << endl;
		cout << "Ваш вариант: ";
		cin >> menu;
		switch (menu) {
			case 1: {
				cout << endl << "Введите имя: ";
				Employees[i].name = input();
			}
				break;
			case 2: {
				cout << endl << "Введите фамилию: ";
				Employees[i].surname = input();
			}
				break;
			case 3: {
				cout << endl << "Введите отчество: ";
				Employees[i].middleName = input();
			}
				break;
			case 4: {
				int temp;
				while (true) {
					cout << endl << "Введите табельный номер: ";
					cin >> temp;
					bool find = false;
					for (int j = 0; j < countOfEmployees; j++)
						if (temp == Employees[j].ID) {
							find = true;
							break;
						}

					if (!find)
						break;
				}
				Employees[i].ID = temp;
			}
					break;
			case 5: {
				Employees[i].year = -1;
				while (!(Employees[i].year > -1 && Employees[i].year < 2023)) {
					cout << endl << "Введите год: ";
					cin >> Employees[i].year;
				}
			}
				break;
			case 6: {
				Employees[i].month = 0;
				while (!(Employees[i].month > 0 && Employees[i].month < 13)) {
					cout << endl << "Введите месяц: ";
					cin >> Employees[i].month;
				}
			}
				break;
			case 7: {
				Employees[i].countOfHours = -1;
				while (!(Employees[i].countOfHours > -1)) {
					cout << endl << "Введите количество проработанных часов: ";
					cin >> Employees[i].countOfHours;
				}
			}
				break;
			case 8: {
				Employees[i].rate = -1;
				while (!(Employees[i].rate > -1)) {
					cout << endl << "Введите почасовой тариф: ";
					cin >> Employees[i].rate;
				}
			}
				break;
			case 9:
				editData();
				break;
			case 0:
				modifyData();
		}

		if (menu > 0 && menu < 8)
			reloadDataFile();
	}
}
void modifyData() {
	while (true) {
		system("cls");
		int menu;
		cout << "Режим редактированния данных" << endl;
		cout << "1 - Просмотр всех данных" << endl;
		cout << "2 - Добавления новой записи" << endl;
		cout << "3 - Удаление записи" << endl;
		cout << "4 - Редактирование записи" << endl;
		cout << "0 - Вернуться в выбор режима работы с данными" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;
		switch (menu) {
			case 1:
				viewData();
				break;
			case 2:
				addData();
				break;
			case 3:
				deleteData();
				break;
			case 4:
				editData();
				break;
			case 0:
				dataAdmin();
		}
	}
}
void dataAdmin() {
	while (true) {
		if (sizeOfName == 0)
			enterDataFileName();

		system("cls");
		int menu;
		cout << "Режим работы с данными" << endl;
		cout << "1 - Режим редактирования" << endl;
		cout << "2 - Режим обработки данных" << endl;
		cout << "3 - Открыть другой файл" << endl;
		cout << "0 - Выйти в главное меню" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;
		switch (menu) {
			case 1:
				modifyData();
				break;
			case 2:
				dataProcessing();
				break;
			case 3:
				enterDataFileName();
				break;
			case 0: {
				sizeOfName = 0;
				fileName = NULL;
				adminMenu();
			}
		}
	}
}

void adminMenu() {
	while(true) {
		system("cls");
		int menu;
		cout << "Меню администратора: " << endl;
		cout << "1 - Управление учётными записями" << endl;
		cout << "2 - Режим работы с данными" << endl;
		cout << "3 - Выход из системы" << endl;
		cout << "0 - Завершение работы" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;
		switch (menu) {
			case 1:
				userControlPanel();
				break;
			case 2:
				dataAdmin(); // работа с данными для админа
				break;
			case 3:
				login();
				break;
			case 0:
				exit();
		}
	}
}
void userMenu() {
	if (sizeOfName == 0)
		enterDataFileName();

	while (true) {
		system("cls");
		int menu;
		cout << "Меню пользователя: " << endl;
		cout << "1 - Просмотр всех данных" << endl;
		cout << "2 - Режим обработки данных" << endl;
		cout << "3 - Открыть другой файл" << endl;
		cout << "4 - Выход из системы" << endl;
		cout << "5 - Завершение работы" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;
		switch (menu) {
			case 1:
				viewData();
				break;
			case 2:
				dataProcessing();
				break;
			case 3:
				enterDataFileName();
				break;
			case 4: {
				sizeOfName = 0;
				fileName = NULL;
				login();
			}
				break;
			case 5:
				exit();
		}
	}
}

void login() {
	while (true) {
		openUserFile();
		system("cls");
		cout << "Введите имя пользователя: ";
		string login = input(), password;
		int i;
		bool find = true;
		while (true) {
			cout << endl << "Введите пароль: ";
			password = inputPassword();
			find = true;
			for (i = 0; i < countOfUsers; i++) {
				if (login == Users[i].login)
					if (password == decrypt(Users[i].hashPassword))
						break;
					else {
						cout << endl << "Введён неправильный пароль. Попробуйте ещё раз!";
						find = false;
					}
			}

			if (find)
				break;
		}

		if (find)
			if (i < countOfUsers) {
				loginStr = Users[i].login;
				role = Users[i].role;
				if (Users[i].access)
					if (Users[i].role)
						adminMenu();
					else {
						userMenu();
					}
				else {
					cout << endl << "Доступ запрещён" << endl;
				}
			} else {
				bool temp;
				cout << endl << "Чтобы зарегестрироваться, необходимо разрешение администратора. Хотите оставить заявку? (1 - да, 0 - нет): ";
				cin >> temp;
				if (temp) {
					cout << endl << "Какие права хотите получить? (1 - администратор, 0 - пользователь): ";
					cin >> temp;
					ofstream openFile("users.txt", ios::app);
					openFile << endl << login << " " << crypt(password) << " " << temp << " " << 0 << ";";
					openFile.close();
				}
			}
	}
}

void exit() {
	if (countOfUsers > 0)
		delete[] Users;

	exit(0);
}