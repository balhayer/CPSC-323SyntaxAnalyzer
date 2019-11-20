// SyntaxAnalyzer.cpp

#include "SyntaxAnalyzer.h"
#include <iostream>
#include <fstream>

SyntaxAnalyzer::SyntaxAnalyzer() {}

SyntaxAnalyzer::SyntaxAnalyzer(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number) {
	analyze(tokens, lexemes, line_number);
}

void SyntaxAnalyzer::analyze(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number) {
	ofstream filename ("parse_result.txt");
	int n = tokens.size();
	int i = 0;

	do {
		cout << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;
		filename << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;

		if (statement(tokens, lexemes, line_number, i, filename)) {
		}
		else {
			filename.close();
			return;
		}
		++i;
		cout << endl;
		filename << endl;
	} while (i < n);

	cout << endl << "Finished." << endl;
	filename << endl << "Finished." << endl;
	filename.close();
}

bool SyntaxAnalyzer::nextToken(const vector<string>& tokens, const vector<string>& lexemes, int& i, ofstream& filename) {
	++i;
	if (i >= tokens.size()) {
		cout << "Error: end of token stream." << endl;
		filename << "Error: end of token stream." << endl;
		--i;
		return false;
	}
	if (i > prev_i) {
		cout << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;
		filename << "-Token: " << tokens[i] << "\t-Lexeme: " << lexemes[i] << endl;
		prev_i = i;
	}
	return true;
}

bool SyntaxAnalyzer::statement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {	
	cout << "\t<Statement> -> <Expression-statement> | <Assign> | <If> | <Decalre> | <While>" << endl;
	filename << "\t<Statement> -> <Expression-statement> | <Assign> | <If> | <Declare> | <While>" << endl;


	// first token
	int startIndex = i;  

	// assigns an expression statement
	if (tokens[i] == "identifier") { 
		if (i + 1 < tokens.size()) {

			// It assigns 
			if (lexemes[i + 1] == "=") {  
				if (assign(tokens, lexemes, line_number, i, filename)) {
					return true;
				}
			}
			else {
				if (expressionStatement(tokens, lexemes, line_number, i, filename)) {
					return true;
				}
			}
		}
		else {
			cout << "Error: at the last end of token stream." << endl;
			filename << "Error: at the last end of token stream." << endl;
			return false;
		}
	}

	else if (tokens[i] == "int" || tokens[i] == "real" || lexemes[i] == "(" || lexemes[i] == "-") { // expression statement
		if (expressionStatement(tokens, lexemes, line_number, i, filename)) {
			return true;
		}		
	}

	else if (lexemes[i] == "int" || lexemes[i] == "float" || lexemes[i] == "bool") { // declare statement
		if (declare(tokens, lexemes, line_number, i, filename)) {
			return true;
		}		
	}

	// if statement
	else if (lexemes[i] == "if") { 
		if (ifStatement(tokens, lexemes, line_number, i, filename)) {
			return true;
		}
	}

	// while statement 
	else if (lexemes[i] == "while"){
		if (whileStatement(tokens, lexemes, line_number, i, filename)) {
			return true;
		}
	}

	// other statement
	else {
		cout << "Error: invalid/wrong statement - line " << line_number[i] << endl;
		filename << "Error: invalid/wrong statement - line " << line_number[i] << endl;
	}
	return false;
}

bool SyntaxAnalyzer::assign(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<Assign> -> identifier = <Expression>;" << endl;
	filename << "\t<Assign> -> identifier = <Expression>;" << endl;

	if (tokens[i] == "identifier") {
		if (!nextToken(tokens, lexemes, i, filename))  return false;
		if (lexemes[i] == "=") {
			if (!nextToken(tokens, lexemes, i, filename)) return false;
			if (expression(tokens, lexemes, line_number, i, filename)) {
				if (!nextToken(tokens, lexemes, i, filename)) return false;
				if (lexemes[i] == ";") {
					return true;
				}
				else {
					cout << "Error: missing a \";\" - line " << line_number[i] << endl;
					filename << "Error: missing a \";\" - line " << line_number[i] << endl;
					return false;
				}
			}
		}
	}
	cout << "Error: invalid assign statement - line " << line_number[i] << endl;
	filename << "Error: invalid assign statement - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::expressionStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename){
	cout << "\t<Expression Statement> -> <Expression>;" << endl;
	filename << "\t<Expression Statement> -> <Expression>;" << endl;

	if (expression(tokens, lexemes, line_number, i, filename)) {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (lexemes[i] == ";") {
			return true;
		}
		else {
			cout << "Error: missing a \";\" - line " << line_number[i] << endl;
			filename << "Error: missing a \";\" - line " << line_number[i] << endl;
			return false;
		}
	}

	cout << "Error: invalid expression statement - line " << line_number[i] << endl;
	filename << "Error: invalid expression statement - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::expression(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<Expression> -> <Term> <ExpressionPrime>" << endl;
	filename << "\t<Expression> -> <Term> <ExpressionPrime>" << endl;

	if (term(tokens, lexemes, line_number, i, filename)) {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (expressionPrime(tokens, lexemes, line_number, i, filename)) {
			return true;
		}
	}	
	return false;
}

bool SyntaxAnalyzer::expressionPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << endl;
	filename << "\t<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << endl;

	if (lexemes[i] == "+" || lexemes[i] == "-") {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (term(tokens, lexemes, line_number, i, filename)) {
			if (!nextToken(tokens, lexemes, i, filename)) return false;
			if (expressionPrime(tokens, lexemes, line_number, i, filename)) {
				return true;
			}
		}
	}
	else { 
		cout << "\t<Empty> -> epsilon" << endl;
		filename << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}
	return false;
}

bool SyntaxAnalyzer::term(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<Term> -> <Factor> <TermPrime>" << endl;
	filename << "\t<Term> -> <Factor> <TermPrime>" << endl;

	if (factor(tokens, lexemes, line_number, i, filename)) {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (termPrime(tokens, lexemes, line_number, i, filename)) {
			return true;
		}
	}

	return false;
}

bool SyntaxAnalyzer::termPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << endl;
	filename << "\t<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << endl;

	if (lexemes[i] == "*" || lexemes[i] == "/") {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (factor(tokens, lexemes, line_number, i, filename)) {
			if (!nextToken(tokens, lexemes, i, filename)) return false;
			if (termPrime(tokens, lexemes, line_number, i, filename)) {
				return true;
			}
		}
	}
	else { 
		cout << "\t<Empty> -> epsilon" << endl;
		filename << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}

	return false;
}

bool SyntaxAnalyzer::factor(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<Factor> -> - <Primary> | <Primary>" << endl;
	filename << "\t<Factor> -> - <Primary> | <Primary>" << endl;

	if (lexemes[i] == "-") {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (primary(tokens, lexemes, line_number, i, filename)) {
			return true;
		}
	}
	else { 		
		if (primary(tokens, lexemes, line_number, i, filename)) {
			return true;
		}
	}
	return false;
}

bool SyntaxAnalyzer::primary(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<Primary> -> identifier | int | ( <Expression> ) | real | true | false" << endl;
	filename << "\t<Primary> -> identifier | int | ( <Expression> ) | real | true | false" << endl;

	if (tokens[i] == "identifier" || tokens[i] == "int" || tokens[i] == "real" || lexemes[i] == "true" || lexemes[i] == "false") {
		return true;
	}
	if (lexemes[i] == "(") {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (expression(tokens, lexemes, line_number, i, filename)) {
			if (!nextToken(tokens, lexemes, i, filename)) return false;
			if (lexemes[i] == ")") {
				return true;
			}
			else {
				cout << "Error: missing a \")\" - line: " << line_number[i] << endl;
				filename << "Error: missing a \")\" - line: " << line_number[i] << endl;
				return false;
			}
		}
	}
	return false;
}

bool SyntaxAnalyzer::declare(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<Decalre> -> <Type> identifier <MoreIDs>;" << endl;
	filename << "\t<Decalre> -> <Type> identifier <MoreIDs>;" << endl;
	cout << "\t<Type> -> int | float | bool" << endl;
	filename << "<Type> -> int | float | bool" << endl;

	if (lexemes[i] == "int" || lexemes[i] == "float" || lexemes[i] == "bool") { // <Type>
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (tokens[i] == "identifier") {
			if (!nextToken(tokens, lexemes, i, filename)) return false;
			if (moreIDs(tokens, lexemes, line_number, i, filename)) {
				if (!nextToken(tokens, lexemes, i, filename)) return false;
				if (lexemes[i] == ";") {
					return true;
				}
				else {
					cout << "Error: missing a \";\" - line " << line_number[i] << endl;
					filename << "Error: missing a \";\" - line " << line_number[i] << endl;
					return false;
				}
			}
		}
	}
	cout << "Error: invalid declare statement - line " << line_number[i] << endl;
	filename << "Error: invalid declare statement - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::moreIDs(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<MoreIDs> -> , id <MoreIDs> | <Empty>" << endl;
	filename << "\t<MoreIDs> -> , id <MoreIDs> | <Empty>" << endl;

	if (lexemes[i] == ",") {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (tokens[i] == "identifier") {
			if (!nextToken(tokens, lexemes, i, filename)) return false;
			if (moreIDs(tokens, lexemes, line_number, i, filename)) {
				return true;
			}
		}
	}
	else { 
		cout << "\t<Empty> -> epsilon" << endl;
		filename << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}
	return false;
}

bool SyntaxAnalyzer::conditional(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<Conditional> -> <Expression> <ConditionalPrime> " << endl;
	filename << "\t<Conditional> -> <Expression> <ConditionalPrime>" << endl;

	if (expression(tokens, lexemes, line_number, i, filename)){
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (conditionalPrime(tokens, lexemes, line_number, i, filename)) {
			return true;
		}
	}
	return false;
}

bool SyntaxAnalyzer::conditionalPrime(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<ConditionalPrime> -> <Relop> <Expression> | <Empty>" << endl;
	filename << "\t<ConditionalPrime> -> <Relop> <Expression> | <Empty>" << endl;

	if (relop(tokens, lexemes, line_number, i, filename)) {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (expression(tokens, lexemes, line_number, i, filename)) {
			return true;
		}
	}
	else{ // epsilon
		cout << "\t<Empty> -> epsilon" << endl;
		filename << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}
	return false;
}

bool SyntaxAnalyzer::relop(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<Relop> -> < | <= | == | <> | >= | >" << endl;
	filename << "\t<Relop> -> < | <= | == | <> | >= | >" << endl;

	// < or = or <>
	if (lexemes[i] == "<") { 
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (lexemes[i] == "=") 
			return true;
		else if (lexemes[i] == ">") 
			return true;
		else{ 		
			--i;
			return true;
		}	
	}

	// == statement
	if (lexemes[i] == "=") { 
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (lexemes[i] == "=")
			return true;
	}
	// > or >=
	if (lexemes[i] == ">") { 
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (lexemes[i] == "=") 
			return true;
		else{ 
			--i;
			return true;
		}
	}

	cout << "Error: invalid relational operator - line " << line_number[i] << endl;
	filename << "Error: invalid relational operator - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::ifStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<If> -> if <Conditional> then <StatementBlock> <ElseBlock> endif" << endl;
	filename << "\t<If> -> if <Conditional> then <StatementBlock> <ElseBlock> endif" << endl;

	if (lexemes[i] == "if") {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (conditional(tokens, lexemes, line_number, i, filename)) {
			if (!nextToken(tokens, lexemes, i, filename)) return false;
			if (lexemes[i] == "then") {
				if (!nextToken(tokens, lexemes, i, filename)) return false;
				if (statementBlock(tokens, lexemes, line_number, i, filename)) {
					if (!nextToken(tokens, lexemes, i, filename)) return false;
					if (elseBlock(tokens, lexemes, line_number, i, filename)) {
						if (!nextToken(tokens, lexemes, i, filename)) {
							cout << "Error: missing \"endif\" - line " << line_number[i] << endl;
							filename << "Error: missing \"endif\" - line " << line_number[i] << endl;
							return false;
						}
						if (lexemes[i] == "endif") {
							return true;
						}
						else {
							cout << "Error: missing \"endif\" - line " << line_number[i] << endl;
							filename << "Error: missing \"endif\" - line " << line_number[i] << endl;
							return false;
						}
					}
				}
			}
			else {
				cout << "Error: missing \"then\" - line " << line_number[i] << endl;
				filename << "Error: missing \"then\" - line " << line_number[i] << endl;
				return false;
			}
		}
	}

	cout << "Error: invalid If block - line " << line_number[i] << endl;
	filename << "Error: invalid If block - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::statementBlock(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<StatementBlock> -> { <Statement> <moreStatement>" << endl;
	filename << "\t<StatementBlock> -> { <Statement> <moreStatement>" << endl;

	if (lexemes[i] == "{") {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (statement(tokens, lexemes, line_number, i, filename)) {
			if (!nextToken(tokens, lexemes, i, filename)) return false;
			if (moreStatement(tokens, lexemes, line_number, i, filename)) {
				return true;
			}
		}
	}

	cout << "Error: invalid { statement block } - line " << line_number[i] << endl;
	filename << "Error: invalid { statement block } - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::moreStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<moreStatement> -> <Statement> <moreStatement> | }" << endl;
	filename << "\t<moreStatement> -> <Statement> <moreStatement> | }" << endl;

	if (lexemes[i] == "}") {
		return true;
	}
	else {
		if (statement(tokens, lexemes, line_number, i, filename)) {
			if (!nextToken(tokens, lexemes, i, filename)) return false;
			if (moreStatement(tokens, lexemes, line_number, i, filename)) {
				return true;
			}
		}
		else return false;
	}

	cout << "Error: missing a \"}\" - line " << line_number[i] << endl;
	filename << "Error: missing a \"}\" - line " << line_number[i] << endl;
	return false;
}

bool SyntaxAnalyzer::elseBlock(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<ElseBlock> -> else <StatementBlock> | <Empty>" << endl;
	filename << "\t<ElseBlock> -> else <StatementBlock> | <Empty>" << endl;

	if (lexemes[i] == "else") {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (statementBlock(tokens, lexemes, line_number, i, filename)) {
			return true;
		}
	}
	else { 
		cout << "\t<Empty> -> epsilon" << endl;
		filename << "\t<Empty> -> epsilon" << endl;
		--i;
		return true;
	}
	return false;
}

bool SyntaxAnalyzer::whileStatement(const vector<string>& tokens, const vector<string>& lexemes, const vector<int>& line_number, int &i, ofstream& filename) {
	cout << "\t<While> -> while <Conditional> do <StatementBlock> whileend" << endl;
	filename << "\t<While> -> while <Conditional> do <StatementBlock> whileend" << endl;

	if (lexemes[i] == "while") {
		if (!nextToken(tokens, lexemes, i, filename)) return false;
		if (conditional(tokens, lexemes, line_number, i, filename)) {
			if (!nextToken(tokens, lexemes, i, filename)) return false;
			if (lexemes[i] == "do") {
				if (!nextToken(tokens, lexemes, i, filename)) return false;
				if (statementBlock(tokens, lexemes, line_number, i, filename)) {
					if (!nextToken(tokens, lexemes, i, filename)) {
						cout << "Error: missing \"whileend\" - line " << line_number[i] << endl;
						filename << "Error: missing whileend - line " << line_number[i] << endl;
						return false;
					}
					if (lexemes[i] == "whileend") {
						return true;
					}
					else {
						cout << "Error: missing \"whileend\" - line " << line_number[i] << endl;
						filename << "Error: missing \"whileend\" - line " << line_number[i] << endl;
						return false;
					}
				}
			}
			else {
				cout << "Error: missing do block - line " << line_number[i] << endl;
				filename << "Error: missing do block - line " << line_number[i] << endl;
				return false;
			}
		}
	}

	cout << "Error: invalid While block - line " << line_number[i] << endl;
	filename << "Error: invalid While block - line " << line_number[i] << endl;
	return false;
}