#ifndef CRYPTO_H
#define CRYPTO_H

#include <iostream>

using namespace std;

string normalLine = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
string cryptedLine = "mdl|cajoz~{wpyrusvktqhei`bnxf}gUHF[WPMR^GCAITY\_]OEXSBJNKQ@VDLZ.\"*?<-50&$;/:%'#2,7(>!8)14=3+69";

string crypt (string line) {
	string result;
	for (int i = 0; i < line.length(); i++)
		result += cryptedLine[normalLine.find(line[i])];

	return result;
}

string decrypt (string line) {
	string result;
	for (int i = 0; i < line.length(); i++)
		result += normalLine[cryptedLine.find(line[i])];

	return result;
}

#endif //CRYPTO_H