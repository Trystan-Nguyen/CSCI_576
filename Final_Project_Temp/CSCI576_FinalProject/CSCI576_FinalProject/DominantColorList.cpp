#include "DominantColorList.h"

DominantColorList::DominantColorList() {
	hueOccurence = {};
	dominantHues = new int[360];
	for (int i = 0; i < 360; ++i) dominantHues[i] = 0;
}

void DominantColorList::addDominantHue(unsigned int h, unsigned int o) {
	dominantHues[h] = 1;
	hueOccurence.push_back(make_tuple(h, o));
}

int DominantColorList::containsSubset(DominantColorList* subsample) {
	int* subSampleDominantHues = subsample->getHueInformation();
	for (int i = 0; i < 360; ++i) {
		if (subSampleDominantHues[i]==1 && !(dominantHues[i]==1)) return -1;
	}
	
	tuple<unsigned int, unsigned int> firstElem = subsample->getFirstHueOccurence();
	tuple<unsigned int, unsigned int> lastElem = subsample->getLastHueOccurence();
	for (int i = 0; i < hueOccurence.size() - subsample->getHueOccurenceSize()+1; ++i) {
		if (hueOccurence[i] == firstElem && hueOccurence[i + hueOccurence.size() - 1] == lastElem) {
			return i;
		}
	}
	
	return -1;
}

void DominantColorList::dumpData(string filepath) {
	std::ofstream of1(filepath + ".hue");
	for (int count = 0; count < 360; count++) {
		of1 << dominantHues[count] << "\n";
	}
	of1.close();

	std::ofstream of2(filepath + ".vec");
	for (tuple<unsigned int, unsigned int> pair : hueOccurence) {
		of2 << get<0>(pair) << " " << get<1>(pair) << "\n";
	}
	of2.close();
}
void DominantColorList::populateData(string filepath) {
	std::ifstream if1(filepath + ".hue");
	int hueExist;
	int i = 0;
	while(if1 >> hueExist){
		dominantHues[i++] = hueExist;
	}
	if1.close();
	
	std::ifstream if2(filepath + ".vec");
	unsigned int a; unsigned int b;
	while (if2 >> a >> b){
		hueOccurence.push_back(std::tuple<unsigned int, unsigned int>(a, b));
	}
	if2.close();
}