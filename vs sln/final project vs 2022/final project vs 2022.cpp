#include <fstream>
#include <iomanip>
#include <iostream>
#include <Windows.h>

#include "input.h"
#include "crypto.h"

using namespace std;

struct user {
	string login; // строка для хранения логина пользователя
	string hashPassword; // строка для хранения шифрованного пароля пользователя
	bool role; // логическая переменная, хранящая права пользователя (1 - admin, 0 - user)
	bool access; // логическая переменная, хранящая возможность доступа в систему (1 - admin allows access to the system, 0 - admin doesn't allow access to the system)
};

int countOfUsers = 0; // целочисленная переменная, хранящая количество пользователей в системе
user *Users = NULL; // указатель типа user, который используется как динамический массив структур, в которой храним информацию об аккауте
const char FILENAME_OF_USERS_DATABASE[] = "users.txt"; // константный массив, хранящий название файла базы данных акканутов пользователей
bool edit = false; // логическая переменная, которая используется, чтобы проверить, что учётные записи в файле и учётные записи в массив структур отличаются

string loginOfUser; // строка, которая хранит логин пользователя, который вошёл в систему
bool roleOfUser; // логическая переменная, которая хранит права пользователя, который вошёл в систему

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

// блок функции-выводов
void outputUsers(); // функция, выводящая массив струтур аккаунтов в таблицу с шапкой
void outputUser(int, bool, int, int); // функция, выводящая отдельного пользователя

// блок функций-алгоритмов
int getMaxLenghtInUsersDatabase(int); // функция, возвращающая максимальную длину строки во всём массиве структур

// блок функций ввода с клавиатуры
int getIndexOfUser(); // функция, в которой пользователь выбирает учётную запись
bool getShowPassword(); // функция, в которой пользователь выбирает показ паролей

void exit(); // функция, которая чистит память при завершении работы, если в ней что-то есть

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
				bool temp; // временная логическая переменная
				cin >> temp;
				temp ? login() : exit(0);
			}
		} else {
			bool temp; // временная логическая переменная
			cout << endl << "Чтобы зарегестрироваться, необходимо разрешение администратора, хотите оставить заявку? (1 - да, 0 - нет): ";
			cin >> temp;
			if (temp) {
				cout << endl << "Какие права хотите получить? (1 - администратор, 0 - пользователь): ";
				cin >> temp;
				addUser();
				Users[countOfUsers - 1].login = loginString;
				Users[countOfUsers - 1].hashPassword = crypt(password);
				Users[countOfUsers - 1].role = temp;
				Users[countOfUsers - 1].access = false;
				toUserFile();
			} else
				exit(0);
		}
}

void openUserFile() {
	ifstream readFile(FILENAME_OF_USERS_DATABASE);
	if (!readFile.is_open()) {
		createUserFile();
		addAdmin_fileWasntExist();
	} else {
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
							Users[countOfUsers - 1].role = stoi(temp);
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
	toUserFile();
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
		delete [] Users;
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
		int menu;
		cout << "Меню администратора" << endl;
		cout << "1 - Управление учётными записями" << endl;
		cout << "2 - Режим работы с данными" << endl;
		cout << "3 - Выход из системы" << endl;
		cout << "0 - Завершение работы" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;
		system("cls");
		switch (menu) {
			case 1:
				userControlPanel();
				break;
			case 2:
				dataAdmin();
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

void userControlPanel() {
	system("cls");
	reopenUserFile();
	while (true) {
		int menu;
		cout << "Меню управления учётными записями" << endl;
		cout << "1 - Просмотр всех учётных записей" << endl;
		cout << "2 - Добавление новой учётной записи" << endl;
		cout << "3 - Редактирование учётной записи" << endl;
		cout << "4 - Удаление учётной записи" << endl;
		cout << "0 - Выйти в главное меню" << endl;
		cout << "Ваш выбор: ";
		cin >> menu;
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
			case 0:
				return;
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
	cin >> Users[countOfUsers - 1].role;
	Users[countOfUsers - 1].access = true;
	edit = true;
	return;
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
		int menu;
		cout << endl << "Выбранный пользователь:" << endl;
		cout << "# | " << setw(Users[index].login) << "Login | " << setw(Users[index].hashPassword) << " Password | Уровень доступа"  <<
		int maxLengthLogin = getMaxLenghtInUsersDatabase(1), maxLengthPassword = getMaxLenghtInUsersDatabase(2);
		cout << "# | " << setw(maxLengthLogin) << " Login | " << setw(maxLengthPassword) << " Password | Уровень доступа | Подтверждённый аккаунт" << endl;
		cout << index + 1 << " | " << setw(maxLengthLogin) << Users[index].login << " | " << setw(maxLengthPassword);
		if (showPasswords)
			cout << decrypt(Users[index].hashPassword);
		else
			for (int i = 0; i < Users[i].hashPassword.length(); i++)
				cout << "*";

		cout << " | " << setw(15) << (Users[index].role ? "admin" : "user") << setw(22) << (Users[index].access ? "+" : "-");

	}
}


void outputUsers() {
	bool showPasswords;
	cout << endl << "Показать пароли? (1 - да, 0 - нет: ";
	cin >> showPasswords;
	int maxLengthLogin = getMaxLenghtInUsersDatabase(1), maxLengthPassword = getMaxLenghtInUsersDatabase(2);
	cout << "# | " << setw(maxLengthLogin) << " Login | " << setw(maxLengthPassword) << " Password | Уровень доступа | Подтверждённый аккаунт" << endl;
	for (int i = 0; i < countOfUsers; i++)
		outputUser(i, showPasswords, maxLengthLogin, maxLengthPassword);
}
void outputUser(int index, bool showPasswords, int maxLengthLogin, int maxLengthPassword) {
	cout << index + 1 << " | " << setw(maxLengthLogin) << Users[index].login << " | " << setw(maxLengthPassword);
	if (showPasswords)
		cout << decrypt(Users[index].hashPassword);
	else
		for (int i = 0; i < Users[i].hashPassword.length(); i++)
			cout << "*";

	cout << " | " << setw(15) << (Users[index].role ? "admin" : "user") << setw(22) << (Users[index].access ? "+" : "-");
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
	int index;
	cout << "Выберите пользователя: ";
	while (true) {
		cin >> index;
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
		cleanMemoryEmployees();

	exit(0);
}
