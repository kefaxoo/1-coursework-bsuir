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

string inputPassword() {
	string line;
	int count = 0;
	while (true)
	{
		char temp = _getch();
		if ((short(temp) == 13 || count == 255) && line != "")
			return line;

		string normalLine = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
		bool find = false;
		for (int i = 0; i < normalLine.length(); i++)
			if (temp == normalLine[i]) {
				find = true;
				break;
			}

		if (find) {
			line += temp;
			cout << "*";
		}
	}
}

#endif //WSTRING_H