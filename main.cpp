#include "Point.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

string getFileName();
vector<Point> getPointVector(ifstream& myFile);
void mergesort();
ostream& operator<<(ostream& os, const vector<Point>& vec);



int main() {
	ifstream myFile;
	vector<Point> points;
	myFile.open(getFileName());
	
	if (myFile.is_open()) {
		cout << "File successfully opened\n" << endl;

		points = getPointVector(myFile);
		

		myFile.close();
	}
	else {
		cout << "File could not be opened\n" << endl;
	}


	return 0;
}

vector<Point> getPointVector(ifstream& myFile) {
	double x;
	double y;
	vector<Point> points;
	while (myFile >> x >> y) {
		points.push_back(Point(x, y));
	}
	return points;
}

string getFileName() {
	
	string fileName;
	cout << "Enter Filename: ";
	getline(cin, fileName);


	return fileName;
	
}

void mergesort(vector<int>& myVec, size_t left, size_t right, ) {

	if (left >= right)
		return;

	size_t middle{ left + (right - left) / 2 };
	mergesort(myVec, left, middle);
	mergesort(myVec, middle + 1, right);
	merge(myVec, left, right, middle, )
}

void merge(const vector<Point>& points, size_t left, size_t right, size_t middle, Point::CompareXCoordinate comparator) {
	size_t lhSize{ middle - left + 1 };
	size_t rhSize{ right - middle };
	vector<Point> lhVec(points.begin()+left, points.begin()+middle+1);
	vector<Point> rhVec(points.begin()+middle+1, points.begin()+right+1);
	
	size_t i{ 0 };
	size_t j{ 0 };
	
	while (i < lhSize && j < rhSize) {
		if (comparator(lhVec[i], rhVec[i])) {

		}
	}

}

ostream& operator<<(ostream& os, const vector<Point>& vec) {
	for (const auto& i : vec) {
		os << "(" << i.getX() << ", " << i.getY() << ")" << "\n";
	}
	return os;
}