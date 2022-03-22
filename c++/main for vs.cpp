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
void cleanMemoryUsers();

void outputUser(int, bool); 
void outputUsers();
int getIndexOfUser();

void showAllUsers();
void addUserInDatabase();
void editUser();
void deleteUser();
void userControlPanel();

void adminMenu();

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

void openUserFile() {
	ifstream openFile("users.txt");
	if (!openFile.is_open()) {
		ofstream createFile("users.txt");

	}

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
void cleanMemoryUsers () {
	if (countOfUsers > 0) {
		countOfUsers = 0;
		user *temp = new user[countOfUsers];
		swap(temp, Users);
		delete [] temp;
	}
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
	cout << endl << "�������� ������? (1 - ��, 0 - ���): ";
	cin >> showPasswords;
	cout << endl << "# | Login | Passwords | ������� ������� | ������������� �������" << endl;
	for (int i = 0; i < countOfUsers; i++)
		outputUser(i, showPasswords);
}
int getIndexOfUser() {
	int index = -1;
	cout << "�������� ������������: ";
	while (!(index >= 0 && index <= countOfUsers))
		cin >> index;

	index--;
	return index;
}

void showAllUsers () {
	system("cls");
	cout << "����� ���������: ";
	outputUsers();
	cout << endl << "������� Esc, ����� ����� � ���� ���������� �������� ��������" << endl;
	while (true)
		if (_getch() == 27)
			userControlPanel();
}
void addUserInDatabase() {
	system("cls");
	ofstream openFile("users.txt", ios::app);
	cout << "���������� ������������" << endl;
	string login;
	while (true) {
		cout << "������� �����: ";
		login = input();
		bool find = false;
		for (int i = 0; i < countOfUsers; i++)
			if (login == Users[i].login) {
				cout << endl << "������� ���������� �����" << endl;
				find = true;
			}

		if (!find)
			break;
	}
	openFile << endl << login << " ";
	cout << endl << "������� ������: ";
	openFile << crypt(input()) << " ";
	cout << endl << "�������� ������� ������� (1 - admin, 0 - user): ";
	bool temp;
	cin >> temp;
	openFile << temp << " 1;";
	openFile.close();
	openUserFile();
	userControlPanel();
}
void editUser() {
	system("cls");
	cout << "�������������� ������� ������" << endl;
	outputUsers();
	int index = getIndexOfUser();
	bool showPasswords;
	cout << endl << "���������� ������? (1 - ��, 0 - ���): ";
	cin >> showPasswords;
	while (true) {
		system("cls");
		int menu;
		cout << "��������� ������������: " << endl;
		cout << "# | Login | Passwords | ������� ������� | ������������� �������" << endl;
		outputUser(index, showPasswords);
		cout << "�������� �������� ��� ���������: " << endl;
		cout << "1 - Login" << endl;
		cout << "2 - Password" << endl;
		cout << "3 - ������� �������" << endl;
		cout << "4 - ����������� �������" << endl;
		cout << "5 - ������� ������� ������������" << endl;
		cout << "0 - ����� �� ������ ��������������" << endl;
		cout << "��� �����: ";
		cin >> menu;
		switch (menu) {
			case 1: {
				cout << endl << "������� ����� �����: ";
				Users[index].login = input();
			}
				break;
			case 2: {
				cout << endl << "������� ����� ������: ";
				Users[index].hashPassword = crypt(input());
			}
					break;
			case 3: {
				cout << endl << "������� ������� ������� (1 - admin, 0 - user): ";
				cin >> Users[index].role;
			}
					break;
			case 4: {
				cout << endl << "������� ������ �������� (1 - �������������, 0 - ���������������): ";
				cin >> Users[index].access;
			}
				break;
			case 5:
				editUser();
				break;
			case 0:
				userControlPanel();
		}
	}
}
void deleteUser() {
	system("cls");
	cout << "�������� ������������" << endl;
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
	cout << "�������� ���������" << endl;
	userControlPanel();
}
void userControlPanel() {
	system("cls");
	int menu;
	cout << "���� ���������� �������� ��������" << endl;
	cout << "1 - �������� ���� ������� �������" << endl;
	cout << "2 - ���������� ����� ������� ������" << endl;
	cout << "3 - �������������� ������� ������" << endl;
	cout << "4 - �������� ������� ������" << endl;
	cout << "0 - ����� � ������� ����" << endl;
	cout << "��� �����: ";
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
			break;
		default:
			userControlPanel();
	}
}

void adminMenu() {
	system("cls");
	int menu;
	cout << "���� ��������������" << endl;
	cout << "1 - ���������� �������� ��������" << endl;
	cout << "2 - ����� ������ � �������" << endl;
	cout << "3 - ����� �� �������" << endl;
	cout << "0 - ���������� ������" << endl;
	cout << "��� �����: ";
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
	openUserFile();
	system("cls");
	cout << "������� ��� ������������: ";
	string login = input(), password;
	int i;
	bool find = true;
	while (true) {
		cout << endl << "������� ������: ";
		password = inputPassword();
		find = true;
		for (i = 0; i < countOfUsers; i++) {
			if (login == Users[i].login)
				if (password == decrypt(Users[i].hashPassword))
					break;
				else {
					cout << endl << "����� ������������ ������. ���������� ��� ���!";
					find = false;
				}
		}

		if (find)
			break;
	}

	if (find)
		if (i < countOfUsers) {
			if (Users[i].access)
				if (Users[i].role)
					adminMenu();
				else
					cout << endl << "user" << endl;
			else {
				cout << endl << "������ ��������" << endl;
				exit(0);
			}
		}
		else {
			bool temp;
			cout << endl << "����� ������������������, ���������� ���������� ��������������. ������ �������� ������? (1 - ��, 0 - ���): ";
			cin >> temp;
			if (temp) {
				cout << endl << "����� ����� ������ ��������? (1 - �������������, 0 - ������������): ";
				cin >> temp;
				ofstream openFile("users.txt", ios::app);
				openFile << endl << login << " " << crypt(password) << " " << temp << " " << 0 << ";";
				openFile.close();
			}
		}
}

void exit() {
	if (countOfUsers > 0)
		delete[] Users;

	exit(0);
}