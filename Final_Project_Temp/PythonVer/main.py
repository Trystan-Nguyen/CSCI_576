import sys

import AudioHandler

if __name__ == "__main__":
	query = 'D:/GitRepos/CSCI_576/Final_Project_Temp/Data/Queries/video2_1.mp4'
	subsignal = AudioHandler.extract_audio(query)

	fullfilepath = 'D:/GitRepos/CSCI_576/Final_Project_Temp/Data/Videos/'
	for i in range(11):
		fullfile = f'{fullfilepath}video{i+1}.mp4'
		signal = AudioHandler.extract_audio(fullfile)
		
		correlation = AudioHandler.audio_similarity(signal, subsignal)
		print(f'Video: {i+1}   Correlation: {correlation}')