import json
import numpy as np

src = None
with open('src_test1.txt', 'r') as f:
	src = np.array(json.loads(f.read()))

query = None
curr_min = 999999999
index = -1
with open('query_test1.txt', 'r') as f:
	query = np.array(json.loads(f.read()))

all_diff = []
for i in range(len(src) - len(query) + 1):
	subSrc = src[i:i+len(query)]
	diff = query - subSrc
	
	curdif = np.sum(np.abs(diff))
	#all_diff.append(curdif)
	#curr_min = min(curr_min, curdif)
	#print(curdif)
	if curdif < curr_min:
		index = i
		curr_min = curdif

print(index)
'''
all_diff.sort()
print(all_diff[0])
print(all_diff[1])
print(all_diff[2])

all_diff = min(np.array(all_diff))
print(all_diff)
'''

