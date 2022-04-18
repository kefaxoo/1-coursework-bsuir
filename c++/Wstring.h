#ifndef WSTRING_H
#define WSTRING_H

#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

string input () {
	string line;
	while (getline(cin, line))
		if (line != "")
			return line;
}

string inputLogin() { // ������� ����� ������
	string normalLine = "-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"; // ���������� ������� � ������
	string line; // ������ ��� ��������� ������
	while (true) { // ����������� ����
		bool allIsGood = true; // ���������� ����������, � ������� ����� ����������� �� �� ������ � �������
		while (getline(cin, line)) // �������� ������
			if (line != "") { // ���� ��� �� ������
				for (int i = 0; i < line.length(); i++) // ���� �� ����� ������
					if (normalLine.find(line[i]) == string::npos) // ���� ��������� �� ����� ������� �� ������ � ���������� ��������
						allIsGood = false; // �� ���������� ���������� ������ �� false

				if (allIsGood) // ���� ��� ������
					return line; // ���������� ������
				else // �����
					break; // ������� �� �����
			}

		line.clear(); // ������� ������
		cout << endl << "����� �������� ������������ �������, ���������� ��� ���" << endl << "������� �����: "; // ������� ��������� � ������������ �������� � ������ � ����������� � ����� ������ �����
	}
}

string inputPassword() {
	string normalLine = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	string line;
	bool allIsGood = true;
	while (true) {
		while (getline(cin, line))
			if (line != "") {
				for (int i = 0; i < line.length(); i++)
					if (normalLine.find(line[i]) == string::npos)
						allIsGood = false;

				if (allIsGood)
					return line;
				else
					break;
			}

		line.clear();
		cout << endl << "������ �������� ������������ �������, ���������� ��� ���" << endl << "������� ������: ";
	}
}

string inputPassword_hide() {
	string line;
	int count = 0;
	string normalLine = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	while (true)
	{
		char temp = _getch();
		if ((short(temp) == 13 || count == 255) && line != "") {
			bool allIsGood = true;
			for (int i = 0; i < line.length(); i++)
				if (normalLine.find(line[i]) == string::npos)
					allIsGood = false;

			if (allIsGood)
				return line;
			else {
				cout << endl << "������ �������� ������������ �������, ���������� ��� ���" << endl << "������� ������: ";
				line.clear();
				count = 0;
				continue;
			}
		}
		
		line += temp;
		count++;
		cout << "*";
	}
}

#endif //WSTRING_H