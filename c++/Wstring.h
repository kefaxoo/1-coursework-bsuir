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

string inputLogin() { // функция ввода логина
	string normalLine = "-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"; // допустимые символы в логине
	string line; // строка для вводимого логина
	while (true) { // бесконечный цикл
		bool allIsGood = true; // логическая переменная, в которой будем отслеживать всё ли хорошо с логином
		while (getline(cin, line)) // получаем строку
			if (line != "") { // если она не пустая
				for (int i = 0; i < line.length(); i++) // цикл по всему логину
					if (normalLine.find(line[i]) == string::npos) // если программа не нашла символа из логина в допустимых символах
						allIsGood = false; // то логическую переменную меняем на false

				if (allIsGood) // если нет ошибок
					return line; // возвращаем строку
				else // иначе
					break; // выходим из цикла
			}

		line.clear(); // очищаем строку
		cout << endl << "Логин содержит недопустимые символы, попробуйте ещё раз" << endl << "Введите логин: "; // выводим сообщение о недопустимых символах в логине и предложение о вводе логина снова
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
		cout << endl << "Пароль содержит недопустимые символы, попробуйте ещё раз" << endl << "Введите пароль: ";
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
				cout << endl << "Пароль содержит недопустимые символы, попробуйте ещё раз" << endl << "Введите пароль: ";
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