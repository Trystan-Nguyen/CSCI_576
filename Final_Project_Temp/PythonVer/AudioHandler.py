from scipy.signal import correlate
import numpy as np
from moviepy.editor import *


def extract_audio(fname):
	video = VideoFileClip(fname)
	return video.audio.to_soundarray().flatten()

def audio_similarity(signal, subsignal):
	corr = correlate(signal, subsignal, mode='same')           
	return max(abs(corr))

