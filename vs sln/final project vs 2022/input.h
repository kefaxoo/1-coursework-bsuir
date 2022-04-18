#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

// �������, � ������� ������������ ������ �����
string inputLogin() {
	string validChars = "-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"; // ���������� ������� � ������
	while (true) {
		string login; // ������ ��� ��������� ������
		bool allIsGood = true; // ���������� ����������, � ������� ������������� �� �� ������ � �������
		while (getline(cin, login))
			if (!login.empty()) {
				for (int i = 0; i < login.length(); i++)
					if (validChars.find(login[i]) == string::npos) {
						allIsGood = false;
						break;
					}

				if (allIsGood)
					return login;
				else
					break;
			}

		login.clear();
		cout << endl << "����� �������� ������������ �������, ���������� ��� ���" << endl << "������� �����: ";
	}
}

// �������, ����������� inputLogin, �� ����� ������������ ������ ������
string inputPassword() {
	string validChars = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	while (true) {
		string password;
		bool allIsGood = true;
		while (getline(cin, password))
			if (!password.empty()) {
				for (int i = 0; i < password.length(); i++)
					if (validChars.find(password[i]) == string::npos) {
						allIsGood = false;
						break;
					}

				if (allIsGood)
					return password;
				else
					break;
			}

		password.clear();
		cout << endl << "������ �������� ������������ �������, ���������� ��� ���" << endl << "������� ������: ";
	}
}

string inputPassword_hide() {
	string password; // ������, �������� ������, �������� �������������
	int count = 0; // ������������� ����������, �������������� ������� ��������� ������
	string validChars = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // ���������� �������

	while (true) {
		char temp = _getch();
		if (short(temp) == 13 || count == 255) {
			if (password.empty()) {
				cout << endl << "�� �� ����� ������, ���������� ��� ���" << endl << "������� ������: ";
				password.clear();
				count = 0;
				continue;
			} else {
				bool allIsGood = true; // ���������� ����������, � ������� ������������� �� �� ������ � �������
				for (int i = 0; i < password.length(); i++)
					if (validChars.find(password[i]) == string::npos) {
						allIsGood = false;
						break;
					}

				if (allIsGood)
					return password;
				else {
					cout << endl << "������ �������� ������������ �������, ���������� ��� ���" << endl << "������� ������: ";
					password.clear();
					count = 0;
					continue;
				}
			}
		}

		password.push_back(temp);
		count++;
		cout << "*";
	}
}

#endif //INPUT_H