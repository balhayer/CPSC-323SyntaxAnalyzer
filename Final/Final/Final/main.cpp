//main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
using namespace std;

string input_file_name() {
	string file_Name;
	cout << "Enter Input File Name (with extensions): ";
	cin >> file_Name;
	return file_Name;
}

void verify_input_file() {
	string file_Name = input_file_name();
	ifstream file(file_Name);
	if (!file.is_open()) {
		cout << "Could not locate." << endl;
	}
	else {
		LexicalAnalyzer la;
		la.lexer(file);
		file.close();
		la.printResult("analysis.txt");
		SyntaxAnalyzer sa;
		sa.analyze(la.return_token(), la.return_Lexemes(), la.return_byLine());
	}
}

int main() {
	verify_input_file();
	cout << endl;
	system("pause");
	return 0;
}
