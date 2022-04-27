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

void enableRussianLanguage(); // функция, подключающая поддержку русского языка в консоли
void login(); // функция авторизации в систему
void exit(); // функция завершения работы с системой

// блок функций, отвечающий за работу с файлом и памятью учетных записей пользователей
void cleanMemoryUsers(); // функция, очищающая память учетных записей пользователей
void addAdmin_fileWasntExist(); // функция, добавляющая в только что созданный файл учетных записей пользователя учетную запись администратора
void addUser(); // функция, добавляющая новый пустой элемент в динамический массив структур
void openUserFile(); // функция, открывающая файл с учетными записями пользователей и переносит их в память
void createUserFile(); // функция, создающая файл с учетными записями пользователей, если он отсутствует
void toUserFile(); // функция, передающая учетные записи пользователей из системы в файл

// блок функций-модулей пользователей разных прав доступа к системе
void adminMenu(); // функция-модуль администратора
void userMenu(); // функция-модуль пользователь

// блок функций, отвечающий за работу с файлом и памятью сотрудников
void enterDataFilename(); // функция, в которой пользователь вводит название файла с сотрудниками
void openDataFile(); // функция, открывающая файл с сотрудникми и переносит их в память
void cleanMemoryData(); // функция, очищающая память сотрудников
void createDataFile(); // функция, создающая файл с сотрудниками, если он отсутствует
void addEmployee(); // функция, добавляющая новый пустой элемент в динамический массив структур

// блок функций-подмодулей администратора
void userControlPanel(); // функция-подмодуль управления учетными записями
void dataAdmin(); // функция-подмодуль работы с данными о сотрудниках

// блок функций-подмодулей управления учетными записями

// блок функций-подмодулей работы с данными о сотрудниках




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
					cout << "Введён неправильный пароль, попробуйте ещё раз";
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
				inputBool() ? login() : exit(0);
			}
		}
		else {
			cout << endl << "Чтобы зарегестрироваться, необходимо разрешение администратора, хотите оставить заявку? (1 - да, 0 - нет): ";
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
				exit(0);
		}
}
void exit() {
	if (countOfUsers != 0 || Users != NULL)
		cleanMemoryUsers();

	if (countOfEmployees != 0 || Employees != NULL)
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
					default:
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
}
void createUserFile() {
	ofstream createFile(FILENAME_OF_USERS_DATABASE); // при стандартном открытии файла в режиме записи, если файл отсутствует, то он создаётся
	createFile.close();
	ifstream checkIfCreatedFileExists(FILENAME_OF_USERS_DATABASE);
	cout << "Файл " << FILENAME_OF_USERS_DATABASE << (checkIfCreatedFileExists.is_open() ? " успешно создан" : " не был создан") << endl; // проверка и вывод сообщения об удачном или нет создании файла
	Sleep(5000);
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
	system("cls");
	while (true) {
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
			editsInStructures = false;
			userControlPanel();
		}
			  break;
		case 2: {
			if (sizeOfName == 0)
				enterDataFilename();

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
			break;
		default:
			cout << "Выбран неправильный номер, попробуйте ещё раз" << endl;
		}
	}
}
void userMenu() {
	if (sizeOfName == 0)
		enterDataFilename();

	openDataFile();

	while (true) {
		system("cls");
		int menu;
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
			enterDataFilename();
			cleanMemoryData();
			openDataFilename();
		}
			  break;
		case 4: {
			sizeOfName = 0;
			delete[] fileName;
			fileName = NULL;
			login();
		}
			  break;
		case 5:
			exit();
		}
	}
}

void enterDataFilename() {
	system("cls");
	cout << "Введите название файла: ";
	string temp;
	temp.append(input() + ".txt");
	bool fileNameWasInSystem = sizeOfName == 0 ? false : true;
	sizeOfName = temp.length();
	if (fileNameWasInSystem)
		delete[] fileName;

	fileName = new char[sizeOfName];
	strcpy(fileName, temp.c_str());
}
void openDataFile() {
	if (countOfEmployees != 0)
		cleanMemoryData();

	enterDataFilename();
	ifstream openFile(fileName);
	system("cls");
	if (!openFile.is_open()) {
		cout << "Файла с информацией о сотрудниках не существует" << endl;
		cout << "Хотите создать? (1 - да, 0 - нет): ";
		bool menu = inputBool();
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
		addEmployee();
		string temp;
		for (int i = 0; i < 500 || line[i] != '\0'; i++) {
			if (line[i] == '|' || line[i] == ';') {
				if (count > 3 && temp == "-")
					temp = "0";

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
	cout << "Файл успешно открыт" << endl;
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
	employee* temp = new employee[countOfEmployees];
	for (int i = 0; i < countOfEmployees - 1; i++)
		temp[i] = Employees[i];

	swap(temp, Employees);
	delete[] temp;
}

void userControlPanel() {
	system("cls");
	reopenUserFile();
	while (true) {
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
				bool temp = inputBool();
				if (temp)
					toDataFile();

				cleanMemoryData();
				editsInStructures = false;
			}

			return;
		}
		default:
			cout << "Выбран неправильный номер, попробуйте ещё раз" << endl;
		}
	}
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
	return;
}
void editUser() {
	system("cls");
	cout << "Редактирование учётной записи" << endl;
	outputUsers();
	int index = getIndexOfUser();
	cout << endl << "Показывать пароли? (1 - да, 0 - нет): ";
	bool showPasswords = inputBool();
	while (true) {
		cout << endl << "Выбранный пользователь:" << endl;
		printf("%*s", to_string(index + 1).length(), "# |");
		printf("%*s", Users[index].login.length(), " Login |");
		printf("%*s", Users[index].hashPassword.length(), " Password |");
		cout << " Уровень доступа | Доступ к системе" << endl;
		cout << index + 1 << " | " << Users[index].login << " | ";
		if (showPasswords)
			cout << decrypt(Users[index].hashPassword);
		else
			for (int i = 0; i < Users[i].hashPassword.length(); i++)
				cout << "*";

		cout << " | " << (Users[index].role ? "администратор  " : "пользователь   ") << " | " << (Users[index].access ? "+" : "-") << endl;
		cout << "Выберите параметр, который хотите изменить:" << endl;
		cout << "1 - Логин" << endl;
		cout << "2 - Пароль" << endl;
		cout << "3 - Уровень доступа" << endl;
		cout << "4 - Доступ к системе" << endl;
		cout << "5 - Выбрать другую учётную запись" << endl;
		cout << "6 - Выход в предыдущее меню" << endl;
		cout << "Ваш выбор: ";
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

				cout << endl << "Пользователь успешно изменён";
				Sleep(5000);
			}
				  break;
			case 2: {
				cout << endl << "Введите пароль: ";
				Users[index].hashPassword = crypt(inputPassword());
				cout << endl << "Пользователь успешно изменён";
				Sleep(5000);
			}
				  break;
			case 3: {
				if (isAdminEditsThemself(index))
					break;

				cout << endl << "Введите уровень доступа (1 - admin, 0 - user): ";
				Users[index].access = inputBool();
				cout << endl << "Пользователь успешно изменён";
				Sleep(5000);
			}
				  break;
			case 4: {
				if (isAdminEditsThemself(index))
					break;

				cout << endl << "Введите доступ к системе (1 - доступ разрешён, 0 - доступ запрещён): ";
				Users[index].access = inputBool();
				cout << endl << "Пользователь успешно изменён";
				Sleep(5000);
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
	}
}
void deleteUser() {
	system("cls");
	cout << "Удаление пользователя" << endl;
	outputUsers();
	int index = getIndexOfUser();
	editsInStructures = true;
	countOfUsers--;
	user *temp = new user[countOfUsers];
	int j = 0;
	for (int i = 0; i <= countOfUsers; i++) {
		if (i == index)
			continue;

		temp[j++] = Users[i];
	}

	swap(temp, Users);
	delete[] temp;
	cout << endl << "Удаление завершено" << endl;
	Sleep(5000);
	return;
}

void outputData() {
	int length[5] = { getMaxLenghtInUsersDatabase(1), getMaxLenghtInUsersDatabase(2), getMaxLenghtInUsersDatabase(3), getMaxLenghtInUsersDatabase(4), getMaxLenghtInUsersDatabase(5) };
	printf("%*s", length[0], "#");
	cout << " | ";
	printf("%*s", length[1], "Имя");
	cout << " | ";
	printf("%*s", length[2], "Фамилия");
	cout << " | ";
	printf("%*s", length[3], "Отчество");
	cout << " | Табельный номер | "; // 15 chars
	printf("%*s", length[4], "Год");
	cout << " | Месяц | Количество проработанных часов | Почасовой тариф" << endl; //|5|30|14 chars
	for (int i = 0; i < countOfEmployees; i++) {
		printf("%*s", length[0], Employees[i].id);
		cout << " | ";
		printf("%*s", length[1], Employees[i].name);
		cout << " | ";
		printf("%*s", length[2], Employees[i].surname);
		cout << " | ";
		printf("%*s", length[3], Employees[i].middleName);
		cout << " | ";
		printf("%*s", 15, Employees[i].tabelNumber);
		cout << " | ";
		printf("%*s", length[4], Employees[i].year);
		cout << " | ";
		printf("%*s", 5, Employees[i].month);
		cout << " | ";
		printf("%*s", 30, Employees[i].countOfHours);
		cout << " | " << Employees[i].rate << endl;
	}
}

void viewData() {
	system("cls");
	cout << "Содержимое файла о сотрудниках:" << endl;
	outputData();
	cout << endl << "Нажмите Esc, чтобы выйти в предыдущее меню" << endl;
	while (true)
		if (_getch() == 27)
			roleOfUser ? modifyData() : userMenu();
}

int getFreeID() {
	int *array = new int[countOfEmployees];
	for (int i = 0; i < countOfEmployees; i++)
		*(array + i) = Employees[i].id;

	for (int i = 0; i < countOfEmployees - 1; i++)
		for (int j = i; j < countOfEmployees; j++)
			if (*(array + i) > *(array + j))
				swap(*(array + i), *(array + j));

	int j = 0;
	for (int i = 0; i < INT_MAX; i++) {
		if (*(array + j) == i) {
			j++;
			continue;
		}

		delete [] array;
		return i;
	}
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
	for (int i = 0; i < INT_MAX; i++) {
		if (*(array + j) == i) {
			j++;
			continue;
		}

		delete[] array;
		return i;
	}
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

int getLocalYear() {
	time_t now = time(0);
	string localTime = string(ctime(&now));
	string year;
	for (int i = localTime.length() - 5; i < localTime.length(); i++)
		year.push_back(localTime[i]);

	return stoi(year);
}

void addData() {
	system("cls");
	cout << "Добавление сотрудника" << endl;
	countOfEmployees++;
	addEmployee();
	Employees[countOfEmployees - 1].id = getFreeID();
	cout << "Введите имя: ";
	Employees[countOfEmployees - 1].name = input();
	cout << endl << "Введите фамилию: ";
	Employees[countOfEmployees - 1].surname = input();
	cout << endl << "Введите отчество: ";
	Employees[countOfEmployees - 1].middleName = input();
	for (int i = 0; i < countOfEmployees; i++) {
		if (tolower(Employees[i].name) == tolower(Employees[countOfEmployees - 1].name) && tolower(Employees[i].surname) == tolower(Employees[countOfEmployees - 1].surname) && tolower(Employees[i].middleName) == tolower(Employees[countOfEmployees - 1].middleName))
			Employees[countOfEmployees - 1].tabelNumber = Employees[i].tabelNumber;
	}

	Employees[countOfEmployees - 1].tabelNumber == -1 ? getFreeTabelNumber() : Employees[countOfEmployees - 1].tabelNumber;
	while (true) {
		cout << endl << "Введите год: ";
		Employees[countOfEmployees - 1].year = inputInt();
		if (Employees[countOfEmployees - 1].year > -1 && Employees[countOfEmployees - 1].year <= getLocalYear())
			break;
		else
			cout << endl << "Вы ввели неправильный год, попробуйте ещё раз";
	}

	while (true) {
		cout << endl << "Введите месяц: ";
		Employees[countOfEmployees - 1].month = inputInt();
		if (Employees[countOfEmployees - 1].month > 0 && Employees[countOfEmployees - 1].month < 13)
			break;
		else
			cout << endl << "Вы ввели неправильный месяц, попробуйте ещё раз";
	}

	while (true) {
		cout << endl << "Введите количество проработанных часов за месяц: ";
		Employees[countOfEmployees - 1].countOfHours = inputDouble();
		if (Employees[countOfEmployees - 1].countOfHours > -1)
			break;
		else
			cout << endl << "Вы ввели неправильное значение, попробуйте ещё раз";
	}

	while (true) {
		cout << endl << "Введите почасовой тариф: ";
		Employees[countOfEmployees - 1].rate = inputDouble();
		if (Employees[countOfEmployees - 1].rate > -1)
			break;
		else
			cout << endl << "Вы ввели неправильное значение, попробуйте ещё раз";
	}

	editsInStructures = true;
	cout << endl << "Сотрудник успешно добавлен в систему";
	Sleep(5000);
	return;
}

int getMaxID() {
	int max = INT_MIN;
	for (int i = 0; i < countOfEmployees; i++)
		max = max < Employees[i].id ? Employees[i].id : max;

	return max;
}

int getID() {
	int id;
	while (true) {
		cout << "Введите номер записи: ";
		id = inputInt();
		if (id >= 0 && id <= getMaxID())
			return id;
		else
			cout << endl << "Вы ввели неверный номер, попробуйте ещё раз";
	}
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
		if (Employees[i].id = id)
			continue;

		temp[j++] = Employees[i];
	}

	swap(temp, Employees);
	delete[] temp;
	editsInStructures = true;
	cout << endl << "Удаление завершено";
	Sleep(5000);
	return;
}

void editData() {
	system("cls");
	cout << "Изменение данных сотрудника" << endl;
	outputData();
	int id = getID(), i;
	for (int i = 0; i < countOfEmployees; i++)
		if (id = Employees[i].id)
			break;

	while (true) {
		int menu;
		cout << endl << "Выбранный сотрудник: " << endl;
		printf("%*s", to_string(Employees[i].id).length(), "#");
		cout << " | ";
		printf("%*s", Employees[i].name.length(), "Имя");
		cout << " | ";
		printf("%*s", Employees[i].surname.length(), "Фамилия");
		cout << " | ";
		printf("%*s", Employees[i].middleName.length(), "Отчество");
		cout << " | ";
		printf("%*s", to_string(Employees[i].tabelNumber).length(), "Табельный номер");
		cout << " | ";
		printf("%*s", to_string(Employees[i].year).length(), "Год");
		cout << " | ";
		printf("%*s", to_string(Employees[i].id).length(), "Месяц");
		cout << " | Количество проработанных часов | Почасовой тариф" << endl; // 30 - chars
		cout << Employees[i].name << " | " << Employees[i].surname << " | " << Employees[i].middleName << " | " << Employees[i].id << " | " << Employees[i].year << " | " << Employees[i].month << " | ";
		printf("%*s", 30, Employees[i].countOfHours);
		cout << " | " << Employees[i].rate << endl;
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
			case 5: {
				while (true) {
					cout << endl << "Введите год: ";
					Employees[i].year = inputInt();
					if (Employees[i].year > -1 && Employees[i].year <= getLocalYear())
						break;
					else
						cout << endl << "Вы ввели неправильный год, попробуйте ещё раз";
				}
			}
				break;
			case 6: {
				while (true) {
					cout << endl << "Введите месяц: ";
					Employees[i].month = inputInt();
					if (Employees[i].month > 0 && Employees[i].month < 13)
						break;
					else
						cout << endl << "Вы ввели неправильный месяц, попробуйте ещё раз";
				}
			}
				break;
			case 7: {
				while (true) {
					cout << endl << "Введите количество проработанных часов: ";
					Employees[i].countOfHours = inputDouble();
					if (Employees[i].countOfHours >= 0)
						break;
					else
						cout << endl << "Вы ввели неправильное количество проработанных часов, попробуйте ещё раз";
				}
			}
				break;
			case 8: {
				while (true) {
					cout << endl << "Введите почасовой тариф: ";
					Employees[i].rate = inputDouble();
					if (Employees[i].rate >= 0)
						break;
					else
						cout << endl << "Вы ввели неправильный почасовой тариф, попробуйте ещё раз";
				}
			}
				break;
			case 9:
				editData();
			case 0: {
				modifyData();
			default:
				cout << endl << "Вы ввели неправильный номер, попробуйте ещё раз";
			}
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
				return;
			default:
				cout << endl << "Выбран неправильный номер, попробуйте ещё раз" << endl;
		}
	}
}

double getSalary (int i, double countOfHours) {
	double salary = 0;
	if (countOfHours > 144) {
		salary += 2 * ((countOfHours - 144) * Employees[i].rate);
		salary += 144 * Employees[i].rate;
	} else
		salary += countOfHours * Employees[i].rate;

	salary -= salary * 0.12;
	return salary;
}

void salary() {
	while (true) {
		system("cls");
		cout << "Расчёт заработной платы" << endl;
		outputData();
		while (true) {
			int id = getID(), i;
			for (int i = 0; i < countOfEmployees; i++)
				if (id == Employees[i].id)
					break;

			while (true) {
				system("cls");
				cout << endl << "Выбранный сотрудник: " << endl;
				printf("%*s", to_string(Employees[i].id).length(), "#");
				cout << " | ";
				printf("%*s", Employees[i].name.length(), "Имя");
				cout << " | ";
				printf("%*s", Employees[i].surname.length(), "Фамилия");
				cout << " | ";
				printf("%*s", Employees[i].middleName.length(), "Отчество");
				cout << " | ";
				printf("%*s", to_string(Employees[i].tabelNumber).length(), "Табельный номер");
				cout << " | ";
				printf("%*s", to_string(Employees[i].year).length(), "Год");
				cout << " | ";
				printf("%*s", to_string(Employees[i].id).length(), "Месяц");
				cout << " | Количество проработанных часов | Почасовой тариф" << endl; // 30 - chars
				cout << Employees[i].name << " | " << Employees[i].surname << " | " << Employees[i].middleName << " | " << Employees[i].id << " | " << Employees[i].year << " | " << Employees[i].month << " | ";
				printf("%*s", 30, Employees[i].countOfHours);
				cout << " | " << Employees[i].rate << endl;
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
				temp = inputBool();
				if (temp)
					return;
				else
					break;
			}
		}
	}
}

void showCriteria() {
	cout << "1 - По номеру записи" << endl;
	cout << "2 - По табельному номеру" << endl;
	cout << "3 - По году" << endl;
	cout << "4 - По месяцу" << endl;
	cout << "5 - По количеству проработанных часов" << endl;
	cout << "6 - По почасовому тарифу" << endl;
	cout << "Ваш выбор: ";
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

void bubbleSort_getParametr() {
	int criteria;
	while (true) {
		system("cls");
		cout << "Пузырьковая сортировка | Выберите параметр:";
		showCriteria();
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

	bubbleSort(criteria);
	cout << endl << "Сортировка выполнена" << endl;
	cout << "Хотите выйти в предыдущее меню или выполнить сортировку ещё раз? (0 - выход в предыдущее меню, 1 - меню выбора параметра сортировки): ";
	inputInt() ? bubbleSort_getParametr() : sort();
}

int getMiddleIndex(int leftIndex, int rightIndex, int criteria) {
	int temp, middleIndex = rightIndex, i = leftIndex - 1;
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

		if (needToSwap) {
			i++;
			swap(Employees[j], Employees[i]);
		}
	}

	i++;
	swap(Employees[i], Employees[rightIndex]);
	return i;
}

void quickSort(int leftIndex, int rightIndex, int criteria) {
	if (rightIndex > leftIndex) {
		int middleIndex = getMiddleIndex(leftIndex, rightIndex, criteria);
		quickSort(leftIndex, middleIndex - 1, criteria);
		quickSort(middleIndex + 1, rightIndex, criteria);
	}
}

void quickSort_getParametr() {
	int criteria;
	while (true) {
		system("cls");
		cout << "Быстрая сортировка | Выберите параметр" << endl;
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

	quickSort(0, countOfEmployees - 1, criteria);
	cout << endl << "Сортировка выполнена" << endl;
	cout << "Хотите выйти в предыдущее меню или выполнить сортировку ещё раз? (0 - выход в предыдущее меню, 1 - меню выбора параметра сортировки): ";
	inputInt() ? quickSort_getParametr() : sort();
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
				bubbleSort_getParametr();
				break;
			case 2:
				quickSort_getParametr();
				break;
			case 3:
				dataProcessing();
			default: {
				cout << endl << "Вы ввели неправильный номер, попробуйте ещё раз";
				Sleep(5000);
			}
		}
	}
}

string getTempParametr (employee employee, int criteria) {
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

void linearSearch() {
	int criteria;
	while (true) {
		system("cls");
		cout << "Линейный поиск" << endl;
		showCriteria();
		cout << "0 - Выход в предыдущее меню" << endl;
		cout << "Ваш выбор: ";
		switch (criteria = inputInt()) {
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				break;
			case 0:
				search();
			default:
				cout << endl << "Вы ввели неправильный номер, попробуйте ещё раз";
		}

		if (criteria > 0 && criteria < 7)
			break;
	}

	
	string criteriaString;
	cout << "Введите параметр поиска: ";
	if (criteria > 0 && criteria < 5)
		criteriaString = to_string(inputInt());
	else {
		criteriaString = to_string(inputDouble());
		if (stod(criteriaString) - stoi(criteriaString) == 0)
			criteriaString = to_string(stoi(criteriaString));
	}

	bool find = false;
	for (int i = 0; i < countOfEmployees; i++) {
		bool needToShow = false;
		if (criteriaString == getTempParametr(Employees[i], criteria)) {
			if (!find)
				cout << endl << "# | Имя | Фамилия | Отчество | Табельный номер | Год | Месяц | Количество проработанных часов | Почасовой тариф" << endl;

			find = true;
			cout << Employees[i].id << " | " << Employees[i].name << " | " << Employees[i].surname << " | " << Employees[i].middleName << " | " << Employees[i].tabelNumber << " | " << Employees[i].year << " | " << Employees[i].month << " | " << Employees[i].countOfHours << " | " << Employees[i].rate << endl;
		}
	}

	if (!find)
		cout << endl << "Совпадений не найдено" << endl;

	cout << "Хотите продолжить поиск или выйти в предыдущее меню? (0 - продолжить поиск, 1 - выход в предыдущее меню): ";
	inputBool() ? linearSearch() : dataProcessing();
}

void dataProcessing() {
	while (true) {
		system("cls");
		int menu;
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


bool isAdminEditsThemself(int index) {
	bool role;
	for (int i = 0; i < countOfUsers; i++)
		if (i == index && loginOfUser == Users[i].login && Users[i].role)
			role = true;

	cout << endl << "Вам запрещено запрещать себе доступ к системе" << endl;
	return role;
}

void outputUsers() {
	cout << endl << "Показать пароли? (1 - да, 0 - нет: ";
	bool showPasswords = inputBool();
	int maxLengthLogin = getMaxLenghtInUsersDatabase(1), maxLengthPassword = getMaxLenghtInUsersDatabase(2);
	printf("%*s", to_string(countOfUsers).length(), "# |");
	printf("%*s", maxLengthLogin, " Login |");
	printf("%*s", maxLengthPassword, " Password |");
	cout << " Уровень доступа | Доступ к системе" << endl;
	for (int i = 0; i < countOfUsers; i++)
		outputUser(i, showPasswords, maxLengthLogin, maxLengthPassword);
}
void outputUser(int index, bool showPasswords, int maxLengthLogin, int maxLengthPassword) {
	printf("%*s", to_string(countOfUsers + 1), index + 1);
	cout << " | ";
	printf("%*s", maxLengthLogin, Users[index].login);
	cout << " | ";
	string password;
	if (showPasswords)
		password = decrypt(Users[index].hashPassword);
	else
		for (int i = 0; i < Users[i].hashPassword.length(); i++)
			password.push_back('*');

	printf("%*s", maxLengthPassword, password);
	cout << " | " << (Users[index].role ? "администратор  " : "пользователь   ") << " | " << (Users[index].access ? "+" : "-") << endl;
}

int getMaxLenghtInUsersDatabase(int criteria) {
	int max = INT_MIN;
	for (int i = 0; i < countOfUsers; i++)
		switch (criteria) {
			case 1:
				max = max < Users[i].login.length() ? Users[i].login.length() : max;
				break;
			case 2:
				max = max < Users[i].hashPassword.length() ? Users[i].hashPassword.length() : max;
		}

	return max;
}
int getMaxLengthInDataDatabase(int criteria) {
	int max = INT_MIN;
	for (int i = 0; i < countOfEmployees; i++)
		switch (criteria) {
			case 1:	
				max = max < to_string(Employees[i].id).length() ? to_string(Employees[i].id).length() : max;
				break;
			case 2:
				max = max < Employees[i].name.length() ? Employees[i].name.length() : max;
				break;
			case 3:
				max = max < Employees[i].surname.length() ? Employees[i].surname.length() : max;
				break;
			case 4:
				max = max < Employees[i].middleName.length() ? Employees[i].middleName.length() : max;
				break;
			case 5:
				max = max < to_string(Employees[i].year).length() ? to_string(Employees[i].year).length() : max;
		}

	return max;
}

int getIndexOfUser() {
	cout << "Выберите пользователя: ";
	while (true) {
		int index = inputInt();
		--index;
		if (index >= 0 && index < countOfUsers)
			return index;
		else
			cout << endl << "Введён неправильный номер пользователя, попробуйте ещё раз" << endl;
	}
}


