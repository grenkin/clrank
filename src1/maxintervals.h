#ifndef MAXINTERVALS_H_INCLUDED
#define MAXINTERVALS_H_INCLUDED

#include "types.h"

struct Interval {
	int a, b;
	Interval(int _a, int _b)
		: a(_a), b(_b)
	{
	}
};

typedef std::vector<Interval> MaxIntervals;

MaxIntervals MaxIntervalsSet(std::vector<std::vector<double> > &deltamin, double delta0, int nu0);

#endif // MAXINTERVALS_H_INCLUDED