#include <iostream>
#include <string>
#include "Lexer.h"
#include "Parser.h"
#include <fstream>
#include "Tree.h"
#include<list>

///If argc is 1 then the program starts in interactive mode, otherwise 
///  the interpreter is run on the file
int main(int argc, char* argv[])
{
	Lexer l;
	Parser p;
	try {
		//no argument to source file
		if (argc == 1) {
			while (true) {
				char c;
				std::string line = "";
				std::cout << "> ";
				while (c = getchar()) {
					if (c == '\n') {
						break;
					}
					else {
						line += c;
					}
				}
				l.tokenizeLine(line);
				p.parse(l);
			}
		}
		else {
			l.processFile(argv[1]);
			p.parse(l);
		}
	}
	catch (std::exception& e) {
		std::cout << e.what();
	}

	return 0;
}