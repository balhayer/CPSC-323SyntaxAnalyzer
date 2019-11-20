// lexicalAnalyzer
#pragma once
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const int ksize = 20;
const string keywords[ksize] = {"int", "float", "bool", "true", "false", "if", "else", "then", "endif", "while", "whileend",
										"do", "doend", "for", "forend", "input", "output", "and", "or", "function" };

const int osize = 8;
const char operators[osize] = {'*', '+', '-', '=', '/', '>', '<', '%'};

const int ssize = 14;
const char separators[ssize] = { '\'', '(', ')', '{', '}', '[', ']', ',', '.', ':', ';', '!', ' ', '\t' };

const int FSM[6][4] = {

	{1, 2, 5, 4},
	{1, 1, 1, 6},
	{5, 2, 5, 3},
	{5, 3, 5, 9},
	{7, 3, 7, 7},
	{5, 5, 5, 8}
};



class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	// It analyze the source file
	void lexer(ifstream& sourceCode);

	// It analyze (1) line
	void lexer(string codeLine); 

	// It prints out the output to the text file
	void printResult(string file_Name);

	// It returns token vector
	vector<string> return_token();

	// It returns lexemes vector
	vector<string> return_Lexemes(); 

	// It returns line vector
	vector<int> return_byLine(); 

private:
	vector<string> tokens;
	vector<string> lexemes;
	vector<int> line_number;

	bool isDigit(char ch);
	bool isLetter(char ch);
	bool isKeyword(string str);
	bool isOperator(char ch);
	bool isSeparator(char ch);

	// It analyze string using Finite State Machine
	void analyze(string word, int line); 
	int charToCol(char ch);
};

