

def compare(vec1, vec2):
	counter = 0
	for i in vec1:
		if i in vec2:
			counter+=1
	return counter > 1

if __name__ == '__main__':
	uri = '../PreprocessData/video'
	for i in range(20):
		index = i + 1
		current_uri = f'{uri}{index}.vec'

		lines = None
		with open(current_uri, 'r') as f:
			lines = f.readlines()

		prev = []
		counter = 0
		for line in lines:
			vec = line.split()
			if not compare(vec, prev):
				counter += 1
			prev = vec

		h_current_uri = f'{uri}{index}.hue'
		h_lines = None
		with open(h_current_uri, 'r') as f:
			h_lines = f.readlines()
		zero_counter = 0
		for line in h_lines:
			h = line.split()
			if len(h) > 0 and h[0] == '0':
				zero_counter += 1

		print(f'Video{index}: {counter} \t {zero_counter}')
		#print(f'\t{zero_counter>200 and counter < 200}')