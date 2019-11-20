//.h
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer();
	SyntaxAnalyzer(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number);

	void analyze(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number);
	bool nextToken(const vector<string>& tokens, const vector<string>& lexemes, int &i, ofstream& filename);

	bool statement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool assign(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool expressionStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool declare(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool ifStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool statementBlock(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool moreStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool elseBlock(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool whileStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);

	bool expression(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool expressionPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool term(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool termPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool factor(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool primary(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);	
	bool moreIDs(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool conditional(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool conditionalPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	bool relop(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename);
	

private:
	int prev_i = -1;
};

