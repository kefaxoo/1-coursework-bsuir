#include <fstream>
#include <iostream>
#include <Windows.h>

#include "crypto.h"
#include "input.h"

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
	int tabelNumber;
	int year;
	int month;
	double countOfHours;
	double rate;
};

employee* Employees;
int countOfEmployees;
int sizeOfName = 0;
char *fileName;

void enableRussianLanguage(); // функция, в которой подключается поддержка русского языка на ввод-вывод консоли

void login(); // функция входа пользователя в систему

// блок функций, отвечающих за работу с файлом базы данных акканутов пользователей 
void openUserFile(); // функция, открывающая файл базы данных аккаунтов пользователей в систему
void createUserFile(); // функция, создающая файл базы данных аккаунтов пользователей
void toUserFile(); // функция, передающая в файл базу данных аккаунтов пользователей из памяти
void reopenUserFile(); // функция, переоткрывающая файл базы данных аккаунтов в систему

// блок функций, отвечающих за работу с массивом структур пользователей
void addAdmin_fileWasntExist(); // функция, создающая аккаунт администратора, если при запуске был создан файл базы данных акканутов пользователей
void cleanMemoryUsers(); // функция, которая чистит память массива структур аккаунтов пользователей
void addUser(); // функция, создающая в памяти компьютера новую структуру с предыдущими пользователями и новой структурой для нового пользователя

// блок функций, отвечающих за меню пользователей с разными правами
void adminMenu(); // функция, открывающая меню администратора
void userMenu(); // функция, открывающая меню пользователя

// блок функций, отвечающих за меню управления учётными записями
void userControlPanel(); // функция, открывающая меню управления учётными записями
void showAllUsers(); // функция, показывающая содержимое массива структур аккаунтов пользователей
void addUserInDatabase(); // функция, добавляющая нового пользователя в массив структур
void editUser(); // функция, изменяющая данные учётной записи
void deleteUser(); // функция, удаляющая пользователя из системы

// блок функций, отвечающих за работу с данными о сотрудниках
void dataAdmin(); // функция, открывающая меню администратора для работы с данными о сотрудниках

// блок функции-выводов
void outputUsers(); // функция, выводящая массив струтур аккаунтов в таблицу с шапкой
void outputUser(int, bool, int, int); // функция, выводящая отдельного пользователя

// блок функций-алгоритмов
int getMaxLenghtInUsersDatabase(int); // функция, возвращающая максимальную длину строки во всём массиве структур

// блок функций ввода с клавиатуры
int getIndexOfUser(); // функция, в которой пользователь выбирает учётную запись

void exit(); // функция, которая чистит память при завершении работы, если в ней что-то есть

void addEmployee();
void enterDataFileName();
void cleanMemoryData();
void openDataFile();
void createDataFile();
void toDataFile();

void modifyData();

bool isAdminEditsThemself(int); // функция, проверяющая не изменяет ли свои параметры админ

int main() {
	enableRussianLanguage();
}

void enableRussianLanguage() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
}

void login() {
	if (countOfUsers == 0 || Users == NULL)
		openUserFile();

	system("cls");
	cout << "Введите логин: ";
	string loginString = inputLogin(); // строка логина, введённого с клавиатуры
	string password; // строка пароля, введённого с клавиатуры
	int i; // целочисленная переменная для цикла и определённого аккаунта из массива структур
	bool find = true; // переменная, которая контролирует поиск аккаунта в массиве структур
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
				bool temp = inputBool(); // временная логическая переменная
				temp ? login() : exit(0);
			}
		}
		else {
			cout << endl << "Чтобы зарегестрироваться, необходимо разрешение администратора, хотите оставить заявку? (1 - да, 0 - нет): ";
			bool temp = inputBool(); // временная логическая переменная
			if (temp) {
				cout << endl << "Какие права хотите получить? (1 - администратор, 0 - пользователь): ";
				temp = inputBool();
				addUser();
				Users[countOfUsers - 1].login = loginString;
				Users[countOfUsers - 1].hashPassword = crypt(password);
				Users[countOfUsers - 1].role = temp;
				Users[countOfUsers - 1].access = false;
				toUserFile();
			}
			else
				exit(0);
		}
}

void openUserFile() {
	ifstream readFile(FILENAME_OF_USERS_DATABASE);
	if (!readFile.is_open()) {
		createUserFile();
		addAdmin_fileWasntExist();
	}
	else {
		cleanMemoryUsers();
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
	ofstream createFile(FILENAME_OF_USERS_DATABASE);
	createFile.close();
	cout << "Файл " << FILENAME_OF_USERS_DATABASE << " успешно создан" << endl;

}
void toUserFile() {
	ofstream writeFile(FILENAME_OF_USERS_DATABASE);
	for (int i = 0; i < countOfUsers; i++) {
		writeFile << Users[i].login << " " << Users[i].hashPassword << " " << Users[i].role << " " << Users[i].access << ";";
		if (i != countOfUsers - 1)
			writeFile << endl;
	}
}
void reopenUserFile() {
	cleanMemoryUsers();
	openUserFile();
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
void cleanMemoryUsers() {
	if (countOfUsers != 0 || Users != NULL) {
		countOfUsers = 0;
		delete[] Users;
		Users = NULL;
	}
}
void addUser() {
	user* temp = new user[++countOfUsers]; // временный массив структур аккаунтов пользователей
	for (int i = 0; i < countOfUsers - 1; i++)
		temp[i] = Users[i];

	swap(temp, Users);
	delete[] temp;
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

void cleanMemoryData() {
	if (countOfEmployees != 0 || Employees != NULL) {
		delete[] Employees;
		Employees = NULL;
		countOfEmployees = 0;
	}
}
void enterDataFileName() {
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
void addEmployee() {
	employee *temp = new employee[countOfEmployees];
	for (int i = 0; i < countOfEmployees - 1; i++)
		temp[i] = Employees[i];

	swap(temp, Employees);
	delete[] temp;
}
void openDataFile() {
	if (countOfEmployees != 0)
		cleanMemoryData();

	enterDataFileName();
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

				switch(count++) {
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
void createDataFile() {
	ofstream openFile(fileName);
	system("cls");
	cout << (openFile.is_open() ? "Файл БД успешно создан" : "Файл БД не создан") << endl;
	openFile.close();
	return;
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
	return;
}
void editUser() {
	system("cls");
	cout << "Редактирование учётной записи" << endl;
	outputUsers();
	int index = getIndexOfUser();
	cout << endl << "Показывать пароли? (1 - да, 0 - нет): ";
	bool showPasswords = inputBool();
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
	while (true) {
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
			}
				  break;
			case 2: {
				cout << endl << "Введите пароль: ";
				Users[index].hashPassword = crypt(inputPassword());
			}
				  break;
			case 3: {
				if (isAdminEditsThemself(index))
					break;

				cout << endl << "Введите уровень доступа (1 - admin, 0 - user): ";
				Users[index].access = inputBool();
			}
				  break;
			case 4: {
				if (isAdminEditsThemself(index))
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
	while (true) {
		bool temp;
		cout << "Хотите удалить ещё одного пользователя или выйти в предыдущее меню? (0 - удаление, 1 - выйти из режима): ";
		temp = inputBool();
		if (temp)
			return;
		else
			deleteUser();
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
	switch (criteria) {
	case 1: {
		for (int i = 0; i < countOfUsers; i++)
			max = max < Users[i].login.length() ? Users[i].login.length() : max;

		return max;
	}
		  break;
	case 2: {
		for (int i = 0; i < countOfUsers; i++)
			max = max < Users[i].hashPassword.length() ? Users[i].hashPassword.length() : max;

		return max;
	}
	}
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

void exit() {
	if (countOfUsers != 0 || Users != NULL)
		cleanMemoryUsers();

	if (countOfEmployees != 0 || Employees != NULL)
		cleanMemoryData();

	exit(0);
}
