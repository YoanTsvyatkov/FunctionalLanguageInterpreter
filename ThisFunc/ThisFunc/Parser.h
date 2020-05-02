#pragma once
#include"Lexer.h"
#include "Tree.h"
#include <stdlib.h>
#include <map>

///Parser with two map containers that store function declarations and count of arguments
///Map containers perform efficiant search operations
class Parser
{
private:
	std::map<std::string, Expression<double>*> map;
	std::map<std::string, int> funcParamCounter;
public:
	void parse(Lexer& l);
	void parseIf(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseNand(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseLe(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseCos(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseSin(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseSqrt(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parsePow(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseDiv(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseMul(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseSub(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseList(Lexer& l, int& paramC, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseHead(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseTail(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName);
	void parseIndex(bool& inFuncDeclaration, std::pair<std::string, TOKENS>& currPair, int& paramC, std::string& funcName);
	void parseSpecialFunction(std::string& funcName, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, Lexer& l, int& paramC, int& retflag);
	Expression<double>* getNextArgument(Lexer& l, int& paramC);
	Expression<double>* getSpecialFunctionArguments(std::pair<std::string, TOKENS>& currPair, Lexer& l, int& paramC);
	Expression<double>* getIfArguments(Lexer& l, int& paramC);
	Expression<double>* getNandArguments(Lexer& l, int& paramC);
	Expression<double>* getLeArguments(Lexer& l, int& paramC);
	Expression<double>* getCosArguments(Lexer& l, int& paramC);
	Expression<double>* getSinArguments(Lexer& l, int& paramC);
	Expression<double>* getSqrtArguments(Lexer& l, int& paramC);
	Expression<double>* getPowArguments(Lexer& l, int& paramC);
	Expression<double>* getDivArguments(Lexer& l, int& paramC);
	Expression<double>* getMulArguments(Lexer& l, int& paramC);
	Expression<double>* getSubArguments(Lexer& l, int& paramC);
	Expression<double>* getAddArguments(Lexer& l, int& paramC);
	Expression<double>* getListArguments(Lexer& l, int& paramC);
	Expression<double>* getHeadArguments(Lexer& l, int& paramC);
	Expression<double>* getTailArguments(Lexer& l, int& paramC);
	Expression<double>* getEQArguments(Lexer& l, int& paramC);
	Expression<double>* getSumArguments(Lexer& l, int& paramC);
	Expression<double>* getConcatArguments(Lexer& l, int& paramC);
	Expression<double>* getMapArguments(Lexer& l, int& paramC);
	void parseEQ(Lexer& l, int& paramC, bool inFunction, DynamicArray<int>& arr);
	void parseMAP(Lexer& l, int& paramC, bool inFunction, DynamicArray<int>& arr);
	void parseSUM(Lexer& l, int& paramC, bool inFunction, DynamicArray<int>& arr);
	void parseCONCAT(Lexer& l, int& paramC, bool inFunction, DynamicArray<int>& arr);
	~Parser();
};

