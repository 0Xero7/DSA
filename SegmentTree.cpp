/*****************************************************************************************************************
Author: Soumya Pattanayak
Github: https://github.com/0Xero7/DSA/

A generic segment tree. 
How to use: 
	constructor -> takes 2 arguments, 1) the vector of objects,
									  2) a lambda to merge two objects
										eg 1: if you want a segment tree to calculate range sums,
										    the lambda should be [] (int a, int b) -> int { return a + b; }
										eg 2: if you want a segment tree to calculate range minumums,
											the lambda should be [] (int a, int b) -> int { return min(a, b); }

		A code snippet for a range sum segment tree:
			vector<int> arr { 1, 2, 3, 4, 5 };
			auto g = [] (int a, int b) -> int { return a + b; };
			SegmentTree<int, decltype(g)> tree(arr, g);

	queries: 1) calculate queries -> T getValue(int l, int r)
				take 2 parameters, the beginning and end of the range, both inclusive
				query ranges are 1-indexed. So, if you want the sum for first 3 elements, the range should be 1-3.

			 2) point update query -> updateValue(int idx, T val)
				supports point updates (so far). updates the value at index "idx" to val, and recalculates required values.
**************************************************************************************************************/


#include <vector>
using namespace std;
#ifdef _MSC_VER
#include <intrin.h>
#endif

using ll = long long;

template <class T, class Merge>
class SegmentTree {
private:
	std::vector<T> arr;
	std::vector<T> f;
	int n;
	Merge merge;

	// this is done to accomodate MSVC as well as GCC
	int popcnt(int& n) {
#ifdef __GNUC__
		return __builtin_popcount(n);
#endif
#ifdef _MSC_VER
		return __popcnt(n);
#endif
	}

	T build(int l) {
		if (2 * l + 1 >= (int)f.size() - 1) { // Leaf Nod
			return f[l];
		}
		return f[l] = merge(build(2 * l + 1), build(2 * l + 2));
	}

	T calcValue(int idx, int nodeL, int nodeR, int reqL, int reqR) {
		if (nodeL >= reqL && nodeR <= reqR) return f[idx];

		int mid = (nodeL + nodeR) / 2;
		bool leftFlag = (mid >= reqL);
		bool rightFlag = (mid + 1 <= reqR);

		if (leftFlag && rightFlag) return merge(calcValue(2 * idx + 1, nodeL, mid, reqL, reqR),
				calcValue(2 * idx + 2, mid + 1, nodeR, reqL, reqR));
		else if (leftFlag) return calcValue(2 * idx + 1, nodeL, mid, reqL, reqR);
		else return calcValue(2 * idx + 2, mid + 1, nodeR, reqL, reqR);
	}

public:
	SegmentTree(std::vector<T> a, Merge merge) :
	  arr(a), merge(merge) {
		this->n = a.size();
		const int N = n;
		while (popcnt(this->n) != 1) {
			++(this->n);
		}
		f.resize(2 * n);

		// Build the segment tree
		for (int i = 0; i < N; ++i) {
			f[n + i - 1] = arr[i];
		}
		build(0);
	}

	T getValue(int l, int r) {
		--l; --r;
		return calcValue(0, 0, n - 1, l, r);
	}


	void updateValue(int idx, T newValue) {
		--idx;
		int i = n + idx - 1;
		arr[idx] = f[i] = newValue;

		i = (i - 1) / 2;
		while (i > 0) {
			f[i] = merge(f[2 * i + 1], f[2 * i + 2]);
			i = (i - 1) / 2;
		}
		f[0] = merge(f[1], f[2]);
	}
};