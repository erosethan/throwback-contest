#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
const double ERROR = 1e-9;
const double M_PI2 = M_PI / 2;
const double M_2PI = M_PI * 2;

struct Double {
	double d; Double(double D) : d(D) {}
	bool operator<(const Double& cmp) const {
		return (fabs(d - cmp.d) < ERROR)?
			false: d < cmp.d;
	}
};

int main() {
	int n, x, y;
	for (int c = 1; true; ++c) {
		scanf("%d", &n);
		if (n == 0) break;
		vector<pii> points;
		for (int i = 0; i < n; ++i) {
			scanf("%d%d", &x, &y);
			points.push_back(make_pair(x, y));
		}
		long long ans = n, not_valid = 0;
		ans = (ans * (ans - 1) * (ans - 2)) / 6;
		for (int i = 0; i < n; ++i) {
			vector<Double> angles;
			for (int j = 0; j < n; ++j) {
				if (i == j) continue;
				x = points[j].first - points[i].first;
				y = points[j].second - points[i].second;
				double angle = !x? M_PI2: atan(fabs(y) / fabs(x));
				if (x >= 0 && y < 0) angle = M_2PI - angle;
				if (x < 0 && y >= 0) angle = M_PI - angle;
				if (x < 0 && y < 0) angle = M_PI + angle;
				angles.push_back(Double(angle));
			}
			sort(angles.begin(), angles.end());
			for (int j = 0; j < n - 1; ++j)
				angles.push_back(Double(angles[j].d + M_2PI));
			for (int j = 0; j < n - 1; ++j) {
				int left = lower_bound(angles.begin(), angles.end(),
					Double(angles[j].d + M_PI2)) - angles.begin();
				int right = upper_bound(angles.begin(), angles.end(),
					Double(angles[j].d + M_PI2 + M_PI)) - angles.begin();
				not_valid += right - left;
			}
		}
		printf("Scenario %d:\n", c);
		printf("There are %lld ", ans - not_valid / 2);
		printf("sites for making valid tracks\n");
	}
	return 0;
}
