#ifndef DELTAMIN_H_INCLUDED
#define DELTAMIN_H_INCLUDED

#include "types.h"

typedef std::vector<int> Polyline;

double Deltamin(Points &points, int a, int b, int nu0, double &gamma);
std::vector<Polyline> MaxPolylines(Points &points, int a, int b, int nu0);
std::vector<Polyline> MinPolylines(Points &points, int a, int b, int nu0);

#endif // DELTAMIN_H_INCLUDED