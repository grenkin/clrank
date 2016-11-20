#include "..\..\src\deltamin.h"
#include "..\..\src\csv.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>

double gamma2(Point p1, Point p2)
{
	return -(p1.y - p2.y)/(p1.x - p2.x);
}

struct PointOfIntersection {
	int i, j;
	double gamma;
};

class CmpPointsOfIntersection {
public:
	bool operator() (const PointOfIntersection &p1, const PointOfIntersection &p2) const
	{
		return p1.gamma < p2.gamma;
	}
};

std::vector<Polyline> TestPolylines(Points &points, int a, int b)
{
	std::vector<Polyline> res;
	res.resize(b-a+1);
	std::vector<PointOfIntersection> pi;
	for (int i = a; i <= b; ++i) {
		for (int j = i+1; j <= b; ++j) {
			PointOfIntersection p;
			p.i = i;
			p.j = j;
			p.gamma = gamma2(points[i], points[j]);
			pi.push_back(p);
		}
	}
	sort(pi.begin(), pi.end(), CmpPointsOfIntersection());
	std::vector<int> order;
	for (int i = b; i >= a; --i)
		order.push_back(i);
	for (int i = 0; i < b-a+1; ++i)
		res[i].push_back(b-i);
	for (int i = 0; i < pi.size(); ++i) {
		for (int j = 0; j < b-a+1; ++j) {
			if (order[j] == pi[i].i) {
				order[j] = pi[i].j;
				res[j].push_back(pi[i].j);
			}
			else if (order[j] == pi[i].j) {
				order[j] = pi[i].i;
				res[j].push_back(pi[i].i);
			}
		}
	}
	return res;
}

int main()
{
	int ok_cnt = 0;
	for (int test = 1; test <= 18; ++test) {
		bool ok = true;
		char s[10];
		itoa(test, s, 10);
		std::ifstream fin(s);
		InputData input_data = ReadInputData(fin);
		fin.close();
		int n = input_data.size();
		Points points;
		int N = n+1;
		for (int i = 0; i < n; ++i) {
			int r = i+1;
			double x = log((double)(N-r)/r);
			double y = log(input_data[i].w);
			Point p(x, y);
			points.push_back(p);
		}
		for (int a = 0; a < n; ++a) {
			for (int b = a+2; b < n; ++b) {
				std::vector<Polyline> polylines = TestPolylines(points, a, b);
				for (int nu0 = 0; nu0 < b-a+1; ++nu0) {
					std::vector<Polyline> max_polylines = MaxPolylines(points, a, b, nu0);
					std::vector<Polyline> min_polylines = MinPolylines(points, a, b, nu0);
					for (int i = 0; i <= nu0; ++i) {
						if (max_polylines[i].size() != polylines[i].size())
							ok = false;
						else {
							for (int j = 0; j < max_polylines[i].size(); ++j) {
								if (max_polylines[i][j] != polylines[i][j])
									ok = false;
							}
						}

						if (min_polylines[i].size() != polylines[b-a-i].size())
							ok = false;
						else {
							for (int j = 0; j < min_polylines[i].size(); ++j) {
								if (min_polylines[i][min_polylines[i].size()-j-1] != polylines[b-a-i][j])
									ok = false;
							}
						}
					}
				}		
			}
		}
		if (ok) {
			std::cout << "Test " << test << " OK\n";
			++ok_cnt;
		}
		else
			std::cout << "Test " << test << " WA\n";
	}
	std::cout << "OK: " << ok_cnt << " WA: " << 18-ok_cnt;

	return 0;
}