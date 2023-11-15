from moviepy.editor import *
from util.img_helpers import *

WIDTH = 352
HEIGHT = 288

def create_hue_occurence_arr(fname):
	video = VideoFileClip(fname)

	hue_occurrence_arr = []
	for frame in video.iter_frames():
		current_hist = create_hue_hist(frame)
		max_index = np.argmax(current_hist)

		hue_occurrence_arr.append(max_index)
		hue_occurrence_arr.append(current_hist[max_index])
		
	return np.array(hue_occurrence_arr)



