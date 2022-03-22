#ifndef WSTRING_H
#define WSTRING_H

#include <iostream>
#include <string>

using namespace std;

string input () {
	string line;
	while (getline(cin, line))
		if (line != "")
			return line;
}

#endif //WSTRING_H