#pragma once

#include <vector>
#include <string>
#include <tuple>

#include <iostream>
#include <fstream>

using namespace std;
class DominantColorList
{
private:
	vector<tuple<int, int>> hueOccurence;
	int* dominantHues;
	double differenceTolerance = 0.15;

public:
	DominantColorList();
	void addDominantHue(int h, int o);

	int* getHueInformation() { return dominantHues; };

	int getHueOccurenceSize() { return hueOccurence.size(); };
	tuple<int, int> getFirstHueOccurence() { return hueOccurence.front(); };
	tuple<int, int> getLastHueOccurence() { return hueOccurence.back(); };
	vector<tuple<int, int>>* getHueOccurence() { return &hueOccurence; };
	int containsSubset(DominantColorList* subsample);
	bool checkIfValidSubset(int offset, vector<tuple<int, int>>* subsample);
	

	void dumpData(string fileInput);
	void populateData(string fileInput);
};

