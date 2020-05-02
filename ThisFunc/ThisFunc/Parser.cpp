#include "Parser.h"


///Iterating all tokens and creating evaluation tree
void Parser::parse(Lexer& l)
{
	std::pair<std::string, TOKENS> currPair;
	DynamicArray<int> argArr;
	bool inFuncDeclaration = false;
	std::string funcName;
	int paramC = 0;

	while (!l.end()) {
		currPair = l.next();

		switch (currPair.second)
		{
		case CONSTANT: {
			parseCONCAT(l, paramC, inFuncDeclaration, argArr);
			break;
		}
		case SPECIAL_FUNCTION: {
			int retflag;
			parseSpecialFunction(funcName, currPair, inFuncDeclaration, argArr, l, paramC, retflag);
			if (retflag == 2) break;
			break;
		}
		case INDEX: {
			parseIndex(inFuncDeclaration, currPair, paramC, funcName);
			break;
		}
		case CONCAT: {
			parseCONCAT(l, paramC, inFuncDeclaration, argArr);
			break;
		}
		case MAP: {
			parseMAP(l, paramC, inFuncDeclaration, argArr);
			break;
		}
		case TAIL: {
			parseTail(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case HEAD: {
			parseHead(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case LIST: {
			parseList(l, paramC, inFuncDeclaration, argArr, funcName);
				break;
			}
		case ADD: {
			parseSUM(l, paramC, inFuncDeclaration, argArr);
			break;
		}
		case SUB: {
			parseSub(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case MUL: {
			parseMul(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case DIV: {
			parseDiv(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case POW: {
			parsePow(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case SQRT: {
			parseSqrt(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case SIN: {
			parseSin(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case COS: {
			parseCos(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case EQ: {
			parseEQ(l, paramC, inFuncDeclaration, argArr);
			break;
		}
		case LE: {
			parseLe(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case NAND: {
			parseNand(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		case IF: {
			parseIf(l, paramC, currPair, inFuncDeclaration, argArr, funcName);
			break;
		}
		default: {	
				throw std::exception("Invalid identifier");
		}
	}

	}
}

void Parser::parseIf(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* thirdArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument,thirdArgument };
	Expression<double>* IF_STATEMENT = new Function<double>(IF, arguments);
	if (!inFuncDeclaration) {
		std::cout << "> ";
		IF_STATEMENT->print(argArr, IF_STATEMENT);
		std::cout << std::endl;

		delete IF_STATEMENT;
	}
	else {
		map[funcName] = IF_STATEMENT;

		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseNand(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	Expression<double>* nand = new Function<double>(NAND, arguments);

	if (!inFuncDeclaration) {
		std::cout << "> ";
		nand->print(argArr, nand);
		std::cout << std::endl;

		delete nand;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = nand;
		map.insert(p);


		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseLe(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);

	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	Expression<double>* le = new Function<double>(LE, arguments);
	if (!inFuncDeclaration) {
		std::cout << "> ";
		le->print(argArr, le);
		std::cout << std::endl;

		delete le;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = le;
		map.insert(p);


		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseCos(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument };
	Expression<double>* cos = new Function<double>(COS, arguments);
	if (!inFuncDeclaration) {
		cos->print(argArr, cos);
		std::cout << std::endl;

		delete cos;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = cos;
		map.insert(p);


		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseSin(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument };
	Expression<double>* sin = new Function<double>(SIN, arguments);
	if (!inFuncDeclaration) {
		std::cout << "> ";
		sin->print(argArr, sin);
		std::cout << std::endl;

		delete sin;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = sin;
		map.insert(p);


		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseSqrt(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument };
	Expression<double>* sqrt = new Function<double>(SQRT, arguments);
	if (!inFuncDeclaration) {
		std::cout << "> ";
		sqrt->print(argArr, sqrt);
		std::cout << std::endl;

		delete sqrt;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = sqrt;
		map.insert(p);


		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parsePow(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);

	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	Expression<double>* pow = new Function<double>(POW, arguments);
	if (!inFuncDeclaration) {
		std::cout << "> ";
		pow->print(argArr, pow);
		std::cout << std::endl;

		delete pow;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = pow;
		map.insert(p);


		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseDiv(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	Expression<double>* div = new Function<double>(DIV, arguments);
	if (!inFuncDeclaration) {
		std::cout << "> ";
		div->print(argArr, div);
		std::cout << std::endl;

		delete div;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = div;
		map.insert(p);


		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseMul(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { secondArgument, firstArgument };
	Expression<double>* mul = new Function<double>(MUL, arguments);

	if (!inFuncDeclaration) {
		std::cout << "> ";
		mul->print(argArr, mul);
		std::cout << std::endl;

		delete mul;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = mul;
		map.insert(p);


		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseSub(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	Expression<double>* sub = new Function<double>(SUB, arguments);
	if (!inFuncDeclaration) {
		std::cout << "> ";
		sub->print(argArr, sub);
		std::cout << std::endl;

		delete sub;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = sub;
		map.insert(p);


		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseList(Lexer& l, int& paramC, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	std::vector<Expression<double>*> arguments;
	std::pair<std::string, TOKENS> currPair = l.next();

	if (currPair.second != OPEN_PARANTHESES) {
		throw std::exception("Invalid function call");
	}

	while (true) {
		if (l.end()) {
			throw std::exception("Invalid function call");
		}

		Expression<double>* argument = getNextArgument(l, paramC);
		if (l.end()) {
			throw std::exception("Invalid function call");
		}

		currPair = l.next();
		arguments.push_back(argument);

		if (currPair.second != COMMA) {
			if (currPair.second == CLOSE_PARANTHESES)
				break;
			else {
				throw std::exception("Invalid function call");
			}

			throw std::exception("Invalid function call");
		}

	}


	Expression<double>* list = new Function<double>(LIST, arguments);
	if (!inFuncDeclaration) {
		std::cout << "> ";
		list->print(argArr, list);
		std::cout << std::endl;
		delete list;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = list;

		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;

		map.insert(p);
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseHead(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);

	if (firstArgument->getType() != LIST)
		throw std::exception("Invalid argument of head function");

	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument };
	Expression<double>* head = new Function<double>(HEAD, arguments);

	if (!inFuncDeclaration) {
		std::cout << "> ";
		head->print(argArr, head);
		std::cout << std::endl;

		delete head;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = head;

		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;

		map.insert(p);
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseTail(Lexer& l, int& paramC, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, std::string& funcName)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);

	if (firstArgument->getType() != LIST)
		throw std::exception("Invalid argument of head function");
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument };
	Expression<double>* tail = new Function<double>(TAIL, arguments);

	if (!inFuncDeclaration) {
		std::cout << "> ";
		tail->print(argArr, tail);
		std::cout << std::endl;

		delete tail;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = funcName;
		p.second = tail;

		std::pair<std::string, int> p1;
		p1.first = funcName;
		p1.second = paramC;

		map.insert(p);
		funcParamCounter.insert(p1);
		inFuncDeclaration = false;
		paramC = 0;
	}
}

void Parser::parseIndex(bool& inFuncDeclaration, std::pair<std::string, TOKENS>& currPair, int& paramC, std::string& funcName)
{
	if (!inFuncDeclaration) {
		throw std::exception("Index is not allowed outside of function declaration");
	}

	int indexValue = atoi(currPair.first.c_str());

	if ((indexValue + 1) > paramC)
		paramC = indexValue + 1;

	Expression<double>* index = new Index(indexValue);
	std::pair<std::string, Expression<double>*> p;
	p.first = funcName;
	p.second = reinterpret_cast<Expression<double>*>(index);

	std::pair<std::string, int> p1;
	p1.first = funcName;
	p1.second = paramC;

	map.insert(p);
	funcParamCounter.insert(p1);
	inFuncDeclaration = false;
}

void Parser::parseSpecialFunction(std::string& funcName, std::pair<std::string, TOKENS>& currPair, bool& inFuncDeclaration, DynamicArray<int>& argArr, Lexer& l, int& paramC, int& retflag)
{
	retflag = 1;
	funcName = currPair.first;

	if (map.find(funcName) != map.end() && !inFuncDeclaration) {
		if (funcParamCounter[funcName] == 0) {
			std::cout << "> ";
			map[currPair.first]->print(argArr, map[currPair.first]);
			std::cout << std::endl;

			if (!l.end()) {
				currPair = l.next();

				if (currPair.second == OPEN_PARANTHESES) {
					if (l.end()) {
						throw std::exception("Invalid function call");
					}

					currPair = l.next();

					if (currPair.second != CLOSE_PARANTHESES) {
						throw std::exception("Invalid function call");
					}
				}
				else {
					l.prev();
				}
			}
			{ retflag = 2; return; };
		}

	}

	if (l.end()) {
		throw std::exception("Invalid function call");
	}

	currPair = l.next();

	if (currPair.second == ARROW) {
		inFuncDeclaration = true;
	}
	else {

		if (currPair.second != OPEN_PARANTHESES) {
			throw std::exception("Invalid function call");
		}

		int cnt = 0;
		while (currPair.second != CLOSE_PARANTHESES)
		{
			if (l.end()) {
				throw std::exception("Invalid function call");
			}
			currPair = l.next();

			if (currPair.second != CONSTANT) {
				if (currPair.second == CLOSE_PARANTHESES) {
					break;
				}
				else {
					throw std::exception("Invalid function call");
				}
			}

			int arg = atoi(currPair.first.c_str());

			cnt++;
			if (l.end()) {
				throw std::exception("Invalid function call");
			}

			currPair = l.next();
			argArr.push_back(arg);

			if (currPair.second != COMMA) {
				if (currPair.second == CLOSE_PARANTHESES) {
					break;
				}
				else {
					throw std::exception("Invalid function call");
				}
			}
		}
		double res = 0;
		std::cout << "> ";
		if (funcParamCounter[funcName] != cnt) {
			throw std::exception("Invalid arguments of function call");
		}

		Expression<double>* func = map[funcName];
		func->print(argArr, func);
		std::cout << std::endl;
		argArr.clear();
		paramC = 0;
	}
}

Expression<double>* Parser::getEQArguments(Lexer& l, int& paramC) {
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	Expression<double>* firstArgument = getNextArgument(l, paramC);

	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}

	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	arguments.push_back(secondArgument);
	return new Function<double>(EQ, arguments);
}

void Parser::parseEQ(Lexer& l, int& paramC, bool inFunction, DynamicArray<int>& arr){
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	std::pair<std::string, int> currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	Expression<double>* eq = new Function<double>(EQ, arguments);
	if (!inFunction) {
		std::cout << "> ";
		eq->print(arr, eq);
		std::cout << std::endl;

		delete eq;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = "EQ";
		p.second = eq;
		map.insert(p);


		std::pair<std::string, int> p1;
		p1.first = p.first;
		p1.second = paramC;
		funcParamCounter.insert(p1);
		inFunction = false;
		paramC = 0;
	}
}

void Parser::parseMAP(Lexer& l, int& paramC, bool inFunction, DynamicArray<int>& arr)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	std::pair<std::string, TOKENS> mapPair = l.next();
	Expression<double>* firstArgument = nullptr;

	switch (mapPair.second)
	{
	case SQRT: {
		firstArgument = new Function<double>(SQRT);
		break;
	}
	case SIN: {
		firstArgument = new Function<double>(SIN);
		break;
	}
	case COS: {
		firstArgument = new Function<double>(COS);
		break;
	}
	case HEAD: {
		firstArgument = new Function<double>(HEAD);
		break;
	}
	case TAIL: {
		firstArgument = new Function<double>(TAIL);
		break;
	}
	case SPECIAL_FUNCTION: {
		if (funcParamCounter[mapPair.first] != 1) {
			throw std::exception("Invalid arguments of map function");
		}

		firstArgument = map[mapPair.first]->clone();
		break;
	}
	default:
		throw std::exception("Invalid argument of map function");
	}

	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	mapPair = l.next();
	if (mapPair.second != COMMA) {
		throw std::exception("Invalid Function Call");
	}

	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	Expression<double>* secondArgument = getNextArgument(l, paramC);

	if (secondArgument->getType() != LIST) {
		throw std::exception("Invalid Argument of map");
	}
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	std::pair<std::string, int> currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	Expression<double>* mapFunc = new Function<double>(MAP, arguments);

	if (!inFunction) {
		std::cout << "> ";
		mapFunc->print(arr, mapFunc);
		std::cout << std::endl;

		delete mapFunc;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = "MAP";
		p.second = mapFunc;

		std::pair<std::string, int> p1;
		p1.first = p.first;
		p1.second = paramC;

		map.insert(p);
		funcParamCounter.insert(p1);
		inFunction = false;
		paramC = 0;
	}
}

void Parser::parseSUM(Lexer& l, int& paramC, bool inFunction, DynamicArray<int>& arr)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	std::pair<std::string, int> currPair = l.next();

	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid function call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	Expression<double>* sum = new Function<double>(ADD, arguments);
	if (!inFunction) {
		std::cout << "> ";
		sum->print(arr, sum);
		std::cout << std::endl;
		delete sum;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = "SUM";
		p.second = sum;

		std::pair<std::string, int> p1;
		p1.first = p.first;
		p1.second = paramC;
		funcParamCounter.insert(p1);

		map.insert(p);
		inFunction = false;
	}
}

void Parser::parseCONCAT(Lexer& l, int& paramC, bool inFunction, DynamicArray<int>& arr)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (firstArgument->getType() != LIST) {
		throw std::exception("Invalid arguments of concat function");
	}
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}

	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (secondArgument->getType() != LIST) {
		throw std::exception("Invalid arguments of concat function");
	}
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	std::pair<std::string, int> currPair = l.next();
	if (currPair.second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	Expression<double>* concat = new Function<double>(CONCAT, arguments);

	if (!inFunction) {
		std::cout << "> ";
		concat->print(arr, concat);
		std::cout << std::endl;
		delete concat;
	}
	else {
		std::pair<std::string, Expression<double>*> p;
		p.first = "CONCAT";
		p.second = concat;

		std::pair<std::string, int> p1;
		p1.first = p.first;
		p1.second = paramC;

		map.insert(p);
		funcParamCounter.insert(p1);
		inFunction = false;
		paramC = 0;
	}
}


Expression<double>* Parser::getNextArgument(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	std::pair<std::string, TOKENS> currPair = l.next();

	if (currPair.second == CONSTANT) {
		double n = std::strtod(currPair.first.c_str(), NULL);
		return new Constant<double>(n);
	}
	else if (currPair.second == INDEX) {
		int index = atoi(currPair.first.c_str());

		if ((index + 1) > paramC)
			paramC = index + 1;

		return new Index(index);
	}
	else {
		switch (currPair.second)
		{
		case CONCAT: {
			return getConcatArguments(l, paramC);
		}
		case MAP: {
			return getMapArguments(l, paramC);
		}
		case TAIL: {
			return getTailArguments(l, paramC);
		}
		case HEAD: {
			return getHeadArguments(l, paramC);
		}
		case LIST: {
			return getListArguments(l, paramC);
		}
		case ADD: {
			return getAddArguments(l, paramC);
		}
		case SUB: {
			return getSubArguments(l, paramC);
		}
		case MUL: {
			return getMulArguments(l, paramC);
		}
		case DIV: {
			return getDivArguments(l, paramC);
		}
		case POW: {
			return getPowArguments(l, paramC);
		}
		case SQRT: {
			return getSqrtArguments(l, paramC);
		}
		case SIN: {
			return getSinArguments(l, paramC);
		}
		case COS: {
			return getCosArguments(l, paramC);
		}
		case LE: {
			return getLeArguments(l, paramC);
		}
		case NAND: {
			return getNandArguments(l, paramC);
		}
		case IF: {
			return getIfArguments(l, paramC);
		}
		case SPECIAL_FUNCTION: {
			return getSpecialFunctionArguments(currPair, l, paramC);
		}
		default:
			throw std::exception("Invalid Function Call");
		}
	}
}

Expression<double>* Parser::getSpecialFunctionArguments(std::pair<std::string, TOKENS>& currPair, Lexer& l, int& paramC)
{
	std::vector<Expression<double>*> arguments;
	std::string funcName = currPair.first;

	if (map.find(funcName) != map.end() && funcParamCounter[funcName] == 0) {
		return map[funcName]->clone();
	}

	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	currPair = l.next();

	if (currPair.second != OPEN_PARANTHESES) {
		throw std::exception("Invalid function call");
	}

	while (true) {
		if (l.end()) {
			throw std::exception("Invalid function call");
		}

		Expression<double>* argument = getNextArgument(l, paramC);
		if (l.end()) {
			throw std::exception("Invalid function call");
		}

		currPair = l.next();
		arguments.push_back(argument);

		if (currPair.second != COMMA) {
			if (currPair.second == CLOSE_PARANTHESES)
				break;
			else if (l.end()) {
				throw std::exception("Invalid function call");
			}

			throw std::exception("Invalid function call");
		}
	}
	return new Function<double>(SPECIAL_FUNCTION, arguments);
}

Expression<double>* Parser::getIfArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* thirdArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	arguments.push_back(secondArgument);
	arguments.push_back(thirdArgument);
	return new Function<double>(IF, arguments);
}

Expression<double>* Parser::getNandArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	arguments.push_back(secondArgument);
	return new Function<double>(NAND, arguments);
}

Expression<double>* Parser::getLeArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	arguments.push_back(secondArgument);
	return new Function<double>(LE, arguments);
}

Expression<double>* Parser::getCosArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	return new Function<double>(COS, arguments);
}

Expression<double>* Parser::getSinArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	return new Function<double>(SIN, arguments);
}

Expression<double>* Parser::getSqrtArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	return new Function<double>(SQRT, arguments);
}

Expression<double>* Parser::getPowArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	arguments.push_back(secondArgument);
	return new Function<double>(POW, arguments);
}

Expression<double>* Parser::getDivArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	arguments.push_back(secondArgument);
	return new Function<double>(DIV, arguments);
}

Expression<double>* Parser::getMulArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}
	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);

	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	arguments.push_back(secondArgument);
	return new Function<double>(MUL, arguments);
}

Expression<double>* Parser::getSubArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	arguments.push_back(secondArgument);
	return new Function<double>(SUB, arguments);
}

Expression<double>* Parser::getAddArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	arguments.push_back(secondArgument);
	return new Function<double>(ADD, arguments);
}

Expression<double>* Parser::getListArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	std::vector<Expression<double>*> arguments;
	std::pair<std::string, TOKENS> currPair = l.next();

	if (currPair.second != OPEN_PARANTHESES) {
		throw std::exception("Invalid function call");
	}

	while (true) {
		if (l.end()) {
			throw std::exception("Invalid function call");
		}

		Expression<double>* argument = getNextArgument(l, paramC);
		if (l.end()) {
			throw std::exception("Invalid function call");
		}

		currPair = l.next();
		arguments.push_back(argument);

		if (currPair.second != COMMA) {
			if (currPair.second == CLOSE_PARANTHESES)
				break;
			else if (l.end()) {
				throw std::exception("Invalid function call");
			}

			throw std::exception("Invalid function call");
		}
	}
	return new Function<double>(LIST, arguments);
}

Expression<double>* Parser::getHeadArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	return new Function<double>(HEAD, arguments);
}

Expression<double>* Parser::getTailArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (firstArgument->getType() != LIST) {
		throw std::exception("Invalid argument of map function");
	}
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	return new Function<double>(TAIL, arguments);
}

Expression<double>* Parser::getSumArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}
	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	if (l.next().second != CLOSE_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments;
	arguments.push_back(firstArgument);
	arguments.push_back(secondArgument);
	return new Function<double>(ADD, arguments);
}

Expression<double>* Parser::getConcatArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	Expression<double>* firstArgument = getNextArgument(l, paramC);
	if (firstArgument->getType() != LIST) {
		throw std::exception("Invalid argument of concat function");
	}

	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}

	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (secondArgument->getType() != LIST) {
		throw std::exception("Invalid argument of concat function");
	}
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}
	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	return new Function<double>(CONCAT, arguments);
}

Expression<double>* Parser::getMapArguments(Lexer& l, int& paramC)
{
	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != OPEN_PARANTHESES) {
		throw std::exception("Invalid Function Call");
	}

	std::pair<std::string, TOKENS> mapPair = l.next();
	Expression<double>* firstArgument = nullptr;

	switch (mapPair.second)
	{
	case SQRT: {
		firstArgument = new Function<double>(SQRT);
		break;
	}
	case SIN: {
		firstArgument = new Function<double>(SIN);
		break;
	}
	case COS: {
		firstArgument = new Function<double>(SIN);
		break;
	}
	case HEAD: {
		firstArgument = new Function<double>(HEAD);
		break;
	}
	case TAIL: {
		firstArgument = new Function<double>(TAIL);
		break;
	}
	case SPECIAL_FUNCTION: {
		if (funcParamCounter[mapPair.first] != 1) {
			throw std::exception("Invalid arguments of map function");
		}

		firstArgument = map[mapPair.first]->clone();
		break;
	}
	default:
		throw std::exception("Invalid argument of map function");
	}

	if (l.end()) {
		throw std::exception("Invalid Function call");
	}

	if (l.next().second != COMMA) {
		throw std::exception("Invalid Function Call");
	}

	Expression<double>* secondArgument = getNextArgument(l, paramC);
	if (secondArgument->getType() != LIST) {
		throw std::exception("Invalid argument of map function");
	}
	if (l.end()) {
		throw std::exception("Invalid Function Call");
	}

	std::vector<Expression<double>*> arguments = { firstArgument, secondArgument };
	return new Function<double>(MAP, arguments);
}

Parser::~Parser()
{
	for (std::pair<std::string, Expression<double>*> it : map) {
		delete it.second;
	}
}
