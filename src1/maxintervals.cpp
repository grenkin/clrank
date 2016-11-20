#include "maxintervals.h"

MaxIntervals MaxIntervalsSet(std::vector<std::vector<double> > &deltamin, double delta0, int nu0)
{
	MaxIntervals max_intervals;
	int n = deltamin.size();
	int k = 2 > nu0-1 ? 2 : nu0-1;
	for (int a = 0; a < n; ++a) {
		for (int b = a+k; b < n; ++b) {
			if (deltamin[a][b] <= delta0 &&
				(a == 0 || deltamin[a-1][b] > delta0) &&
				(b == n-1 || deltamin[a][b+1] > delta0))
			{
				Interval interval(a, b);
				max_intervals.push_back(interval);
			}
		}
	}
	return max_intervals;
}