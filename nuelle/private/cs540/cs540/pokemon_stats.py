import csv
import math
import sys
import matplotlib.pyplot as plt
import scipy.cluster
import numpy as np

# takes in a string with a path to a CSV file formatted as in the link above, 
# and returns the first 20 data points (without the Generation and Legendary columns but retaining all other columns) in a single structure.
def load_data(filepath):
	thelist = []
	with open(filepath, newline='') as csvfile:
		reader = csv.DictReader(csvfile)
		i = 0
		for row in reader:
			i += 1
			if i == 1:
				pass
			elif i == 21:
				break
			thedict = {}
			#,Name,Type 1,Type 2,Total,HP,Attack,Defense,Sp. Atk,Sp. Def,Speed
			thedict['#'] = int(row['#'])
			thedict['Name'] = row['Name']
			thedict['Type 1'] = row['Type 1']
			thedict['Type 2'] = row['Type 2']
			thedict['Total'] = int(row['Total'])
			thedict['HP'] = int(row['HP'])
			thedict['Attack'] = int(row['Attack'])
			thedict['Defense'] = int(row['Defense'])
			thedict['Sp. Atk'] = int(row['Sp. Atk'])
			thedict['Sp. Def'] = int(row['Sp. Def'])
			thedict['Speed'] = int(row['Speed'])
			thelist.append(thedict)
	return thelist

# takes in one row from the data loaded from the previous function, 
# calculates the corresponding x, y values for that Pokemon as specified above, and returns them in a single structure.
#"x" will represent the Pokemon's total offensive strength, which is defined by Attack + Sp. Atk + Speed. 
# Similarly, "y" will represent the Pokemon's total defensive strength, which is defined by Defense + Sp. Def + HP.
def calculate_x_y(stats):
	x  = stats['Attack'] + stats['Sp. Atk'] + stats['Speed']
	y = stats['Defense'] + stats['Sp. Def'] + stats['HP']
	return(x,y)

def cal_dis(cluster1, cluster2, data):
	# print(cluster1, cluster2)
	min_dist = sys.maxsize
	if isinstance(data[cluster1], tuple):
		if isinstance(data[cluster2], tuple):
			min_dist = math.sqrt(math.pow(data[cluster2][0] - data[cluster1][0], 2) + math.pow(data[cluster2][1] - data[cluster1][1], 2))
		elif isinstance(data[cluster2], list):
			for j in range(len(data[cluster2])): # length of the list
				# print('list item 1', data[cluster2][j][0], 'list item 2', data[cluster2][j][1])
				distance = math.sqrt(math.pow(data[cluster2][j][0] - data[cluster1][0], 2) + math.pow(data[cluster2][j][1] - data[cluster1][1], 2))
				if distance < min_dist:
					min_dist = distance
	elif isinstance(data[cluster1], list):
		if isinstance(data[cluster2], tuple):
			for j in range(len(data[cluster1])):
				# print(data[cluster2][j][0], data[cluster2][j][1])
				distance = math.sqrt(math.pow(data[cluster2][0] - data[cluster1][j][0], 2) + math.pow(data[cluster2][1] - data[cluster1][j][1], 2))
				if distance < min_dist:
					min_dist = distance
		elif isinstance(data[cluster2], list):
			for i in range(len(data[cluster1])):
				for j in range(len(data[cluster2])):
					# print(data[cluster2][j][0], data[cluster2][j][1])
					distance = math.sqrt(math.pow(data[cluster2][j][0] - data[cluster1][i][0], 2) + math.pow(data[cluster2][j][1] - data[cluster1][i][1], 2))
					if distance < min_dist:
						min_dist = distance


	return min_dist

# performs single linkage hierarchical agglomerative clustering on the Pokemon 
# with the (x,y) feature representation, and returns a data structure representing the clustering.
def hac(dataset):
	final_hac = []
	data = {}
	distances = {}
	for i in range(len(dataset)):
		data[i] = dataset[i]
	# print(data)
	m = len(dataset)
	# print(m)
	rows, cols = (m-1, 4) 
	arr = [[0]*cols]*rows 
	# min_dist = sys.maxsize
	minc1 = 0
	minc2 = 0

	for i in range(m-1):
		min_dist = sys.maxsize
		to_append = []
		for cluster1 in data:
			for cluster2 in data:
				if cluster1 != cluster2:
					dist_c1_c2 = cal_dis(cluster1, cluster2, data)
					# distances[cluster1,cluster2] = dist_c1_c2
					if dist_c1_c2 < min_dist:
						min_dist = dist_c1_c2
						minc1 = cluster1
						minc2 = cluster2
						# print('saved', minc1, minc2)

					elif dist_c1_c2 == min_dist: # if the distance of the two cluster 1 and cluster2 == dist bt minc1 and minc2, see which has the smallest index and keep those
						# print('minc1 and minc2', minc1, minc2, 'cluster1 and cluster2', cluster1, cluster2)
						if cluster1 < minc1: # if cluster1 has smaller index, keep the smaller, new pair cluster1
							min_dist = dist_c1_c2
							minc1 = cluster1
							minc2 = cluster2
							# print('cluster1 has smaller than minc1')

						elif cluster1 == minc1: # if they are equal, then look @ the second number
							if cluster2 < minc2: # if cluster2 has smaller index, keep smaller new pair cluster 2
								min_dist = dist_c1_c2
								minc1 = cluster1
								minc2 = cluster2
								# print('cluster2 has smaller than minc2')
							# if cluster2 > minc2, keep minc2.

						

		# print(min_dist)
		# print(data)
		# print('minc1, minc2', minc1, minc2)
		# Add new info to the data and combine clusters 1 and 2
		# print(data[cluster1])
		# print(data[cluster2])
		# print('minc1, minc2', minc1, minc2)
		c1val = data.pop(minc1)
		c2val = data.pop(minc2)
		# print('c1val', c1val, 'c2val', c2val)
		# print(distances)
		# print(data)

		if minc1 < minc2:
			to_append.append(minc1)
			to_append.append(minc2)

			if isinstance(c1val, tuple) and isinstance(c2val, tuple):
				data[m+i] = [c1val, c2val]
			elif isinstance(c1val, tuple):
				if isinstance(c2val, list):
					data[m+i] = [c1val] + c2val
			elif isinstance(c2val, tuple):
				if isinstance(c1val, list):
					data[m+i] = c1val + [c2val]
			else: # both are lists
				data[m+i] = c1val + c2val
		else:
			to_append.append(minc2)
			to_append.append(minc1)
			if isinstance(c1val, tuple) and isinstance(c2val, tuple):
				data[m+i] = [c2val, c1val]
			elif isinstance(c1val, tuple):
				if isinstance(c2val, list):
					data[m+i] = c2val + [c1val] 
			elif isinstance(c2val, tuple):
				if isinstance(c1val, list):
					data[m+i] = [c2val] + c1val 
			else: # both are lists
				data[m+i] = c2val + c1val 

		to_append.append(min_dist)
		total_points = 0
		
		if isinstance(c1val, tuple):
			if isinstance(c2val, tuple):
				total_points += 2
			elif isinstance(c2val, list):
				total_points += len(c2val) + 1 # 1 for tuple, len for list
		elif isinstance(c2val, tuple):
			if isinstance(c1val, list):
				total_points += len(c1val) + 1
		else:
			total_points += len(c1val) + len(c2val)

		to_append.append(total_points)
		# print(to_append)
		# print(data)
		final_hac.append(to_append)

	# print(final_hac)
	m = np.asmatrix(final_hac)
	return m
	






