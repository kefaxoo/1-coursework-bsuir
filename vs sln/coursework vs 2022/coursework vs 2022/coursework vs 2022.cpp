#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>

#include "crypto.h"
#include "input.h"

#pragma warning (disable: 4996)

using namespace std;

struct user {
	string login; // строка для хранения логина пользователя
	string hashPassword; // строка для хранения шифрованного пароля пользователя
	bool role; // логическая переменная, хранящая права пользователя (1 - admin, 0 - user)
	bool access; // логическая переменная, хранящая возможность доступа в систему (1 - admin allows access to the system, 0 - admin doesn't allow access to the system)
};

int countOfUsers = 0; // целочисленная переменная, хранящая количество пользователей в системе
user* Users = NULL; // указатель типа user, который используется как динамический массив структур, в которой храним информацию об аккауте
const char FILENAME_OF_USERS_DATABASE[] = "users.txt"; // константный массив, хранящий название файла базы данных акканутов пользователей
bool editsInStructures = false; // логическая переменная, которая используется, чтобы проверить, что учётные записи в файле и учётные записи в массив структур отличаются
int maxLengthInUsersDatabase[3] = { 5, 6, 15 }; // 0 - login, 1 - password, 2 - role

string loginOfUser; // строка, которая хранит логин пользователя, который вошёл в систему
bool roleOfUser; // логическая переменная, которая хранит права пользователя, который вошёл в систему

struct employee {
	int id;
	string name;
	string surname;
	string middleName;
	int tabelNumber = -1;
	int year;
	int month;
	double countOfHours;
	double rate;
};

employee* Employees;
int countOfEmployees;
int sizeOfName = 0;
char *fileName;
int maxLengthInDataDatabase[7] = { 3, 7, 8, 15, 3, 5, 30 }; // 0 - name, 1 - surname, 2 - middleName, 3 - tabelNumber, 4 - year, 5 - month, 6 - countOfHours

void enableRussianLanguage(); // функция, подключающая поддержку русского языка в консоли
void login(); // функция авторизации в систему
void exit(); // функция завершения работы с системой

// блок функций, отвечающий за работу с файлом и динамическим массивом структур учетных записей пользователей
void cleanMemoryUsers(); // функция, очищающая динамический массив структур учетных записей пользователей
void addAdmin_fileWasntExist(); // функция, добавляющая учетную запись администратора при создании файла users.txt, если файл при запуске не существовал
void addUser(); // функция, добавляющая новую пустую структуру в динамический массив структур
void openUserFile(); // функция, открывающая файл с учетными записями пользователями и пере-дающая информацию из файла в динамический массив структур учетных записей пользователей
void createUserFile(); // функция, создающая файл с учетными записями пользователей, если он отсутствует
void toUserFile(); // функция, передающая информацию из динамического массива структур учетных записей пользователей в файл

// блок функций-модулей пользователей разных прав доступа к системе
void adminMenu(); // функция-модуль администратора
void userMenu(); // функция-модуль пользователя

// блок функций, отвечающих за работу с файлом и динамическим массивом структур сотрудников
void enterDataFilename(); // функция, в которой пользователь вводит название файла с сотрудниками
void openDataFile(); // функция, открывающая файл с сотрудниками и передающая информацию из файла в динамический массив структур сотрудников
void cleanMemoryData(); // функция, очищающая динамический массив структур сотрудников
void createDataFile(); // функция, создающая файл с сотрудниками, если он отсутствует
void addEmployee(); // функция, добавляющая новую пустую структуру в динамический массив сотрудников
void toDataFile(); // функция, передающая информацию из динамического массива структур сотрудников в файл

// блок функций-подмодулей администратора
void userControlPanel(); // функция-подмодуль управления учетными записями пользователей
void dataAdmin(); // функция-подмодуль работы с данными о сотрудниках

// блок функций-подмодуля управления учетными записями пользователей
void showAllUsers(); // функция просмотра содержимого динамического массива структур учетных записей пользователей
void addUserInDatabase(); // функция, добавляющая новую учетную запись пользователя в динамический массив структур
void editUser(); // функция редактирования учетной записи пользователя в динамическом массиве структур
void deleteUser(); // функция удаления учетной записи пользователя из динамического массива структур

// блок функций-подмодулей управления учетными записями
void outputUsers(); // функция вывода учетных записей пользователей с шапкой
void outputUser(int, bool); // функция вывода одной учётной записи пользователя
int getIndexOfUser(); // функция, получающая индекс учетной записи пользователя в динамическом массиве структур
bool isAdminEditsThemself(int, bool); // функция, которая проверяет: не редактирует ли администратор самого себя
void updateMaxLengthOfUserDatabase(); // функция, обновляющая максимальную длину компонентов структуры

// блок функций-подмодуля работы с данными о сотрудниках
void modifyData(); // функция-подмодуль режима редактирования данных
void dataProcessing(); // функция-подмодуль режима обработки данных

// блок функций-подмодуля режима обработки данных
void salary(); // функция вывода зарплаты сотрудников
void linearSearch(); // функция линейного поиска данных сотрудников
void sort(); // функция сортировки данных сотрудников

// блок функций-подмодуля режима редактирования данных о сотрудниках
void viewData(); // функция просмотра содержимого динамического массива структур сотрудников
void addData(); // функция добавления записи в динамический массив структур сотрудников
void deleteData(); // функция удаления записи из динамического массива структур сотрудников
void editData(); // функция редактирования записи в динамическом массиве структур сотрудников

// блок функций-подмодулей режима редактирования данных о сотрудниках
void outputData(); // функция вывода всех записей
void outputOneData(int); // функция вывода одной записи
int getLocalYear(); // функция получения года в системе
string tolower(string); // функция, опускающая регистр всего слова
int getFreeTabelNumber(); // функция получения свободного табельного номера
int getFreeID(); // функция получения свободного номера
void updateMaxLengthOfDataDatabase(); // функция, обновляющая максимальную длину компонентов структуры

// блок функций-подмодулей режим обработки данных о сотрудниках
double getSalary(int, double); // функция вычисления зарплаты по количеству часов и почасовому тарифу
void sort_getParametr(bool); // функция выбора критерия сортировки
void showCriteria(); // функция вывода критериев сортировки
void bubbleSort(int); // функция пузырьковой сортировки
void quickSort(int, int, int); // функция быстрой сортировки
int getMiddleIndex(int, int, int); // вспомогательная функция быстрой сортировки
string getTempParametr(employee, int); // функция получения параметра из структуры сотрудника для поиска
int getID(); // функция получения номера записи в динамическом массиве структур сотрудников
int getMaxID(); // функция получения максимального номера записи в динамическом массиве структур сотрудников

int main() {
	enableRussianLanguage();
	login();
}

void enableRussianLanguage() {
	SetConsoleOutputCP(1251); // включение поддержки русского языка на вывод из консоли
	SetConsoleCP(1251); // включение поддержки русского языка на ввод в консоль
}
void login() {
	openUserFile();
	system("cls");
	cout << "Введите логин: ";
	string loginString = inputLogin(); // строка логина, введённого с клавиатуры
	string password; // строка пароля, введённого с клавиатуры
	int i; // целочисленная переменная для цикла и определённого аккаунта из массива структур
	bool find; // переменная, которая контролирует поиск аккаунта в массиве структур
	while (true) {
		cout << endl << "Введите пароль: ";
		password = inputPassword_hide();
		find = true;
		for (i = 0; i < countOfUsers; i++) {
			if (loginString == Users[i].login)
				if (password == decrypt(Users[i].hashPassword))
					break;
				else {
					cout << endl << "Введён неправильный пароль, попробуйте ещё раз";
					find = false;
					continue;
				}
		}

		if (find)
			break;
	}

	if (find)
		if (i < countOfUsers) {
			loginOfUser = Users[i].login;
			roleOfUser = Users[i].role;
			if (Users[i].access)
				roleOfUser ? adminMenu() : userMenu();
			else {
				cout << endl << "Доступ запрещён" << endl << "Хотите зайти в систему под другим логином? (0 - нет, 1 - да): ";
				inputBool() ? login() : exit();
			}
		}
		else {
			cout << endl << "Чтобы зарегестрироваться, необходимо разрешение администратора, хотите оставить заявку? (0 - нет, 1 - да): ";
			if (inputBool()) {
				addUser();
				Users[countOfUsers - 1].login = loginString;
				Users[countOfUsers - 1].hashPassword = crypt(password);
				cout << endl << "Какие права хотите получить? (1 - администратор, 0 - пользователь): ";
				Users[countOfUsers - 1].role = inputBool();
				Users[countOfUsers - 1].access = false;
				toUserFile();
			}
			else
				exit();
		}
}
void exit() {
	cleanMemoryUsers();
	cleanMemoryData();
	exit(0);
}

void cleanMemoryUsers() {
	if (countOfUsers != 0 || Users != NULL) { // если в системе что-то есть
		countOfUsers = 0;
		delete [] Users;
		Users = NULL;
	}
}
void addAdmin_fileWasntExist() {
	Users = new user[++countOfUsers]; // создаём структуру размера countOfUsers + 1
	cout << "Введите логин администратора: ";
	Users[0].login = inputLogin();
	cout << endl << "Введите пароль администратора: ";
	Users[0].hashPassword = crypt(inputPassword());
	Users[0].role = true;
	Users[0].access = true;
}
void addUser() {
	user* temp = new user[++countOfUsers]; // временный массив структур аккаунтов пользователей
	for (int i = 0; i < countOfUsers - 1; i++)
		temp[i] = Users[i];

	swap(temp, Users);
	delete[] temp;
}
void openUserFile() {
	cleanMemoryUsers();
	ifstream readFile(FILENAME_OF_USERS_DATABASE);
	if (!readFile.is_open()) {
		createUserFile();
		addAdmin_fileWasntExist();
	}
	else {
		char line[500]; // символьный массив для получения информации из файла
		while (!readFile.eof()) {
			readFile.getline(line, 500);
			int count = 0; // целочисленная переменная для хода по структуре
			addUser();
			string temp; // временная строка для будущих компонентов структуры
			for (int i = 0; i < 500 || line[i] != '\0'; i++) {
				if (line[i] == ' ' || line[i] == ';') {
					if (count < 2)
						maxLengthInUsersDatabase[count] = maxLengthInUsersDatabase[count] < temp.length() ? temp.length() : maxLengthInUsersDatabase[count];

					switch (count++) {
						case 0:
							Users[countOfUsers - 1].login = temp;
							break;
						case 1:
							Users[countOfUsers - 1].hashPassword = temp;
							break;
						case 2:
							Users[countOfUsers - 1].role = stoi(temp); // stoi - перевод string в int, но так как bool хранит значение 1 или 0, то stoi можно использовать для перевода из string в bool
							break;
						case 3:
							Users[countOfUsers - 1].access = stoi(temp);
					}

					temp.clear();
					continue;
				}

				temp.push_back(line[i]);
			}
		}
	}

	readFile.close();
	updateMaxLengthOfUserDatabase();
}
void createUserFile() {
	ofstream createFile(FILENAME_OF_USERS_DATABASE); // при стандартном открытии файла в режиме записи, если файл отсутствует, то он создаётся
	createFile.close();
	ifstream checkIfCreatedFileExists(FILENAME_OF_USERS_DATABASE);
	cout << "Файл " << FILENAME_OF_USERS_DATABASE << (checkIfCreatedFileExists.is_open() ? " успешно создан" : " не был создан") << endl; // проверка и вывод сообщения об удачном или нет создании файла
}
void toUserFile() {
	ofstream writeFile(FILENAME_OF_USERS_DATABASE);
	for (int i = 0; i < countOfUsers; i++) {
		writeFile << Users[i].login << " " << Users[i].hashPassword << " " << Users[i].role << " " << Users[i].access << ";";
		if (i != countOfUsers - 1)
			writeFile << endl;
	}

	writeFile.close();
}

void adminMenu() {
	while (true) {
		system("cls");
		cout << "Меню администратора" << endl;
		cout << "1 - Управление учётными записями" << endl;
		cout << "2 - Режим работы с данными" << endl;
		cout << "3 - Выход из системы" << endl;
		cout << "0 - Завершение работы" << endl;
		cout << "Ваш выбор: ";
		int menu = inputInt();
		system("cls");
		switch (menu) {
			case 1: {
				openUserFile();
				editsInStructures = false;
				userControlPanel();
			}
				  break;
			case 2: {
				openDataFile();
				editsInStructures = false;
				dataAdmin();
			}
				  break;
			case 3:
				login();
				break;
			case 0:
				exit();
			default: {
				cout << "Выбран неправильный номер, попробуйте ещё раз" << endl;
				Sleep(5000);
			}
		}
	}
}
void userMenu() {
	if (countOfEmployees == 0)
		openDataFile();

	while (true) {
		system("cls");
		cout << "Меню пользователя" << endl;
		cout << "1 - Просмотр всех данных" << endl;
		cout << "2 - Режим обработки данных" << endl;
		cout << "3 - Открыть другой файл" << endl;
		cout << "4 - Выход из системы" << endl;
		cout << "5 - Завершение работы" << endl;
		cout << "Ваш выбор: ";
		switch (inputInt()) {
			case 1:
				viewData();
				break;
			case 2:
				dataProcessing();
				break;
			case 3: {
				cleanMemoryData();
				openDataFile();
			}
				break;
			case 4: {
				cleanMemoryData();
				login();
			}
				break;
			case 5:
				exit();
			default:
				cout << endl << "Вы ввели неправильный номер, попробуйте ещё раз";
		}
	}
}

void enterDataFilename() {
	system("cls");
	cout << "Введите название файла: ";
	string temp;
	temp.append(input() + ".txt");
	sizeOfName = temp.length();
	fileName = new char[sizeOfName];
	strcpy(fileName, temp.c_str());
}
void openDataFile() {
	enterDataFilename();
	ifstream openFile(fileName);
	system("cls");
	if (!openFile.is_open()) {
		cout << "Файла с информацией о сотрудниках не существует" << endl << "Хотите создать? (1 - да, 0 - нет): ";
		inputBool() ? createDataFile() : roleOfUser ? adminMenu() : userMenu();
	}

	cleanMemoryData();
	char line[500];
	while (!openFile.eof()) {
		openFile.getline(line, 500);
		int count = 0;
		addEmployee();
		string temp;
		for (int i = 0; i < 500 || line[i] != '\0'; i++) {
			if (line[i] == '|' || line[i] == ';') {
				temp = count > 3 && temp == "-" ? "0" : temp;
				if (count < 7)
					maxLengthInDataDatabase[count] = maxLengthInDataDatabase[count] < temp.length() ? temp.length() : maxLengthInDataDatabase[count];

				switch (count++) {
					case 0:
						Employees[countOfEmployees - 1].id = stoi(temp);
						break;
					case 1:
						Employees[countOfEmployees - 1].name = temp;
						break;
					case 2:
						Employees[countOfEmployees - 1].surname = temp;
						break;
					case 3:
						Employees[countOfEmployees - 1].middleName = temp;
						break;
					case 4:
						Employees[countOfEmployees - 1].tabelNumber = stoi(temp);
						break;
					case 5:
						Employees[countOfEmployees - 1].year = stoi(temp);
						break;
					case 6:
						Employees[countOfEmployees - 1].month = stoi(temp);
						break;
					case 7:
						Employees[countOfEmployees - 1].countOfHours = stod(temp);
						break;
					case 8:
						Employees[countOfEmployees - 1].rate = stod(temp);
				}

				temp.clear();
				continue;
			}

			temp.push_back(line[i]);
		}
	}

	openFile.close();
	updateMaxLengthOfDataDatabase();
	cout << "Файл успешно открыт" << endl;
	Sleep(5000);
}
void cleanMemoryData() {
	if (countOfEmployees != 0 || Employees != NULL) {
		delete[] Employees;
		Employees = NULL;
		countOfEmployees = 0;
	}
}
void createDataFile() {
	ofstream openFile(fileName);
	system("cls");
	cout << (openFile.is_open() ? "Файл БД успешно создан" : "Файл БД не создан") << endl;
	openFile.close();
	return;
}
void addEmployee() {
	employee* temp = new employee[++countOfEmployees];
	for (int i = 0; i < countOfEmployees - 1; i++)
		temp[i] = Employees[i];

	swap(temp, Employees);
	delete[] temp;
}
void toDataFile() {
	ofstream openFile(fileName);
	for (int i = 0; i < countOfEmployees; i++) {
		openFile << Employees[i].id << "|" << Employees[i].name << "|" << Employees[i].surname << "|" << Employees[i].middleName << "|" << Employees[i].tabelNumber << "|" << Employees[i].year << "|" << Employees[i].month << "|" << Employees[i].countOfHours << "|" << Employees[i].rate << ";";
		if (i != countOfEmployees - 1)
			openFile << endl;
	}

	openFile.close();
}

void userControlPanel() {
	while (true) {
		system("cls");
		cout << "Меню управления учётными записями" << endl;
		cout << "1 - Просмотр всех учётных записей" << endl;
		cout << "2 - Добавление новой учётной записи" << endl;
		cout << "3 - Редактирование учётной записи" << endl;
		cout << "4 - Удаление учётной записи" << endl;
		cout << "0 - Выйти в главное меню" << endl;
		cout << "Ваш выбор: ";
		int menu = inputInt();
		system("cls");
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
			case 0: {
				if (editsInStructures) {
					cout << "Вы внесли изменения в файл базу данных пользователей, хотите обновить файл? (0 - нет, 1 - да): ";
					bool temp = inputBool();
					if (temp)
						toUserFile();

					cleanMemoryUsers();
					editsInStructures = false;
				}

				return;
			}
			default:
				cout << "Выбран неправильный номер, попробуйте ещё раз" << endl;
		}
	}
}
void dataAdmin() {
	while (true) {
		system("cls");
		cout << "Режим работы с данными" << endl;
		cout << "1 - Режим редактирования" << endl;
		cout << "2 - Режим обработки данных" << endl;
		cout << "0 - Выйти в главное меню" << endl;
		cout << "Ваш выбор: ";
		int menu = inputInt();
		switch (menu) {
			case 1:
				modifyData();
				break;
			case 2:
				dataProcessing();
				break;
			case 3:
				openDataFile();
				break;
			case 0: {
				if (editsInStructures) {
					cout << "Вы внесли изменения в файл базу данных сотрудников, хотите обновить файл? (0 - нет, 1 - да): ";
					if (inputBool())
						toDataFile();
				}

				cleanMemoryData();
				editsInStructures = false;
				adminMenu();
			}
			default:
				cout << "Выбран неправильный номер, попробуйте ещё раз" << endl;
		}
	}
}

void showAllUsers() {
	system("cls");
	cout << "Режим просмотра: ";
	outputUsers();
	cout << endl << "Нажмите Esc, чтобы выйти в меню управления учётными записями";
	while (true)
		if (_getch() == 27)
			return;
}
void addUserInDatabase() {
	system("cls");
	addUser();
	cout << "Добавление пользователя" << endl;
	string login;
	while (true) {
		cout << "Введите логин: ";
		login = inputLogin();
		bool find = false;
		for (int i = 0; i < countOfUsers - 1; i++)
			if (login == Users[i].login) {
				cout << endl << "Введите уникальный логин" << endl;
				find = true;
				break;
			}

		if (!find)
			break;
	}

	Users[countOfUsers - 1].login = login;
	cout << endl << "Введите пароль: ";
	Users[countOfUsers - 1].hashPassword = crypt(inputPassword());
	cout << endl << "Выберите уровень доступа (1 - admin, 0 - user): ";
	Users[countOfUsers - 1].role = inputBool();
	Users[countOfUsers - 1].access = true;
	editsInStructures = true;
	cout << endl << "Пользователь успешно добавлен в систему";
	Sleep(5000);
	updateMaxLengthOfUserDatabase();
	editsInStructures = true;
	return;
}
void editUser() {
	system("cls");
	cout << "Редактирование учётной записи" << endl;
	outputUsers();
	int index = getIndexOfUser();
	if (index == -1)
		return;

	cout << endl << "Показывать пароли? (1 - да, 0 - нет): ";
	bool showPasswords = inputBool();
	while (true) {
		cout << endl << "Выбранный пользователь:" << endl;
		outputUser(index, showPasswords);
		cout << "Выберите параметр, который хотите изменить:" << endl << "1 - Логин" << endl << "2 - Пароль" << endl << "3 - Уровень доступа" << endl << "4 - Доступ к системе" << endl << "5 - Выбрать другую учётную запись" << endl << "6 - Выход в предыдущее меню" << endl << "Ваш выбор: ";
		int menu = inputInt();
		switch (menu) {
			case 1: {
				while (true) {
					cout << endl << "Введите логин: ";
					string login = inputLogin();
					bool find = false;
					for (int i = 0; i < countOfUsers; i++)
						if (i == index)
							continue;
						else
							if (login == Users[i].login)
								find = true;

					if (!find) {
						Users[index].login = login;
						break;
					}
					else
						cout << endl << "Данный логин уже используется, попробуйте другой";
				}
			}
				  break;
			case 2: {
				cout << endl << "Введите пароль: ";
				Users[index].hashPassword = crypt(inputPassword());
			}
				  break;
			case 3: {
				if (isAdminEditsThemself(index, 1))
					break;

				cout << endl << "Введите уровень доступа (1 - admin, 0 - user): ";
				Users[index].access = inputBool();
			}
				  break;
			case 4: {
				if (isAdminEditsThemself(index, 1))
					break;

				cout << endl << "Введите доступ к системе (1 - доступ разрешён, 0 - доступ запрещён): ";
				Users[index].access = inputBool();
			}
				  break;
			case 5:
				editUser();
				break;
			case 6:
				return;
			default:
				cout << endl << "Вы ввели неправильный номер, попробуйте ещё раз" << endl;
		}

		if ((menu > 0 && menu < 5) && Users[index].login != loginOfUser && Users[index].role != roleOfUser) {
			cout << endl << "Пользователь успешно изменён";
			editsInStructures = true;
			updateMaxLengthOfUserDatabase();
			Sleep(5000);
		}
	}
}
void deleteUser() {
	system("cls");
	cout << "Удаление пользователя" << endl;
	outputUsers();
	int index = getIndexOfUser();
	if (index == -1)
		return;

	if (isAdminEditsThemself(index, 0)) {
		Sleep(5000);
		return;
	}

	editsInStructures = true;
	countOfUsers--;
	user* temp = new user[countOfUsers];
	int j = 0;
	for (int i = 0; i <= countOfUsers; i++) {
		if (i == index)
			continue;

		temp[j++] = Users[i];
	}

	swap(temp, Users);
	delete[] temp;
	cout << endl << "Удаление завершено" << endl;
	updateMaxLengthOfUserDatabase();
	Sleep(5000);
	editsInStructures = true;
	return;
}

void outputUsers() {
	cout << endl << "Показать пароли? (1 - да, 0 - нет): ";
	bool showPasswords = inputBool();
	printf("%*s%*s%*s%*s%*s%*s%*s", to_string(countOfUsers).length(), "#", 3, " | ", maxLengthInUsersDatabase[0], "Login", 3, " | ", maxLengthInUsersDatabase[1], "Password", 3, " | ", maxLengthInUsersDatabase[2], "Уровень доступа");
	cout << " | Доступ к системе" << endl;
	for (int i = 0; i < countOfUsers; i++)
		outputUser(i, showPasswords);
}
void outputUser(int index, bool showPasswords) {
	printf("%*i", to_string(countOfUsers).length(), index + 1);
	cout << " | ";
	printf("%*s", maxLengthInUsersDatabase[0], Users[index].login.c_str());
	cout << " | ";
	string password;
	if (showPasswords)
		password = decrypt(Users[index].hashPassword);
	else
		for (int i = 0; i < Users[index].hashPassword.length(); i++)
			password.push_back('*');

	printf("%*s", maxLengthInUsersDatabase[1], password.c_str());
	cout << " | ";
	printf("%*s", maxLengthInUsersDatabase[2], Users[index].role ? "администратор" : "пользователь");
	cout << " | " << (Users[index].access ? "+" : "-") << endl;
}
int getIndexOfUser() {
	cout << "Выберите пользователя: ";
	while (true) {
		int index = inputInt();
		--index;
		if (index >= 0 && index < countOfUsers)
			return index;
		else {
			cout << endl << "Введён неправильный номер пользователя, попробуйте ещё раз" << endl;
			Sleep(5000);
			return -1;
		}
	}
}
bool isAdminEditsThemself(int index, bool mode) { // mode - true - edit, false - delete
	bool role = false;
	for (int i = 0; i < countOfUsers; i++)
		if (i == index && loginOfUser == Users[i].login && Users[i].role)
			role = true;

	if (role)
		cout << endl << (mode ? "Вам запрещено изменять себе права доступа" : "Вам запрещено удалять свою учетную запись") << endl;

	return role;
}
void updateMaxLengthOfUserDatabase() {
	maxLengthInUsersDatabase[0] = 5;
	maxLengthInUsersDatabase[1] = 6;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < countOfUsers; j++)
			switch (i) {
				case 0:
					maxLengthInUsersDatabase[i] = maxLengthInUsersDatabase[i] < Users[j].login.length() ? Users[j].login.length() : maxLengthInUsersDatabase[i];
					break;
				case 1:
					maxLengthInUsersDatabase[i] = maxLengthInUsersDatabase[i] < Users[j].hashPassword.length() ? Users[j].hashPassword.length() : maxLengthInUsersDatabase[i];
			}
}

void dataProcessing() {
	while (true) {
		system("cls");
		cout << "Режим обработки" << endl;
		cout << "1 - Расчёт заработной платы" << endl;
		cout << "2 - Поиск данных" << endl;
		cout << "3 - Сортировка данных" << endl;
		cout << "0 - Выход в предыдущее меню" << endl;
		cout << "Ваш выбор: ";
		switch (inputInt()) {
			case 1:
				salary();
				break;
			case 2:
				linearSearch();
				break;
			case 3:
				sort();
				break;
			case 0:
				roleOfUser ? dataAdmin() : userMenu();
		}
	}
}
void modifyData() {
	system("cls");
	cout << "Режим редактированния данных" << endl;
	cout << "1 - Просмотр всех данных" << endl;
	cout << "2 - Добавления новой записи" << endl;
	cout << "3 - Удаление записи" << endl;
	cout << "4 - Редактирование записи" << endl;
	cout << "0 - Вернуться в предыдущее меню" << endl;
	cout << "Ваш выбор: ";
	while (true) {
		int menu = inputInt();
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
			default:
				cout << endl << "Выбран неправильный номер, попробуйте ещё раз" << endl;
		}
	}
}

void salary() {
	while (true) {
		while (true) {
			system("cls");
			cout << "Расчёт заработной платы" << endl;
			outputData();
			int id = getID(), i;
			for (i = 0; i < countOfEmployees; i++)
				if (id == Employees[i].id)
					break;

			while (true) {
				system("cls");
				cout << endl << "Выбранный сотрудник: " << endl;
				printf("%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s", to_string(countOfEmployees).length(), "#", 3, " | ", maxLengthInDataDatabase[0], "Имя", 3, " | ", maxLengthInDataDatabase[1], "Фамилия", 3, " | ", maxLengthInDataDatabase[2], "Отчество", 3, " | ", maxLengthInDataDatabase[3], "Табельный номер", 3, " | ", maxLengthInDataDatabase[4], "Год", 3, " | ", maxLengthInDataDatabase[5], "Месяц", 3, " | ", maxLengthInDataDatabase[6], "Количество проработанных часов");
				cout << " | Почасовой тариф" << endl;
				outputOneData(i);
				cout << "Рассчитать зарплату за период или по информации записи (0 - за период, 1 - по информации записи): ";
				bool temp = inputBool();
				if (temp)
					cout << endl << "Зарплата данного сотрудника: " << getSalary(i, Employees[i].countOfHours) << endl;
				else {
					double tempDouble;
					while (true) {
						cout << endl << "Введите требуемый период: ";
						tempDouble = inputDouble();
						if (tempDouble >= 0)
							break;
						else
							cout << endl << "Вы ввели неправильное количество проработанных часов, попробуйте ещё раз";
					}

					cout << endl << "Зарплата данного сотрудника за " << tempDouble << " часов: " << getSalary(i, tempDouble) << endl;
				}

				cout << "Хотите выйти в предыдущее меню или выбрать другого сотрудника? (0 - предыдущее меню, 1 - выбрать другого сотрудника): ";
				if (inputBool())
					break;
				else
					dataProcessing();
			}
		}
	}
}
void linearSearch() {
	int criteria;
	while (true) {
		system("cls");
		cout << "Линейный поиск" << endl;
		showCriteria();
		cout << "0 - Выход в предыдущее меню" << endl << "Ваш выбор: ";
		switch (criteria = inputInt()) {
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				break;
			case 0:
				dataProcessing();
			default:
				cout << endl << "Вы ввели неправильный номер, попробуйте ещё раз";
		}

		if (criteria > 0 && criteria < 7)
			break;
	}

	string criteriaString;
	switch (criteria) {
		case 1:
		case 2:
			while (true) {
				cout << "Введите параметр поиска: ";
				if (stoi(criteriaString = to_string(inputInt())) > 0)
					break;
				else
					cout << endl << "Вы ввели неправильное значение, попробуйте ещё раз" << endl;
			}
			break;
		case 3:
			criteriaString = to_string(inputYear_Data(getLocalYear()));
			break;
		case 4:
			criteriaString = to_string(inputMonth_Data());
			break;
		case 5:
			while (true) {
				cout << "Введите параметр поиска: ";
				criteriaString = to_string(inputDouble());
				if (stod(criteriaString) > 0) {
					if (stod(criteriaString) - stoi(criteriaString) == 0)
						criteriaString = to_string(stoi(criteriaString));

					break;
				}
				else
					cout << endl << "Вы ввели неправильное значение, попробуйте ещё раз" << endl;
			}
			break;
	}
	
	bool find = false;
	for (int i = 0; i < countOfEmployees; i++)
		if (criteriaString == getTempParametr(Employees[i], criteria)) {
			if (!find) {
				printf("%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s", to_string(countOfEmployees).length(), "#", 3, " | ", maxLengthInDataDatabase[0], "Имя", 3, " | ", maxLengthInDataDatabase[1], "Фамилия", 3, " | ", maxLengthInDataDatabase[2], "Отчество", 3, " | ", maxLengthInDataDatabase[3], "Табельный номер", 3, " | ", maxLengthInDataDatabase[4], "Год", 3, " | ", maxLengthInDataDatabase[5], "Месяц", 3, " | ", maxLengthInDataDatabase[6], "Количество проработанных часов");
				cout << " | Почасовой тариф" << endl;
			}

			find = true;
			outputOneData(i);
		}

	if (!find)
		cout << endl << "Совпадений не найдено" << endl;

	cout << "Хотите продолжить поиск или выйти в предыдущее меню? (0 - продолжить поиск, 1 - выход в предыдущее меню): ";
	inputBool() ? dataProcessing() : linearSearch();
}
void sort() {
	while (true) {
		system("cls");
		cout << "Сортировка данных" << endl;
		cout << "1 - Пузырьковая сортировка" << endl;
		cout << "2 - Быстрая сортировка" << endl;
		cout << "0 - Выход в предыдущее меню" << endl;
		cout << "Ваш выбор: ";
		switch (inputInt()) {
			case 1:
				sort_getParametr(true);
				break;
			case 2:
				sort_getParametr(false);
				break;
			case 0:
				dataProcessing();
			default: {
				cout << endl << "Вы ввели неправильный номер, попробуйте ещё раз";
				Sleep(5000);
			}
		}
	}
}

void viewData() {
	system("cls");
	cout << "Содержимое файла о сотрудниках:" << endl;
	outputData();
	cout << endl << "Нажмите Esc, чтобы выйти в предыдущее меню" << endl;
	while (true)
		if (_getch() == 27)
			if (roleOfUser)
				modifyData();
			else
				return;
}
void addData() {
	system("cls");
	cout << "Добавление сотрудника" << endl;
	addEmployee();
	Employees[countOfEmployees - 1].id = getFreeID();
	cout << "Введите имя: ";
	Employees[countOfEmployees - 1].name = input();
	cout << endl << "Введите фамилию: ";
	Employees[countOfEmployees - 1].surname = input();
	cout << endl << "Введите отчество: ";
	Employees[countOfEmployees - 1].middleName = input();
	for (int i = 0; i < countOfEmployees; i++)
		if (tolower(Employees[i].name) == tolower(Employees[countOfEmployees - 1].name) && tolower(Employees[i].surname) == tolower(Employees[countOfEmployees - 1].surname) && tolower(Employees[i].middleName) == tolower(Employees[countOfEmployees - 1].middleName))
			Employees[countOfEmployees - 1].tabelNumber = Employees[i].tabelNumber;

	Employees[countOfEmployees - 1].tabelNumber == -1 ? getFreeTabelNumber() : Employees[countOfEmployees - 1].tabelNumber;
	Employees[countOfEmployees - 1].year = inputYear_Data(getLocalYear());
	Employees[countOfEmployees - 1].month = inputMonth_Data();
	Employees[countOfEmployees - 1].countOfHours = inputCountOfHours_Data();
	Employees[countOfEmployees - 1].rate = inputRate_Data();
	editsInStructures = true;
	cout << endl << "Сотрудник успешно добавлен в систему";
	updateMaxLengthOfDataDatabase();
	Sleep(5000);
	modifyData();
}
void deleteData() {
	system("cls");
	cout << "Удаление сотрудника" << endl;
	outputData();
	int id = getID();
	countOfEmployees--;
	employee* temp = new employee[countOfEmployees];
	int j = 0;
	for (int i = 0; i <= countOfEmployees; i++) {
		if (Employees[i].id == id)
			continue;

		temp[j++] = Employees[i];
	}

	swap(temp, Employees);
	editsInStructures = true;
	cout << endl << "Удаление завершено";
	updateMaxLengthOfDataDatabase();
	Sleep(5000);
	modifyData();
}
void editData() {
	system("cls");
	cout << "Изменение данных сотрудника" << endl;
	outputData();
	int id = getID(), i;
	for (i = 0; i < countOfEmployees; i++)
		if (id == Employees[i].id)
			break;

	while (true) {
		int menu;
		cout << endl << "Выбранный сотрудник: " << endl;
		printf("%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s", to_string(countOfEmployees).length(), "#", 3, " | ", maxLengthInDataDatabase[0], "Имя", 3, " | ", maxLengthInDataDatabase[1], "Фамилия", 3, " | ", maxLengthInDataDatabase[2], "Отчество", 3, " | ", maxLengthInDataDatabase[3], "Табельный номер", 3, " | ", maxLengthInDataDatabase[4], "Год", 3, " | ", maxLengthInDataDatabase[5], "Месяц", 3, " | ", maxLengthInDataDatabase[6], "Количество проработанных часов");
		cout << " | Почасовой тариф" << endl;
		outputOneData(i);
		cout << "Выберите данные для изменения: " << endl << "1 - Имя" << endl << "2 - Фамилия" << endl << "3 - Отчество" << endl << "4 - Табельный номер" << endl << "5 - Год" << endl << "6 - Месяц" << endl << "7 - Количество проработанных часов" << endl << "8 - Почасовой тариф" << endl << "9 - Выбрать другого сотрудника" << endl << "0 - Выйти из данного режима" << endl << "Ваш вариант: ";
		switch (menu = inputInt()) {
			case 1: {
				cout << endl << "Введите имя: ";
				Employees[i].name = input();
			}
				  break;
			case 2: {
				cout << endl << "Введите фамилия: ";
				Employees[i].surname = input();
			}
				  break;
			case 3: {
				cout << endl << "Введите отчество: ";
				Employees[i].middleName = input();
			}
				  break;
			case 4: {
				while (true) {
					cout << endl << "Введите табельный номер: ";
					Employees[i].tabelNumber = inputInt();
					bool find = false;
					for (int j = 0; j < countOfEmployees; j++)
						if (Employees[i].tabelNumber == Employees[j].tabelNumber) {
							find = true;
							break;
						}

					if (!find)
						break;
					else
						cout << endl << "Вы ввели неправильный табельный номер, попробуйте ещё раз";
				}
			}
				break;
			case 5:
				Employees[i].year = inputYear_Data(getLocalYear());
				break;
			case 6:
				Employees[i].month = inputMonth_Data();
				break;
			case 7: 
				Employees[i].countOfHours = inputCountOfHours_Data();
				break;
			case 8:
				Employees[i].rate = inputRate_Data();
				break;
			case 9:
				editData();
			case 0:
				modifyData();
			default:
				cout << endl << "Вы ввели неправильный номер, попробуйте ещё раз";
		}

		if (menu > 0 && menu < 9) {
			cout << endl << "Запись о сотруднике успешно изменена";
			editsInStructures = true;
			updateMaxLengthOfDataDatabase();
			Sleep(5000);
		}
	}
}

void outputData() {
	printf("%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s%*s", to_string(countOfEmployees).length(), "#", 3, " | ", maxLengthInDataDatabase[0], "Имя", 3, " | ", maxLengthInDataDatabase[1], "Фамилия", 3, " | ", maxLengthInDataDatabase[2], "Отчество", 3, " | ", maxLengthInDataDatabase[3], "Табельный номер", 3, " | ", maxLengthInDataDatabase[4], "Год", 3, " | ", maxLengthInDataDatabase[5], "Месяц", 3, " | ", maxLengthInDataDatabase[6], "Количество проработанных часов");
	cout << " | Почасовой тариф" << endl;
	for (int i = 0; i < countOfEmployees; i++)
		outputOneData(i);
}
void outputOneData(int index) {
	printf("%*i%*s%*s%*s%*s%*s%*s%*s%*d%*s%*i%*s%*i%*s%*f", to_string(countOfEmployees).length(), Employees[index].id, 3, " | ", maxLengthInDataDatabase[0], Employees[index].name.c_str(), 3, " | ", maxLengthInDataDatabase[1], Employees[index].surname.c_str(), 3, " | ", maxLengthInDataDatabase[2], Employees[index].middleName.c_str(), 3, " | ", maxLengthInDataDatabase[3], Employees[index].tabelNumber, 3, " | ", maxLengthInDataDatabase[4], Employees[index].year, 3, " | ", maxLengthInDataDatabase[5], Employees[index].month, 3, " | ", maxLengthInDataDatabase[6], Employees[index].countOfHours);
	cout << " | " << Employees[index].rate << endl;
}
int getLocalYear() {
	time_t now = time(0);
	string localTime = string(ctime(&now));
	string year;
	for (int i = localTime.length() - 5; i < localTime.length(); i++)
		year.push_back(localTime[i]);

	return stoi(year);
}
string tolower(string line) {
	string result;
	for (int i = 0; i < line.length(); i++) {
		if ((line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 128 && line[i] <= 143)) {
			result.push_back(line[i] + 32);
			continue;
		}

		if ((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 160 && line[i] <= 175) || (line[i] >= 224 && line[i] <= 239) || line[i] == 241) {
			result.push_back(line[i]);
			continue;
		}

		if (line[i] >= 144 && line[i] <= 159) {
			result.push_back(line[i] + 80);
			continue;
		}

		if (line[i] == 240)
			result.push_back(line[i]);
	}

	return result;
}
int getFreeTabelNumber() {
	int* array = new int[countOfEmployees];
	for (int i = 0; i < countOfEmployees; i++)
		*(array + i) = Employees[i].tabelNumber;

	for (int i = 0; i < countOfEmployees - 1; i++)
		for (int j = i; j < countOfEmployees; j++)
			if (*(array + i) > *(array + j))
				swap(*(array + i), *(array + j));

	int j = 0;
	for (int i = 1; i <= INT_MAX; i++) {
		if (*(array + j) == i) {
			j++;
			continue;
		}

		delete[] array;
		return i;
	}
}
int getFreeID() {
	int* array = new int[countOfEmployees];
	for (int i = 0; i < countOfEmployees; i++)
		*(array + i) = Employees[i].id;

	for (int i = 0; i < countOfEmployees - 1; i++)
		for (int j = i; j < countOfEmployees; j++)
			if (*(array + i) > *(array + j))
				swap(*(array + i), *(array + j));

	int j = 0;
	for (int i = 1; i <= INT_MAX; i++) {
		if (*(array + j) == i) {
			j++;
			continue;
		}

		delete [] array;
		return i;
	}
}
void updateMaxLengthOfDataDatabase() {
	maxLengthInDataDatabase[0] = 3;
	maxLengthInDataDatabase[1] = 7;
	maxLengthInDataDatabase[2] = 8;
	maxLengthInDataDatabase[3] = 15;
	maxLengthInDataDatabase[4] = 3;
	maxLengthInDataDatabase[5] = 5;
	maxLengthInDataDatabase[6] = 30;
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < countOfEmployees; j++)
			switch (i) {
				case 0:
					maxLengthInDataDatabase[0] = maxLengthInDataDatabase[0] < Employees[j].name.length() ? Employees[j].name.length() : maxLengthInDataDatabase[0];
					break;
				case 1:
					maxLengthInDataDatabase[1] = maxLengthInDataDatabase[1] < Employees[j].surname.length() ? Employees[j].surname.length() : maxLengthInDataDatabase[1];
					break;
				case 2:
					maxLengthInDataDatabase[2] = maxLengthInDataDatabase[2] < Employees[j].middleName.length() ? Employees[j].middleName.length() : maxLengthInDataDatabase[2];
					break;
				case 3:
					maxLengthInDataDatabase[3] = maxLengthInDataDatabase[3] < to_string(Employees[j].tabelNumber).length() ? to_string(Employees[j].tabelNumber).length() : maxLengthInDataDatabase[3];
					break;
				case 4:
					maxLengthInDataDatabase[4] = maxLengthInDataDatabase[4] < to_string(Employees[j].year).length() ? to_string(Employees[j].year).length() : maxLengthInDataDatabase[4];
					break;
				case 5:
					maxLengthInDataDatabase[5] = maxLengthInDataDatabase[5] < to_string(Employees[j].month).length() ? to_string(Employees[j].month).length() : maxLengthInDataDatabase[5];
					break;
				case 6:
					maxLengthInDataDatabase[6] = maxLengthInDataDatabase[6] < to_string(Employees[j].countOfHours).length() ? to_string(Employees[j].countOfHours).length() : maxLengthInDataDatabase[6];
			}
}

double getSalary(int i, double countOfHours) {
	double salary = 0;
	if (countOfHours > 144) {
		salary += 2 * ((countOfHours - 144) * Employees[i].rate);
		salary += 144 * Employees[i].rate;
	}
	else
		salary += countOfHours * Employees[i].rate;

	salary -= salary * 0.12;
	return salary;
}
void sort_getParametr(bool typeOfSort /*1 - bubble, 0 - quick*/) {
	int criteria;
	while (true) {
		cout << (typeOfSort ? "Пузырьковая" : "Быстрая") << " сортировка | Выберите параметр" << endl;
		showCriteria();
		cout << "Ваш выбор: ";
		switch (criteria = inputInt()) {
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				break;
			default:
				cout << endl << "Вы ввели неправильный номер, попробуйте ещё раз";
		}

		if (criteria > 0 && criteria < 7)
			break;
	}

	typeOfSort ? bubbleSort(criteria) : quickSort(0, countOfEmployees - 1, criteria);
	cout << endl << "Сортировка выполнена" << endl;
	cout << "Хотите выйти в предыдущее меню или выполнить сортировку ещё раз? (0 - выход в предыдущее меню, 1 - меню выбора параметра сортировки): ";
	inputBool() ? sort_getParametr(typeOfSort) : sort();
}
void showCriteria() {
	cout << "1 - По номеру записи" << endl;
	cout << "2 - По табельному номеру" << endl;
	cout << "3 - По году" << endl;
	cout << "4 - По месяцу" << endl;
	cout << "5 - По количеству проработанных часов" << endl;
	cout << "6 - По почасовому тарифу" << endl;
}
void bubbleSort(int criteria) {
	for (int i = 0; i < countOfEmployees - 1; i++) {
		for (int j = i; j < countOfEmployees; j++) {
			bool needToSwap = false;
			switch (criteria) {
				case 1:
					needToSwap = Employees[i].id > Employees[j].id;
					break;
				case 2:
					needToSwap = Employees[i].tabelNumber > Employees[j].tabelNumber;
					break;
				case 3:
					needToSwap = Employees[i].year > Employees[j].year;
					break;
				case 4:
					needToSwap = Employees[i].month > Employees[j].month;
					break;
				case 5:
					needToSwap = Employees[i].countOfHours > Employees[j].countOfHours;
					break;
				case 6:
					needToSwap = Employees[i].rate > Employees[j].rate;
			}

			if (needToSwap)
				swap(Employees[i], Employees[j]);
		}
	}
}
void quickSort(int leftIndex, int rightIndex, int criteria) {
	if (rightIndex > leftIndex) {
		int middleIndex = getMiddleIndex(leftIndex, rightIndex, criteria);
		quickSort(leftIndex, middleIndex - 1, criteria);
		quickSort(middleIndex + 1, rightIndex, criteria);
	}
}
int getMiddleIndex(int leftIndex, int rightIndex, int criteria) {
	int middleIndex = rightIndex, i = leftIndex - 1;
	for (int j = leftIndex; j <= rightIndex - 1; j++) {
		bool needToSwap = false;
		switch (criteria) {
			case 1:
				needToSwap = Employees[j].id <= Employees[middleIndex].id;
				break;
			case 2:
				needToSwap = Employees[j].tabelNumber <= Employees[middleIndex].tabelNumber;
				break;
			case 3:
				needToSwap = Employees[j].year <= Employees[middleIndex].year;
				break;
			case 4:
				needToSwap = Employees[j].month <= Employees[middleIndex].month;
				break;
			case 5:
				needToSwap = Employees[j].countOfHours <= Employees[middleIndex].countOfHours;
				break;
			case 6:
				needToSwap = Employees[j].rate <= Employees[middleIndex].rate;
		}

		if (needToSwap)
			swap(Employees[j], Employees[++i]);
	}

	swap(Employees[++i], Employees[rightIndex]);
	return i;
}
string getTempParametr(employee employee, int criteria) {
	switch (criteria) {
		case 1:
			return to_string(employee.id);
		case 2:
			return to_string(employee.tabelNumber);
		case 3:
			return to_string(employee.year);
		case 4:
			return to_string(employee.month);
		case 5:
			return round(employee.countOfHours) == employee.countOfHours ? to_string(int(round(employee.countOfHours))) : to_string(employee.countOfHours);
		case 6:
			return round(employee.rate) == employee.rate ? to_string(int(round(employee.rate))) : to_string(employee.rate);
	}
}
int getID() {
	int id;
	while (true) {
		cout << "Введите номер записи: ";
		id = inputInt();
		if (id >= 0 && id <= getMaxID())
			for (int i = 0; i < countOfEmployees; i++)
				if (id == Employees[i].id)
					return id;

		cout << endl << "Вы ввели неверный номер, попробуйте ещё раз" << endl;
	}
}
int getMaxID() {
	int max = INT_MIN;
	for (int i = 0; i < countOfEmployees; i++)
		max = max < Employees[i].id ? Employees[i].id : max;

	return max;
}