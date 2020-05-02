#pragma once
#include <string>
#include <math.h>
#include <list>
#include <vector>
#include "Lexer.h"
#include <assert.h>
#include <map>

///Abstract class of the expressions in ThisFunc
template <typename T>
class Expression {
public:
	virtual Expression* clone()const = 0;
	virtual void print(DynamicArray<int>& darr, Expression<T>* root)const = 0;
	virtual TOKENS getType()const = 0;
	virtual ~Expression() = default;
	virtual T getValue(DynamicArray<int>& darr, Expression<T>* root)const = 0;
};

template<typename T>
class Constant : public Expression<T> {
private:
	T value;
public:
	Constant(const T& value) : value(value) {}

	Constant* clone()const override;

	TOKENS getType()const override;

	void print(DynamicArray<int>& darr, Expression<T>* root)const override;

	T getValue(DynamicArray<int>& darr, Expression<T>* root)const override;
};

class Index : public Expression<double> {
private:
	int index;
public:
	Index(const int& index) : index(index) {}

	void print(DynamicArray<int>& darr, Expression<double>* root)const override;

	double getValue(DynamicArray<int>& darr, Expression<double>* root)const override;

	TOKENS getType()const override;

	Expression<double>* clone()const override;
};


///Expression tree of function calls
template<typename T>
class Function : public Expression<T>
{
public:
	Function(const Function<T>& func);

	Function<T>& operator=(const Function<T>& rhs);

	Function(TOKENS type);

	Function(TOKENS type, std::vector<Expression<T>*>& arguments);

	size_t size()const;

	void print(DynamicArray<int>& darr, Expression<T>* root)const override;

	Function* clone()const override;

	TOKENS getType()const override;

	T getValue(DynamicArray<int>& darr, Expression<T>* root)const override;

	~Function();

private:
	TOKENS functionType;
	std::vector<Expression<T>*> arguments;

	void copy(const Function<T>& func);
	void clear();
};

inline void Index::print(DynamicArray<int>& darr, Expression<double>* root) const {
	std::cout << darr[index];
}

inline double Index::getValue(DynamicArray<int>& darr, Expression<double>* root) const {
	return darr[index];
}

inline TOKENS Index::getType() const {
	return INDEX;
}

inline Expression<double>* Index::clone() const {
	return new Index(*this);
}

template<typename T>
inline void Function<T>::copy(const Function<T>& func) {
	functionType = func.functionType;

	for (size_t i = 0; i < func.arguments.size(); i++)
		arguments.push_back(func.arguments[i]->clone());
}

template<typename T>
inline Function<T>& Function<T>::operator=(const Function<T>& rhs) {
	if (this != &rhs) {
		clear();
		copy(rhs);
	}

	return *this;
}


template<typename T>
inline void Function<T>::clear() {
	for (size_t i = 0; i < arguments.size(); i++) {
		delete arguments[i];
	}
}

template<typename T>
inline Function<T>::Function(const Function<T>& func) {
	copy(func);
}

template<typename T>
inline Function<T>::Function(TOKENS type) {
	functionType = type;
}

template<typename T>
inline Function<T>::Function(TOKENS type, std::vector<Expression<T>*>& arguments)
	: functionType(type), arguments(arguments) {
}

template<typename T>
inline size_t Function<T>::size() const {
	return arguments.size();
}

template<typename T>
inline void Function<T>::print(DynamicArray<int>& darr, Expression<T>* root) const {
	if (functionType == LIST) {
		std::cout << "[";
		for (size_t i = 0; i < arguments.size(); i++) {
			if (i == arguments.size() - 1) {
				arguments[i]->print(darr, root);
			}
			else {
				arguments[i]->print(darr, root);
				std::cout << ", ";
			}

			if (!darr.empty())
				darr.pop_front();
		}
		std::cout << ']';
	}
	else if (functionType == HEAD) {
		Function<T>* listPtr = dynamic_cast<Function<T>*>(arguments[0]);
		listPtr->arguments[0]->print(darr, root);
	}
	else if (functionType == TAIL) {
		std::cout << "[";
		Function<T>* listPtr = dynamic_cast<Function<T>*>(arguments[0]);

		for (size_t i = 1; i < listPtr->arguments.size(); i++) {
			if (i < listPtr->arguments.size() - 1) {
				listPtr->arguments[i]->print(darr, root);
				std::cout << ", ";
			}
			else {
				listPtr->arguments[i]->print(darr, root);
				if (!darr.empty())
					darr.pop_front();
			}
		}

		std::cout << "]";
	}
	else if (functionType == MAP) {
		Function<T>* oldListPtr = dynamic_cast<Function<T>*>(arguments[1]);

		std::vector<Expression<T>*> args;

		for (Expression<T>* expr : oldListPtr->arguments) {
			if (arguments[0]->getType() != INDEX) {
				Function<T>* func = dynamic_cast<Function<T>*>(arguments[0]);

				if (func->getType() == HEAD || func->getType() == TAIL) {
					if (expr->getType() != LIST)
						throw std::exception("Invalid argument of map function");
				}

				func->arguments.push_back(expr);
				args.push_back(func->clone());
				func->arguments.pop_back();

				if(expr->getType() != LIST)
				darr.push_back(expr->getValue(darr, root));
			}
			else {
				darr.push_back(expr->getValue(darr, root));
				args.push_back(expr->clone());
			}
		}

		Expression<T>* newList = new Function<T>(LIST, args);
		newList->print(darr, root);
		delete newList;
	}
	else if (functionType == CONCAT) {
		std::vector<Expression<T>*> args;

		Function<T>* list1 = dynamic_cast<Function<T>*>(arguments[0]);
		Function<T>* list2 = dynamic_cast<Function<T>*>(arguments[1]);

		for (Expression<T>* expr : list1->arguments) {
			args.push_back(expr->clone());
		}

		for (Expression<T>* expr : list2->arguments) {
			args.push_back(expr->clone());
		}

		Expression<T>* list = new Function<T>(LIST, args);
		list->print(darr, root);
		delete list;
	}
	else if (functionType == SPECIAL_FUNCTION) {
		arguments[0]->print(darr, root);
	}
	else {
		std::cout << getValue(darr, root);
	}
}

template<typename T>
inline Function<T>* Function<T>::clone() const {
	return new Function<T>(*this);
}

template<typename T>
inline TOKENS Function<T>::getType() const {
	return functionType;
}

template<typename T>
inline T Function<T>::getValue(DynamicArray<int>& darr, Expression<T>* root) const {
	switch (functionType)
	{
	case ADD:
		return arguments[0]->getValue(darr, root) + arguments[1]->getValue(darr, root);
	case SUB:
		return arguments[0]->getValue(darr, root) - arguments[1]->getValue(darr, root);
	case MUL:
		return arguments[0]->getValue(darr, root) * arguments[1]->getValue(darr, root);
	case DIV:
	{
		double secValue = arguments[1]->getValue(darr, root);
		if (secValue == 0) {
			throw std::logic_error("Division by 0");
		}
		if (arguments[0])
			return arguments[0]->getValue(darr, root) / secValue;
	}
	case POW:
		return std::pow(arguments[0]->getValue(darr, root), arguments[1]->getValue(darr, root));
	case SQRT:
		return std::sqrt(arguments[0]->getValue(darr, root));
	case SIN:
		return std::sin(arguments[0]->getValue(darr, root) * 3.14159265 / 180);
	case IF: {
		if (arguments[0]->getValue(darr, root)) {
			return arguments[1]->getValue(darr, root);
		}
		else {
			return arguments[2]->getValue(darr, root);
		}
	}
	case COS:
		return std::cos(arguments[0]->getValue(darr, root) * 3.14159265 / 180);
	case EQ:
		return arguments[0]->getValue(darr, root) == arguments[1]->getValue(darr, root);
	case NAND:
		return (!arguments[0]->getValue(darr, root) || !arguments[1]->getValue(darr, root));
	case LE:
		return arguments[0]->getValue(darr, root) <= arguments[1]->getValue(darr, root);
	case SPECIAL_FUNCTION:
	{
		T  arg = arguments[0]->getValue(darr, root);
		DynamicArray<int> darrCopy = darr;
		darr.clear();
		darr.push_back(arg);
		T funcRes = root->getValue(darr, root);
		darr.clear();
		darr = darrCopy;
		return funcRes;
	}
	default:
		throw std::exception("Invalid function call");
	}
}

template<typename T>
inline Function<T>::~Function() {
	clear();
}

template<typename T>
inline Constant<T>* Constant<T>::clone() const {
	return new Constant<T>(*this);
}

template<typename T>
inline TOKENS Constant<T>::getType() const {
	return CONSTANT;
}

template<typename T>
inline void Constant<T>::print(DynamicArray<int>& darr, Expression<T>* root) const {
	std::cout << value;
}

template<typename T>
inline T Constant<T>::getValue(DynamicArray<int>& darr, Expression<T>* root) const {
	return value;
}
