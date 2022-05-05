#include <iostream>
#include <cstdio>

using namespace std;

int main() {
	for (int i = 0; i < 256; i++) {
		cout << char(i) << " ";
		printf("%*i", 3, i);
		cout << endl;
	}
}