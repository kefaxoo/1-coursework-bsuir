#ifndef CRYPTO_H
#define CRYPTO_H

#include <iostream>

using namespace std;

string normalLine = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // строка нешифрованных символов
string cryptedLine = "mdl|cajoz~{wpyrusvktqhei`bnxf}gUHF[WPMR^GCAITY\_]OEXSBJNKQ@VDLZ.\"*?<-50&$;/:%'#2,7(>!8)14=3+69"; // строка шифрованных символов

string crypt(string line) { // функция, шифрующая пароль
	string result; // строка для шифрованного пароля
	for (int i = 0; i < line.length(); i++) // цикл по всему нешифрованному паролю
		result.push_back(cryptedLine[normalLine.find(line[i])]); // добавляем в конец шифрованного пароль символ, который у нас шифруется

	return result; // возвращаем шифрованный пароль
}

string decrypt(string line) {
	string result;
	for (int i = 0; i < line.length(); i++)
		result += normalLine[cryptedLine.find(line[i])];

	return result;
}

#endif //CRYPTO_H