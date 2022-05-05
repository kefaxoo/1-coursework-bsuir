#ifndef CRYPTO_H
#define CRYPTO_H

#include <iostream>

using namespace std;

string normalLine = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // строка нешифрованных символов
string cryptedLine = "mdl|cajoz~{wpyrusvktqhei`bnxf}gUHF[WPMR^GCAITY\_]OEXSBJNKQ@VDLZ.\"*?<-50&$;/:%'#2,7(>!8)14=3+69"; // строка шифрованных символов

string crypt(string line) { // функция, шифрующая строку
	string result; // строка для шифрованной строки
	for (int i = 0; i < line.length(); i++) // цикл по всей нешифрованной строке
		result.push_back(cryptedLine[normalLine.find(line[i])]); // добавляем в конец шифрованной строки символ, который у нас шифруется

	return result; // возвращаем зашифрованную строку
}

string decrypt(string line) { // аналогичная функция, дешифрующая строку
	string result;
	for (int i = 0; i < line.length(); i++)
		result += normalLine[cryptedLine.find(line[i])];

	return result;
}

#endif //CRYPTO_H
