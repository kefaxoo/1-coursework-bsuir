#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>

using namespace std;

string validChars = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; // строка нешифрованных и допустимых символов
string cryptedChars = "mdl|cajoz~{wpyrusvktqhei`bnxf}gUHF[WPMR^GCAITY\_]OEXSBJNKQ@VDLZ.\"*?<-50&$;/:%'#2,7(>!8)14=3+69"; // строка шифрованных и допустимых символов

string crypt (string line) { // функция шифрования
	string crypted;
	for (int i = 0; i < line.length(); i++)
		crypted.push_back(cryptedChars[validChars.find(line[i])]);

	return crypted;
}

string decrypt (string line) { // функция дешифрования
	string decrypted;
	for (int i = 0; i < line.length(); i++)
		decrypted.push_back(validChars[cryptedChars.find(line[i])]);

	return decrypted;
}

#endif CRYPTO_H