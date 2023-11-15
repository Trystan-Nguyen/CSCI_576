import numpy as np

def rgbToHue(red, green, blue):
	b = blue / 255;
	g = green / 255;
	r = red / 255;

	maxColor = max(b, max(g, r));
	minColor = min(b, min(g, r));
	c = maxColor - minColor;

	h = 0;
	if c <= 0:
		h = 0;
	elif r == maxColor: 
		segment = (g - b) / c;
		if b > g:
			h = segment + 6.0
		else:
			h = segment + 0.0
	
	elif g == maxColor:
		segment = (b - r) / c;
		h = segment + 2
	
	elif b == maxColor:
		segment = (r - g) / c
		h = segment + 4
	

	hue = round(h * 60)
	if hue == 360:
		hue = 0

	return hue

def create_hue_hist(img):
	hue_hist = np.array([0]*360)
	for i in range(len(img)):
		for j in range(len(img[i])):
			hue_hist[rgbToHue(img[i][j][2], img[i][j][1], img[i][j][0])] += 1
	return hue_hist

def compare_hue_hist(hue_hist1, hue_hist2):
	return pow(np.sum(pow(hue_hist1-hue_hist2,2)), 1/2)