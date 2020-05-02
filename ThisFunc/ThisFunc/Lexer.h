#pragma once
#include <string>
#include<fstream>
#include "DynArray.h"

enum TOKENS
{
	ADD, SUB, MUL, DIV, POW, SQRT, SIN, COS, EQ, LE, NAND, IF, LIST, HEAD,
	TAIL, MAP, CONCAT, CONSTANT, OPEN_PARANTHESES, CLOSE_PARANTHESES,
	COMMA, SPECIAL_FUNCTION, ARROW, INDEX
};

///Lexer that converts sequance of characters into sequance of tokens
///The two containers store tokens and names of declared functions
class Lexer {
public:
	DynamicArray<std::pair<std::string, TOKENS>>& processFile(const char* fileName);
	void processArr(DynamicArray<std::string>& darr);
	void tokenizeLine(const std::string& str);
	void findNumber(const std::string& str, std::string& res);
	size_t size()const { return tokens.getSize(); }
	bool end()const { return currToken == size(); }
	std::pair<std::string, TOKENS>& next();
	std::pair<std::string, TOKENS>& prev();
private:
	DynamicArray<std::pair<std::string, TOKENS>> tokens;
	DynamicArray<std::string> specialFunctions;
	int currToken = 0; ///< Index of current token
};