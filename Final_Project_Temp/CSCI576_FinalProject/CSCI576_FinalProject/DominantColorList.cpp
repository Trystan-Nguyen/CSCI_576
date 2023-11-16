#include "DominantColorList.h"

DominantColorList::DominantColorList() {
	hueOccurence = {};
	dominantHues = new int[360];
	for (int i = 0; i < 360; ++i) dominantHues[i] = 0;
}

void DominantColorList::addDominantHue(int h, int o) {
	++dominantHues[h];
	hueOccurence.push_back(make_tuple(h, o));
}

int DominantColorList::containsSubset(DominantColorList* subsample) {
	int* subSampleDominantHues = subsample->getHueInformation();
	for (int i = 0; i < 360; ++i) {
		if (subSampleDominantHues[i] != 0 && dominantHues[i] == 0) {
			//printf("\n\nEND EARLY at %d\n\n", i);
			return -1;
		}
	}

	
	tuple<int, int> firstElem = subsample->getFirstHueOccurence();
	tuple<int, int> lastElem = subsample->getLastHueOccurence();
	vector<tuple<int, int>>* subSignal = subsample->getHueOccurence();

	for (int i = 0; i < hueOccurence.size() - subsample->getHueOccurenceSize()+1; ++i) {
		int difference1 = abs(get<0>(hueOccurence[i]) - get<0>(firstElem));
		difference1 = min(difference1, 360 - difference1);
		int difference2 = abs(get<0>(hueOccurence[i + subsample->getHueOccurenceSize() - 1]) - get<0>(lastElem));
		difference2 = min(difference2, 360 - difference2);
		
		/*
		if (i == 8129) {
			printf("%d - %d\n", get<0>(hueOccurence[i]), get<0>(firstElem));
			printf("%d - %d\n\n", get<1>(hueOccurence[i]), get<1>(firstElem));

			printf("%d - %d\n", get<0>(hueOccurence[i + subsample->getHueOccurenceSize() - 1]), get<0>(lastElem));
			printf("%d - %d\n\n", get<1>(hueOccurence[i + subsample->getHueOccurenceSize() - 1]), get<1>(lastElem));
		}
		*/

		if (difference1 < 60 && 
			double(abs(get<1>(hueOccurence[i]) - get<1>(firstElem))) / get<1>(firstElem) < differenceTolerance &&
			difference2 < 60 &&
			double(abs(get<1>(hueOccurence[i + subsample->getHueOccurenceSize() - 1]) - get<1>(lastElem))) / get<1>(lastElem) < differenceTolerance) {
				//printf("CHECK Entry: %d\n", i);
				if (checkIfValidSubset(i, subSignal)) return i;
		}
	}
	
	return -1;
}

bool DominantColorList::checkIfValidSubset(int offset, vector<tuple<int, int>>* subsample) {
	for (int i = 0; i < subsample->size(); ++i) {
		int difference = abs(get<0>(hueOccurence[i + offset]) - get<0>((*subsample)[i]));
		difference = min(difference, 360 - difference);
		if (difference > 60) {
			/*
			if (offset == 8880) {
				printf("Missed at subsignal index: %d", i);
				printf("\t%d - %d\n", get<0>(hueOccurence[i + offset]), get<0>((*subsample)[i]));
			}
			*/
			return false;
		}
	}
	return true;
}

void DominantColorList::dumpData(string filepath) {
	std::ofstream of1(filepath + ".hue");
	for (int count = 0; count < 360; count++) {
		of1 << dominantHues[count] << "\n";
	}
	of1.close();

	std::ofstream of2(filepath + ".vec");
	for (tuple<int, int> pair : hueOccurence) {
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
	int a; int b;
	while (if2 >> a >> b){
		hueOccurence.push_back(std::tuple<int, int>(a, b));
	}
	if2.close();
}