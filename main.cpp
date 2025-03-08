#include "Point.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <utility>
using namespace std;
inline double square(double a) { return a * a; }
string getFileName();
vector<Point> getPointVector(ifstream& myFile);
template<typename Comparator>
void mergesort(vector<Point>& points, size_t left, size_t right, Comparator comp);
ostream& operator<<(ostream& os, const vector<Point>& vec);
double findDistance(const Point& a, const Point& b);
pair<Point, Point> findSmallestDistance(const Point& a, const Point& b);




int main() {
	ifstream myFile;
	vector<Point> points;
	Point::CompareXCoordinate xComp;
	Point::CompareYCoordinate yComp;
	myFile.open(getFileName());
	
	if (myFile.is_open()) {
		cout << "File successfully opened\n" << endl;

		points = getPointVector(myFile);
		size_t left{ 0 };
		size_t right{ points.size() - 1 };
		mergesort(points, left, right, xComp);
		cout << divideGraph(points, 0, points.size()-1);
		

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
void mergesort(vector<Point>& points, size_t left, size_t right, Comparator comp) {

	if (left >= right)
		return;
	size_t middle{ left + (right - left) / 2 };
	mergesort(points, left, middle, comp);
	mergesort(points, middle + 1, right, comp);
	merge(points, left, right, middle, comp);
}

template <typename Comparator>
void merge(vector<Point>& points, size_t left, size_t right, size_t middle, Comparator comp) {
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

ostream& operator<<(ostream& os, const vector<Point>& vec) {
	for (const auto& i : vec) {
		os << i << endl;
	}
	return os;
}

double findDistance(const Point& a, const Point& b) {
	double xDist = a.getX() - b.getX();
	double yDist = a.getY() - a.getY();
	return sqrt(square(xDist) + square(yDist));
}

double stripClosest(vector<Point> strip, double smallestDist) {
	double minDist = smallestDist;
	Point::CompareXCoordinate comp;
	mergesort(strip, 0, strip.size(), comp);
	for (size_t i = 0; i < strip.size(); i++) {
		for (size_t j = i + 1; j < strip.size() && (strip[j].getY() - strip[i].getY()) < minDist; j++) {
			if (findDistance(strip[i], strip[j]) < minDist){
				minDist = findDistance(strip[i], strip[j]);
			}
		}
	}
	return minDist;
}

double divideGraph(const vector<Point>& points, size_t low, size_t high) {
	if (high - low == 1) {
		return findDistance(points[low], points[high]);
	}
	if (low != high - 1) {
		size_t midIndex = (high - low) / 2 + low;
		Point midPoint = points[midIndex];
		double dist1 = divideGraph(points, low, midIndex - 1);
		double dist2 = divideGraph(points, midIndex + 1, high);
		double smallestDist = dist1 > dist2 ? dist2 : dist1;

		vector<Point> strip;
		for (size_t i = low; i <= high; i++) {
			if (abs(points[i].getX() - midPoint.getX()) < smallestDist) {
				strip.push_back(points[i]);
			}
		}
		double potentialClosest = stripClosest(strip, smallestDist);

		double result = smallestDist < stripClosest(strip, smallestDist) ? smallestDist : stripClosest(strip, smallestDist);
		return result;
	}
	return smallestDist
}
