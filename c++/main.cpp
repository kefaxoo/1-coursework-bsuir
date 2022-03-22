#include <iostream>
#include <fstream>
#include "Wstring.h"
#include "crypto.h"
#include <string>
#include <Windows.h>

using namespace std;

struct user {
	string login;
	string hashPassword;
	bool role; // 1 - admin, 0 - user
	bool access; // 1 - admin allows access to the system 0 - admin doesn't allow access to the system
};

user *Users;
int countOfUsers;

struct employee {
	string name;
	string surname;
	string middleName;
	int ID;
	int year;
	int month;
	int countOfHours;
	double rate;
};

user *addUser();
void openUserFile();
void exit();
void userControlPanel();
void adminMenu();
void login();
void showAllUsers();

int main () {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	openUserFile();
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

void openUserFile() {
	ifstream openFile("users.txt");
	if (!openFile.is_open()) {
		cout << "Файла с пользователями не существует" << endl;
		exit(0);
	}

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

void exit() {
	if (countOfUsers > 0)
		delete[] Users;

	exit(0);
}

void showAllUsers() {
	system("cls");
	bool showPasswords;
	cout << endl << "Показать пароли? (1 - да, 0 - нет): ";
	cin >> showPasswords;
	cout << endl << "# | Login | Passwords | Уровень доступа | Подтверждённый акканут" << endl;
	for (int i = 0; i < countOfUsers; i++) {
		cout << i + 1 << " | " << Users[i].login << " | ";
		if (showPasswords)
			cout << decrypt(Users[i].hashPassword);
		else
			for (int j = 0; j < Users[i].hashPassword.length(); j++)
				cout << "*";

		cout << " | ";
		if (Users[i].role)
			cout << "admin";
		else
			cout << "user";

		cout << " | ";
		if (Users[i].access)
			cout << "+";
		else
			cout << "-";

		cout << endl;
	}

	cout << endl << "Нажмите Esc, чтобы выйти в меню управления учётными записями" << endl;
	while (true)
		if (_getch() == 27)
			userControlPanel();
}


void userControlPanel() {
	system("cls");
	int menu;
	cout << endl << "Меню управления учётными записями" << endl;
	cout << "1 - Просмотр всех учётных записей" << endl;
	cout << "2 - Добавление новой учётной записи" << endl;
	cout << "3 - Редактирование учётной записи" << endl;
	cout << "4 - Удаление учётной записи" << endl;
	cout << "0 - Выйти в главное меню" << endl;
	cin >> menu;
	switch (menu) {
	case 1:
		showAllUsers();
		break;
	/*case 2:
		addUser();
		break;
	case 3:
		editUser();
		break;
	case 4:
		deleteUser();
		break;*/
	case 0:
		adminMenu();
		break;
	default:
		userControlPanel();
	}
}

void adminMenu() {
	system("cls");
	int menu;
	cout << endl << "Меню администратора" << endl;
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
		break;
	case 3:
		login();
		break;
	case 0:
		exit();
	default:
		adminMenu();
	}
}

void login() {
	system("cls");
	cout << "Введите имя пользователя: ";
	string login = input(), password;
	int i;
	bool find = true;
	while (true) {
		cout << endl << "Введите пароль: ";
		password = inputPassword();
		find = true;
		for (i = 0; i < countOfUsers; i++)
			if (login == Users[i].login)
				if (password == decrypt(Users[i].hashPassword))
					break;
				else {
					cout << endl << "Введён неправильный пароль. Попробуйте ещё раз!";
					find = false;
				}

		if (find)
			break;
	}

	if (find) {
		if (Users[i].access) {
			if (Users[i].role)
				adminMenu();
			//else
				//userMenu();
		}
		else {
			cout << endl << "Доступ запрещён";
			exit(0);
		}
	}
	else {
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