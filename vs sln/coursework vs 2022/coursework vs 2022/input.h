﻿#ifndef INPUT_H
#define INPUT_H

#undef max

#include <iostream>
#include <string>
#include <conio.h>
#include <limits>

using namespace std;

string input() { // функция ввода строк
	string line;
	while (getline(cin, line))
		if (!line.empty())
			return line;
}

// функция, в которой пользователь вводит логин
string inputLogin() {
	string validChars = "-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"; // допустимые символы в логине
	while (true) {
		string login; // строка для вводимого логина
		bool allIsGood = true; // логическая переменная, в которой отслеживается всё ли хорошо с логином
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
		cout << endl << "Логин содержит недопустимые символы, попробуйте ещё раз" << endl << "Введите логин: ";
	}
}

// функция, аналогичная inputLogin, но здесь пользователь вводит пароль
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
		cout << endl << "Пароль содержит недопустимые символы, попробуйте ещё раз" << endl << "Введите пароль: ";
	}
}

string inputPassword_hide() { // функция, аналогичная inputPassword, но на экране пользователь видит звёздочки, вместо символов пароля
	string password; // строка, хранящая пароль, вводимый пользователем
	int count = 0; // целочисленная переменная, контролирующая пределы вводимого пароля
	string validChars = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // допустимые символы

	while (true) {
		char temp = _getch();
		if (short(temp) == 13 || count == 255) {
			if (password.empty()) {
				cout << endl << "Вы не ввели пароль, попробуйте ещё раз" << endl << "Введите пароль: ";
				password.clear();
				count = 0;
				continue;
			}
			else {
				bool allIsGood = true; // логическая переменная, в которой отслеживается всё ли хорошо с логином
				for (int i = 0; i < password.length(); i++)
					if (validChars.find(password[i]) == string::npos) {
						allIsGood = false;
						break;
					}

				if (allIsGood)
					return password;
				else {
					cout << endl << "Пароль содержит недопустимые символы, попробуйте ещё раз" << endl << "Введите пароль: ";
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

bool inputBool() { // функция ввода значений типа bool
	while (true) {
		bool temp; // создаём временную переменную для ввода
		cin >> temp; // вводим значение
		if (cin.good()) // если пользователь ввел вреное значение
			return temp; // возвращаем значение
		else { // иначе
			cin.clear(); // сбрасываем флаги ошибок
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка потока
			cout << endl << "Некорректный ввод, попробуйте ещё раз" << endl << "Введите значение: "; // вывод сообщения об ошибке
		}
	}
}

int inputInt() { // функция ввода значений типа int
	while (true) {
		int temp; // создаём временную переменную для ввода
		cin >> temp; // вводим значение
		if (cin.good()) // если пользователь ввел вреное значение
			return temp; // возвращаем значение
		else { // иначе
			cin.clear(); // сбрасываем флаги ошибок
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка потока
			cout << endl << "Некорректный ввод, попробуйте ещё раз" << endl << "Введите значение: "; // вывод сообщения об ошибке
		}
	}
}

double inputDouble() { // функция ввода значений типа double
	while (true) {
		double temp; // создаём временную переменную для ввода
		cin >> temp; // вводим значение
		if (cin.good()) // если пользователь ввел вреное значение
			return temp; // возвращаем значение
		else { // иначе
			cin.clear(); // сбрасываем флаги ошибок
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка потока
			cout << endl << "Некорректный ввод, попробуйте ещё раз" << endl << "Введите значение: "; // вывод сообщения об ошибке
		}
	}
}

int inputYear_Data(int localYear) { // функция ввода года
	while (true) {
		cout << endl << "Введите год: ";
		int year = inputInt(); // переменная, в которую пользователь вводит год
		if (year > -1 && year <= localYear) // проверка года на действительность
			return year;
		else
			cout << endl << "Вы ввели неправильный год, попробуйте ещё раз";
	}
}

int inputMonth_Data() { // функция ввода месяца
	while (true) {
		cout << endl << "Введите месяц: ";
		int month = inputInt(); // переменная, в которую пользователь вводит месяц
		if (month > 0 && month < 13) // проверка месяца на действительность
			return month;
		else
			cout << endl << "Вы ввели неправильный месяц, попробуйте ещё раз";
	}
}

double inputCountOfHours_Data() { // функция ввода количества часов
	while (true) {
		cout << endl << "Введите количество проработанных часов: ";
		double countOfHours = inputDouble(); // переменная, в которую пользователь вводит количество часов
		if (countOfHours >= 0) // проверка колмчества на действительность
			return countOfHours;
		else
			cout << endl << "Вы ввели неправильное количество проработанных часов, попробуйте ещё раз";
	}
}

double inputRate_Data() { // функция ввода тарифа
	while (true) {
		cout << endl << "Введите почасовой тариф: ";
		double rate = inputDouble(); // переменная, в которую пользователь вводит тариф
		if (rate >= 0) // проверка тарифа на действительность
			return rate;
		else
			cout << endl << "Вы ввели неправильный почасовой тариф, попробуйте ещё раз";
	}
}

#endif //INPUT_H
