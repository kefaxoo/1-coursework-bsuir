#ifndef WSTRING_H
#define WSTRING_H

#include <iostream>
#include <string>

using namespace std;

string input () {
	string line;
	int count = 0;
	while (true) {
		char temp = getchar();
		if (short(temp) == 10 || count == 255)
			return line;

		line += temp;
		count++;
	}
}

#endif //WSTRING_H