#include "numin.h"
#include "deltamin.h"

int Numin(Points &points, int a, int b, double delta0)
{
	int numin = b-a+1;
	for (int nu = 0; nu <= b-a+1; ++nu) {
		double gamma1;
		if (Deltamin(points, a, b, nu, gamma1) <= delta0) {
			numin = nu;
			break;
		}
	}
	return numin;
}