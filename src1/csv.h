#ifndef CSV_H_INCLUDED
#define CSV_H_INCLUDED

#include "types.h"

InputData ReadInputData(std::ifstream&);

class EWrongFieldsNumber {
public:
	int line;
	EWrongFieldsNumber(int l)
		: line(l)
	{
	}
};

class EWrongHeader {
};

class EWrongSymbol {
public:
	int line, pos;
	EWrongSymbol(int l, int p)
		: line(l), pos(p)
	{
	}
};

class EWEquals0 {
public:
	int line;
	EWEquals0(int l)
		: line(l)
	{
	}
};

class EWrongR {
public:
	int line;
	EWrongR(int l)
		: line(l)
	{
	}
};

class ENotIncreasingW {
public:
	int line;
	ENotIncreasingW(int l)
		: line(l)
	{
	}
};

class EEmptyFile {
};

#endif // CSV_H_INCLUDED