import cv2 
import numpy as np 

def rgb_to_hue(r,g,b):
	red = r/255
	green = g/255
	blue = b/255
	cmax = max(red, blue, green)
	cmin = min(red, blue, green)
	delta = cmax-cmin
	if delta == 0:
		return 0
	elif cmax == red:
		return (60 * (round((green - blue) / delta) % 6))
	elif cmax == green:
		return (60 * (((blue - red) / delta) + 2))
	elif cmax == blue:
		return (60 * (((red - green) / delta) + 4))


def get_scenes(uri_str):
	cap = cv2.VideoCapture(uri_str) 
	
	if cap.isOpened() is False: 
		print("Error opening video file") 

	prev_frame = None
	
	while cap.isOpened():
		ret, frame = cap.read()

		if ret == True:
			current_frame = [[0]*len(frame[0])]*len(frame)
			
			count = 0

			for i in range(len(frame)):
				for j in range(len(frame[i])):
					print(count)
					current_frame = [rgb_to_hue(c[2], c[1], c[0]) for c in frame[i]]
					count += 1
			if prev_frame is None:
				prev_frame = current_frame
			else:
				cumulative_deviation = 0
				for i in range(len(current_frame)):
					for j in range(len(current_frame[i])):
						deviation = abs(current_frame[i][j] - prev_frame[i][j])
						cumulative_deviation += min(deviation, 360-deviation)

				print(cumulative_deviation)

		else:
			break