#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <string>
#include <vector>

struct InputDataRecord {
	std::string name;
	double w;
	InputDataRecord(std::string _name, double _w)
		: name(_name), w(_w)
	{
	}
};

typedef std::vector<InputDataRecord> InputData;

struct Point {
	double x, y;
	Point(double _x, double _y)
		: x(_x), y(_y)
	{
	}
};

typedef std::vector<Point> Points;

#endif // TYPES_H_INCLUDED