#include "deltamin.h"

namespace {

	double gamma2(Point p1, Point p2)
	{
		return -(p1.y - p2.y)/(p1.x - p2.x);
	}

	double phi(Point p, double gamma)
	{
		return p.y + gamma*p.x;
	}

	double MinDifference(Points &points, Polyline &max_p, Polyline &min_p, double &gamma)
	{
		double m;
		double g1, g2, g;
		int i = 0;
		int j = min_p.size()-1;
		g1 = gamma2(points[max_p[i]], points[max_p[i+1]]);
		g2 = gamma2(points[min_p[j]], points[min_p[j-1]]);
		g = g1 < g2 ? g1 : g2;
		m = phi(points[max_p[i]], g) - phi(points[min_p[j]], g);
		gamma = g;
		while (1) {
			if (i == max_p.size()-1 && j == 0)
				break;
			if (i == max_p.size()-1) {
				g = gamma2(points[min_p[j]], points[min_p[j-1]]);
				--j;
			}
			else if (j == 0) {
				g = gamma2(points[max_p[i]], points[max_p[i+1]]);
				++i;
			}
			else {
				g1 = gamma2(points[max_p[i]], points[max_p[i+1]]);
				g2 = gamma2(points[min_p[j]], points[min_p[j-1]]);
				if (g1 < g2) {
					g = g1;
					++i;
				}
				else {
					g = g2;
					--j;
				}
			}
			double phi1 = phi(points[max_p[i]], g) - phi(points[min_p[j]], g);
			if (phi1 < m) {
				m = phi1;
				gamma = g;
			}
		}
		return m;
	}

}

double Deltamin(Points &points, int a, int b, int nu0, double &gamma)
//должны выполняться неравенства:
//  b-a+1 >= 3,
//  nu0 <= b-a+1
{
	if (!(b-a+1 >= 3))
		throw;
	if (!(nu0 <= b-a+1))
		throw;
	if (nu0 == b-a+1) {
		gamma = 1.0;
		return 0.0;
	}
	std::vector<Polyline> max_polylines = MaxPolylines(points, a, b, nu0);
	std::vector<Polyline> min_polylines = MinPolylines(points, a, b, nu0);
	double m;
	for (int nu = 0; nu <= nu0; ++nu) {
		double gamma1;
		double md = MinDifference(points, max_polylines[nu], min_polylines[nu0-nu], gamma1);
		if (nu == 0 || md < m) {
			m = md;
			gamma = gamma1;
		}
	}
	return m/2;
}

std::vector<Polyline> MaxPolylines(Points &points, int a, int b, int nu0)
//должны выполняться неравенства:
//  b-a+1 >= 3,
//  nu0 < b-a+1
{
	if (!(b-a+1 >= 3))
		throw;
	if (!(nu0 < b-a+1))
		throw;
	std::vector<Polyline> k;
	k.resize(nu0+1);
	k[0].push_back(b);
	k[0].push_back(b-1);
	if (nu0 > 0) {
		k[1].push_back(b-1);
		k[1].push_back(b);
	}
	for (int q = b-2; q >= a; --q) {
		std::vector<int> old_tail, new_tail;
		for (int nu = 0; nu <= nu0; ++nu) {
			if (nu == b-q) {
				k[nu].push_back(q);
				for (int i = 0; i < old_tail.size(); ++i)
					k[nu].push_back(old_tail[i]);
				break;
			}
			else {
				int l = k[nu].size()-1;
				while (l > 0) {
					double gamma = gamma2(points[k[nu][l]], points[k[nu][l-1]]);
					if (phi(points[k[nu][l]], gamma) > phi(points[q], gamma))
						break;
					--l;
				}
				new_tail.clear();
				for (int i = l; i < k[nu].size(); ++i)
					new_tail.push_back(k[nu][i]);
				int s = k[nu].size();
				for (int i = l+1; i < s; ++i)
					k[nu].pop_back();
				k[nu].push_back(q);
				for (int i = 0; i < old_tail.size(); ++i)
					k[nu].push_back(old_tail[i]);
				old_tail = new_tail;
			}
		}
	}
	return k;
}

std::vector<Polyline> MinPolylines(Points &points, int a, int b, int nu0)
//должны выполняться неравенства:
//  b-a+1 >= 3,
//  nu0 < b-a+1
{
	if (!(b-a+1 >= 3))
		throw;
	if (!(nu0 < b-a+1))
		throw;
	std::vector<Polyline> k;
	k.resize(nu0+1);
	k[0].push_back(b);
	k[0].push_back(b-1);
	if (nu0 > 0) {
		k[1].push_back(b-1);
		k[1].push_back(b);
	}
	for (int q = b-2; q >= a; --q) {
		std::vector<int> old_tail, new_tail;
		for (int nu = 0; nu <= nu0; ++nu) {
			if (nu == b-q) {
				k[nu].push_back(q);
				for (int i = 0; i < old_tail.size(); ++i)
					k[nu].push_back(old_tail[i]);
				break;
			}
			else {
				int l = k[nu].size()-1;
				while (l > 0) {
					double gamma = gamma2(points[k[nu][l]], points[k[nu][l-1]]);
					if (phi(points[k[nu][l]], gamma) < phi(points[q], gamma))
						break;
					--l;
				}
				new_tail.clear();
				for (int i = l; i < k[nu].size(); ++i)
					new_tail.push_back(k[nu][i]);
				int s = k[nu].size();
				for (int i = l+1; i < s; ++i)
					k[nu].pop_back();
				k[nu].push_back(q);
				for (int i = 0; i < old_tail.size(); ++i)
					k[nu].push_back(old_tail[i]);
				old_tail = new_tail;
			}
		}
	}
	return k;
}
