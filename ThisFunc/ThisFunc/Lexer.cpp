#include "Lexer.h"

///Finding number in string
void Lexer::findNumber(const std::string& str, std::string& res) {
	bool hasDot = false;
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == '-') {
			if (i == 0) {
				res += str[i];
			}
			else {
				throw std::exception("Invalid number");
			}
		}
		else if (str[i] >= '0' && str[i] <= '9') {
			res += str[i];
		}
		else if (str[i] == '.') {
			if (!hasDot) {
				res += str[i];
				hasDot = true;
			}
			else {
				throw std::exception("Invalid number");
			}
		}
		else {
			if (res == "-") {
				throw std::exception("Invalid identifier");
			}
			return;
		}
	}

	if (str == "-") {
		throw std::exception("Invalid identifier");
	}
}

std::pair<std::string, TOKENS>& Lexer::next() {
	int index = currToken;
	currToken++;
	return tokens[index];
}

std::pair<std::string, TOKENS>& Lexer::prev()
{
	if (currToken == 0) {
		return tokens[currToken];
	}
	else {
		return tokens[--currToken];
	}
}

///Converting lines to tokens
void Lexer::processArr(DynamicArray<std::string>& darr) {
	for (size_t i = 0; i < darr.getSize(); i++) {
		tokenizeLine(darr[i]);
	}
}

///Converting string into list of tokens
void Lexer::tokenizeLine(const std::string& str)
{
	std::string currString = "";
	std::string prevString = "";
	for (size_t j = 0; str[j] != '\0'; j++) {
		currString += str[j];

		if (str[j] != ' ' && str[j] != '\t' && str[j] != '\n' && !(str[j] >= '0' && str[j] <= '9')) {
			prevString += str[j];
		}

		if ((str[j] >= '0' && str[j] <= '9') || (j > 0 && str[j] == '-' && str[j - 1] != '<') ||
			(j == 0 && str[j] == '-')) {
			std::string strNum = "";
			findNumber(str.substr(j), strNum);

			std::pair<std::string, TOKENS> p;
			p.first = strNum;
			if (j > 0 && str[j - 1] == '#') {
				for (size_t i = 0; strNum[i] != '\0'; i++) {
					if (strNum[i] == '.')
						throw std::exception("Invalid index value");
				}
				p.second = INDEX;
			}
			else {
				p.second = CONSTANT;
			}

			j += (strNum.size() - 1);
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "add") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = ADD;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "sub") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = SUB;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "mul") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = MUL;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "div") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = DIV;
			tokens.push_back(p);
			currString = "";
		}
		else if (currString == "pow") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = POW;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "sqrt") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = SQRT;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "sin") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = SIN;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "cos") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = COS;
			tokens.push_back(p);
			currString = "";
		}
		else if (currString == "eq") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = EQ;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "le") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = LE;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "nand") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = NAND;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}

		else if (currString == "(") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = OPEN_PARANTHESES;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (str[j] == '(') {
			if (specialFunctions.find(prevString)) {
				std::pair <std::string, TOKENS> p1;
				std::pair<std::string, TOKENS> p2;
				p1.first = prevString;
				p1.second = SPECIAL_FUNCTION;
				p2.first = str[j];
				p2.second = OPEN_PARANTHESES;
				tokens.push_back(p1);
				tokens.push_back(p2);
			}
			else {
				if (prevString.size() != 0) {
					throw std::exception("Invalid identifier");
				}
			}
			currString = "";
			prevString = "";
		}
		else if (specialFunctions.find(currString)) {
			std::pair <std::string, TOKENS> p;
			p.first = currString;
			p.second = SPECIAL_FUNCTION;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == ")") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = CLOSE_PARANTHESES;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "if") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = IF;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == ",") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = COMMA;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "list") {
			std::pair<std::string, TOKENS> p;
			p.first = currString;
			p.second = LIST;
			tokens.push_back(p);
			currString = "";
			prevString = "";
		}
		else if (currString == "head") {
		std::pair<std::string, TOKENS> p;
		p.first = currString;
		p.second = HEAD;
		tokens.push_back(p);
		currString = "";
		prevString = "";
		}
		else if (currString == "tail") {
		std::pair<std::string, TOKENS> p;
		p.first = currString;
		p.second = TAIL;
		tokens.push_back(p);
		currString = "";
		prevString = "";
		}
		else if (currString == "map") {
		std::pair<std::string, TOKENS> p;
		p.first = currString;
		p.second = MAP;
		tokens.push_back(p);
		currString = "";
		prevString = "";
		}
		else if (currString == "concat") {
		std::pair<std::string, TOKENS> p;
		p.first = currString;
		p.second = CONCAT;
		tokens.push_back(p);
		currString = "";
		}
		else if (currString == "<-") {
		std::pair<std::string, TOKENS> p1;
		std::pair<std::string, TOKENS> p2;

		prevString = prevString.substr(0, prevString.size() - 2);
		if (prevString.size() == 0) {
			throw std::exception("Invalid function declaration");
		}
		p2.first = prevString;
		p2.second = SPECIAL_FUNCTION;

		p1.first = currString;
		p1.second = ARROW;
		tokens.push_back(p2);
		tokens.push_back(p1);
		specialFunctions.push_back(prevString);
		currString = "";
		prevString = "";
		}
		else if (str[j] == '\n') {
			if (prevString.size() != 0) {
				throw std::exception("Invalid identifier");
			}

			currString = "";
		}
		else if (j <= str.size() - 1 && str[j + 1] == '\0') {
			if (prevString.size() != 0) {
				throw std::exception("Invalid identifier");
			}
		}
		else if (str[j] == ' ' || str[j] == '\t') {
			currString = "";
		}
	}
}

//Reading from file
DynamicArray<std::pair<std::string, TOKENS>>& Lexer::processFile(const char* fileName) {
	std::ifstream file(fileName);

	if (!file) {
		throw std::exception("Error opening file!");
	}

	DynamicArray<std::string> strings;
	std::string currString = "";

	char c;
	while (file.get(c)) {
		if (c == '\n') {
			currString += c;
			strings.push_back(currString);
			currString = "";
		}
		else {
			currString += c;
		}
	}

	if (currString != "") {
		strings.push_back(currString);
	}

	processArr(strings);

	return tokens;
}