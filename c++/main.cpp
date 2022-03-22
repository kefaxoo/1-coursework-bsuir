#include <iostream>
#include <fstream>
#include "Wstring.h"
#include "crypto.h"
#include <string>

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

user *addUser () {
	user *newArray = new user[countOfUsers];
	for (int i = 0; i < countOfUsers - 1; i++)
		newArray[i] = Users[i];

	swap(newArray, Users);
	delete [] newArray;
	return Users;
}

void openUserFile () {
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
			if (line[i] == '|' || line[i] == ';') {
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

void login () {
	cout << "Введите имя пользователя: ";
	string login = input(), password;
	int i;
	while (true) {
		cout << endl << "Введите пароль: ";
		password = input();
		for (i = 0; i < countOfUsers; i++)
			if (login == Users[i].login) {
				if (password == decrypt(Users[i].hashPassword))
					break;
				else
					cout << endl << "Введён неправильный пароль. Попробуйте ещё раз!";
			}
	}

	if (i != countOfUsers - 1) {
		if (Users[i].access)
			if (Users[i].role)
				cout << endl << "admin" << endl;
			else
				cout << endl << "user" << endl;
	} else {
		bool temp;
		cout << endl << "Чтобы зарегестрироваться, необходимо разрешение администратора. Хотите оставить заявку? (1 - да, 0 - нет): ";
		cin >> temp;
		if (temp) {
			cout << endl << "Какие права хотите получить? (1 - администратор, 0 - пользователь): ";
			cin >> temp;
			ofstream openFile("users.txt", ios::app);
			openFile << endl << login << "|" << crypt(password) << "|" << temp << "|" << 0 << ";";
			openFile.close();
		}
	}
}

int main () {
	setlocale(LC_CTYPE, "RUSSIAN");
	openUserFile();
	login();
}