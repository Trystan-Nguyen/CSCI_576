#include "AudioHandler.h"

AudioHandler::AudioHandler() {
	audioByFrame = {};
}

void AudioHandler::processAudio() {
	AudioFile<double> audioFile;
	audioFile.load(filepath);

	int numSamples = audioFile.getNumSamplesPerChannel();
	double sum = 0;
	for (int i = 0; i < numSamples; i++)
	{
		if (i != 0 && i % 1470 == 0) {
			audioByFrame.push_back(sum);
			sum = 0;
		}
		sum += abs(audioFile.samples[0][i]);
	}
}

int AudioHandler::compareAudio(AudioHandler* sample) {
	vector<double> subsignal = *sample->getAudioData();
	double minDifference = 999999;
	int index = -1;

	for (int i = 0; i < audioByFrame.size() - subsignal.size() + 1; ++i) {
		double diff = 0;
		for (int j = 0; j < subsignal.size(); ++j) {
			diff += abs(audioByFrame[i + j] - subsignal[j]);
		}
		if (diff < minDifference) {
			index = i;
			minDifference = diff;
		}
		printf("%d %f\n", i, diff);
	}

	printf("%d %f\n", index, minDifference);
	return index;
}

void AudioHandler::dumpData(string filepath) {
	std::ofstream of1(filepath + ".audio");
	for (double sample : audioByFrame)
		of1 << sample << "\n";
	
	of1.close();
}

void AudioHandler::populateData(string filepath) {
	std::ifstream if1(filepath + ".audio");
	double a;
	while (if1 >> a) {
		audioByFrame.push_back(a);
	}
	if1.close();

}