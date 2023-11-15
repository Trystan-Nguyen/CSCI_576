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
	vector<tuple<unsigned int, unsigned int>> hueOccurence;
	int* dominantHues;


public:
	DominantColorList();
	void addDominantHue(unsigned int h, unsigned int o);

	int* getHueInformation() { return dominantHues; };

	int getHueOccurenceSize() { return hueOccurence.size(); };
	tuple<unsigned int, unsigned int> getFirstHueOccurence() { return hueOccurence.front(); };
	tuple<unsigned int, unsigned int> getLastHueOccurence() { return hueOccurence.back(); };
	int containsSubset(DominantColorList* subsample);

	void dumpData(string fileInput);
	void populateData(string fileInput);
};

