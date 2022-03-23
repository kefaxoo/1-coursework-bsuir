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

		line += temp;
		cout << "*";
	}
}

#endif //WSTRING_H