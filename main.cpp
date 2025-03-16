#include "Point.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <utility>
#include <iomanip>
using namespace std;

#define pairPoints pair<Point,Point>

inline double square(double a) { return a * a; }

string getFileName();

vector<Point> getPointVector(ifstream & myFile);

template <typename Comparator>
void merge(vector<Point>& points, size_t left, size_t right, size_t middle, Comparator comp);

template<typename Comparator>
void mergesort(vector<Point>&points, size_t left, size_t right, Comparator comp);

ostream& operator<<(ostream & os, const vector<Point>&vec);
double findDistance(const Point & a, const Point & b);
pair<Point, Point> divide(vector<Point>& p, size_t floor, size_t size);
ostream& operator<<(ostream& os, pair<Point, Point>p);
inline double min(double x, double y);
inline pair<Point, Point> min(pair<Point, Point> x, pair<Point, Point> y);


int main() {
	ifstream myFile;
	vector<Point> points;
	Point::CompareXCoordinate xComp;
	myFile.open(getFileName());

	if (myFile.is_open()) {
		cout << "File successfully opened\n" << endl;

		

		points = getPointVector(myFile);
		mergesort(points, 0, points.size()-1, xComp);
		pairPoints closest = divide(points, 0, points.size() - 1);
		cout << "Closest points are: ("
			<< fixed << setprecision(2)
			<< closest.first << ") and ("
			<< closest.second << ") with distance = "
			<< fixed << setprecision(6)
			<< closest << endl;
		myFile.close();
	}
	else {
		cout << "File could not be opened\n" << endl;
	}

	return 0;
}

vector<Point> getPointVector(ifstream& myFile) {
	vector<Point> points;
	Point temp;
	while (myFile >> temp) {
		points.push_back(temp);
	}

	return points;
}

string getFileName() {

	string fileName;
	cout << "Enter Filename: ";
	getline(cin, fileName);


	return fileName;

}

template <typename Comparator>
void mergesort(vector<Point>&points, size_t left, size_t right, Comparator comp) {
	if (points.size() == 0 || points.size() == 1) {
		return;
	}
	if (left >= right) {
		return;
	}
	size_t middle{ left + (right - left) / 2 };
	mergesort(points, left, middle, comp);
	mergesort(points, middle + 1, right, comp);
	merge(points, left, right, middle, comp);
}

template <typename Comparator>
void merge(vector<Point>&points, size_t left, size_t right, size_t middle, Comparator comp) {
	size_t lhSize{ middle + 1 - left };
	size_t rhSize{ right - middle };
	vector<Point> lhVec(points.begin() + left, points.begin() + middle + 1);
	vector<Point> rhVec(points.begin() + middle + 1, points.begin() + right + 1);

	size_t i{ 0 };
	size_t j{ 0 };
	size_t k{ left };
	// compare every element in the sublist.
	// put it back in the biglist.
	while (i < lhSize && j < rhSize) {
		if (comp(lhVec[i], rhVec[j])) {
			points[k] = lhVec[i];
			i++;
		}
		else {
			points[k] = rhVec[j];
			j++;

		}
		++k;
	}
	// if there's leftovers:
	while (i < lhSize) {
		points[k++] = lhVec[i++];
	}
	while (j < rhSize) {
		points[k++] = rhVec[j++];
	}

}

// constant time
inline double min(double x, double y) {
	return (x < y ? x : y);
}
// constant time
inline pair<Point, Point> min(pair<Point, Point> x, pair<Point, Point> y) {
	return findDistance(x.first, x.second) < findDistance(y.first, y.second) ? x : y;
}

ostream& operator<<(ostream & os, const vector<Point>&vec) {
	for (const auto& i : vec) {
		os << i << endl;
	}
	return os;
}

ostream& operator<<(ostream& os, pair<Point, Point>p) {
	os << p.first << " " << p.second;
	return os;
}

inline double findDistance(const Point& a, const Point& b) {
	double xDist = a.getX() - b.getX();
	double yDist = a.getY() - b.getY();
	return sqrt(square(xDist) + square(yDist));
}

// nlogn
pair<Point, Point> stripClosest(vector<Point>& strip, double dist) {
	if (strip.size() < 2)
		return pairPoints(Point(INFINITY, INFINITY), Point(INFINITY, INFINITY));

	double min = dist;
	Point::CompareYCoordinate compY;
	// nlog(n)
	mergesort(strip, 0, strip.size() - 1, compY);
	pair<Point, Point> closestPair(Point(INFINITY, INFINITY), Point(INFINITY, INFINITY));
	// at most 7 runs
	for (size_t i = 0; i < strip.size(); i++) {
		for (size_t j = i + 1; j < strip.size() && (strip[j].getY() - strip[i].getY()) < min; j++) {
			if (findDistance(strip[i], strip[j]) < min) {
				min = findDistance(strip[i], strip[j]);
				closestPair.first = strip[i];
				closestPair.second = strip[j];
			}
		}
	}
	return closestPair;
}

pair<Point, Point> divide(vector<Point>& p, size_t floor, size_t size) {

	// if there's zero points
	// constant time
	if (floor == size)
		return pairPoints(Point(INFINITY, INFINITY), Point(INFINITY, INFINITY));

	// if theres 3 or less points
	// runs at MOST seven times
	if (size - floor <= 2) {
		// just do it brute force.
		pairPoints closestPair{ p[floor], p[floor + 1] };
		double minDist = findDistance(closestPair.first, closestPair.second);
		// sum from i=0 to 3
		for (size_t i = floor; i <= size; i++) {
			// sum from i+1 to 3
			for (size_t j = i + 1; j <= size; j++){
				double dist = findDistance(p[i], p[j]);
				if (dist < minDist) {
					minDist = dist;
					closestPair = { p[i], p[j] };
				}
			}
		}
		return closestPair;
	}

	size_t mid = (floor + size) / 2;
	Point midPoint = p[mid];

	// divide it into 2 sub problems
	// let b = 2 for the master theorem
	// 2 recursive calls, let a = 2 in the master theorem.
	pair<Point, Point> leftClosest = divide(p, floor, mid);
	double leftDist = findDistance(leftClosest.first, leftClosest.second);
	pair<Point, Point> rightClosest = divide(p, mid+1, size);
	double rightDist = findDistance(rightClosest.first, rightClosest.second);

	// constant time, gets dominated by asymptote
	double smallestDist = min(leftDist, rightDist);
	pair<Point, Point> closestPair = min(leftClosest, rightClosest);

	//O(N)
	vector<Point> strip;
	for (size_t i = floor; i <= size; i++) {
		if (abs(p[i].getX() - midPoint.getX()) < smallestDist) {
			strip.push_back(p[i]);
		}
	}
	// constant time, gets dominated by asymptote
	pair<Point, Point> stripPair = stripClosest(strip, smallestDist);
	double stripDist = findDistance(stripPair.first, stripPair.second);

	return (stripDist < smallestDist) ? stripPair : closestPair;
}
